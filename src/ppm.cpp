/*
 * 설명: PPM(P3) 규격에 맞춰 구 노멀 기반 색상을 렌더링한다.
 * 버전: v0.3.0
 * 관련 문서: design/protocol/contract.md, design/renderer/v0.3.0-camera-sampling.md
 * 테스트: tests/integration/ppm_integration_test.cpp
 */
#include "raytracer/ppm.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <random>
#include <sstream>

#include "raytracer/camera.hpp"
#include "raytracer/ray.hpp"
#include "raytracer/sphere.hpp"
#include "raytracer/vec3.hpp"

namespace raytracer {
namespace {

int ClampColor(int value) { return std::clamp(value, 0, 255); }

Color RayColor(const Ray& r, const Hittable& world) {
    HitRecord record;
    if (world.Hit(r, 0.001, std::numeric_limits<double>::infinity(), record)) {
        const Color normal_color = 0.5 * (record.normal + Vec3(1.0, 1.0, 1.0));
        return normal_color;
    }

    const Vec3 unit_direction = UnitVector(r.direction());
    const double t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}

int ToChannel(double value) {
    const double clamped = std::clamp(value, 0.0, 0.999);
    return ClampColor(static_cast<int>(std::lround(255.0 * clamped)));
}

}  // namespace

std::string RenderSphereNormalImage(const RenderOptions& options) {
    Sphere sphere(Point3(0.0, 0.0, -1.0), 0.5);
    const double aspect_ratio = static_cast<double>(options.width) / static_cast<double>(options.height);
    const Camera camera(options.vertical_fov_degrees, aspect_ratio);
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
                pixel_color += RayColor(r, sphere);
            }

            const Color averaged_color = pixel_color / static_cast<double>(options.samples_per_pixel);

            const int ir = ToChannel(averaged_color.x());
            const int ig = ToChannel(averaged_color.y());
            const int ib = ToChannel(averaged_color.z());

            output << ir << ' ' << ig << ' ' << ib << "\n";
        }
    }

    return output.str();
}

}  // namespace raytracer
