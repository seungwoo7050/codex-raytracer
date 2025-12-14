#include <gtest/gtest.h>

#include "raytracer/vec3.hpp"

TEST(Vec3Test, AdditionAndScalarMultiplication) {
    raytracer::Vec3 a(1.0, 2.0, 3.0);
    raytracer::Vec3 b(4.0, -1.0, 0.5);

    auto sum = a + b;
    EXPECT_DOUBLE_EQ(sum.x(), 5.0);
    EXPECT_DOUBLE_EQ(sum.y(), 1.0);
    EXPECT_DOUBLE_EQ(sum.z(), 3.5);

    auto scaled = 2.0 * a;
    EXPECT_DOUBLE_EQ(scaled.x(), 2.0);
    EXPECT_DOUBLE_EQ(scaled.y(), 4.0);
    EXPECT_DOUBLE_EQ(scaled.z(), 6.0);
}
