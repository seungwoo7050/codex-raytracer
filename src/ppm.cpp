/*
 * 설명: PPM(P3) 규격에 맞춰 결정적인 그라디언트 이미지를 생성한다.
 * 버전: v0.1.0
 * 관련 문서: design/protocol/contract.md
 * 테스트: tests/integration/ppm_integration_test.cpp
 */
#include "raytracer/ppm.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>

namespace raytracer {
namespace {

int ClampColor(int value) { return std::clamp(value, 0, 255); }

}  // namespace

std::string GenerateGradientImage(const RenderOptions& options) {
    std::ostringstream output;
    output << "P3\n";
    output << options.width << ' ' << options.height << "\n";
    output << "255\n";

    for (int y = 0; y < options.height; ++y) {
        for (int x = 0; x < options.width; ++x) {
            const double u = (options.width == 1)
                                 ? 0.0
                                 : static_cast<double>(x) / static_cast<double>(options.width - 1);
            const double v = (options.height == 1)
                                 ? 0.0
                                 : static_cast<double>(y) / static_cast<double>(options.height - 1);

            const int r = ClampColor(static_cast<int>(std::lround(255.0 * u)));
            const int g = ClampColor(static_cast<int>(std::lround(255.0 * (1.0 - v))));
            const int b = 128;

            output << r << ' ' << g << ' ' << b << "\n";
        }
    }

    return output.str();
}

}  // namespace raytracer
