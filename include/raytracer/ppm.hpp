/*
 * 설명: PPM(P3) 형식의 그라디언트 이미지를 생성한다.
 * 버전: v0.1.0
 * 관련 문서: design/protocol/contract.md
 * 테스트: tests/integration/ppm_integration_test.cpp
 */
#pragma once

#include <string>

namespace raytracer {

struct RenderOptions {
    int width = 256;
    int height = 256;
};

std::string GenerateGradientImage(const RenderOptions& options);

}  // namespace raytracer
