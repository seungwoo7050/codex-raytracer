/*
 * 설명: 3차원 벡터를 표현하고 기하 연산을 제공한다.
 * 버전: v0.4.0
 * 관련 문서: design/renderer/v0.4.0-materials.md
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

    Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }

    Vec3& operator+=(const Vec3& other) {
        e[0] += other.e[0];
        e[1] += other.e[1];
        e[2] += other.e[2];
        return *this;
    }

    Vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vec3& operator/=(double t) { return *this *= 1 / t; }

    double length() const { return std::sqrt(length_squared()); }
    double length_squared() const { return e[0] * e[0] + e[1] * e[1] + e[2] * e[2]; }

    bool NearZero() const {
        constexpr double kEpsilon = 1e-8;
        return (std::fabs(e[0]) < kEpsilon) && (std::fabs(e[1]) < kEpsilon) && (std::fabs(e[2]) < kEpsilon);
    }

    double operator[](int i) const { return e[i]; }

private:
    double e[3];
};

using Point3 = Vec3;
using Color = Vec3;

inline Vec3 operator+(const Vec3& u, const Vec3& v) {
    return Vec3(u.x() + v.x(), u.y() + v.y(), u.z() + v.z());
}

inline Vec3 operator-(const Vec3& u, const Vec3& v) {
    return Vec3(u.x() - v.x(), u.y() - v.y(), u.z() - v.z());
}

inline Vec3 operator*(const Vec3& u, const Vec3& v) {
    return Vec3(u.x() * v.x(), u.y() * v.y(), u.z() * v.z());
}

inline Vec3 operator*(double t, const Vec3& v) { return Vec3(t * v.x(), t * v.y(), t * v.z()); }
inline Vec3 operator*(const Vec3& v, double t) { return t * v; }

inline Vec3 operator/(const Vec3& v, double t) { return (1 / t) * v; }

inline double Dot(const Vec3& u, const Vec3& v) {
    return u.x() * v.x() + u.y() * v.y() + u.z() * v.z();
}

inline Vec3 Cross(const Vec3& u, const Vec3& v) {
    return Vec3(u.y() * v.z() - u.z() * v.y(), u.z() * v.x() - u.x() * v.z(), u.x() * v.y() - u.y() * v.x());
}

inline Vec3 UnitVector(const Vec3& v) { return v / v.length(); }

inline Vec3 Reflect(const Vec3& v, const Vec3& n) { return v - 2.0 * Dot(v, n) * n; }

inline Vec3 Refract(const Vec3& uv, const Vec3& n, double etai_over_etat) {
    const double cos_theta = std::fmin(Dot(-uv, n), 1.0);
    const Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    const Vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

}  // namespace raytracer
