#include <iostream>
#include <jtxlib/math/mat4.hpp>
#include <jtxlib/math/transform.hpp>
#include "renderer.hpp"
#include "model.hpp"

std::string toString(const jtx::Vec3f &v) {
    return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
}

int main() {
    auto m4lookAt = jtx::lookAt(jtx::Vec3f)

    return 0;
}
