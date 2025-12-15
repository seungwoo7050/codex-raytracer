/*
 * 설명: Hittable 객체에 평행 이동과 Y축 회전을 적용하는 변환 래퍼를 제공한다.
 * 버전: v0.9.0
 * 관련 문서: design/renderer/v0.8.0-cornell.md, design/renderer/v0.9.0-volume.md
 * 테스트: tests/unit/quad_test.cpp
 */
#pragma once

#include <memory>

#include "raytracer/aabb.hpp"
#include "raytracer/hittable.hpp"
#include "raytracer/vec3.hpp"

namespace raytracer {

class Translate : public Hittable {
public:
    Translate(std::shared_ptr<Hittable> object, const Vec3& offset);

    bool Hit(const Ray& r, double t_min, double t_max, HitRecord& record, std::mt19937& generator) const override;
    bool BoundingBox(double time0, double time1, Aabb& output_box) const override;

private:
    std::shared_ptr<Hittable> object_;
    Vec3 offset_;
};

class RotateY : public Hittable {
public:
    RotateY(std::shared_ptr<Hittable> object, double angle_degrees);

    bool Hit(const Ray& r, double t_min, double t_max, HitRecord& record, std::mt19937& generator) const override;
    bool BoundingBox(double time0, double time1, Aabb& output_box) const override;

private:
    std::shared_ptr<Hittable> object_;
    double sin_theta_ = 0.0;
    double cos_theta_ = 1.0;
    bool has_box_ = false;
    Aabb bbox_;
};

}  // namespace raytracer
