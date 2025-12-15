#include <gtest/gtest.h>

#include "raytracer/ppm.hpp"

TEST(PpmIntegrationTest, GeneratesExpectedSphereNormalsWithSampling) {
    raytracer::RenderOptions options;
    options.width = 3;
    options.height = 2;
    options.samples_per_pixel = 4;
    options.seed = 1;

    const std::string expected =
        "P3\n"
        "3 2\n"
        "255\n"
        "136 184 255\n"
        "137 184 255\n"
        "149 191 255\n"
        "149 167 248\n"
        "195 213 254\n"
        "192 217 255\n";

    const std::string actual = raytracer::RenderSphereNormalImage(options);

    EXPECT_EQ(actual, expected);
}

TEST(PpmIntegrationTest, ProducesIdenticalImageWithSameSeed) {
    raytracer::RenderOptions options;
    options.width = 5;
    options.height = 3;
    options.samples_per_pixel = 6;
    options.seed = 1234;

    const std::string first = raytracer::RenderSphereNormalImage(options);
    const std::string second = raytracer::RenderSphereNormalImage(options);

    EXPECT_EQ(second, first);
}
