/*
 * 설명: 원점과 방향, 시간을 포함하는 레이를 표현한다.
 * 버전: v0.5.0
 * 관련 문서: design/renderer/v0.5.0-blur.md
 * 테스트: tests/unit/sphere_test.cpp
 */
#pragma once

#include "raytracer/vec3.hpp"

namespace raytracer {

class Ray {
public:
    Ray() = default;
    Ray(const Point3& origin, const Vec3& direction, double time = 0.0) : orig(origin), dir(direction), tm(time) {}

    const Point3& origin() const { return orig; }
    const Vec3& direction() const { return dir; }
    double time() const { return tm; }

    Point3 At(double t) const { return orig + t * dir; }

private:
    Point3 orig;
    Vec3 dir;
    double tm = 0.0;
};

}  // namespace raytracer
