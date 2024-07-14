#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "buffers.hpp"
#include "rasterizer.hpp"

namespace jswr {
    class Renderer {
    public:
        Renderer(int width, int height, const char *title) : width(width), height(height), rasterizer(width, height) { // NOLINT(*-pro-type-member-init)
            setupWindow(title);
            setupVAO();
            setupShaders();
            setupTexture();
        }

        ~Renderer() {
            glDeleteTextures(1, &texture);
            glfwDestroyWindow(window);
            glfwTerminate();
        }

        void render();

        bool shouldClose() {
            return glfwWindowShouldClose(window);
        }

    private:
        int width, height;
        GLFWwindow *window;
        Rasterizer rasterizer;

        GLuint VAO, VBO, EBO;
        GLuint texture;
        GLuint shader;

        void setupVAO();

        void setupShaders();

        void setupTexture();

        void setupWindow(const char *title);
    };
}
