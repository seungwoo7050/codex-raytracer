# CLONE_GUIDE.md

## 개요
`ray-tracer`를 로컬에서 재현하기 위한 최소 절차를 요약한다. v1.0.0에서는 Cornell Box 내부에 두 개의 ConstantMedium 볼륨을 배치한 Cornell smoke
장면을 BVH로 가속하고 Cosine/Hittable/Mixture PDF를 활용해 천장 광원을 직접 샘플링하며 Translate/RotateY 변환, 발광 재질, Isotropic 위상 함수를 포함한다.

> 주의: 렌더 산출물(PPM/PNG)과 벤치마크 결과는 **커밋하지 않는다**(gitignore).

---

## 요구 사항
- CMake 3.20+
- C++17 컴파일러 (GCC/Clang)
- 인터넷(테스트용 GoogleTest FetchContent)

---

## 빌드 (1줄)
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build
```

## 테스트 (1줄)
```bash
ctest --test-dir build --output-on-failure
```

## 실행 (1줄)
```bash
./build/raytracer --width 256 --height 256 --spp 10 --max-depth 20 --seed 1 > output.ppm
```
- 광원 직접 샘플링이 적용되어 있으므로 동일 시드를 유지하면 결과가 완전히 일치한다.

## BVH 벤치마크
텍스트로 hit 시간만 확인하는 비교 도구다.
```bash
./build/bvh_benchmark
```
> 결과 숫자는 참고용이며 파일로 저장하더라도 커밋하지 않는다.

---

## PPM 보기
PPM은 텍스트 이미지 포맷이다.
- Linux: ImageMagick `display output.ppm`
- macOS: Preview로 바로는 어려울 수 있어 PNG로 변환 권장

변환(로컬에서만):
```bash
convert output.ppm output.png
```

> `output.ppm`, `output.png`는 커밋 금지.

---

## 결정성(테스트/비교) 팁
- 모든 랜덤은 단일 `std::mt19937`로 직렬 소비된다. 고정 시드를 사용하면 PDF 샘플링까지 동일하게 재현된다.
- 예: `--seed 1234`
- 정확한 규칙은 `design/protocol/contract.md`를 정본으로 한다. 통합 테스트는 4x4 Cornell mini 결과 문자열을 그대로 비교한다.
