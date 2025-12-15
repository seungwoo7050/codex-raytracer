# ray-tracer (C++17)

ASCII PPM(P3) 이미지를 출력하는 교육용 CPU 레이트레이서다. v0.6.0에서는 AABB/BVH로 가속된 defocus blur 카메라와 셔터 시간으로 모션 블러가 적용된
moving sphere를 포함해 멀티샘플링 Lambertian/Metal/Dielectric 장면을 결정적으로 렌더링하며, CLI 규약과 출력 형식은 `design/protocol/contract.md`를 따른다.

## 빠른 시작
- 빌드: `cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build`
- 테스트: `ctest --test-dir build --output-on-failure`
- 실행: `./build/raytracer --width 256 --height 256 --spp 10 --max-depth 10 --seed 1 > output.ppm`
- BVH 벤치마크: `./build/bvh_benchmark`

자세한 안내는 `CLONE_GUIDE.md`를 참고한다.
