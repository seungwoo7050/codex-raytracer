#include <gtest/gtest.h>

#include <memory>

#include "raytracer/material.hpp"
#include "raytracer/quad.hpp"
#include "raytracer/ray.hpp"
#include "raytracer/transform.hpp"

TEST(QuadTest, HitInsideQuadReturnsUvAndNormal) {
    auto material = std::make_shared<raytracer::Lambertian>(raytracer::Color(0.5, 0.5, 0.5));
    raytracer::Quad quad(raytracer::Point3(0.0, 0.0, 0.0), raytracer::Vec3(2.0, 0.0, 0.0),
                         raytracer::Vec3(0.0, 2.0, 0.0), material);

    raytracer::Ray ray(raytracer::Point3(1.0, 1.0, 1.0), raytracer::Vec3(0.0, 0.0, -1.0));
    raytracer::HitRecord record;

    const bool hit = quad.Hit(ray, 0.001, 10.0, record);

    ASSERT_TRUE(hit);
    EXPECT_NEAR(record.t, 1.0, 1e-6);
    EXPECT_DOUBLE_EQ(record.p.x(), 1.0);
    EXPECT_DOUBLE_EQ(record.p.y(), 1.0);
    EXPECT_DOUBLE_EQ(record.p.z(), 0.0);
    EXPECT_NEAR(record.u, 0.5, 1e-6);
    EXPECT_NEAR(record.v, 0.5, 1e-6);
    EXPECT_TRUE(record.front_face);
    EXPECT_DOUBLE_EQ(record.normal.z(), 1.0);
}

TEST(QuadTest, TranslateMovesIntersectionPoint) {
    auto material = std::make_shared<raytracer::Lambertian>(raytracer::Color(0.2, 0.3, 0.4));
    auto quad = std::make_shared<raytracer::Quad>(raytracer::Point3(0.0, 0.0, 0.0), raytracer::Vec3(1.0, 0.0, 0.0),
                                                  raytracer::Vec3(0.0, 1.0, 0.0), material);
    raytracer::Translate translated(quad, raytracer::Vec3(0.0, 0.0, 1.0));

    raytracer::Ray ray(raytracer::Point3(0.5, 0.5, 3.0), raytracer::Vec3(0.0, 0.0, -1.0));
    raytracer::HitRecord record;

    const bool hit = translated.Hit(ray, 0.001, 10.0, record);

    ASSERT_TRUE(hit);
    EXPECT_NEAR(record.p.z(), 1.0, 1e-6);
    EXPECT_DOUBLE_EQ(record.material->Emitted(record.u, record.v, record.p).length(), 0.0);
}
