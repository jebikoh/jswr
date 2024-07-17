#pragma once

#include <jtxlib/math/mat4.hpp>
#include <jtxlib/math/vec3.hpp>
#include <jtxlib/math/vecmath.hpp>

namespace jswr {
    class Camera {
    public:
        jtx::Vec3f position;
        jtx::Vec3f direction;
        jtx::Vec3f up;

        jtx::Vec3f right;
        jtx::Vec3f vup;

        Camera() : position(), direction(), up(), right(){};
        Camera(const jtx::Vec3f &position, const jtx::Vec3f &target, const jtx::Vec3f &up) {
            this->position = position;
            this->direction = (target - position).normalize();
            this->up = jtx::normalize(up);

            this->update();
        }
        ~Camera() = default;

        [[nodiscard]] jtx::Mat4 getViewMatrix() const {
            return jtx::lookAt(right, vup, direction, position);
        }

        void update() {
            this->right = jtx::cross(this->direction, this->up).normalize();
            this->vup = jtx::cross(this->right, this->direction).normalize();
        }
    };
}