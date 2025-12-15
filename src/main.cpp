/*
 * 설명: CLI 인자를 해석해 Cornell smoke 장면을 BVH로 가속해 결정적으로 렌더링한다.
 * 버전: v0.9.0
 * 관련 문서: design/protocol/contract.md, design/renderer/v0.8.0-cornell.md, design/renderer/v0.9.0-volume.md
 * 테스트: tests/integration/ppm_integration_test.cpp
 */
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
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
        } else if (arg == "--spp") {
            if (!HasNext(argc, i)) {
                std::cerr << "오류: --spp 옵션에 값이 필요하다." << std::endl;
                return 1;
            }
            try {
                options.samples_per_pixel = std::stoi(argv[++i]);
            } catch (const std::exception&) {
                std::cerr << "오류: --spp 값은 정수여야 한다." << std::endl;
                return 1;
            }
        } else if (arg == "--max-depth") {
            if (!HasNext(argc, i)) {
                std::cerr << "오류: --max-depth 옵션에 값이 필요하다." << std::endl;
                return 1;
            }
            try {
                options.max_depth = std::stoi(argv[++i]);
            } catch (const std::exception&) {
                std::cerr << "오류: --max-depth 값은 정수여야 한다." << std::endl;
                return 1;
            }
        } else if (arg == "--seed") {
            if (!HasNext(argc, i)) {
                std::cerr << "오류: --seed 옵션에 값이 필요하다." << std::endl;
                return 1;
            }
            try {
                const unsigned long parsed = std::stoul(argv[++i]);
                if (parsed > std::numeric_limits<std::uint32_t>::max()) {
                    std::cerr << "오류: --seed 값이 32비트 범위를 초과했다." << std::endl;
                    return 1;
                }
                options.seed = static_cast<std::uint32_t>(parsed);
            } catch (const std::exception&) {
                std::cerr << "오류: --seed 값은 0 이상 정수여야 한다." << std::endl;
                return 1;
            }
        } else {
            std::cerr << "오류: 지원하지 않는 옵션." << std::endl;
            return 1;
        }
    }

    if (options.width < 1 || options.height < 1) {
        std::cerr << "오류: 해상도는 1 이상 정수만 허용한다." << std::endl;
        return 1;
    }

    if (options.samples_per_pixel < 1) {
        std::cerr << "오류: --spp 값은 1 이상 정수여야 한다." << std::endl;
        return 1;
    }

    if (options.max_depth < 1) {
        std::cerr << "오류: --max-depth 값은 1 이상 정수여야 한다." << std::endl;
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

    const std::string image = raytracer::RenderMaterialImage(options);

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
