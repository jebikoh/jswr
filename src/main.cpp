#include <iostream>
#include "jtxlib/math.hpp"
#include "renderer.hpp"

int main() {
    jswr::Renderer renderer(800, 600, "JSWR");

    while (!renderer.shouldClose()) {
        renderer.render();
    }

    return 0;
}
