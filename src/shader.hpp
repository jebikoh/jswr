#pragma once

#include <jtxlib/math/vec3.hpp>
#include <jtxlib/math/vec4.hpp>
#include <jtxlib/math/vec2.hpp>
#include <array>
#include <functional>
#include <utility>

#include "model.hpp"

namespace jswr {
    // Vertex input is just a Vertex struct
    // Other inputs are passed as uniforms
    struct VertexOutput {
        jtx::Vec4f position;
        std::array<float, 16> varying;
    };

    struct FragmentInput {
        jtx::Vec3f barycentric;
        std::array<float, 16> varying;
    };

    struct FragmentOutput {
        jtx::Vec4f color;
    };


    template<typename Uniform, typename Varying>
    class ShaderProgram {
    public:
        using VertexShader   = std::function<VertexOutput(const Vertex&, const Uniform&)>;
        using FragmentShader = std::function<FragmentOutput(const FragmentInput&, const Uniform&, const Varying&)>;

        ShaderProgram(VertexShader vertexShader, FragmentShader fragmentShader) : vertexShader(std::move(vertexShader)), fragmentShader(std::move(fragmentShader)) {};
        ~ShaderProgram() = default;

        VertexOutput vertex(const Vertex &vertex) {
            return vertexShader(vertex, uniforms);
        }

        FragmentOutput fragment(const FragmentInput &fragmentInput) {
            return fragmentShader(fragmentInput, uniforms, varyings);
        }

        Uniform uniforms;
    private:
        VertexShader vertexShader;
        FragmentShader fragmentShader;
        Varying varyings;
    };
}