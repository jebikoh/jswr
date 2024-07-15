#include <iostream>
#include "jtxlib/math.hpp"
#include "renderer.hpp"
#include "model.hpp"

std::string toString(const jtx::Vec3f &v) {
    return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
}

int main() {
    jswr::Model model;
    jswr::loadModel("../assets/cube/cube.obj", model);

    std::cout << "Model has " << model.meshes.size() << " meshes" << std::endl;

    return 0;
}
