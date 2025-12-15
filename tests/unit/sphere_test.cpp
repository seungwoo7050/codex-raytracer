#include <gtest/gtest.h>

#include <memory>

#include "raytracer/material.hpp"
#include "raytracer/ray.hpp"
#include "raytracer/sphere.hpp"
#include "raytracer/vec3.hpp"

TEST(SphereTest, HitsCenteredSphereFromFront) {
    auto material = std::make_shared<raytracer::Lambertian>(raytracer::Color(1.0, 1.0, 1.0));
    raytracer::Sphere sphere(raytracer::Point3(0.0, 0.0, -1.0), 0.5, material);
    raytracer::Ray ray(raytracer::Point3(0.0, 0.0, 0.0), raytracer::Vec3(0.0, 0.0, -1.0));

    raytracer::HitRecord record;
    const bool hit = sphere.Hit(ray, 0.001, 100.0, record);

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
    const bool hit = sphere.Hit(ray, 0.001, 100.0, record);

    EXPECT_FALSE(hit);
}
