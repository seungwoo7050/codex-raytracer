# ray-tracer (C++17)

ASCII PPM(P3) 이미지를 출력하는 교육용 CPU 레이트레이서다. v0.8.0에서는 Quad/Box로 구성된 Cornell Box를 area light와 함께 BVH로 가속해 결정적으로
렌더링하며 Lambertian/Metal/Dielectric/발광 재질과 Translate/RotateY 변환을 지원한다. CLI 규약과 출력 형식은 `design/protocol/contract.md`를 따른다.

## 빠른 시작
- 빌드: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build`
- 테스트: `ctest --test-dir build --output-on-failure`
- 실행: `./build/raytracer --width 256 --height 256 --spp 10 --max-depth 20 --seed 1 > output.ppm`
- BVH 벤치마크: `./build/bvh_benchmark`

자세한 안내는 `CLONE_GUIDE.md`를 참고한다.
