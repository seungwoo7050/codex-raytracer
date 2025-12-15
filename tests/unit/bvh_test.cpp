/*
 * 설명: BVH 트리가 원본 HittableList와 동일한 hit 결과를 반환하는지 검증한다.
 * 버전: v0.6.0
 * 관련 문서: design/renderer/v0.6.0-bvh.md
 * 테스트: tests/unit/bvh_test.cpp
 */
#include <gtest/gtest.h>

#include <limits>
#include <memory>
#include <vector>

#include "raytracer/bvh.hpp"
#include "raytracer/hittable_list.hpp"
#include "raytracer/material.hpp"
#include "raytracer/ray.hpp"
#include "raytracer/sphere.hpp"
#include "raytracer/vec3.hpp"

namespace {

double Inf() { return std::numeric_limits<double>::infinity(); }

}  // namespace

TEST(BvhTest, MatchesHittableListHits) {
    using raytracer::BvhNode;
    using raytracer::Dielectric;
    using raytracer::HitRecord;
    using raytracer::HittableList;
    using raytracer::Lambertian;
    using raytracer::Metal;
    using raytracer::MovingSphere;
    using raytracer::Point3;
    using raytracer::Ray;
    using raytracer::Sphere;
    using raytracer::Vec3;

    HittableList world;
    const auto red = std::make_shared<Lambertian>(raytracer::Color(0.8, 0.3, 0.3));
    const auto gold = std::make_shared<Metal>(raytracer::Color(0.8, 0.6, 0.2), 0.0);
    const auto glass = std::make_shared<Dielectric>(1.5);

    world.Add(std::make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, red));
    world.Add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.5), 0.3, gold));
    world.Add(std::make_shared<MovingSphere>(Point3(0.5, 0.0, -1.0), Point3(0.5, 0.5, -1.0), 0.0, 1.0, 0.25, glass));

    BvhNode bvh(world.Objects(), 0.0, 1.0);

    std::vector<Ray> rays;
    rays.emplace_back(Point3(0.0, 0.0, 0.0), Vec3(0.0, 0.0, -1.0), 0.0);
    rays.emplace_back(Point3(0.5, 0.25, 0.5), Vec3(0.0, 0.0, -1.0), 0.5);
    rays.emplace_back(Point3(2.0, 1.0, 0.0), Vec3(-1.0, -0.25, -1.0), 0.3);
    rays.emplace_back(Point3(0.0, 2.0, 0.0), Vec3(0.0, 1.0, 0.0), 0.0);

    for (const auto& ray : rays) {
        HitRecord list_record;
        HitRecord bvh_record;

        const bool list_hit = world.Hit(ray, 0.001, Inf(), list_record);
        const bool bvh_hit = bvh.Hit(ray, 0.001, Inf(), bvh_record);

        EXPECT_EQ(list_hit, bvh_hit);
        if (list_hit) {
            EXPECT_NEAR(list_record.t, bvh_record.t, 1e-12);
            EXPECT_NEAR(list_record.p.x(), bvh_record.p.x(), 1e-12);
            EXPECT_NEAR(list_record.p.y(), bvh_record.p.y(), 1e-12);
            EXPECT_NEAR(list_record.p.z(), bvh_record.p.z(), 1e-12);
            EXPECT_NEAR(list_record.normal.x(), bvh_record.normal.x(), 1e-12);
            EXPECT_NEAR(list_record.normal.y(), bvh_record.normal.y(), 1e-12);
            EXPECT_NEAR(list_record.normal.z(), bvh_record.normal.z(), 1e-12);
            EXPECT_EQ(list_record.front_face, bvh_record.front_face);
            EXPECT_EQ(list_record.material.get(), bvh_record.material.get());
        }
    }
}
