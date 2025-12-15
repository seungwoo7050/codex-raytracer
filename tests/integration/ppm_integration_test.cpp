#include <gtest/gtest.h>

#include "raytracer/ppm.hpp"

TEST(PpmIntegrationTest, RendersCornellMiniSceneDeterministically) {
    raytracer::RenderOptions options;
    options.width = 4;
    options.height = 4;
    options.samples_per_pixel = 4;
    options.max_depth = 10;
    options.seed = 3;

    const std::string expected =
        "P3\n"
        "4 4\n"
        "255\n"
        "0 0 0\n"
        "0 0 0\n"
        "0 0 0\n"
        "0 0 0\n"
        "0 0 0\n"
        "255 255 255\n"
        "0 0 0\n"
        "0 0 0\n"
        "0 0 0\n"
        "0 0 0\n"
        "0 0 0\n"
        "0 0 0\n"
        "0 0 0\n"
        "73 39 23\n"
        "0 0 0\n"
        "0 0 0\n";

    const std::string actual = raytracer::RenderMaterialImage(options);

    EXPECT_EQ(actual, expected);
}

TEST(PpmIntegrationTest, ProducesIdenticalCornellImageWithSameSeed) {
    raytracer::RenderOptions options;
    options.width = 3;
    options.height = 3;
    options.samples_per_pixel = 2;
    options.max_depth = 8;
    options.seed = 11;

    const std::string first = raytracer::RenderMaterialImage(options);
    const std::string second = raytracer::RenderMaterialImage(options);

    EXPECT_EQ(second, first);
}
