/*
 * 설명: 여러 개의 물체를 순회하며 RNG를 전달해 가장 가까운 교차를 찾고 경계 상자를 제공하는 리스트 래퍼를 제공한다.
 * 버전: v0.9.0
 * 관련 문서: design/renderer/v0.6.0-bvh.md, design/renderer/v0.9.0-volume.md
 * 테스트: tests/unit/sphere_test.cpp, tests/unit/bvh_test.cpp
 */
#pragma once

#include <memory>
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

private:
    std::vector<std::shared_ptr<Hittable>> objects_;
};

}  // namespace raytracer
