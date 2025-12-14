/*
 * 설명: PPM(P3) 규격에 맞춰 구 노멀 기반 색상을 렌더링한다.
 * 버전: v0.2.0
 * 관련 문서: design/protocol/contract.md, design/renderer/v0.2.0-sphere-hit.md
 * 테스트: tests/integration/ppm_integration_test.cpp
 */
#include "raytracer/ppm.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <sstream>

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
    const double aspect_ratio = static_cast<double>(options.width) / static_cast<double>(options.height);
    const double viewport_height = 2.0;
    const double viewport_width = 2.0 * aspect_ratio;
    const double focal_length = 1.0;

    const Point3 origin(0.0, 0.0, 0.0);
    const Vec3 horizontal(viewport_width, 0.0, 0.0);
    const Vec3 vertical(0.0, viewport_height, 0.0);
    const Point3 lower_left_corner = origin - horizontal / 2.0 - vertical / 2.0 - Vec3(0.0, 0.0, focal_length);

    Sphere sphere(Point3(0.0, 0.0, -1.0), 0.5);

    std::ostringstream output;
    output << "P3\n";
    output << options.width << ' ' << options.height << "\n";
    output << "255\n";

    for (int y = 0; y < options.height; ++y) {
        for (int x = 0; x < options.width; ++x) {
            const double u = (options.width == 1) ? 0.0 : static_cast<double>(x) / static_cast<double>(options.width - 1);
            const double v = (options.height == 1)
                                 ? 0.0
                                 : static_cast<double>(options.height - 1 - y) /
                                       static_cast<double>(options.height - 1);

            const Ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            const Color pixel_color = RayColor(r, sphere);

            const int ir = ToChannel(pixel_color.x());
            const int ig = ToChannel(pixel_color.y());
            const int ib = ToChannel(pixel_color.z());

            output << ir << ' ' << ig << ' ' << ib << "\n";
        }
    }

    return output.str();
}

}  // namespace raytracer
