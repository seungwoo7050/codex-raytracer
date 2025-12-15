/*
 * 설명: 동일한 레이 집합에 대해 BVH 사용 전후 hit 시간을 비교해 텍스트로 출력한다.
 * 버전: v0.9.0
 * 관련 문서: design/renderer/v0.6.0-bvh.md, design/renderer/v0.9.0-volume.md
 * 테스트: (수동 실행)
 */
#include <chrono>
#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <vector>

#include "raytracer/bvh.hpp"
#include "raytracer/hittable_list.hpp"
#include "raytracer/material.hpp"
#include "raytracer/random.hpp"
#include "raytracer/ray.hpp"
#include "raytracer/sphere.hpp"
#include "raytracer/vec3.hpp"

using namespace raytracer;

struct Measurement {
    std::chrono::duration<double, std::milli> elapsed;
    int hit_count = 0;
};

HittableList BuildBenchmarkWorld(std::mt19937& generator) {
    HittableList world;
    const auto ground = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.Add(std::make_shared<Sphere>(Point3(0.0, -1000.0, 0.0), 1000.0, ground));

    for (int x = -6; x <= 6; ++x) {
        for (int z = -6; z <= 6; ++z) {
            const double center_x = static_cast<double>(x) + 0.6 * RandomDouble(generator);
            const double center_z = static_cast<double>(z) + 0.6 * RandomDouble(generator);
            const Point3 center(center_x, 0.2, center_z);

            const Color albedo(RandomDouble(generator, 0.2, 0.8), RandomDouble(generator, 0.2, 0.8),
                               RandomDouble(generator, 0.2, 0.8));
            const auto material = std::make_shared<Lambertian>(albedo);
            world.Add(std::make_shared<Sphere>(center, 0.2, material));
        }
    }

    const auto mirror = std::make_shared<Metal>(Color(0.8, 0.8, 0.9), 0.0);
    world.Add(std::make_shared<Sphere>(Point3(0.0, 1.0, -1.0), 1.0, mirror));

    const auto glass = std::make_shared<Dielectric>(1.5);
    world.Add(std::make_shared<Sphere>(Point3(-2.0, 1.0, 0.0), 1.0, glass));

    return world;
}

std::vector<Ray> GenerateRays(std::mt19937& generator, size_t count) {
    std::vector<Ray> rays;
    rays.reserve(count);
    for (size_t i = 0; i < count; ++i) {
        const Point3 origin(RandomDouble(generator, -6.0, 6.0), RandomDouble(generator, 0.2, 3.0), 6.0);
        const Vec3 direction = UnitVector(Vec3(RandomDouble(generator, -2.0, 2.0), RandomDouble(generator, -0.5, 1.5),
                                               -RandomDouble(generator, 2.0, 8.0)));
        const double time = RandomDouble(generator, 0.0, 1.0);
        rays.emplace_back(origin, direction, time);
    }
    return rays;
}

Measurement MeasureHits(const Hittable& world, const std::vector<Ray>& rays, std::uint32_t seed) {
    int hits = 0;
    std::mt19937 generator(seed);
    const auto start = std::chrono::steady_clock::now();
    for (const auto& ray : rays) {
        HitRecord record;
        if (world.Hit(ray, 0.001, std::numeric_limits<double>::infinity(), record, generator)) {
            ++hits;
        }
    }
    const auto end = std::chrono::steady_clock::now();
    return {end - start, hits};
}

int main() {
    std::mt19937 generator(2024);
    HittableList world = BuildBenchmarkWorld(generator);
    std::vector<std::shared_ptr<Hittable>> objects = world.Objects();
    BvhNode bvh(objects, 0.0, 1.0);

    const std::vector<Ray> rays = GenerateRays(generator, 20000);

    const Measurement list_measure = MeasureHits(world, rays, 2025);
    const Measurement bvh_measure = MeasureHits(bvh, rays, 2025);

    std::cout << "샘플 레이 개수: " << rays.size() << "\n";
    std::cout << "리스트 hit 시간(ms): " << list_measure.elapsed.count() << "\n";
    std::cout << "BVH hit 시간(ms): " << bvh_measure.elapsed.count() << "\n";
    std::cout << "hit 카운트 차이: " << (list_measure.hit_count - bvh_measure.hit_count) << "\n";

    return 0;
}
