/*
 * 설명: 결정적 랜덤 값을 생성하고 벡터 샘플링 유틸리티를 제공한다.
 * 버전: v0.4.0
 * 관련 문서: design/renderer/v0.4.0-materials.md
 * 테스트: tests/unit/material_scatter_test.cpp
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

}  // namespace raytracer
