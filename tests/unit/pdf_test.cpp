#include <gtest/gtest.h>

#include <memory>
#include <random>

#include "raytracer/material.hpp"
#include "raytracer/pdf.hpp"
#include "raytracer/quad.hpp"
#include "raytracer/vec3.hpp"

TEST(PdfTest, CosinePdfValueMatchesNormalDirection) {
    raytracer::CosinePdf pdf(raytracer::Vec3(0.0, 0.0, 1.0));
    const double expected = 1.0 / 3.1415926535897932385;

    EXPECT_NEAR(pdf.Value(raytracer::Vec3(0.0, 0.0, 1.0)), expected, 1e-12);

    std::mt19937 generator(42);
    const raytracer::Vec3 generated = pdf.Generate(generator);
    EXPECT_GT(generated.z(), 0.0);
}

TEST(PdfTest, QuadPdfValueUsesGeometry) {
    const auto white = std::make_shared<raytracer::Lambertian>(raytracer::Color(1.0, 1.0, 1.0));
    raytracer::Quad quad(raytracer::Point3(0.0, 0.0, 0.0), raytracer::Vec3(1.0, 0.0, 0.0), raytracer::Vec3(0.0, 1.0, 0.0), white);

    const double pdf = quad.PdfValue(raytracer::Point3(0.5, 0.5, -1.0), raytracer::Vec3(0.0, 0.0, 1.0));
    EXPECT_NEAR(pdf, 1.0, 1e-12);

    std::mt19937 generator(7);
    const raytracer::Vec3 random_direction = quad.Random(raytracer::Point3(0.5, 0.5, -1.0), generator);
    EXPECT_GT(raytracer::Dot(random_direction, raytracer::Vec3(0.0, 0.0, 1.0)), 0.0);
}

TEST(PdfTest, MixturePdfBlendsComponents) {
    const auto cosine = std::make_shared<raytracer::CosinePdf>(raytracer::Vec3(0.0, 0.0, 1.0));
    const auto uniform = std::make_shared<raytracer::UniformSpherePdf>();
    raytracer::MixturePdf mixture(cosine, uniform);

    const double reference = 0.5 * (cosine->Value(raytracer::Vec3(0.0, 0.0, 1.0)) + uniform->Value(raytracer::Vec3(0.0, 0.0, 1.0)));
    EXPECT_NEAR(mixture.Value(raytracer::Vec3(0.0, 0.0, 1.0)), reference, 1e-12);
}
