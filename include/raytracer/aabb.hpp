/*
 * 설명: 축 정렬 경계 상자(AABB)를 정의하고 레이와의 교차 여부를 판단한다.
 * 버전: v0.6.0
 * 관련 문서: design/renderer/v0.6.0-bvh.md
 * 테스트: tests/unit/bvh_test.cpp
 */
#pragma once

#include <algorithm>

#include "raytracer/ray.hpp"
#include "raytracer/vec3.hpp"

namespace raytracer {

class Aabb {
public:
    Aabb() = default;

    Aabb(const Point3& minimum, const Point3& maximum) : minimum_(minimum), maximum_(maximum) {}

    const Point3& minimum() const { return minimum_; }
    const Point3& maximum() const { return maximum_; }

    bool Hit(const Ray& r, double t_min, double t_max) const {
        for (int axis = 0; axis < 3; ++axis) {
            const double inv_dir = 1.0 / r.direction()[axis];
            double t0 = (minimum_[axis] - r.origin()[axis]) * inv_dir;
            double t1 = (maximum_[axis] - r.origin()[axis]) * inv_dir;
            if (inv_dir < 0.0) {
                std::swap(t0, t1);
            }

            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;
            if (t_max <= t_min) {
                return false;
            }
        }

        return true;
    }

private:
    Point3 minimum_;
    Point3 maximum_;
};

inline Aabb SurroundingBox(const Aabb& box0, const Aabb& box1) {
    const Point3 small(std::min(box0.minimum().x(), box1.minimum().x()),
                       std::min(box0.minimum().y(), box1.minimum().y()),
                       std::min(box0.minimum().z(), box1.minimum().z()));
    const Point3 big(std::max(box0.maximum().x(), box1.maximum().x()), std::max(box0.maximum().y(), box1.maximum().y()),
                     std::max(box0.maximum().z(), box1.maximum().z()));
    return Aabb(small, big);
}

}  // namespace raytracer
