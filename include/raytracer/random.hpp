/*
 * 설명: 결정적 랜덤 값을 생성하고 벡터 샘플링 유틸리티를 제공한다.
 * 버전: v1.0.0
 * 관련 문서: design/renderer/v1.0.0-overview.md
 * 테스트: tests/unit/material_scatter_test.cpp, tests/unit/pdf_test.cpp
 */
#pragma once

#include <random>

#include "raytracer/vec3.hpp"

namespace raytracer {

inline double RandomDouble(std::mt19937& generator, double min = 0.0, double max = 1.0) {
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
}

inline Vec3 RandomInUnitSphere(std::mt19937& generator) {
    while (true) {
        const Vec3 candidate(RandomDouble(generator, -1.0, 1.0), RandomDouble(generator, -1.0, 1.0),
                             RandomDouble(generator, -1.0, 1.0));
        if (candidate.length_squared() < 1.0) {
            return candidate;
        }
    }
}

inline Vec3 RandomUnitVector(std::mt19937& generator) { return UnitVector(RandomInUnitSphere(generator)); }

inline Vec3 RandomInUnitDisk(std::mt19937& generator) {
    while (true) {
        const Vec3 candidate(RandomDouble(generator, -1.0, 1.0), RandomDouble(generator, -1.0, 1.0), 0.0);
        if (candidate.length_squared() < 1.0) {
            return candidate;
        }
    }
}

inline Vec3 RandomCosineDirection(std::mt19937& generator) {
    const double r1 = RandomDouble(generator);
    const double r2 = RandomDouble(generator);
    const double z = std::sqrt(1.0 - r2);
    const double phi = 2.0 * std::acos(-1.0) * r1;
    const double x = std::cos(phi) * std::sqrt(r2);
    const double y = std::sin(phi) * std::sqrt(r2);
    return Vec3(x, y, z);
}

inline Vec3 RandomToSphere(double radius, double distance_squared, std::mt19937& generator) {
    const double r1 = RandomDouble(generator);
    const double r2 = RandomDouble(generator);
    const double z = 1.0 + r2 * (std::sqrt(1.0 - radius * radius / distance_squared) - 1.0);
    const double phi = 2.0 * std::acos(-1.0) * r1;
    const double x = std::cos(phi) * std::sqrt(1.0 - z * z);
    const double y = std::sin(phi) * std::sqrt(1.0 - z * z);
    return Vec3(x, y, z);
}

}  // namespace raytracer
