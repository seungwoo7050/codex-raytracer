/*
 * 설명: 직교 정규 기저를 구성해 지역 좌표 변환을 제공한다.
 * 버전: v1.0.0
 * 관련 문서: design/renderer/v1.0.0-overview.md
 * 테스트: tests/unit/pdf_test.cpp
 */
#pragma once

#include <cmath>

#include "raytracer/vec3.hpp"

namespace raytracer {

class Onb {
public:
    Onb() = default;

    const Vec3& U() const { return axis_[0]; }
    const Vec3& V() const { return axis_[1]; }
    const Vec3& W() const { return axis_[2]; }

    Vec3 Local(double a, double b, double c) const { return a * axis_[0] + b * axis_[1] + c * axis_[2]; }
    Vec3 Local(const Vec3& a) const { return a.x() * axis_[0] + a.y() * axis_[1] + a.z() * axis_[2]; }

    void BuildFromW(const Vec3& n) {
        axis_[2] = UnitVector(n);
        const Vec3 helper = (std::fabs(axis_[2].x()) > 0.9) ? Vec3(0.0, 1.0, 0.0) : Vec3(1.0, 0.0, 0.0);
        axis_[1] = UnitVector(Cross(axis_[2], helper));
        axis_[0] = Cross(axis_[2], axis_[1]);
    }

private:
    Vec3 axis_[3];
};

}  // namespace raytracer
