/*
 * 설명: Hittable들을 BVH로 구성해 경계 상자를 이용한 빠른 hit 판정을 수행한다.
 * 버전: v0.6.0
 * 관련 문서: design/renderer/v0.6.0-bvh.md
 * 테스트: tests/unit/bvh_test.cpp
 */
#include "raytracer/bvh.hpp"

#include <algorithm>
#include <memory>
#include <stdexcept>

#include "raytracer/hittable_list.hpp"

namespace raytracer {

BvhNode::BvhNode(std::vector<std::shared_ptr<Hittable>> objects, double time0, double time1) {
    if (objects.empty()) {
        throw std::invalid_argument("BVH에 빈 객체 목록이 전달되었다.");
    }
    Build(objects, 0, objects.size(), time0, time1);
}

BvhNode::BvhNode(const HittableList& list, double time0, double time1) : BvhNode(CopyObjects(list.Objects()), time0, time1) {}

BvhNode::BvhNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end, double time0, double time1) {
    Build(objects, start, end, time0, time1);
}

void BvhNode::Build(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end, double time0, double time1) {
    const int axis = ChooseSplitAxis(objects, start, end, time0, time1);
    auto comparator = [axis, time0, time1](const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b) {
        return BoxComparator(a, b, axis, time0, time1);
    };

    const size_t object_span = end - start;

    if (object_span == 1) {
        left_ = right_ = objects[start];
    } else if (object_span == 2) {
        if (comparator(objects[start], objects[start + 1])) {
            left_ = objects[start];
            right_ = objects[start + 1];
        } else {
            left_ = objects[start + 1];
            right_ = objects[start];
        }
    } else {
        std::sort(objects.begin() + static_cast<std::ptrdiff_t>(start), objects.begin() + static_cast<std::ptrdiff_t>(end),
                  comparator);

        const size_t mid = start + object_span / 2;
        left_ = std::shared_ptr<BvhNode>(new BvhNode(objects, start, mid, time0, time1));
        right_ = std::shared_ptr<BvhNode>(new BvhNode(objects, mid, end, time0, time1));
    }

    Aabb box_left;
    Aabb box_right;

    if (!left_->BoundingBox(time0, time1, box_left) || !right_->BoundingBox(time0, time1, box_right)) {
        throw std::runtime_error("BVH 노드 생성 중 경계 상자를 계산할 수 없다.");
    }

    box_ = SurroundingBox(box_left, box_right);
}

std::vector<std::shared_ptr<Hittable>> BvhNode::CopyObjects(const std::vector<std::shared_ptr<Hittable>>& source) {
    return std::vector<std::shared_ptr<Hittable>>(source.begin(), source.end());
}

bool BvhNode::BoxComparator(const std::shared_ptr<Hittable>& a, const std::shared_ptr<Hittable>& b, int axis, double time0,
                            double time1) {
    Aabb box_a;
    Aabb box_b;

    if (!a->BoundingBox(time0, time1, box_a) || !b->BoundingBox(time0, time1, box_b)) {
        throw std::runtime_error("BVH 비교 중 경계 상자를 계산할 수 없다.");
    }

    return box_a.minimum()[axis] < box_b.minimum()[axis];
}

int BvhNode::ChooseSplitAxis(const std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end, double time0,
                             double time1) {
    if (end <= start) {
        throw std::invalid_argument("BVH 분할 축 계산에 잘못된 인덱스가 전달되었다.");
    }

    Aabb aggregate_box;
    bool first_box = true;

    for (size_t i = start; i < end; ++i) {
        Aabb object_box;
        if (!objects[i]->BoundingBox(time0, time1, object_box)) {
            throw std::runtime_error("BVH 축 선택 중 경계 상자를 계산할 수 없다.");
        }
        aggregate_box = first_box ? object_box : SurroundingBox(aggregate_box, object_box);
        first_box = false;
    }

    const Vec3 diag = aggregate_box.maximum() - aggregate_box.minimum();
    if (diag.x() >= diag.y() && diag.x() >= diag.z()) {
        return 0;
    }
    if (diag.y() >= diag.z()) {
        return 1;
    }
    return 2;
}

bool BvhNode::Hit(const Ray& r, double t_min, double t_max, HitRecord& record) const {
    if (!box_.Hit(r, t_min, t_max)) {
        return false;
    }

    HitRecord left_record;
    HitRecord right_record;

    const bool hit_left = left_->Hit(r, t_min, t_max, left_record);
    const bool hit_right = right_->Hit(r, t_min, hit_left ? left_record.t : t_max, right_record);

    if (hit_left && hit_right) {
        record = right_record.t < left_record.t ? right_record : left_record;
        return true;
    }
    if (hit_left) {
        record = left_record;
        return true;
    }
    if (hit_right) {
        record = right_record;
        return true;
    }

    return false;
}

bool BvhNode::BoundingBox(double /*time0*/, double /*time1*/, Aabb& output_box) const {
    output_box = box_;
    return true;
}

}  // namespace raytracer
