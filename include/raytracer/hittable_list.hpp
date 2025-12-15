/*
 * 설명: 여러 개의 물체를 순회하며 RNG를 전달해 가장 가까운 교차를 찾고 PDF 샘플링에 필요한 정보를 제공한다.
 * 버전: v1.0.0
 * 관련 문서: design/renderer/v1.0.0-overview.md
 * 테스트: tests/unit/sphere_test.cpp, tests/unit/bvh_test.cpp, tests/unit/pdf_test.cpp
 */
#pragma once

#include <memory>
#include <random>
#include <vector>

#include "raytracer/aabb.hpp"
#include "raytracer/hittable.hpp"

namespace raytracer {

class HittableList : public Hittable {
public:
    HittableList() = default;

    explicit HittableList(std::shared_ptr<Hittable> object) { Add(std::move(object)); }

    void Clear() { objects_.clear(); }

    void Add(std::shared_ptr<Hittable> object) { objects_.push_back(std::move(object)); }

    bool Hit(const Ray& r, double t_min, double t_max, HitRecord& record, std::mt19937& generator) const override {
        HitRecord temp_record;
        bool hit_anything = false;
        double closest_so_far = t_max;

        for (const auto& object : objects_) {
            if (object->Hit(r, t_min, closest_so_far, temp_record, generator)) {
                hit_anything = true;
                closest_so_far = temp_record.t;
                record = temp_record;
            }
        }

        return hit_anything;
    }

    bool BoundingBox(double time0, double time1, Aabb& output_box) const override {
        if (objects_.empty()) {
            return false;
        }

        Aabb temp_box;
        bool first_box = true;

        for (const auto& object : objects_) {
            if (!object->BoundingBox(time0, time1, temp_box)) {
                return false;
            }
            output_box = first_box ? temp_box : SurroundingBox(output_box, temp_box);
            first_box = false;
        }

        return true;
    }

    const std::vector<std::shared_ptr<Hittable>>& Objects() const { return objects_; }

    double PdfValue(const Point3& origin, const Vec3& direction) const override {
        if (objects_.empty()) {
            return 0.0;
        }

        double sum = 0.0;
        for (const auto& object : objects_) {
            sum += object->PdfValue(origin, direction);
        }

        return sum / static_cast<double>(objects_.size());
    }

    Vec3 Random(const Point3& origin, std::mt19937& generator) const override {
        if (objects_.empty()) {
            return Vec3(1.0, 0.0, 0.0);
        }

        std::uniform_int_distribution<std::size_t> distribution(0, objects_.size() - 1);
        return objects_[distribution(generator)]->Random(origin, generator);
    }

private:
    std::vector<std::shared_ptr<Hittable>> objects_;
};

}  // namespace raytracer
