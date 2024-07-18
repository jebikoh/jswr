#pragma once

#include <buffers.hpp>
#include "model.hpp"
#include "camera.hpp"
#include "shader.hpp"

namespace jswr {
    template<typename Uniform, typename Varying>
    class Rasterizer {
    public:
        Rasterizer(int width, int height) : width(width), height(height), frameBuffer(width, height) {}
        ~Rasterizer() = default;


        void clear(const Color &color) {
            frameBuffer.clear(color);
        }

        FrameBuffer &getFrameBuffer() {
            return frameBuffer;
        }

        void draw(const Model &model, const Camera &camera);

    private:
        int width, height;
        FrameBuffer frameBuffer;
        ShaderProgram<Uniform, Varying> shader;
    };
} // jswr