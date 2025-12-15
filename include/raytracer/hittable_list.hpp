/*
 * 설명: 여러 개의 물체를 순회하며 가장 가까운 교차를 찾는 리스트 래퍼를 제공한다.
 * 버전: v0.4.0
 * 관련 문서: design/renderer/v0.4.0-materials.md
 * 테스트: tests/unit/sphere_test.cpp
 */
#pragma once

#include <memory>
#include <vector>

#include "raytracer/hittable.hpp"

namespace raytracer {

class HittableList : public Hittable {
public:
    HittableList() = default;

    explicit HittableList(std::shared_ptr<Hittable> object) { Add(std::move(object)); }

    void Clear() { objects_.clear(); }

    void Add(std::shared_ptr<Hittable> object) { objects_.push_back(std::move(object)); }

    bool Hit(const Ray& r, double t_min, double t_max, HitRecord& record) const override {
        HitRecord temp_record;
        bool hit_anything = false;
        double closest_so_far = t_max;

        for (const auto& object : objects_) {
            if (object->Hit(r, t_min, closest_so_far, temp_record)) {
                hit_anything = true;
                closest_so_far = temp_record.t;
                record = temp_record;
            }
        }

        return hit_anything;
    }

private:
    std::vector<std::shared_ptr<Hittable>> objects_;
};

}  // namespace raytracer
