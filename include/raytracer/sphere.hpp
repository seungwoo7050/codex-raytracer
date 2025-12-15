/*
 * 설명: 고정 구와 시간에 따라 이동하는 구의 레이 교차 및 경계 상자를 계산한다.
 * 버전: v0.6.0
 * 관련 문서: design/renderer/v0.6.0-bvh.md
 * 테스트: tests/unit/sphere_test.cpp, tests/unit/bvh_test.cpp
 */
#pragma once

#include <memory>

#include "raytracer/aabb.hpp"
#include "raytracer/hittable.hpp"

namespace raytracer {

class Sphere : public Hittable {
public:
    Sphere(const Point3& center, double radius, std::shared_ptr<Material> material)
        : center_(center), radius_(radius), material_(std::move(material)) {}

    bool Hit(const Ray& r, double t_min, double t_max, HitRecord& record) const override;
    bool BoundingBox(double time0, double time1, Aabb& output_box) const override;

private:
    Point3 center_;
    double radius_;
    std::shared_ptr<Material> material_;
};

class MovingSphere : public Hittable {
public:
    MovingSphere(const Point3& center_start, const Point3& center_end, double time_start, double time_end, double radius,
                 std::shared_ptr<Material> material)
        : center_start_(center_start),
          center_end_(center_end),
          time_start_(time_start),
          time_end_(time_end),
          radius_(radius),
          material_(std::move(material)) {}

    bool Hit(const Ray& r, double t_min, double t_max, HitRecord& record) const override;
    bool BoundingBox(double time0, double time1, Aabb& output_box) const override;

private:
    Point3 Center(double time) const;

    Point3 center_start_;
    Point3 center_end_;
    double time_start_;
    double time_end_;
    double radius_;
    std::shared_ptr<Material> material_;
};

}  // namespace raytracer
