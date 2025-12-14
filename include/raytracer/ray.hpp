/*
 * 설명: 원점과 방향으로 정의되는 레이를 표현한다.
 * 버전: v0.2.0
 * 관련 문서: design/renderer/v0.2.0-sphere-hit.md
 * 테스트: tests/unit/sphere_test.cpp
 */
#pragma once

#include "raytracer/vec3.hpp"

namespace raytracer {

class Ray {
public:
    Ray() = default;
    Ray(const Point3& origin, const Vec3& direction) : orig(origin), dir(direction) {}

    const Point3& origin() const { return orig; }
    const Vec3& direction() const { return dir; }

    Point3 At(double t) const { return orig + t * dir; }

private:
    Point3 orig;
    Vec3 dir;
};

}  // namespace raytracer
