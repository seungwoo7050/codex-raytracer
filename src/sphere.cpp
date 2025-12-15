/*
 * 설명: 고정 구와 이동 구의 레이 교차, 경계 상자, 샘플링 PDF를 계산한다.
 * 버전: v1.0.0
 * 관련 문서: design/renderer/v1.0.0-overview.md
 * 테스트: tests/unit/sphere_test.cpp, tests/unit/bvh_test.cpp, tests/unit/pdf_test.cpp
 */
#include "raytracer/sphere.hpp"

#include <cmath>
#include <limits>

#include "raytracer/onb.hpp"
#include "raytracer/random.hpp"

namespace raytracer {

namespace {

void GetSphereUv(const Point3& p, double& u, double& v) {
    const double theta = std::acos(-p.y());
    const double phi = std::atan2(-p.z(), p.x()) + std::acos(-1.0);
    const double pi = std::acos(-1.0);
    u = phi / (2.0 * pi);
    v = theta / pi;
}

}  // namespace

bool Sphere::Hit(const Ray& r, double t_min, double t_max, HitRecord& record, std::mt19937& /*generator*/) const {
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
    GetSphereUv(outward_normal, record.u, record.v);
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

bool MovingSphere::Hit(const Ray& r, double t_min, double t_max, HitRecord& record, std::mt19937& /*generator*/) const {
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
    GetSphereUv(outward_normal, record.u, record.v);
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

double Sphere::PdfValue(const Point3& origin, const Vec3& direction) const {
    HitRecord record;
    std::mt19937 dummy_generator(0);
    if (!Hit(Ray(origin, direction), 0.001, std::numeric_limits<double>::infinity(), record, dummy_generator)) {
        return 0.0;
    }

    const double distance_squared = (center_ - origin).length_squared();
    const double cos_theta_max = std::sqrt(1.0 - radius_ * radius_ / distance_squared);
    const double solid_angle = 2.0 * std::acos(-1.0) * (1.0 - cos_theta_max);
    return 1.0 / solid_angle;
}

Vec3 Sphere::Random(const Point3& origin, std::mt19937& generator) const {
    const Vec3 direction = center_ - origin;
    Onb onb;
    onb.BuildFromW(direction);
    return onb.Local(RandomToSphere(radius_, direction.length_squared(), generator));
}

}  // namespace raytracer
