/*
 * 설명: 구와 레이의 교차를 계산한다.
 * 버전: v0.4.0
 * 관련 문서: design/renderer/v0.4.0-materials.md
 * 테스트: tests/unit/sphere_test.cpp
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

}  // namespace raytracer
