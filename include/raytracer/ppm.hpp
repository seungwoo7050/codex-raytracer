/*
 * 설명: PPM(P3) 규격으로 구 노멀 기반 색상을 렌더링한다.
 * 버전: v0.3.0
 * 관련 문서: design/protocol/contract.md, design/renderer/v0.3.0-camera-sampling.md
 * 테스트: tests/integration/ppm_integration_test.cpp
 */
#pragma once

#include <cstdint>
#include <string>

namespace raytracer {

struct RenderOptions {
    int width = 256;
    int height = 256;
    int samples_per_pixel = 10;
    double vertical_fov_degrees = 90.0;
    std::uint32_t seed = 1;
};

std::string RenderSphereNormalImage(const RenderOptions& options);

}  // namespace raytracer
