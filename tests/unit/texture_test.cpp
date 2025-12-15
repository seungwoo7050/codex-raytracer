#include <gtest/gtest.h>

#include <vector>

#include "raytracer/texture.hpp"

TEST(TextureTest, CheckerTextureAlternatesBetweenColors) {
    const raytracer::Color even_color(1.0, 1.0, 1.0);
    const raytracer::Color odd_color(0.1, 0.2, 0.3);
    raytracer::CheckerTexture texture(even_color, odd_color, 5.0);

    const raytracer::Point3 even_point(0.1, 0.1, 0.1);
    const raytracer::Point3 odd_point(0.1, 0.1, -0.1);

    const raytracer::Color even_sample = texture.Value(0.0, 0.0, even_point);
    const raytracer::Color odd_sample = texture.Value(0.0, 0.0, odd_point);

    EXPECT_DOUBLE_EQ(even_sample.x(), even_color.x());
    EXPECT_DOUBLE_EQ(even_sample.y(), even_color.y());
    EXPECT_DOUBLE_EQ(even_sample.z(), even_color.z());

    EXPECT_DOUBLE_EQ(odd_sample.x(), odd_color.x());
    EXPECT_DOUBLE_EQ(odd_sample.y(), odd_color.y());
    EXPECT_DOUBLE_EQ(odd_sample.z(), odd_color.z());
}

TEST(TextureTest, NoiseTextureValuesStayWithinUnitRange) {
    const raytracer::NoiseTexture texture(1.5);
    const std::vector<raytracer::Point3> samples = {
        raytracer::Point3(0.0, 0.0, 0.0),
        raytracer::Point3(0.5, 0.5, 0.5),
        raytracer::Point3(1.2, 3.4, 5.6),
    };

    for (const auto& point : samples) {
        const raytracer::Color value = texture.Value(0.0, 0.0, point);
        EXPECT_GE(value.x(), 0.0);
        EXPECT_GE(value.y(), 0.0);
        EXPECT_GE(value.z(), 0.0);
        EXPECT_LE(value.x(), 1.0);
        EXPECT_LE(value.y(), 1.0);
        EXPECT_LE(value.z(), 1.0);
    }
}
