#include <gtest/gtest.h>

#include "raytracer/ppm.hpp"

TEST(PpmIntegrationTest, GeneratesExpectedSphereNormals) {
    raytracer::RenderOptions options;
    options.width = 4;
    options.height = 2;

    const std::string expected =
        "P3\n"
        "4 2\n"
        "255\n"
        "165 201 255\n"
        "150 192 255\n"
        "150 192 255\n"
        "165 201 255\n"
        "217 232 255\n"
        "232 241 255\n"
        "232 241 255\n"
        "217 232 255\n";

    const std::string actual = raytracer::RenderSphereNormalImage(options);

    EXPECT_EQ(actual, expected);
}
