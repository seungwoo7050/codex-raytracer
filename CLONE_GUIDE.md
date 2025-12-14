# CLONE_GUIDE.md

## 개요
`ray-tracer`를 로컬에서 빌드·테스트·렌더 실행까지 재현하는 가이드다.

> 주의: 렌더 산출물(PPM/PNG)은 **커밋하지 않는다**(gitignore).

---

## 요구 사항
- CMake 3.20+
- C++17 컴파일러 (GCC/Clang)
- (테스트) GoogleTest는 CMake에서 FetchContent 또는 서브모듈 등 텍스트 방식으로 구성(프로젝트 정책에 따름)

---

## 빌드
```bash
mkdir -p build
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

---

## 테스트 실행
```bash
ctest --test-dir build --output-on-failure
```

---

## 실행(렌더)
기본 실행(예시):
```bash
./build/raytracer > output.ppm
```

(선택) 출력 파일 지정 옵션이 있는 경우:
```bash
./build/raytracer --output output.ppm
```

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
랜덤을 쓰는 버전부터는, 항상 고정 시드를 사용하면 비교가 쉬워진다.
- 예: `--seed 1234`
- 정확한 규칙은 `design/protocol/contract.md`를 정본으로 한다.
