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
        "200 218 180\n"
        "184 206 128\n"
        "199 217 180\n"
        "159 180 0\n"
        "169 180 57\n"
        "168 195 0\n";

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
