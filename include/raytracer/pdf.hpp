/*
 * 설명: 광원 및 표면 샘플링을 위한 PDF 계층과 샘플 생성을 제공한다.
 * 버전: v1.0.0
 * 관련 문서: design/renderer/v1.0.0-overview.md
 * 테스트: tests/unit/pdf_test.cpp
 */
#pragma once

#include <cmath>
#include <memory>
#include <random>

#include "raytracer/hittable.hpp"
#include "raytracer/onb.hpp"
#include "raytracer/random.hpp"

namespace raytracer {

class Pdf {
public:
    virtual ~Pdf() = default;
    virtual double Value(const Vec3& direction) const = 0;
    virtual Vec3 Generate(std::mt19937& generator) const = 0;
};

class CosinePdf : public Pdf {
public:
    explicit CosinePdf(const Vec3& w) { uvw_.BuildFromW(w); }

    double Value(const Vec3& direction) const override {
        const double cosine = Dot(UnitVector(direction), uvw_.W());
        return cosine > 0.0 ? cosine / kPi : 0.0;
    }

    Vec3 Generate(std::mt19937& generator) const override { return uvw_.Local(RandomCosineDirection(generator)); }

private:
    static constexpr double kPi = 3.1415926535897932385;
    Onb uvw_;
};

class SpherePdf : public Pdf {
public:
    SpherePdf(const Point3& origin, const Point3& center, double radius) : origin_(origin), center_(center), radius_(radius) {}

    double Value(const Vec3& direction) const override {
        (void)direction;
        const Vec3 to_center = center_ - origin_;
        const double distance_squared = to_center.length_squared();
        const double cos_theta_max = std::sqrt(1.0 - radius_ * radius_ / distance_squared);
        const double solid_angle = 2.0 * kPi * (1.0 - cos_theta_max);
        return 1.0 / solid_angle;
    }

    Vec3 Generate(std::mt19937& generator) const override {
        const Vec3 direction = center_ - origin_;
        const Onb onb_builder = [&]() {
            Onb basis;
            basis.BuildFromW(direction);
            return basis;
        }();
        return onb_builder.Local(RandomToSphere(radius_, direction.length_squared(), generator));
    }

private:
    static constexpr double kPi = 3.1415926535897932385;
    Point3 origin_;
    Point3 center_;
    double radius_ = 0.0;
};

class UniformSpherePdf : public Pdf {
public:
    double Value(const Vec3& /*direction*/) const override { return uniform_pdf_; }

    Vec3 Generate(std::mt19937& generator) const override { return RandomUnitVector(generator); }

private:
    static constexpr double uniform_pdf_ = 1.0 / (4.0 * 3.1415926535897932385);
};

class HittablePdf : public Pdf {
public:
    HittablePdf(const std::shared_ptr<Hittable>& hittable, const Point3& origin) : hittable_(hittable), origin_(origin) {}

    double Value(const Vec3& direction) const override { return hittable_ ? hittable_->PdfValue(origin_, direction) : 0.0; }

    Vec3 Generate(std::mt19937& generator) const override { return hittable_ ? hittable_->Random(origin_, generator) : Vec3(1.0, 0.0, 0.0); }

private:
    std::shared_ptr<Hittable> hittable_;
    Point3 origin_;
};

class MixturePdf : public Pdf {
public:
    MixturePdf(std::shared_ptr<Pdf> p0, std::shared_ptr<Pdf> p1) : p0_(std::move(p0)), p1_(std::move(p1)) {}

    double Value(const Vec3& direction) const override {
        return 0.5 * p0_->Value(direction) + 0.5 * p1_->Value(direction);
    }

    Vec3 Generate(std::mt19937& generator) const override {
        if (RandomDouble(generator) < 0.5) {
            return p0_->Generate(generator);
        }
        return p1_->Generate(generator);
    }

private:
    std::shared_ptr<Pdf> p0_;
    std::shared_ptr<Pdf> p1_;
};

}  // namespace raytracer
