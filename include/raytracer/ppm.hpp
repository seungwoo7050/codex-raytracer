/*
 * 설명: Cornell Box 기반 장면을 BVH로 가속해 Quad/박스/발광 재질을 PPM(P3) 규격으로 렌더링한다.
 * 버전: v0.8.0
 * 관련 문서: design/protocol/contract.md, design/renderer/v0.8.0-cornell.md
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
    double vertical_fov_degrees = 40.0;
    int max_depth = 20;
    std::uint32_t seed = 1;
    double aperture = 0.0;
    double shutter_open_time = 0.0;
    double shutter_close_time = 0.0;
};

std::string RenderMaterialImage(const RenderOptions& options);

}  // namespace raytracer
