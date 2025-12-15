#include <gtest/gtest.h>

#include <memory>
#include <random>

#include "raytracer/material.hpp"
#include "raytracer/ray.hpp"
#include "raytracer/sphere.hpp"
#include "raytracer/vec3.hpp"

TEST(SphereTest, HitsCenteredSphereFromFront) {
    auto material = std::make_shared<raytracer::Lambertian>(raytracer::Color(1.0, 1.0, 1.0));
    raytracer::Sphere sphere(raytracer::Point3(0.0, 0.0, -1.0), 0.5, material);
    raytracer::Ray ray(raytracer::Point3(0.0, 0.0, 0.0), raytracer::Vec3(0.0, 0.0, -1.0));

    raytracer::HitRecord record;
    std::mt19937 generator(1);
    const bool hit = sphere.Hit(ray, 0.001, 100.0, record, generator);

    EXPECT_TRUE(hit);
    EXPECT_NEAR(record.t, 0.5, 1e-9);
    EXPECT_DOUBLE_EQ(record.p.x(), 0.0);
    EXPECT_DOUBLE_EQ(record.p.y(), 0.0);
    EXPECT_DOUBLE_EQ(record.p.z(), -0.5);
    EXPECT_TRUE(record.front_face);
    EXPECT_DOUBLE_EQ(record.normal.x(), 0.0);
    EXPECT_DOUBLE_EQ(record.normal.y(), 0.0);
    EXPECT_DOUBLE_EQ(record.normal.z(), 1.0);
}

TEST(SphereTest, MissesWhenRaySkimsPast) {
    auto material = std::make_shared<raytracer::Lambertian>(raytracer::Color(1.0, 1.0, 1.0));
    raytracer::Sphere sphere(raytracer::Point3(0.0, 0.0, -1.0), 0.5, material);
    raytracer::Ray ray(raytracer::Point3(0.0, 1.0, 0.0), raytracer::Vec3(0.0, 0.0, -1.0));

    raytracer::HitRecord record;
    std::mt19937 generator(1);
    const bool hit = sphere.Hit(ray, 0.001, 100.0, record, generator);

    EXPECT_FALSE(hit);
}

TEST(SphereTest, MovingSphereTracksRayTime) {
    auto material = std::make_shared<raytracer::Lambertian>(raytracer::Color(1.0, 1.0, 1.0));
    raytracer::MovingSphere sphere(raytracer::Point3(0.0, 0.0, -1.0), raytracer::Point3(0.0, -0.25, -1.0), 0.0, 1.0,
                                   0.5, material);

    raytracer::Ray ray_start(raytracer::Point3(0.0, 0.0, 0.0), raytracer::Vec3(0.0, 0.0, -1.0), 0.0);
    raytracer::HitRecord record_start;
    std::mt19937 generator(2);
    ASSERT_TRUE(sphere.Hit(ray_start, 0.001, 100.0, record_start, generator));
    const raytracer::Point3 estimated_center_start = record_start.p - 0.5 * record_start.normal;
    EXPECT_NEAR(estimated_center_start.y(), 0.0, 1e-6);

    raytracer::Ray ray_end(raytracer::Point3(0.0, 0.0, 0.0), raytracer::Vec3(0.0, 0.0, -1.0), 1.0);
    raytracer::HitRecord record_end;
    ASSERT_TRUE(sphere.Hit(ray_end, 0.001, 100.0, record_end, generator));
    const raytracer::Point3 estimated_center_end = record_end.p - 0.5 * record_end.normal;
    EXPECT_NEAR(estimated_center_end.y(), -0.25, 1e-6);
}
