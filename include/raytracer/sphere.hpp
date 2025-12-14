/*
 * 설명: 단순 구 형상의 레이 교차를 계산한다.
 * 버전: v0.2.0
 * 관련 문서: design/renderer/v0.2.0-sphere-hit.md
 * 테스트: tests/unit/sphere_test.cpp
 */
#pragma once

#include "raytracer/hittable.hpp"

namespace raytracer {

class Sphere : public Hittable {
public:
    Sphere(const Point3& center, double radius) : center_(center), radius_(radius) {}

    bool Hit(const Ray& r, double t_min, double t_max, HitRecord& record) const override;

private:
    Point3 center_;
    double radius_;
};

}  // namespace raytracer
