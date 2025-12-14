# VERSION_PROMPTS.md

ray-tracer (C++17 CPU 레이트레이서)용 "버전별 복붙 프롬프트" 모음.

사용 방법:
- 아래에서 대상 버전 섹션(예: v0.4.0)을 통째로 복사해서 코딩 에이전트에게 전달한다.
- 한 번에 반드시 **1개 버전만** 진행한다(버전 혼합 금지).
- CLI/출력/결정성 규칙 등 외부 동작을 바꾸면 구현 전에 `design/protocol/contract.md`부터 고정한다.

---

## 병렬 개발(Modern IRC + Ray Tracer)용 마스터 프롬프트 (선택)

두 프로젝트를 "같은 0.1.0 스텝"으로 병렬 개발하려면 아래 템플릿을 사용한다.
- 멀티 워크스페이스(또는 2개 에이전트) 환경에서:
  - Workspace A: modern-irc 저장소
  - Workspace B: ray-tracer 저장소

```text
[Workspace A: modern-irc]
대상 버전: v0.X.0
→ modern-irc 저장소의 VERSION_PROMPTS.md에서 v0.X.0 섹션 전체를 실행하라.

[Workspace B: ray-tracer]
대상 버전: v0.X.0
→ ray-tracer 저장소의 VERSION_PROMPTS.md에서 v0.X.0 섹션 전체를 실행하라.

공통 금지:
- 버전 혼합 금지(각 저장소는 정확히 1개 버전만)
- 바이너리 커밋 금지
- 식별자 영어, 주석/사람용 문서 한국어
```

---

## 공통 프롬프트(항상 포함)

```text
레포 루트의 문서를 다음 순서로 모두 읽어라:
AGENTS.md → STACK_DESIGN.md → PRODUCT_SPEC.md → CODING_GUIDE.md → VERSIONING.md → VERSION_PROMPTS.md → README.md

반드시 VERSIONING.md에서 "버전 1개"만 선택해서 작업해라. (버전 혼합 금지)

바이너리 파일은 절대 커밋하지 마라.
코드 식별자는 영어, 모든 주석/사람용 문서는 한국어로 작성해라.

CLI/출력/결정성 규칙 등 외부 동작을 추가/변경하면 구현 전에
`design/protocol/contract.md`(한국어)부터 먼저 만들어/갱신해서 고정해라.
그 문서와 불일치하는 구현/테스트는 금지다.

작업 순서:
1) 계획(생성/수정 파일 목록)
2) (필요 시) contract.md 선 고정
3) 구현
4) 테스트(단위 + 통합)
5) 문서 생성/갱신(CLONE_GUIDE.md, design/*)
6) VERSIONING.md 상태 갱신
```

---

## v0.1.0 프롬프트 — 스켈레톤 + 계약 고정 + PPM 출력 + 통합 테스트

```text
대상 버전: v0.1.0

목표:
- CMake 프로젝트 스켈레톤을 만든다.
- PPM(P3) 포맷으로 "고정" 그라디언트 이미지를 출력한다.
- 구현 전에 `design/protocol/contract.md`(한국어)를 생성하고 아래를 고정한다:
  - CLI 최소 규약(입력 없음/기본값, 또는 --width/--height/--output 정책)
  - PPM(P3) 헤더/바디 형식(줄바꿈 규칙 포함)
  - 컬러 범위/클램프 정책(예: 0..255)
  - 결정성 규칙(이 버전은 랜덤 없음이 가장 안전)

테스트(필수):
- 단위 테스트 1개(예: Vec3 기초 연산)
- 통합 테스트 1개:
  - 실행 결과 PPM 문자열이 기대값과 정확히 일치(아주 작은 4x2)

산출 문서:
- CLONE_GUIDE.md(한국어): 빌드/테스트/실행 1줄씩
- design/renderer/v0.1.0-overview.md(한국어): 모듈 요약

완료 처리:
- VERSIONING.md v0.1.0 완료 표시

금지:
- 랜덤/재질/교차 등 v0.2.0 이상 기능 구현 금지
```

---

## v0.2.0 프롬프트 — Vec3/Ray + Sphere 교차 + 디버그 셰이딩

```text
대상 버전: v0.2.0

목표:
- Vec3, Ray를 구현한다.
- Sphere 교차(hit)와 노멀 기반 디버그 컬러를 구현한다.

선행:
- 외부 출력이 바뀌면(contract.md) 먼저 갱신한다.

테스트(필수):
- 단위: sphere hit(정면/빗겨감) 케이스
- 통합: 작은 이미지 렌더 결과가 고정 seed 없이도 재현되게 기대값 비교

산출 문서:
- design/renderer/v0.2.0-sphere-hit.md
```

---

## v0.3.0 프롬프트 — 카메라 + 안티앨리어싱 + 최소 CLI

```text
대상 버전: v0.3.0

목표:
- Camera(FOV/종횡비) 구현
- 멀티샘플링 안티앨리어싱
- (필요 시) CLI: --width/--spp/--seed 도입

반드시 선행(CLI 추가 시):
- contract.md 갱신:
  - 각 플래그 이름/기본값/범위
  - --seed 규칙과 결정성 기대

테스트(필수):
- 통합: 동일 seed로 동일 PPM 재현(2회 실행 비교)

산출 문서:
- design/renderer/v0.3.0-camera-sampling.md
```

---

## v0.4.0 프롬프트 — 재질 1차 + 재귀 깊이 + 감마 정책

```text
대상 버전: v0.4.0

목표:
- Lambertian/Metal/Dielectric 구현
- rayColor 재귀 + maxDepth
- 감마 보정 정책 고정(도입 시 contract)

테스트(필수):
- 단위: 산란 방향/감쇠 최소 2개
- 통합: 고정 seed로 작은 이미지 재현

산출 문서:
- design/renderer/v0.4.0-materials.md
```

---

## v0.5.0 프롬프트 — Defocus blur + Motion blur

```text
대상 버전: v0.5.0

목표:
- defocus/aperture로 피사계 심도 구현
- moving sphere로 motion blur 구현

테스트(필수):
- 통합: 고정 seed로 결과 재현

산출 문서:
- design/renderer/v0.5.0-blur.md
```

---

## v0.6.0 프롬프트 — BVH + AABB + 텍스트 벤치

```text
대상 버전: v0.6.0

목표:
- AABB + BVH 구축
- BVH 유무 성능 비교(숫자만 텍스트로 출력/기록. 바이너리 산출물 금지)

테스트(필수):
- 단위: BVH hit이 원본 리스트 hit과 일치(몇 케이스)

산출 문서:
- design/renderer/v0.6.0-bvh.md
```

---

## v0.7.0 프롬프트 — 텍스처 시스템

```text
대상 버전: v0.7.0

목표:
- Solid/Checker/Noise 텍스처
- (선택) 이미지 텍스처 로더(stb_image.h) 추가

선행(이미지 로더 도입 시):
- STACK_DESIGN.md에 승인 추가(또는 이미 승인되어 있는지 확인)

테스트(필수):
- 단위: checker/noise 샘플 값 범위

산출 문서:
- design/renderer/v0.7.0-textures.md
```

---

## v0.8.0 프롬프트 — Quad/Light/Transform + Cornell

```text
대상 버전: v0.8.0

목표:
- Quad 프리미티브
- 발광 재질(Area light)
- Transform(translate/rotate)
- Cornell Box 씬

테스트(필수):
- 단위: quad hit
- 통합: cornell mini 씬 결정성(고정 seed)

산출 문서:
- design/renderer/v0.8.0-cornell.md
```

---

## v0.9.0 프롬프트 — Volume + Smoke

```text
대상 버전: v0.9.0

목표:
- ConstantMedium 볼륨
- Cornell smoke 씬

테스트(필수):
- 통합: 작은 smoke 이미지 스모크(고정 seed)

산출 문서:
- design/renderer/v0.9.0-volume.md
```

---

## v1.0.0 프롬프트 — PDF/중요도 샘플링 + 안정화

```text
대상 버전: v1.0.0

목표:
- PDF(Cosine/Sphere/Hittable/Mixture) + ONB
- 광원 직접 샘플링 + Mixture로 분산 감소
- contract/test/docs 정리(동결)

테스트(필수):
- 단위: PDF value/generate 기본 성질
- 통합: 동일 seed로 Cornell mini 결과 재현

산출 문서:
- design/renderer/v1.0.0-overview.md
- CLONE_GUIDE.md 최종 정리

완료 처리:
- VERSIONING.md v1.0.0 완료 표시
- known limitations(3줄 이내)
```
