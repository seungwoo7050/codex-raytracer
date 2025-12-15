#include <gtest/gtest.h>

#include "raytracer/ppm.hpp"

TEST(PpmIntegrationTest, GeneratesExpectedMaterialImageWithSampling) {
    raytracer::RenderOptions options;
    options.width = 3;
    options.height = 2;
    options.samples_per_pixel = 4;
    options.max_depth = 5;
    options.seed = 1;

    const std::string expected =
        "P3\n"
        "3 2\n"
        "255\n"
        "186 216 255\n"
        "187 217 255\n"
        "195 221 255\n"
        "174 194 180\n"
        "131 149 156\n"
        "197 210 189\n";

    const std::string actual = raytracer::RenderMaterialImage(options);

    EXPECT_EQ(actual, expected);
}

TEST(PpmIntegrationTest, ProducesIdenticalImageWithSameSeed) {
    raytracer::RenderOptions options;
    options.width = 4;
    options.height = 3;
    options.samples_per_pixel = 3;
    options.max_depth = 6;
    options.seed = 42;

    const std::string first = raytracer::RenderMaterialImage(options);
    const std::string second = raytracer::RenderMaterialImage(options);

    EXPECT_EQ(second, first);
}
