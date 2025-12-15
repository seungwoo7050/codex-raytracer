/*
 * 설명: 단순 구 형상의 레이 교차를 계산한다.
 * 버전: v0.4.0
 * 관련 문서: design/renderer/v0.4.0-materials.md
 * 테스트: tests/unit/sphere_test.cpp
 */
#pragma once

#include <memory>

#include "raytracer/hittable.hpp"

namespace raytracer {

class Sphere : public Hittable {
public:
    Sphere(const Point3& center, double radius, std::shared_ptr<Material> material)
        : center_(center), radius_(radius), material_(std::move(material)) {}

    bool Hit(const Ray& r, double t_min, double t_max, HitRecord& record) const override;

private:
    Point3 center_;
    double radius_;
    std::shared_ptr<Material> material_;
};

}  // namespace raytracer
