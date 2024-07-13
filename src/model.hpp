#pragma once
#include <jtxlib/simd/avxfloat.hpp>
#include <vector>
#include <cstdint>
#include "jtxlib/math/vec2.hpp"

namespace jswr {
    struct VertexBuffer {
        std::vector<jtx::AVXVec3f> x, y, z;
        std::vector<jtx::AVXVec3f> nx, ny, nz;
        std::vector<jtx::Vec2f> u, v;
    };

    struct IndexBuffer {
        std::vector<uint32_t> indices;
    };

    class model {

    };

} // jswr

