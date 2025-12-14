#include <gtest/gtest.h>

#include "raytracer/ppm.hpp"

TEST(PpmIntegrationTest, GeneratesExpectedGradient) {
    raytracer::RenderOptions options;
    options.width = 4;
    options.height = 2;

    const std::string expected =
        "P3\n"
        "4 2\n"
        "255\n"
        "0 255 128\n"
        "85 255 128\n"
        "170 255 128\n"
        "255 255 128\n"
        "0 0 128\n"
        "85 0 128\n"
        "170 0 128\n"
        "255 0 128\n";

    const std::string actual = raytracer::GenerateGradientImage(options);

    EXPECT_EQ(actual, expected);
}
