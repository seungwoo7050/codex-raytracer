/*
 * 설명: 단색, 체커, 퍼린 노이즈 기반 텍스처를 정의하고 샘플러를 제공한다.
 * 버전: v0.7.0
 * 관련 문서: design/renderer/v0.7.0-textures.md
 * 테스트: tests/unit/texture_test.cpp
 */
#pragma once

#include <array>
#include <cmath>
#include <memory>
#include <random>

#include "raytracer/random.hpp"
#include "raytracer/vec3.hpp"

namespace raytracer {

class Texture {
public:
    virtual ~Texture() = default;
    virtual Color Value(double u, double v, const Point3& p) const = 0;
};

class SolidColor : public Texture {
public:
    SolidColor() : color_value_(0.0, 0.0, 0.0) {}
    explicit SolidColor(const Color& color) : color_value_(color) {}

    Color Value(double /*u*/, double /*v*/, const Point3& /*p*/) const override { return color_value_; }

private:
    Color color_value_;
};

class CheckerTexture : public Texture {
public:
    CheckerTexture(std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd, double scale)
        : even_(std::move(even)), odd_(std::move(odd)), scale_(scale) {}

    CheckerTexture(const Color& even_color, const Color& odd_color, double scale)
        : even_(std::make_shared<SolidColor>(even_color)),
          odd_(std::make_shared<SolidColor>(odd_color)),
          scale_(scale) {}

    Color Value(double u, double v, const Point3& p) const override {
        const double sines = std::sin(scale_ * p.x()) * std::sin(scale_ * p.y()) * std::sin(scale_ * p.z());
        if (sines < 0.0) {
            return odd_->Value(u, v, p);
        }
        return even_->Value(u, v, p);
    }

private:
    std::shared_ptr<Texture> even_;
    std::shared_ptr<Texture> odd_;
    double scale_;
};

class Perlin {
public:
    Perlin() {
        std::mt19937 generator(20240701);
        for (int i = 0; i < kPointCount; ++i) {
            random_vectors_[i] = UnitVector(Vec3(RandomDouble(generator, -1.0, 1.0), RandomDouble(generator, -1.0, 1.0),
                                                 RandomDouble(generator, -1.0, 1.0)));
        }
        perm_x_ = GeneratePermutation(20240701);
        perm_y_ = GeneratePermutation(20240731);
        perm_z_ = GeneratePermutation(20240801);
    }

    double Noise(const Point3& p) const {
        const double u = p.x() - std::floor(p.x());
        const double v = p.y() - std::floor(p.y());
        const double w = p.z() - std::floor(p.z());

        const int i = static_cast<int>(std::floor(p.x()));
        const int j = static_cast<int>(std::floor(p.y()));
        const int k = static_cast<int>(std::floor(p.z()));

        Vec3 c[2][2][2];
        for (int di = 0; di < 2; ++di) {
            for (int dj = 0; dj < 2; ++dj) {
                for (int dk = 0; dk < 2; ++dk) {
                    const int index = perm_x_[(i + di) & 255] ^ perm_y_[(j + dj) & 255] ^ perm_z_[(k + dk) & 255];
                    c[di][dj][dk] = random_vectors_[index];
                }
            }
        }

        return PerlinInterp(c, u, v, w);
    }

    double Turbulence(const Point3& p, int depth = 7) const {
        double accum = 0.0;
        Point3 temp_p = p;
        double weight = 1.0;

        for (int i = 0; i < depth; ++i) {
            accum += weight * Noise(temp_p);
            weight *= 0.5;
            temp_p = 2.0 * temp_p;
        }

        return std::fabs(accum);
    }

private:
    static constexpr int kPointCount = 256;

    static std::array<int, kPointCount> GeneratePermutation(unsigned int seed) {
        std::array<int, kPointCount> perm{};
        for (int i = 0; i < kPointCount; ++i) {
            perm[i] = i;
        }

        std::mt19937 generator(seed);
        for (int i = kPointCount - 1; i > 0; --i) {
            std::uniform_int_distribution<int> distribution(0, i);
            const int target = distribution(generator);
            std::swap(perm[i], perm[target]);
        }
        return perm;
    }

    static double PerlinInterp(const Vec3 c[2][2][2], double u, double v, double w) {
        const double uu = u * u * (3.0 - 2.0 * u);
        const double vv = v * v * (3.0 - 2.0 * v);
        const double ww = w * w * (3.0 - 2.0 * w);
        double accum = 0.0;

        for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
                for (int k = 0; k < 2; ++k) {
                    const Vec3 weight_v(u - i, v - j, w - k);
                    const double blend = (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) *
                                         (k * ww + (1 - k) * (1 - ww));
                    accum += blend * Dot(c[i][j][k], weight_v);
                }
            }
        }

        return accum;
    }

    std::array<Vec3, kPointCount> random_vectors_{};
    std::array<int, kPointCount> perm_x_{};
    std::array<int, kPointCount> perm_y_{};
    std::array<int, kPointCount> perm_z_{};
};

class NoiseTexture : public Texture {
public:
    explicit NoiseTexture(double scale) : scale_(scale) {}

    Color Value(double /*u*/, double /*v*/, const Point3& p) const override {
        const double noise_value = perlin_.Turbulence(scale_ * p);
        const double normalized = 0.5 * (1.0 + std::sin(scale_ * p.z() + 10.0 * noise_value));
        return Color(1.0, 1.0, 1.0) * normalized;
    }

private:
    Perlin perlin_;
    double scale_;
};

}  // namespace raytracer
