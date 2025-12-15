/*
 * 설명: Hittable 객체에 평행 이동과 Y축 회전을 적용해 교차와 경계를 변환한다.
 * 버전: v0.8.0
 * 관련 문서: design/renderer/v0.8.0-cornell.md
 * 테스트: tests/unit/quad_test.cpp
 */
#include "raytracer/transform.hpp"

#include <cmath>
#include <limits>

namespace raytracer {

Translate::Translate(std::shared_ptr<Hittable> object, const Vec3& offset) : object_(std::move(object)), offset_(offset) {}

bool Translate::Hit(const Ray& r, double t_min, double t_max, HitRecord& record) const {
    Ray moved_ray(r.origin() - offset_, r.direction(), r.time());
    if (!object_->Hit(moved_ray, t_min, t_max, record)) {
        return false;
    }

    record.p += offset_;
    record.SetFaceNormal(moved_ray, record.normal);
    return true;
}

bool Translate::BoundingBox(double time0, double time1, Aabb& output_box) const {
    if (!object_->BoundingBox(time0, time1, output_box)) {
        return false;
    }

    output_box = Aabb(output_box.minimum() + offset_, output_box.maximum() + offset_);
    return true;
}

RotateY::RotateY(std::shared_ptr<Hittable> object, double angle_degrees) : object_(std::move(object)) {
    const double radians = angle_degrees * 3.1415926535897932385 / 180.0;
    sin_theta_ = std::sin(radians);
    cos_theta_ = std::cos(radians);

    has_box_ = object_->BoundingBox(0.0, 0.0, bbox_);
    if (!has_box_) {
        return;
    }

    Point3 min_point(std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(),
                     std::numeric_limits<double>::infinity());
    Point3 max_point(-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity(),
                     -std::numeric_limits<double>::infinity());

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                const double x = i * bbox_.maximum().x() + (1 - i) * bbox_.minimum().x();
                const double y = j * bbox_.maximum().y() + (1 - j) * bbox_.minimum().y();
                const double z = k * bbox_.maximum().z() + (1 - k) * bbox_.minimum().z();

                const double rotated_x = cos_theta_ * x + sin_theta_ * z;
                const double rotated_z = -sin_theta_ * x + cos_theta_ * z;

                const Vec3 tester(rotated_x, y, rotated_z);
                min_point = Point3(std::fmin(min_point.x(), tester.x()), std::fmin(min_point.y(), tester.y()),
                                   std::fmin(min_point.z(), tester.z()));
                max_point = Point3(std::fmax(max_point.x(), tester.x()), std::fmax(max_point.y(), tester.y()),
                                   std::fmax(max_point.z(), tester.z()));
            }
        }
    }

    bbox_ = Aabb(min_point, max_point);
}

bool RotateY::Hit(const Ray& r, double t_min, double t_max, HitRecord& record) const {
    const double orig_x = cos_theta_ * r.origin().x() - sin_theta_ * r.origin().z();
    const double orig_z = sin_theta_ * r.origin().x() + cos_theta_ * r.origin().z();
    const Point3 origin(orig_x, r.origin().y(), orig_z);

    const double dir_x = cos_theta_ * r.direction().x() - sin_theta_ * r.direction().z();
    const double dir_z = sin_theta_ * r.direction().x() + cos_theta_ * r.direction().z();
    const Vec3 direction(dir_x, r.direction().y(), dir_z);

    const Ray rotated_ray(origin, direction, r.time());

    if (!object_->Hit(rotated_ray, t_min, t_max, record)) {
        return false;
    }

    const double hit_x = cos_theta_ * record.p.x() + sin_theta_ * record.p.z();
    const double hit_z = -sin_theta_ * record.p.x() + cos_theta_ * record.p.z();

    const double normal_x = cos_theta_ * record.normal.x() + sin_theta_ * record.normal.z();
    const double normal_z = -sin_theta_ * record.normal.x() + cos_theta_ * record.normal.z();

    record.p = Point3(hit_x, record.p.y(), hit_z);
    const Vec3 world_normal(normal_x, record.normal.y(), normal_z);
    record.SetFaceNormal(r, world_normal);

    return true;
}

bool RotateY::BoundingBox(double /*time0*/, double /*time1*/, Aabb& output_box) const {
    if (!has_box_) {
        return false;
    }
    output_box = bbox_;
    return true;
}

}  // namespace raytracer
