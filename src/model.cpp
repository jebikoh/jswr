// model.cpp
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "model.hpp"

#include <jtxlib/math/vec3.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace jswr {
    //region Helper functions
    static jtx::Mat4 aiMatrix4x4ToMat4(const aiMatrix4x4 &m) {
        return {
            m.a1, m.a2, m.a3, m.a4,
            m.b1, m.b2, m.b3, m.b4,
            m.c1, m.c2, m.c3, m.c4,
            m.d1, m.d2, m.d3, m.d4
        };
    }

    static jtx::Vec3f getMaterialVec3(aiMaterial *material, const char *key, unsigned int type, unsigned int index) {
        aiColor3D color;
        if (material->Get(key, type, index, color) == AI_SUCCESS) {
            return {color.r, color.g, color.b};
        }
        return {-1.0f, -1.0f, -1.0f};
    }

    static float getMaterialFloat(aiMaterial *material, const char *key, unsigned int type, unsigned int index) {
        float value;
        if (material->Get(key, type, index, value) == AI_SUCCESS) {
            return value;
        }
        return -1.0f;
    }
    //endregion

    static bool loadTexture(Model &model, aiMaterial *material, aiTextureType type, std::vector<uint32_t> &textures) {
        // Texture data is stored in the model
        // Texture IDs (their index in the buffer) are stored in the material
        // When adding a texture, we check if the path is already in the buffer, and just return the ID if so.
        for (unsigned int i = 0; i < material->GetTextureCount(type); ++i) {
            aiString str;
            material->GetTexture(type, i, &str);
            bool loaded = false;
            for (uint32_t j = 0; j < textures.size(); ++j) {
                if (model.textureBuffer[j].path == str.C_Str()) {
                    textures.push_back(j);
                    loaded = true;
                    break;
                }
            }
            if (!loaded) {
                Texture tex;
                std::string filename = str.C_Str();

                int width, height, channels;
                unsigned char *data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
                if (!data) {
                    std::cerr << "Error: Failed to load texture " << filename << std::endl;
                    return false;
                }
                tex.data = std::vector<uint8_t>(data, data + width * height * channels);
                stbi_image_free(data);

                tex.width = width;
                tex.height = height;
                tex.channels = channels;
                tex.type = static_cast<TextureType>(type);
                tex.path = filename;

                auto texID = static_cast<uint32_t>(model.textureBuffer.size());
                model.textureBuffer.push_back(tex);
                textures.push_back(texID);
            }
        }
        return true;
    }

    static bool loadMaterials(const aiScene *scene, Model &model) {
        model.materials.reserve(scene->mNumMaterials);

        for (unsigned int i = 0; i < scene->mNumMaterials; ++i) {
            aiMaterial *material = scene->mMaterials[i];
            Material m;

            m.ambient   = getMaterialVec3(material, AI_MATKEY_COLOR_AMBIENT);
            m.diffuse   = getMaterialVec3(material, AI_MATKEY_COLOR_DIFFUSE);
            m.specular  = getMaterialVec3(material, AI_MATKEY_COLOR_SPECULAR);
            m.shininess = getMaterialFloat(material, AI_MATKEY_SHININESS);

            if (!loadTexture(model, material, aiTextureType_DIFFUSE, m.diffuse_maps)) return false;
            if (!loadTexture(model, material, aiTextureType_SPECULAR, m.specular_maps)) return false;
            if (!loadTexture(model, material, aiTextureType_NORMALS, m.normal_maps)) return false;
            if (!loadTexture(model, material, aiTextureType_HEIGHT, m.height_maps)) return false;

            model.materials.emplace_back(m);
        }

        return true;
    }

    static void processMesh(aiMesh *mesh, Model &model, const aiScene *scene, jtx::Mat4 &transform) {
        Mesh m;
        m.vertex_offset = static_cast<size_t>(model.vertexBuffer.size());
        m.index_offset = static_cast<size_t>(model.indexBuffer.size());
        m.model_matrix = transform;
        if (mesh->mMaterialIndex >= 0 && mesh->mMaterialIndex < model.materials.size()) {
            m.material_id = mesh->mMaterialIndex;
        } else {
            std::cerr << "Warning: Invalid material index for mesh" << std::endl;
            m.material_id = 0;
        }

        // Process vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
            Vertex v;
            v.position = {mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z};
            if (mesh->HasNormals()) {
                v.normal = {mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z};
            }
            if (mesh->mTextureCoords[0]) {
                v.uv = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
            } else {
                v.uv = {0.0f, 0.0f};
            }
            model.vertexBuffer.push_back(v);
        }
        // Process indices
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            aiFace face = mesh->mFaces[i];
            if (face.mNumIndices == 3) {
                model.indexBuffer.push_back({face.mIndices[0], face.mIndices[1], face.mIndices[2]});
            } else {
                std::cerr << "Warning: Skipping on-triangular face in mesh" << std::endl;
            }
        }

        m.num_vertices = mesh->mNumVertices;
        m.index_count = static_cast<uint32_t>(model.indexBuffer.size() - m.index_offset);

        model.meshes.emplace_back(m);
    }

    static void processNode(aiNode *node, const aiScene *scene, Model &model, const jtx::Mat4 &pTransform) {
        jtx::Mat4 transform = pTransform * aiMatrix4x4ToMat4(node->mTransformation);

        for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
            processMesh(scene->mMeshes[node->mMeshes[i]], model, scene, transform);
        }

        for (unsigned int i = 0; i < node->mNumChildren; ++i) {
            processNode(node->mChildren[i], scene, model, transform);
        }
    }

    bool loadModel(const char *path, Model &model) {
        Assimp::Importer importer;
        const aiScene *scene = importer.ReadFile(path,
                                                 aiProcess_Triangulate |
                                                 aiProcess_FlipUVs |
                                                 aiProcess_GenNormals |
                                                 aiProcess_JoinIdenticalVertices);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cerr << "Error: " << importer.GetErrorString() << std::endl;
            return false;
        }

        if(!loadMaterials(scene, model)) return false;
        processNode(scene->mRootNode, scene, model, jtx::Mat4::identity());

        return true;
    }
} // jswr