#pragma once

#include <vector>
#include <cstdint>
#include <jtxlib/simd/avxfloat.hpp>
#include <jtxlib/math/vec2.hpp>
#include <jtxlib/math/vec3.hpp>

namespace jswr {
    struct VertexBuffer {
        // Store position and normals in SoA format
        std::vector<jtx::AVXVec3f> position;
        std::vector<jtx::AVXVec3f> normal;
        // Store UVs in AoS format--they are normally accessed together
        std::vector<jtx::Vec2f> uv;
        // Used to handle number of vertices are not a multiple of 8
        size_t num_vertices = 0;

        // Can add more constructors later if needed
        VertexBuffer() = default;
        ~VertexBuffer() = default;

        // Add vertex to buffer (only really used by model importer)
        void addVertex(const jtx::Vec3f& pos, const jtx::Vec3f& norm, const jtx::Vec2f& uv_) {
            if (num_vertices % 8 == 0) {
                position.emplace_back();
                normal.emplace_back();
            }
            int avx_lane = num_vertices % 8;
            position.back().x[avx_lane] = pos.x;
            position.back().y[avx_lane] = pos.y;
            position.back().z[avx_lane] = pos.z;
            normal.back().x[avx_lane] = norm.x;
            normal.back().y[avx_lane] = norm.y;
            normal.back().z[avx_lane] = norm.z;

            uv.emplace_back(uv_);
            num_vertices++;
        }

        void addVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v) {
            // Create new AVX struct if we need to
            if (num_vertices % 8 == 0) {
                position.emplace_back();
                normal.emplace_back();
            }
            int avx_lane = num_vertices % 8;
            position.back().x[avx_lane] = x;
            position.back().y[avx_lane] = y;
            position.back().z[avx_lane] = z;
            normal.back().x[avx_lane] = nx;
            normal.back().y[avx_lane] = ny;
            normal.back().z[avx_lane] = nz;

            uv.emplace_back(u,v );
            num_vertices++;
        }

        [[nodiscard]] size_t size() const { return num_vertices; }
    };

    struct Face {
        uint32_t v0, v1, v2;
    };

    struct IndexBuffer {
        std::vector<Face> indices;
        size_t num_faces = 0;

        IndexBuffer() = default;

        void addFace(const Face& face) {
            indices.push_back(face);
            num_faces++;
        }

        void addFace(uint32_t v0, uint32_t v1, uint32_t v2) {
            indices.push_back({v0, v1, v2});
            num_faces++;
        }

        [[nodiscard]] size_t size() const { return num_faces; }
    };
}