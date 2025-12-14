# ray-tracer (C++17)

ASCII PPM(P3) 이미지를 출력하는 교육용 CPU 레이트레이서다. v0.2.0에서는 단일 구에 대한 노멀 기반 색상을 결정적으로 렌더링하며, CLI 규약과 출력 형식은 `design/protocol/contract.md`를 따른다.

## 빠른 시작
- 빌드: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build`
- 테스트: `ctest --test-dir build --output-on-failure`
- 실행: `./build/raytracer --width 256 --height 256 > output.ppm`

자세한 안내는 `CLONE_GUIDE.md`를 참고한다.
