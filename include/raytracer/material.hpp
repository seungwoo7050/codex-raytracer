/*
 * 설명: 표면 재질과 볼륨 위상 함수를 정의하고 텍스처 기반 반사/굴절/발광/PDF 샘플링 동작을 계산한다.
 * 버전: v1.0.0
 * 관련 문서: design/renderer/v1.0.0-overview.md
 * 테스트: tests/unit/material_scatter_test.cpp, tests/unit/texture_test.cpp, tests/unit/pdf_test.cpp
 */
#pragma once

#include <cmath>
#include <memory>
#include <random>

#include "raytracer/hittable.hpp"
#include "raytracer/pdf.hpp"
#include "raytracer/random.hpp"
#include "raytracer/ray.hpp"
#include "raytracer/texture.hpp"
#include "raytracer/vec3.hpp"

namespace raytracer {

struct ScatterRecord {
    Ray specular_ray;
    bool is_specular = false;
    Color attenuation;
    std::shared_ptr<Pdf> pdf;
};

class Material {
public:
    virtual ~Material() = default;
    virtual bool Scatter(const Ray& r_in, const HitRecord& record, ScatterRecord& scatter_record,
                         std::mt19937& generator) const = 0;
    virtual double ScatteringPdf(const Ray& r_in, const HitRecord& record, const Ray& scattered) const {
        (void)r_in;
        (void)record;
        (void)scattered;
        return 0.0;
    }
    virtual Color Emitted(double /*u*/, double /*v*/, const Point3& /*p*/) const { return Color(0.0, 0.0, 0.0); }
};

class Lambertian : public Material {
public:
    explicit Lambertian(const Color& albedo) : albedo_(std::make_shared<SolidColor>(albedo)) {}
    explicit Lambertian(std::shared_ptr<Texture> texture) : albedo_(std::move(texture)) {}

    bool Scatter(const Ray& /*r_in*/, const HitRecord& record, ScatterRecord& scatter_record,
                 std::mt19937& /*generator*/) const override {
        scatter_record.is_specular = false;
        scatter_record.attenuation = albedo_->Value(record.u, record.v, record.p);
        scatter_record.pdf = std::make_shared<CosinePdf>(record.normal);
        return true;
    }

    double ScatteringPdf(const Ray& r_in, const HitRecord& record, const Ray& scattered) const override {
        (void)r_in;
        const double cosine = Dot(record.normal, UnitVector(scattered.direction()));
        return cosine < 0.0 ? 0.0 : cosine / kPi;
    }

private:
    static constexpr double kPi = 3.1415926535897932385;
    std::shared_ptr<Texture> albedo_;
};

class Metal : public Material {
public:
    Metal(const Color& albedo, double fuzz) : albedo_(albedo), fuzz_(fuzz < 1.0 ? fuzz : 1.0) {}

    bool Scatter(const Ray& r_in, const HitRecord& record, ScatterRecord& scatter_record,
                 std::mt19937& generator) const override {
        const Vec3 unit_direction = UnitVector(r_in.direction());
        const Vec3 reflected = Reflect(unit_direction, record.normal);
        const Vec3 scattered_direction = reflected + fuzz_ * RandomInUnitSphere(generator);
        scatter_record.specular_ray = Ray(record.p, scattered_direction, r_in.time());
        scatter_record.attenuation = albedo_;
        scatter_record.is_specular = true;
        return Dot(scatter_record.specular_ray.direction(), record.normal) > 0;
    }

private:
    Color albedo_;
    double fuzz_;
};

class Dielectric : public Material {
public:
    explicit Dielectric(double refraction_index) : refraction_index_(refraction_index) {}

    bool Scatter(const Ray& r_in, const HitRecord& record, ScatterRecord& scatter_record,
                 std::mt19937& generator) const override {
        scatter_record.attenuation = Color(1.0, 1.0, 1.0);
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

        scatter_record.specular_ray = Ray(record.p, direction, r_in.time());
        scatter_record.is_specular = true;
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

class DiffuseLight : public Material {
public:
    explicit DiffuseLight(const Color& emit) : emit_(emit) {}

    bool Scatter(const Ray& /*r_in*/, const HitRecord& /*record*/, ScatterRecord& /*scatter_record*/,
                 std::mt19937& /*generator*/) const override {
        return false;
    }

    Color Emitted(double /*u*/, double /*v*/, const Point3& /*p*/) const override { return emit_; }

private:
    Color emit_;
};

class Isotropic : public Material {
public:
    explicit Isotropic(const Color& albedo) : albedo_(std::make_shared<SolidColor>(albedo)) {}
    explicit Isotropic(std::shared_ptr<Texture> texture) : albedo_(std::move(texture)) {}

    bool Scatter(const Ray& r_in, const HitRecord& record, ScatterRecord& scatter_record,
                 std::mt19937& generator) const override {
        scatter_record.is_specular = false;
        scatter_record.attenuation = albedo_->Value(record.u, record.v, record.p);
        scatter_record.pdf = std::make_shared<UniformSpherePdf>();
        scatter_record.specular_ray = Ray(record.p, RandomInUnitSphere(generator), r_in.time());
        return true;
    }

    double ScatteringPdf(const Ray& /*r_in*/, const HitRecord& /*record*/, const Ray& /*scattered*/) const override {
        return uniform_pdf_;
    }

private:
    static constexpr double uniform_pdf_ = 1.0 / (4.0 * 3.1415926535897932385);
    std::shared_ptr<Texture> albedo_;
};

}  // namespace raytracer
