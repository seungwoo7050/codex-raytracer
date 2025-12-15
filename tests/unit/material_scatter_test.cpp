#include <gtest/gtest.h>

#include <random>

#include "raytracer/material.hpp"
#include "raytracer/ray.hpp"
#include "raytracer/vec3.hpp"

TEST(MaterialScatterTest, LambertianReturnsAlbedoAndUpwardDirection) {
    raytracer::Lambertian material(raytracer::Color(0.3, 0.6, 0.9));
    raytracer::HitRecord record;
    record.p = raytracer::Point3(0.0, 0.0, 0.0);
    record.normal = raytracer::Vec3(0.0, 0.0, 1.0);
    record.front_face = true;

    std::mt19937 generator(123);
    raytracer::Ray incoming(raytracer::Point3(0.0, 0.0, 1.0), raytracer::Vec3(0.0, 0.0, -1.0));
    raytracer::Color attenuation;
    raytracer::Ray scattered;

    const bool scattered_ok = material.Scatter(incoming, record, attenuation, scattered, generator);

    EXPECT_TRUE(scattered_ok);
    EXPECT_DOUBLE_EQ(attenuation.x(), 0.3);
    EXPECT_DOUBLE_EQ(attenuation.y(), 0.6);
    EXPECT_DOUBLE_EQ(attenuation.z(), 0.9);
    EXPECT_GT(raytracer::Dot(scattered.direction(), record.normal), 0.0);
}

TEST(MaterialScatterTest, MetalReflectsWithoutFuzzWhenAligned) {
    raytracer::Metal material(raytracer::Color(0.8, 0.8, 0.8), 0.0);
    raytracer::HitRecord record;
    record.p = raytracer::Point3(0.0, 0.0, 0.0);
    record.normal = raytracer::Vec3(0.0, 0.0, 1.0);
    record.front_face = true;

    std::mt19937 generator(1);
    raytracer::Ray incoming(raytracer::Point3(0.0, 0.0, 0.0), raytracer::Vec3(0.0, 0.0, -1.0));
    raytracer::Color attenuation;
    raytracer::Ray scattered;

    const bool scattered_ok = material.Scatter(incoming, record, attenuation, scattered, generator);

    EXPECT_TRUE(scattered_ok);
    EXPECT_DOUBLE_EQ(attenuation.x(), 0.8);
    EXPECT_DOUBLE_EQ(attenuation.y(), 0.8);
    EXPECT_DOUBLE_EQ(attenuation.z(), 0.8);
    EXPECT_DOUBLE_EQ(scattered.direction().x(), 0.0);
    EXPECT_DOUBLE_EQ(scattered.direction().y(), 0.0);
    EXPECT_DOUBLE_EQ(scattered.direction().z(), 1.0);
}

TEST(MaterialScatterTest, DielectricUsesTotalInternalReflectionWhenNecessary) {
    raytracer::Dielectric material(1.5);
    raytracer::HitRecord record;
    record.p = raytracer::Point3(0.0, 0.0, 0.0);
    record.normal = raytracer::Vec3(0.0, 0.0, 1.0);
    record.front_face = false;

    std::mt19937 generator(7);
    raytracer::Ray incoming(raytracer::Point3(0.0, 0.0, 0.0), raytracer::Vec3(0.0, 1.0, 0.0));
    raytracer::Color attenuation;
    raytracer::Ray scattered;

    const bool scattered_ok = material.Scatter(incoming, record, attenuation, scattered, generator);

    EXPECT_TRUE(scattered_ok);
    EXPECT_DOUBLE_EQ(attenuation.x(), 1.0);
    EXPECT_DOUBLE_EQ(attenuation.y(), 1.0);
    EXPECT_DOUBLE_EQ(attenuation.z(), 1.0);
    EXPECT_DOUBLE_EQ(scattered.direction().x(), 0.0);
    EXPECT_DOUBLE_EQ(scattered.direction().y(), 1.0);
    EXPECT_DOUBLE_EQ(scattered.direction().z(), 0.0);
}
