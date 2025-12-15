/*
 * 설명: Quad와 Box의 레이 교차 및 경계 상자를 계산한다.
 * 버전: v0.9.0
 * 관련 문서: design/renderer/v0.8.0-cornell.md, design/renderer/v0.9.0-volume.md
 * 테스트: tests/unit/quad_test.cpp
 */
#include "raytracer/quad.hpp"

#include <algorithm>
#include <cmath>

namespace raytracer {
namespace {

constexpr double kEpsilon = 1e-8;
constexpr double kPadding = 1e-4;

double LengthSquared(const Vec3& v) { return v.length_squared(); }

}  // namespace

Quad::Quad(const Point3& q, const Vec3& u, const Vec3& v, std::shared_ptr<Material> material)
    : q_(q), u_(u), v_(v), material_(std::move(material)) {
    const Vec3 normal = Cross(u_, v_);
    normal_ = UnitVector(normal);
    d_ = Dot(normal_, q_);
    area_ = normal.length();
    SetBoundingBox();
}

bool Quad::Hit(const Ray& r, double t_min, double t_max, HitRecord& record, std::mt19937& /*generator*/) const {
    const double denominator = Dot(normal_, r.direction());
    if (std::fabs(denominator) < kEpsilon) {
        return false;
    }

    const double t = (d_ - Dot(normal_, r.origin())) / denominator;
    if (t < t_min || t > t_max) {
        return false;
    }

    const Point3 intersection = r.At(t);
    const Vec3 planar_vector = intersection - q_;
    const double alpha = Dot(planar_vector, u_);
    const double beta = Dot(planar_vector, v_);

    if (!IsInside(alpha, beta)) {
        return false;
    }

    record.t = t;
    record.p = intersection;
    record.u = alpha / LengthSquared(u_);
    record.v = beta / LengthSquared(v_);
    record.material = material_;
    record.SetFaceNormal(r, normal_);

    return true;
}

bool Quad::BoundingBox(double /*time0*/, double /*time1*/, Aabb& output_box) const {
    output_box = bbox_;
    return true;
}

bool Quad::IsInside(double alpha, double beta) const {
    return (alpha >= 0.0) && (beta >= 0.0) && (alpha <= LengthSquared(u_)) && (beta <= LengthSquared(v_));
}

void Quad::SetBoundingBox() {
    const Point3 p0 = q_;
    const Point3 p1 = q_ + u_;
    const Point3 p2 = q_ + v_;
    const Point3 p3 = q_ + u_ + v_;

    const double min_x = std::min({p0.x(), p1.x(), p2.x(), p3.x()}) - kPadding;
    const double min_y = std::min({p0.y(), p1.y(), p2.y(), p3.y()}) - kPadding;
    const double min_z = std::min({p0.z(), p1.z(), p2.z(), p3.z()}) - kPadding;
    const double max_x = std::max({p0.x(), p1.x(), p2.x(), p3.x()}) + kPadding;
    const double max_y = std::max({p0.y(), p1.y(), p2.y(), p3.y()}) + kPadding;
    const double max_z = std::max({p0.z(), p1.z(), p2.z(), p3.z()}) + kPadding;

    bbox_ = Aabb(Point3(min_x, min_y, min_z), Point3(max_x, max_y, max_z));
}

Box::Box(const Point3& min_point, const Point3& max_point, std::shared_ptr<Material> material)
    : min_(min_point), max_(max_point) {
    const double dx = max_point.x() - min_point.x();
    const double dy = max_point.y() - min_point.y();
    const double dz = max_point.z() - min_point.z();

    sides_.Add(std::make_shared<Quad>(Point3(min_point.x(), min_point.y(), max_point.z()), Vec3(dx, 0.0, 0.0),
                                      Vec3(0.0, dy, 0.0), material));
    sides_.Add(std::make_shared<Quad>(Point3(min_point.x(), min_point.y(), min_point.z()), Vec3(0.0, dy, 0.0),
                                      Vec3(dx, 0.0, 0.0), material));
    sides_.Add(std::make_shared<Quad>(Point3(min_point.x(), max_point.y(), min_point.z()), Vec3(0.0, 0.0, dz),
                                      Vec3(dx, 0.0, 0.0), material));
    sides_.Add(std::make_shared<Quad>(Point3(min_point.x(), min_point.y(), min_point.z()), Vec3(0.0, 0.0, dz),
                                      Vec3(0.0, dy, 0.0), material));
    sides_.Add(std::make_shared<Quad>(Point3(max_point.x(), min_point.y(), min_point.z()), Vec3(0.0, dy, 0.0),
                                      Vec3(0.0, 0.0, dz), material));
    sides_.Add(std::make_shared<Quad>(Point3(min_point.x(), min_point.y(), min_point.z()), Vec3(dx, 0.0, 0.0),
                                      Vec3(0.0, 0.0, dz), material));
}

bool Box::Hit(const Ray& r, double t_min, double t_max, HitRecord& record, std::mt19937& generator) const {
    return sides_.Hit(r, t_min, t_max, record, generator);
}

bool Box::BoundingBox(double /*time0*/, double /*time1*/, Aabb& output_box) const {
    output_box = Aabb(min_, max_);
    return true;
}

}  // namespace raytracer
