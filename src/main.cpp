/*
 * 설명: CLI 인자를 해석해 구 노멀 기반 PPM을 결정적으로 생성하고 출력한다.
 * 버전: v0.2.0
 * 관련 문서: design/protocol/contract.md, design/renderer/v0.2.0-sphere-hit.md
 * 테스트: tests/integration/ppm_integration_test.cpp
 */
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#include "raytracer/ppm.hpp"

namespace {

bool HasNext(int argc, int index) { return index + 1 < argc; }

int ParseOptions(int argc, char* argv[], raytracer::RenderOptions& options, std::string& output_path) {
    output_path = "-";
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--width") {
            if (!HasNext(argc, i)) {
                std::cerr << "오류: --width 옵션에 값이 필요하다." << std::endl;
                return 1;
            }
            try {
                options.width = std::stoi(argv[++i]);
            } catch (const std::exception&) {
                std::cerr << "오류: --width 값은 정수여야 한다." << std::endl;
                return 1;
            }
        } else if (arg == "--height") {
            if (!HasNext(argc, i)) {
                std::cerr << "오류: --height 옵션에 값이 필요하다." << std::endl;
                return 1;
            }
            try {
                options.height = std::stoi(argv[++i]);
            } catch (const std::exception&) {
                std::cerr << "오류: --height 값은 정수여야 한다." << std::endl;
                return 1;
            }
        } else if (arg == "--output") {
            if (!HasNext(argc, i)) {
                std::cerr << "오류: --output 옵션에 경로가 필요하다." << std::endl;
                return 1;
            }
            output_path = argv[++i];
        } else {
            std::cerr << "오류: 지원하지 않는 옵션." << std::endl;
            return 1;
        }
    }

    if (options.width < 1 || options.height < 1) {
        std::cerr << "오류: 해상도는 1 이상 정수만 허용한다." << std::endl;
        return 1;
    }

    return 0;
}

}  // namespace

int main(int argc, char* argv[]) {
    raytracer::RenderOptions options;
    std::string output_path;

    const int parse_result = ParseOptions(argc, argv, options, output_path);
    if (parse_result != 0) {
        return parse_result;
    }

    const std::string image = raytracer::RenderSphereNormalImage(options);

    if (output_path == "-") {
        std::cout << image;
        return 0;
    }

    std::ofstream file(output_path, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        std::cerr << "오류: 파일을 열 수 없다." << std::endl;
        return 2;
    }

    file << image;
    if (!file) {
        std::cerr << "오류: 파일 기록에 실패했다." << std::endl;
        return 2;
    }

    return 0;
}
