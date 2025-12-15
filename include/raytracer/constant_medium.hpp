/*
 * 설명: 경계 Hittable 내부에 균일 밀도 매질을 정의해 산란 거리를 샘플링한다.
 * 버전: v0.9.0
 * 관련 문서: design/renderer/v0.9.0-volume.md
 * 테스트: tests/integration/ppm_integration_test.cpp
 */
#pragma once

#include <memory>
#include <random>

#include "raytracer/hittable.hpp"
#include "raytracer/material.hpp"
#include "raytracer/texture.hpp"

namespace raytracer {

class ConstantMedium : public Hittable {
public:
    ConstantMedium(std::shared_ptr<Hittable> boundary, double density, std::shared_ptr<Texture> texture);
    ConstantMedium(std::shared_ptr<Hittable> boundary, double density, const Color& albedo);

    bool Hit(const Ray& r, double t_min, double t_max, HitRecord& record, std::mt19937& generator) const override;
    bool BoundingBox(double time0, double time1, Aabb& output_box) const override;

private:
    std::shared_ptr<Hittable> boundary_;
    double neg_inv_density_;
    std::shared_ptr<Material> phase_function_;
};

}  // namespace raytracer
