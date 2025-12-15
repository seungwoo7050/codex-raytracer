/*
 * 설명: defocus blur와 셔터 시간을 포함한 카메라에서 레이를 생성한다.
 * 버전: v0.5.0
 * 관련 문서: design/renderer/v0.5.0-blur.md
 * 테스트: tests/integration/ppm_integration_test.cpp
 */
#pragma once

#include <cmath>
#include <random>

#include "raytracer/random.hpp"
#include "raytracer/ray.hpp"

namespace raytracer {

inline double DegreesToRadians(double degrees) {
    constexpr double kPi = 3.1415926535897932385;
    return degrees * kPi / 180.0;
}

class Camera {
public:
    Camera(const Point3& look_from, const Point3& look_at, const Vec3& vup, double vertical_fov_degrees,
           double aspect_ratio, double aperture, double focus_dist, double time_open, double time_close)
        : origin_(look_from), lens_radius_(aperture / 2.0), time0_(time_open), time1_(time_close) {
        const double theta = DegreesToRadians(vertical_fov_degrees);
        const double h = std::tan(theta / 2.0);
        const double viewport_height = 2.0 * h;
        const double viewport_width = aspect_ratio * viewport_height;

        w_ = UnitVector(look_from - look_at);
        u_ = UnitVector(Cross(vup, w_));
        v_ = Cross(w_, u_);

        horizontal_ = focus_dist * viewport_width * u_;
        vertical_ = focus_dist * viewport_height * v_;
        lower_left_corner_ = origin_ - horizontal_ / 2.0 - vertical_ / 2.0 - focus_dist * w_;
    }

    Ray GetRay(double s, double t, std::mt19937& generator) const {
        const Vec3 rd = lens_radius_ * RandomInUnitDisk(generator);
        const Vec3 offset = u_ * rd.x() + v_ * rd.y();
        const double time = RandomDouble(generator, time0_, time1_);
        return Ray(origin_ + offset, lower_left_corner_ + s * horizontal_ + t * vertical_ - origin_ - offset, time);
    }

private:
    Point3 origin_;
    Vec3 u_;
    Vec3 v_;
    Vec3 w_;
    double lens_radius_ = 0.0;
    Vec3 horizontal_;
    Vec3 vertical_;
    Point3 lower_left_corner_;
    double time0_ = 0.0;
    double time1_ = 0.0;
};

}  // namespace raytracer
