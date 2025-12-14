# CODING_GUIDE.md
Coding rules for this repository (C++17 CPU ray tracer).

> NOTE
> - This file is written in English for AI/tooling.
> - All generated docs and code comments MUST be Korean.

---

## 1) Global rules

### 1.1 Language
- Identifiers: English.
- Comments: Korean only.

### 1.2 No binaries in repo
- Do not commit non-text files.
- Do not commit rendered outputs (*.ppm/*.png) or benchmark artifacts.

### 1.3 Required file header comment (Korean)
For significant `.hpp/.cpp` files add:
- 설명(역할)
- 버전: vX.Y.Z
- 관련 문서 경로(있으면)
- 테스트 경로(있으면)

Example:
/*
 * 설명: 카메라 레이 생성과 샘플링 좌표계를 제공한다.
 * 버전: v0.3.0
 * 관련 문서: design/renderer/v0.3.0-camera.md
 * 테스트: tests/unit/camera_test.cpp
 */

---

## 2) C++17 rules
- Standard: C++17 only.
- Prefer value types; avoid shared ownership unless necessary.
- Avoid global state (especially RNG).
- Use `constexpr` and small inline functions for hot math paths when appropriate.

---

## 3) Floating point & color rules
- Use double precision as baseline (`double`) unless a version explicitly changes it.
- Define and centralize:
  - clamp policy (0..0.999 or 0..1)
  - gamma correction (if used)
  - color write policy (linear→gamma)

---

## 4) RNG rules (determinism)
- Provide an explicit RNG class:
  - seedable
  - does not rely on `rand()`
- Pass RNG to functions that need randomness OR keep it as a member of the renderer.
- Tests must set a fixed seed.

---

## 5) Testing rules
### 5.1 Unit tests
- Vec3 operations (dot/cross/unit)
- Ray point-at-parameter
- Sphere hit & normal correctness

### 5.2 Integration tests
- Render a tiny image (e.g., 8x8) with fixed seed and a fixed scene.
- Compare:
  - the entire PPM string (small) OR
  - a stable hash of the PPM string (text-only)

---

## 6) Documentation rules
- External behavior changes => update `design/protocol/contract.md` first.
- After tests are green, update:
  - CLONE_GUIDE.md
  - design/renderer/vX.Y.Z-*.md (short is OK)
