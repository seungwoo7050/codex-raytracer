# AGENTS.md
Repository seed rules for AI coding agents (C++17 CPU Ray Tracer CLI).

> NOTE
> - This file is written in English for AI/tooling.
> - All generated docs and ALL source code comments MUST be written in Korean.

---

## 0) Seed vs Generated

### Seed (must exist at repo init)
- README.md (human overview)
- AGENTS.md
- STACK_DESIGN.md
- PRODUCT_SPEC.md
- CODING_GUIDE.md
- VERSIONING.md
- CLONE_GUIDE.md (human, Korean; keep updated)

### Generated during development
- src/, include/, tests/
- design/** (Korean design docs)
- tools/** (text-only helper scripts)
- third_party/** (header-only, text-only, if needed)

---

## 1) Mandatory read order
Agents MUST read in this order before doing anything:
1. AGENTS.md
2. STACK_DESIGN.md
3. PRODUCT_SPEC.md
4. CODING_GUIDE.md
5. VERSIONING.md
6. VERSION_PROMPTS.md
7. README.md

---

## 2) Hard constraints

### 2.1 Language policy (MANDATORY)
- Code identifiers: English.
- ALL source code comments: Korean.
- ALL human-facing docs created/updated by agents: Korean.
  - CLONE_GUIDE.md
  - design/**

This seed file may remain English.

### 2.2 Binary file policy (MANDATORY)
- Agents MUST NOT add or commit binary files.
- "Binary" means non-text files (archives, images, compiled outputs, etc.).
- Do NOT commit rendered outputs: *.ppm, *.png, *.jpg, *.exr
- Benchmarks/logs must remain local.

### 2.3 Stack immutability
Agents MUST follow STACK_DESIGN.md.
- No major dependency additions without explicit human instruction + seed doc update.

### 2.4 One target version per change set
Every change set MUST target exactly one version from VERSIONING.md.

---

## 3) External contract policy (MANDATORY)

### 3.1 Canonical contract file (fixed path)
The ONLY canonical external interface definition file is:
- `design/protocol/contract.md` (Korean)

This contract MUST be created in v0.1.0 and MUST be updated whenever any external interface changes:
- CLI arguments (scene/width/spp/depth/seed/output)
- Output format details (PPM header, color space/gamma, clamping)
- Exit codes / error messages (minimal)

### 3.2 Contract-first rule
If you introduce or change any external behavior:
1) Update `design/protocol/contract.md` FIRST
2) Then implement code
3) Then add tests that validate the contract

---

## 4) Testing policy (MANDATORY)

### 4.1 Do not rely on unit tests only
When CLI/output behavior changes, provide BOTH:
- Unit tests (math, intersections, PDFs)
- Integration tests (render a tiny deterministic image and compare)

### 4.2 Determinism requirement
- All tests that rely on randomness MUST use a fixed seed.
- Determinism rules must be documented in contract.md.

---

## 5) Standard development loop (MANDATORY)
1) Select one target version from VERSIONING.md
2) Plan changes (list files to create/modify)
3) If external interface changes:
   - update design/protocol/contract.md first
4) Implement (Korean comments)
5) Add/update tests (unit + integration)
6) Run tests and fix until green
7) Generate/update docs (Korean):
   - CLONE_GUIDE.md
   - design/**
8) Update VERSIONING.md status

---

## 6) Missing-info policy
- For internal details not specified: choose the minimal safe option without expanding scope.
- For external interfaces: you MUST document them in contract.md; do not improvise silently.
