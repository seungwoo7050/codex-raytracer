/*
 * 설명: PPM(P3) 규격에 맞춰 Lambertian/Metal/Dielectric 장면을 렌더링한다.
 * 버전: v0.4.0
 * 관련 문서: design/protocol/contract.md, design/renderer/v0.4.0-materials.md
 * 테스트: tests/integration/ppm_integration_test.cpp
 */
#include "raytracer/ppm.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <sstream>

#include "raytracer/camera.hpp"
#include "raytracer/hittable_list.hpp"
#include "raytracer/material.hpp"
#include "raytracer/ray.hpp"
#include "raytracer/sphere.hpp"
#include "raytracer/vec3.hpp"

namespace raytracer {
namespace {

int ClampColor(int value) { return std::clamp(value, 0, 255); }

int ToChannel(double value) {
    const double clamped = std::clamp(value, 0.0, 0.999);
    return ClampColor(static_cast<int>(std::lround(255.0 * clamped)));
}

Color RayColor(const Ray& r, int depth, const Hittable& world, std::mt19937& generator) {
    if (depth <= 0) {
        return Color(0.0, 0.0, 0.0);
    }

    HitRecord record;
    if (world.Hit(r, 0.001, std::numeric_limits<double>::infinity(), record)) {
        Ray scattered;
        Color attenuation;
        if (record.material && record.material->Scatter(r, record, attenuation, scattered, generator)) {
            return attenuation * RayColor(scattered, depth - 1, world, generator);
        }
        return Color(0.0, 0.0, 0.0);
    }

    const Vec3 unit_direction = UnitVector(r.direction());
    const double t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

void WriteColor(std::ostringstream& output, const Color& pixel_color) {
    const Color gamma_corrected(std::sqrt(pixel_color.x()), std::sqrt(pixel_color.y()), std::sqrt(pixel_color.z()));
    const int ir = ToChannel(gamma_corrected.x());
    const int ig = ToChannel(gamma_corrected.y());
    const int ib = ToChannel(gamma_corrected.z());
    output << ir << ' ' << ig << ' ' << ib << "\n";
}

HittableList BuildScene() {
    HittableList world;

    const auto ground_material = std::make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    const auto center_material = std::make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    const auto left_material = std::make_shared<Dielectric>(1.5);
    const auto right_material = std::make_shared<Metal>(Color(0.8, 0.6, 0.2), 0.0);

    world.Add(std::make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, ground_material));
    world.Add(std::make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, center_material));
    world.Add(std::make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, left_material));
    world.Add(std::make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, right_material));

    return world;
}

}  // namespace

std::string RenderMaterialImage(const RenderOptions& options) {
    const double aspect_ratio = static_cast<double>(options.width) / static_cast<double>(options.height);
    const Camera camera(options.vertical_fov_degrees, aspect_ratio);
    HittableList world = BuildScene();
    std::mt19937 generator(options.seed);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    std::ostringstream output;
    output << "P3\n";
    output << options.width << ' ' << options.height << "\n";
    output << "255\n";

    for (int y = 0; y < options.height; ++y) {
        for (int x = 0; x < options.width; ++x) {
            Color pixel_color(0.0, 0.0, 0.0);
            for (int sample = 0; sample < options.samples_per_pixel; ++sample) {
                const double u = (options.width == 1)
                                     ? 0.5
                                     : (static_cast<double>(x) + distribution(generator)) /
                                           (static_cast<double>(options.width) - 1.0);
                const double v = (options.height == 1)
                                     ? 0.5
                                     : (static_cast<double>(options.height - 1 - y) + distribution(generator)) /
                                           (static_cast<double>(options.height) - 1.0);

                const Ray r = camera.GetRay(u, v);
                pixel_color += RayColor(r, options.max_depth, world, generator);
            }

            const Color averaged_color = pixel_color / static_cast<double>(options.samples_per_pixel);
            WriteColor(output, averaged_color);
        }
    }

    return output.str();
}

}  // namespace raytracer
