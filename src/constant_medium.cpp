/*
 * 설명: 경계 Hittable 내부에서 지수 분포로 산란 거리를 샘플링하는 균일 매질을 구현한다.
 * 버전: v0.9.0
 * 관련 문서: design/renderer/v0.9.0-volume.md
 * 테스트: tests/integration/ppm_integration_test.cpp
 */
#include "raytracer/constant_medium.hpp"

#include <algorithm>
#include <cmath>
#include <limits>

#include "raytracer/random.hpp"

namespace raytracer {

ConstantMedium::ConstantMedium(std::shared_ptr<Hittable> boundary, double density, std::shared_ptr<Texture> texture)
    : boundary_(std::move(boundary)), neg_inv_density_(-1.0 / density),
      phase_function_(std::make_shared<Isotropic>(std::move(texture))) {}

ConstantMedium::ConstantMedium(std::shared_ptr<Hittable> boundary, double density, const Color& albedo)
    : ConstantMedium(std::move(boundary), density, std::make_shared<SolidColor>(albedo)) {}

bool ConstantMedium::Hit(const Ray& r, double t_min, double t_max, HitRecord& record, std::mt19937& generator) const {
    HitRecord rec1;
    HitRecord rec2;

    if (!boundary_->Hit(r, -std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity(), rec1,
                        generator)) {
        return false;
    }

    if (!boundary_->Hit(r, rec1.t + 0.0001, std::numeric_limits<double>::infinity(), rec2, generator)) {
        return false;
    }

    if (rec1.t < t_min) {
        rec1.t = t_min;
    }
    if (rec2.t > t_max) {
        rec2.t = t_max;
    }

    if (rec1.t >= rec2.t) {
        return false;
    }

    if (rec1.t < 0.0) {
        rec1.t = 0.0;
    }

    const double ray_length = r.direction().length();
    const double distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    const double random_value = std::max(RandomDouble(generator), 1e-12);
    const double hit_distance = neg_inv_density_ * std::log(random_value);

    if (hit_distance > distance_inside_boundary) {
        return false;
    }

    record.t = rec1.t + hit_distance / ray_length;
    record.p = r.At(record.t);

    record.normal = Vec3(1.0, 0.0, 0.0);
    record.front_face = true;
    record.material = phase_function_;

    return true;
}

bool ConstantMedium::BoundingBox(double time0, double time1, Aabb& output_box) const {
    return boundary_->BoundingBox(time0, time1, output_box);
}

}  // namespace raytracer
