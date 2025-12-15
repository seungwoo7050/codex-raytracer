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
        "185 203 185\n"
        "170 191 134\n"
        "199 217 180\n"
        "141 161 40\n"
        "139 153 72\n"
        "136 162 57\n";

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
