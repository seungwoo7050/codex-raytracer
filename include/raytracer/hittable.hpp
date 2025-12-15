/*
 * 설명: 레이와 물체의 교차 정보를 표현하고 RNG를 전달하는 추상 인터페이스를 정의한다.
 * 버전: v0.9.0
 * 관련 문서: design/renderer/v0.6.0-bvh.md, design/renderer/v0.7.0-textures.md, design/renderer/v0.9.0-volume.md
 * 테스트: tests/unit/sphere_test.cpp, tests/unit/bvh_test.cpp
 */
#pragma once

#include <memory>
#include <random>

#include "raytracer/aabb.hpp"
#include "raytracer/ray.hpp"

namespace raytracer {

class Material;

struct HitRecord {
    Point3 p;
    Vec3 normal;
    double t = 0.0;
    bool front_face = true;
    double u = 0.0;
    double v = 0.0;
    std::shared_ptr<Material> material;

    void SetFaceNormal(const Ray& r, const Vec3& outward_normal) {
        front_face = Dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual ~Hittable() = default;
    virtual bool Hit(const Ray& r, double t_min, double t_max, HitRecord& record, std::mt19937& generator) const = 0;
    virtual bool BoundingBox(double time0, double time1, Aabb& output_box) const = 0;
};

}  // namespace raytracer
