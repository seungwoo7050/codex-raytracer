/*
 * 설명: PPM(P3) 규격으로 구 노멀 기반 색상을 렌더링한다.
 * 버전: v0.2.0
 * 관련 문서: design/protocol/contract.md, design/renderer/v0.2.0-sphere-hit.md
 * 테스트: tests/integration/ppm_integration_test.cpp
 */
#pragma once

#include <string>

namespace raytracer {

struct RenderOptions {
    int width = 256;
    int height = 256;
};

std::string RenderSphereNormalImage(const RenderOptions& options);

}  // namespace raytracer
