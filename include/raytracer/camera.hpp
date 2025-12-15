/*
 * 설명: 시야각과 종횡비를 기반으로 레이를 생성하는 핀홀 카메라를 제공한다.
 * 버전: v0.3.0
 * 관련 문서: design/renderer/v0.3.0-camera-sampling.md
 * 테스트: tests/integration/ppm_integration_test.cpp
 */
#pragma once

#include <cmath>

#include "raytracer/ray.hpp"

namespace raytracer {

inline double DegreesToRadians(double degrees) {
    constexpr double kPi = 3.1415926535897932385;
    return degrees * kPi / 180.0;
}

class Camera {
public:
    Camera(double vertical_fov_degrees, double aspect_ratio) : origin_(0.0, 0.0, 0.0) {
        const double theta = DegreesToRadians(vertical_fov_degrees);
        const double h = std::tan(theta / 2.0);
        const double viewport_height = 2.0 * h;
        const double viewport_width = aspect_ratio * viewport_height;
        const double focal_length = 1.0;

        horizontal_ = Vec3(viewport_width, 0.0, 0.0);
        vertical_ = Vec3(0.0, viewport_height, 0.0);
        lower_left_corner_ = origin_ - horizontal_ / 2.0 - vertical_ / 2.0 - Vec3(0.0, 0.0, focal_length);
    }

    Ray GetRay(double u, double v) const {
        return Ray(origin_, lower_left_corner_ + u * horizontal_ + v * vertical_ - origin_);
    }

private:
    Point3 origin_;
    Vec3 horizontal_;
    Vec3 vertical_;
    Point3 lower_left_corner_;
};

}  // namespace raytracer
