# CLONE_GUIDE.md

## 개요
`ray-tracer`를 로컬에서 재현하기 위한 최소 절차를 요약한다. v0.2.0에서는 단일 구의 노멀을 색상으로 표현한 이미지를 렌더링한다.

> 주의: 렌더 산출물(PPM/PNG)은 **커밋하지 않는다**(gitignore).

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
./build/raytracer --width 256 --height 256 > output.ppm
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
