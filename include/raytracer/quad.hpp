/*
 * 설명: Quad와 Box 기하를 정의하고 경계 상자, UV, 샘플링 PDF 정보를 계산한다.
 * 버전: v1.0.0
 * 관련 문서: design/renderer/v1.0.0-overview.md
 * 테스트: tests/unit/quad_test.cpp, tests/unit/pdf_test.cpp
 */
#pragma once

#include <memory>
#include <random>

#include "raytracer/aabb.hpp"
#include "raytracer/hittable.hpp"
#include "raytracer/hittable_list.hpp"
#include "raytracer/material.hpp"
#include "raytracer/vec3.hpp"

namespace raytracer {

class Quad : public Hittable {
public:
    Quad(const Point3& q, const Vec3& u, const Vec3& v, std::shared_ptr<Material> material);

    bool Hit(const Ray& r, double t_min, double t_max, HitRecord& record, std::mt19937& generator) const override;
    bool BoundingBox(double time0, double time1, Aabb& output_box) const override;
    double PdfValue(const Point3& origin, const Vec3& direction) const override;
    Vec3 Random(const Point3& origin, std::mt19937& generator) const override;

private:
    Point3 q_;
    Vec3 u_;
    Vec3 v_;
    Vec3 normal_;
    double d_ = 0.0;
    double area_ = 0.0;
    std::shared_ptr<Material> material_;
    Aabb bbox_;

    bool IsInside(double alpha, double beta) const;
    void SetBoundingBox();
};

class Box : public Hittable {
public:
    Box(const Point3& min_point, const Point3& max_point, std::shared_ptr<Material> material);

    bool Hit(const Ray& r, double t_min, double t_max, HitRecord& record, std::mt19937& generator) const override;
    bool BoundingBox(double time0, double time1, Aabb& output_box) const override;

private:
    Point3 min_;
    Point3 max_;
    HittableList sides_;
};

}  // namespace raytracer
