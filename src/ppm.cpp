/*
 * 설명: Cornell smoke 볼륨 장면을 BVH로 가속하고 광원 PDF를 혼합해 PPM(P3) 규격으로 렌더링한다.
 * 버전: v1.0.0
 * 관련 문서: design/protocol/contract.md, design/renderer/v1.0.0-overview.md
 * 테스트: tests/integration/ppm_integration_test.cpp
 */
#include "raytracer/ppm.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <memory>
#include <random>
#include <sstream>

#include "raytracer/bvh.hpp"
#include "raytracer/camera.hpp"
#include "raytracer/constant_medium.hpp"
#include "raytracer/hittable_list.hpp"
#include "raytracer/material.hpp"
#include "raytracer/pdf.hpp"
#include "raytracer/quad.hpp"
#include "raytracer/random.hpp"
#include "raytracer/ray.hpp"
#include "raytracer/transform.hpp"
#include "raytracer/vec3.hpp"

namespace raytracer {
namespace {

int ClampColor(int value) { return std::clamp(value, 0, 255); }

int ToChannel(double value) {
    const double clamped = std::clamp(value, 0.0, 0.999);
    return ClampColor(static_cast<int>(std::lround(255.0 * clamped)));
}

Color RayColor(const Ray& r, int depth, const Hittable& world, const std::shared_ptr<Hittable>& lights,
               std::mt19937& generator) {
    if (depth <= 0) {
        return Color(0.0, 0.0, 0.0);
    }

    HitRecord record;
    if (!world.Hit(r, 0.001, std::numeric_limits<double>::infinity(), record, generator)) {
        return Color(0.0, 0.0, 0.0);
    }

    const Color emitted = (record.material && record.front_face) ? record.material->Emitted(record.u, record.v, record.p)
                                                                  : Color(0.0, 0.0, 0.0);

    if (!record.material) {
        return emitted;
    }

    ScatterRecord scatter_record;
    if (!record.material->Scatter(r, record, scatter_record, generator)) {
        return emitted;
    }

    if (scatter_record.is_specular) {
        return emitted + scatter_record.attenuation * RayColor(scatter_record.specular_ray, depth - 1, world, lights, generator);
    }

    if (!scatter_record.pdf) {
        return emitted;
    }

    std::shared_ptr<Pdf> light_pdf = lights ? std::make_shared<HittablePdf>(lights, record.p) : nullptr;
    std::shared_ptr<Pdf> mixed_pdf = light_pdf ? std::make_shared<MixturePdf>(light_pdf, scatter_record.pdf) : scatter_record.pdf;

    const Vec3 direction = mixed_pdf->Generate(generator);
    const Ray scattered(record.p, direction, r.time());
    const double pdf_value = mixed_pdf->Value(scattered.direction());
    if (pdf_value <= 0.0) {
        return emitted;
    }

    const double scattering_pdf = record.material->ScatteringPdf(r, record, scattered);
    const Color recursive = RayColor(scattered, depth - 1, world, lights, generator);
    return emitted + scatter_record.attenuation * scattering_pdf * recursive / pdf_value;
}

void WriteColor(std::ostringstream& output, const Color& pixel_color) {
    const Color gamma_corrected(std::sqrt(pixel_color.x()), std::sqrt(pixel_color.y()), std::sqrt(pixel_color.z()));
    const int ir = ToChannel(gamma_corrected.x());
    const int ig = ToChannel(gamma_corrected.y());
    const int ib = ToChannel(gamma_corrected.z());
    output << ir << ' ' << ig << ' ' << ib << "\n";
}

HittableList BuildCornellSmoke(HittableList& lights) {
    HittableList world;

    const auto red = std::make_shared<Lambertian>(Color(0.65, 0.05, 0.05));
    const auto white = std::make_shared<Lambertian>(Color(0.73, 0.73, 0.73));
    const auto green = std::make_shared<Lambertian>(Color(0.12, 0.45, 0.15));
    const auto light = std::make_shared<DiffuseLight>(Color(15.0, 15.0, 15.0));

    world.Add(std::make_shared<Quad>(Point3(555.0, 0.0, 0.0), Vec3(0.0, 0.0, 555.0), Vec3(0.0, 555.0, 0.0), green));
    world.Add(std::make_shared<Quad>(Point3(0.0, 0.0, 0.0), Vec3(0.0, 555.0, 0.0), Vec3(0.0, 0.0, 555.0), red));
    const auto ceiling_light = std::make_shared<Quad>(Point3(213.0, 554.0, 227.0), Vec3(130.0, 0.0, 0.0), Vec3(0.0, 0.0, 105.0), light);
    world.Add(ceiling_light);
    lights.Add(ceiling_light);
    world.Add(std::make_shared<Quad>(Point3(0.0, 555.0, 0.0), Vec3(555.0, 0.0, 0.0), Vec3(0.0, 0.0, 555.0), white));
    world.Add(std::make_shared<Quad>(Point3(0.0, 0.0, 0.0), Vec3(555.0, 0.0, 0.0), Vec3(0.0, 0.0, 555.0), white));
    world.Add(std::make_shared<Quad>(Point3(0.0, 0.0, 555.0), Vec3(555.0, 0.0, 0.0), Vec3(0.0, 555.0, 0.0), white));

    std::shared_ptr<Hittable> short_box = std::make_shared<Box>(Point3(0.0, 0.0, 0.0), Point3(165.0, 165.0, 165.0), white);
    short_box = std::make_shared<RotateY>(short_box, -18.0);
    short_box = std::make_shared<Translate>(short_box, Vec3(130.0, 0.0, 65.0));
    world.Add(std::make_shared<ConstantMedium>(short_box, 0.01, Color(0.0, 0.0, 0.0)));

    std::shared_ptr<Hittable> tall_box = std::make_shared<Box>(Point3(0.0, 0.0, 0.0), Point3(165.0, 330.0, 165.0), white);
    tall_box = std::make_shared<RotateY>(tall_box, 15.0);
    tall_box = std::make_shared<Translate>(tall_box, Vec3(265.0, 0.0, 295.0));
    world.Add(std::make_shared<ConstantMedium>(tall_box, 0.01, Color(1.0, 1.0, 1.0)));

    return world;
}

}  // namespace

std::string RenderMaterialImage(const RenderOptions& options) {
    const double aspect_ratio = static_cast<double>(options.width) / static_cast<double>(options.height);
    const Point3 look_from(278.0, 278.0, -800.0);
    const Point3 look_at(278.0, 278.0, 0.0);
    const Vec3 vup(0.0, 1.0, 0.0);
    const double focus_dist = (look_from - look_at).length();
    const Camera camera(look_from, look_at, vup, options.vertical_fov_degrees, aspect_ratio, options.aperture, focus_dist,
                       options.shutter_open_time, options.shutter_close_time);

    HittableList lights;
    HittableList world = BuildCornellSmoke(lights);
    const std::shared_ptr<BvhNode> bvh_tree =
        world.Objects().empty() ? nullptr : std::make_shared<BvhNode>(world, options.shutter_open_time, options.shutter_close_time);
    const Hittable& world_view = bvh_tree ? static_cast<const Hittable&>(*bvh_tree) : static_cast<const Hittable&>(world);
    const std::shared_ptr<Hittable> lights_view = lights.Objects().empty() ? nullptr : std::make_shared<HittableList>(lights);

    std::mt19937 generator(options.seed);

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
                                     : (static_cast<double>(x) + RandomDouble(generator)) /
                                           (static_cast<double>(options.width) - 1.0);
                const double v = (options.height == 1)
                                     ? 0.5
                                     : (static_cast<double>(options.height - 1 - y) + RandomDouble(generator)) /
                                           (static_cast<double>(options.height) - 1.0);

                const Ray r = camera.GetRay(u, v, generator);
                pixel_color += RayColor(r, options.max_depth, world_view, lights_view, generator);
            }

            const Color averaged_color = pixel_color / static_cast<double>(options.samples_per_pixel);
            WriteColor(output, averaged_color);
        }
    }

    return output.str();
}

}  // namespace raytracer
