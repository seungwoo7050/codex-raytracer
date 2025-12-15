/*
 * 설명: 레이와 물체의 교차 정보를 표현하고 추상 인터페이스를 정의한다.
 * 버전: v0.4.0
 * 관련 문서: design/renderer/v0.4.0-materials.md
 * 테스트: tests/unit/sphere_test.cpp
 */
#pragma once

#include <memory>

#include "raytracer/ray.hpp"

namespace raytracer {

class Material;

struct HitRecord {
    Point3 p;
    Vec3 normal;
    double t = 0.0;
    bool front_face = true;
    std::shared_ptr<Material> material;

    void SetFaceNormal(const Ray& r, const Vec3& outward_normal) {
        front_face = Dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual ~Hittable() = default;
    virtual bool Hit(const Ray& r, double t_min, double t_max, HitRecord& record) const = 0;
};

}  // namespace raytracer
