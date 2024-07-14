#pragma once
#include <vector>
#include <cstdint>
#include "jtxlib/math/vec2.hpp"
#include "jtxlib/math/vec3.hpp"

namespace jswr {
    struct Vertex {
        jtx::Vec3f position;
        jtx::Vec3f normal;
        jtx::Vec2f uv;
    };

    struct Face {
        uint32_t v0, v1, v2;
    };

    struct Mesh {
        uint32_t vertex_offset, num_vertices;
        uint32_t index_offset, index_count;
    };

    class Model {
    public:
        std::vector<Vertex> vertexBuffer;
        std::vector<Face> indexBuffer;
        std::vector<Mesh> meshes;

        Model() = default;
        ~Model() = default;
    };
} // jswr

