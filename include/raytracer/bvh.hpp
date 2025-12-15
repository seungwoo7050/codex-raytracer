/*
 * 설명: Hittable 트리로 구성된 BVH 노드를 정의하고 경계 상자 기반 가속 hit 함수를 제공한다.
 * 버전: v0.6.0
 * 관련 문서: design/renderer/v0.6.0-bvh.md
 * 테스트: tests/unit/bvh_test.cpp
 */
#pragma once

#include <memory>
#include <vector>

#include "raytracer/aabb.hpp"
#include "raytracer/hittable.hpp"

namespace raytracer {

class HittableList;

class BvhNode : public Hittable {
public:
    BvhNode() = default;
    BvhNode(std::vector<std::shared_ptr<Hittable>> objects, double time0, double time1);
    BvhNode(const HittableList& list, double time0, double time1);

    bool Hit(const Ray& r, double t_min, double t_max, HitRecord& record) const override;
    bool BoundingBox(double time0, double time1, Aabb& output_box) const override;

private:
    BvhNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end, double time0, double time1);

    void Build(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end, double time0, double time1);
    static std::vector<std::shared_ptr<Hittable>> CopyObjects(const std::vector<std::shared_ptr<Hittable>>& source);
    static bool BoxComparator(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b, int axis,
                              double time0, double time1);
    static int ChooseSplitAxis(const std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end,
                               double time0, double time1);

    std::shared_ptr<Hittable> left_;
    std::shared_ptr<Hittable> right_;
    Aabb box_;
};

}  // namespace raytracer
