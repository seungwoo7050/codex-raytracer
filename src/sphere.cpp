/*
 * 설명: 고정 구와 이동 구의 레이 교차 및 경계 상자를 계산한다.
 * 버전: v0.6.0
 * 관련 문서: design/renderer/v0.6.0-bvh.md
 * 테스트: tests/unit/sphere_test.cpp, tests/unit/bvh_test.cpp
 */
#include "raytracer/sphere.hpp"

#include <cmath>

namespace raytracer {

bool Sphere::Hit(const Ray& r, double t_min, double t_max, HitRecord& record) const {
    const Vec3 oc = r.origin() - center_;
    const double a = r.direction().length_squared();
    const double half_b = Dot(oc, r.direction());
    const double c = oc.length_squared() - radius_ * radius_;

    const double discriminant = half_b * half_b - a * c;
    if (discriminant < 0) {
        return false;
    }

    const double sqrt_d = std::sqrt(discriminant);

    double root = (-half_b - sqrt_d) / a;
    if (root < t_min || root > t_max) {
        root = (-half_b + sqrt_d) / a;
        if (root < t_min || root > t_max) {
            return false;
        }
    }

    record.t = root;
    record.p = r.At(record.t);
    const Vec3 outward_normal = (record.p - center_) / radius_;
    record.SetFaceNormal(r, outward_normal);
    record.material = material_;

    return true;
}

bool Sphere::BoundingBox(double /*time0*/, double /*time1*/, Aabb& output_box) const {
    const Vec3 radius_vec(radius_, radius_, radius_);
    output_box = Aabb(center_ - radius_vec, center_ + radius_vec);
    return true;
}

Point3 MovingSphere::Center(double time) const {
    const double time_span = time_end_ - time_start_;
    if (time_span == 0.0) {
        return center_start_;
    }

    const double time_ratio = (time - time_start_) / time_span;
    return center_start_ + time_ratio * (center_end_ - center_start_);
}

bool MovingSphere::Hit(const Ray& r, double t_min, double t_max, HitRecord& record) const {
    const Point3 center = Center(r.time());
    const Vec3 oc = r.origin() - center;
    const double a = r.direction().length_squared();
    const double half_b = Dot(oc, r.direction());
    const double c = oc.length_squared() - radius_ * radius_;

    const double discriminant = half_b * half_b - a * c;
    if (discriminant < 0) {
        return false;
    }

    const double sqrt_d = std::sqrt(discriminant);

    double root = (-half_b - sqrt_d) / a;
    if (root < t_min || root > t_max) {
        root = (-half_b + sqrt_d) / a;
        if (root < t_min || root > t_max) {
            return false;
        }
    }

    record.t = root;
    record.p = r.At(record.t);
    const Vec3 outward_normal = (record.p - center) / radius_;
    record.SetFaceNormal(r, outward_normal);
    record.material = material_;

    return true;
}

bool MovingSphere::BoundingBox(double time0, double time1, Aabb& output_box) const {
    const Vec3 radius_vec(radius_, radius_, radius_);
    const Aabb box0(Center(time0) - radius_vec, Center(time0) + radius_vec);
    const Aabb box1(Center(time1) - radius_vec, Center(time1) + radius_vec);
    output_box = SurroundingBox(box0, box1);
    return true;
}

}  // namespace raytracer
