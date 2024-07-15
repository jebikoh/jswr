// model.hpp
#pragma once

#include <vector>
#include <cstdint>
#include <assimp/material.h>

#include "jtxlib/math/vec2.hpp"
#include "jtxlib/math/vec3.hpp"
#include "jtxlib/math/mat4.hpp"

namespace jswr {
    struct Vertex {
        jtx::Vec3f position;
        jtx::Vec3f normal;
        jtx::Vec2f uv;
    };

    struct Face {
        uint32_t v0, v1, v2;
    };

    enum class TextureType {
        DIFFUSE = aiTextureType_DIFFUSE,
        SPECULAR = aiTextureType_SPECULAR,
        NORMAL = aiTextureType_NORMALS,
        HEIGHT = aiTextureType_HEIGHT
    };

    struct Texture {
        uint32_t width, height, channels;
        TextureType type;
        std::string path;
        std::vector<uint8_t> data;
    };

    struct Material {
        jtx::Vec3f ambient;
        jtx::Vec3f diffuse;
        jtx::Vec3f specular;
        float shininess;
        std::vector<uint32_t> diffuse_maps;
        std::vector<uint32_t> specular_maps;
        std::vector<uint32_t> normal_maps;
        std::vector<uint32_t> height_maps;
    };

    struct Mesh {
        uint32_t vertex_offset, num_vertices;
        uint32_t index_offset, index_count;
        // Material ID is its index in the materials array.
        uint32_t material_id;
        jtx::Mat4 model_matrix;
    };

    struct Model {
        std::vector<Vertex> vertexBuffer;
        std::vector<Face> indexBuffer;
        std::vector<Texture> textureBuffer;
        std::vector<Material> materials;
        std::vector<Mesh> meshes;
    };

    bool loadModel(const char *path, Model &model);
} // jswr

