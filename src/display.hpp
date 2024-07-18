#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "buffers.hpp"

namespace jswr {
    class Display {
    public:
        Display(int width, int height, const char *title) : width(width), height(height) { // NOLINT(*-pro-type-member-init)
            setupWindow(title);
            setupVAO();
            setupShaders();
            setupTexture();
        }

        ~Display() {
            glDeleteTextures(1, &texture);
            glfwDestroyWindow(window);
            glfwTerminate();
        }

        void swap(const FrameBuffer &frameBuffer);

        bool shouldClose() {
            return glfwWindowShouldClose(window);
        }

    private:
        int width, height;
        GLFWwindow *window;

        GLuint VAO, VBO, EBO;
        GLuint texture;
        GLuint shader;

        void setupVAO();

        void setupShaders();

        void setupTexture();

        void setupWindow(const char *title);
    };

} // jswr
