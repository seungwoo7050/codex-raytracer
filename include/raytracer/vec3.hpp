/*
 * 설명: 3차원 벡터를 표현하고 기본 연산을 제공한다.
 * 버전: v0.1.0
 * 관련 문서: design/renderer/v0.1.0-overview.md
 * 테스트: tests/unit/vec3_test.cpp
 */
#pragma once

#include <cmath>

namespace raytracer {

class Vec3 {
public:
    Vec3() : e{0.0, 0.0, 0.0} {}
    Vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    Vec3 operator+(const Vec3& other) const {
        return Vec3(e[0] + other.e[0], e[1] + other.e[1], e[2] + other.e[2]);
    }

    Vec3 operator*(double t) const { return Vec3(e[0] * t, e[1] * t, e[2] * t); }

private:
    double e[3];
};

inline Vec3 operator*(double t, const Vec3& v) { return v * t; }

}  // namespace raytracer
