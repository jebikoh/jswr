#include <iostream>
#include "jtxlib/math.hpp"

int main() {
    jtx::Vec4f v1(1.0f, 2.0f, 3.0f, 4.0f);
    jtx::Vec4f v2(5.0f, 6.0f, 7.0f, 8.0f);
    jtx::Vec4f v3 = v1 + v2;

    std::cout << "v1: " << v1.x << ", " << v1.y << ", " << v1.z << ", " << v1.w << std::endl;
    return 0;
}
