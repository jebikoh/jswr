#pragma once

#include <buffers.hpp>

namespace jswr {
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

    private:
        int width, height;
        FrameBuffer frameBuffer;
    };
} // jswr