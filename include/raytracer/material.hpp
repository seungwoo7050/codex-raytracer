/*
 * 설명: 표면 재질을 정의하고 반사/굴절 산란을 계산한다.
 * 버전: v0.5.0
 * 관련 문서: design/renderer/v0.5.0-blur.md
 * 테스트: tests/unit/material_scatter_test.cpp
 */
#pragma once

#include <cmath>
#include <memory>
#include <random>

#include "raytracer/hittable.hpp"
#include "raytracer/random.hpp"
#include "raytracer/ray.hpp"
#include "raytracer/vec3.hpp"

namespace raytracer {

class Material {
public:
    virtual ~Material() = default;
    virtual bool Scatter(const Ray& r_in, const HitRecord& record, Color& attenuation, Ray& scattered,
                         std::mt19937& generator) const = 0;
};

class Lambertian : public Material {
public:
    explicit Lambertian(const Color& albedo) : albedo_(albedo) {}

    bool Scatter(const Ray& r_in, const HitRecord& record, Color& attenuation, Ray& scattered,
                 std::mt19937& generator) const override {
        Vec3 scatter_direction = record.normal + RandomUnitVector(generator);
        if (scatter_direction.NearZero()) {
            scatter_direction = record.normal;
        }

        scattered = Ray(record.p, scatter_direction, r_in.time());
        attenuation = albedo_;
        return true;
    }

private:
    Color albedo_;
};

class Metal : public Material {
public:
    Metal(const Color& albedo, double fuzz) : albedo_(albedo), fuzz_(fuzz < 1.0 ? fuzz : 1.0) {}

    bool Scatter(const Ray& r_in, const HitRecord& record, Color& attenuation, Ray& scattered,
                 std::mt19937& generator) const override {
        const Vec3 unit_direction = UnitVector(r_in.direction());
        const Vec3 reflected = Reflect(unit_direction, record.normal);
        const Vec3 scattered_direction = reflected + fuzz_ * RandomInUnitSphere(generator);
        scattered = Ray(record.p, scattered_direction, r_in.time());
        attenuation = albedo_;
        return Dot(scattered.direction(), record.normal) > 0;
    }

private:
    Color albedo_;
    double fuzz_;
};

class Dielectric : public Material {
public:
    explicit Dielectric(double refraction_index) : refraction_index_(refraction_index) {}

    bool Scatter(const Ray& r_in, const HitRecord& record, Color& attenuation, Ray& scattered,
                 std::mt19937& generator) const override {
        attenuation = Color(1.0, 1.0, 1.0);
        const double refraction_ratio = record.front_face ? (1.0 / refraction_index_) : refraction_index_;

        const Vec3 unit_direction = UnitVector(r_in.direction());
        const double cos_theta = std::fmin(Dot(-unit_direction, record.normal), 1.0);
        const double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

        const bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        Vec3 direction;
        if (cannot_refract || Reflectance(cos_theta, refraction_ratio) > RandomDouble(generator)) {
            direction = Reflect(unit_direction, record.normal);
        } else {
            direction = Refract(unit_direction, record.normal, refraction_ratio);
        }

        scattered = Ray(record.p, direction, r_in.time());
        return true;
    }

private:
    static double Reflectance(double cosine, double ref_idx) {
        const double r0 = (1.0 - ref_idx) / (1.0 + ref_idx);
        const double r0_squared = r0 * r0;
        return r0_squared + (1.0 - r0_squared) * std::pow(1.0 - cosine, 5.0);
    }

    double refraction_index_;
};

}  // namespace raytracer
