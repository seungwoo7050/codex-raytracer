# VERSIONING.md

ray-tracer (C++17 CPU 레이트레이서) 버전 로드맵.

원칙:
- 한 번의 변경 세트(커밋/PR)는 **정확히 1개 버전**만 타겟으로 한다.
- CLI/출력/결정성 규칙 등 외부 동작이 바뀌면 **구현 전에** `design/protocol/contract.md`부터 갱신한다.
- 렌더 결과 이미지(PPM/PNG 등)와 벤치마크 산출물은 **커밋 금지**.

---

## 상태 표기
- ⬜ 계획(Planned)
- 🟨 진행중(In progress)
- ✅ 완료(Done)

---

## 버전 로드맵 (0.1.0 단위)

### v0.1.0 — 스켈레톤 + 계약 고정 + PPM 출력 + 테스트 러너
- 상태: ✅
- 목표:
  - CMake 기반 빌드 스켈레톤 + 실행 파일 1개
  - `design/protocol/contract.md` 생성(최소: CLI, PPM 포맷, 시드 규칙)
  - PPM(P3)로 간단한 그라디언트 이미지 출력(고정)
- 필수 테스트:
  - 단위 1개(예: Vec3 더하기/스칼라 곱)
  - 통합 1개: 4x2 PPM 문자열과 정확히 일치

### v0.2.0 — Vec3/Ray + Sphere 교차 + 단색 셰이딩
- 상태: ✅
- 목표:
  - Vec3, Ray 구현
  - Sphere 교차 + 노멀 기반 색상(디버그 셰이딩)
- 필수 테스트:
  - Sphere hit 단위 테스트
  - 작은 이미지 1장 통합 테스트

### v0.3.0 — 카메라 + 뷰포트 + 안티앨리어싱
- 상태: ✅
- 목표:
  - Camera(FOV/종횡비) 구현
  - 멀티샘플링(안티앨리어싱)
  - CLI: --width/--spp/--seed 최소 도입(도입 시 contract 먼저)
- 필수 테스트:
  - 고정 seed로 동일 PPM 재현

### v0.4.0 — 재질 1차: Lambertian/Metal/Dielectric + 재귀 깊이
- 상태: 🟨
- 목표:
  - 재귀 rayColor + maxDepth
  - Lambertian/Metal/Dielectric
  - 감마 보정 정책 고정(도입 시 contract)
- 필수 테스트:
  - 재질 산란 단위 테스트 최소 2개

### v0.5.0 — Defocus blur + Motion blur
- 상태: 🟨
- 목표:
  - 피사계 심도(defocus/aperture)
  - moving sphere 기반 motion blur
- 필수 테스트:
  - 결정성 유지(고정 seed)

### v0.6.0 — BVH + AABB + 간단 벤치(텍스트)
- 상태: ⬜
- 목표:
  - AABB, BVH 구축
  - BVH 유무에 따른 성능 측정(숫자만 텍스트로 기록)
- 필수 테스트:
  - BVH hit 단위 테스트

### v0.7.0 — 텍스처: Checker/Noise/Image
- 상태: ⬜
- 목표:
  - Solid/Checker/Perlin noise
  - (선택) stb_image로 이미지 텍스처 로딩
- 필수 테스트:
  - Noise/Checker 샘플링 단위 테스트

### v0.8.0 — Quad/Light/Transform + Cornell Box
- 상태: ⬜
- 목표:
  - Quad 프리미티브
  - 발광 재질(Area light)
  - Translate/Rotate 인스턴싱
  - Cornell Box 기본 씬
- 필수 테스트:
  - Quad hit 단위 테스트

### v0.9.0 — Volume(ConstantMedium) + Smoke 씬
- 상태: ⬜
- 목표:
  - ConstantMedium
  - Cornell smoke 씬
- 필수 테스트:
  - 볼륨 샘플링 스모크(작은 이미지)

### v1.0.0 — PDF/중요도 샘플링 + 안정화
- 상태: ⬜
- 목표:
  - PDF(Cosine/Sphere/Hittable/Mixture), ONB
  - 광원 직접 샘플링 + Mixture
  - contract/test/docs 정리(동결)
- 필수 테스트:
  - 작은 Cornell 씬 결정성 통합 테스트

---

## Known limitations (기록)
- (비워둠) 각 버전 완료 시 여기에 "현재 미지원"을 짧게 기록한다.
