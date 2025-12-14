# STACK_DESIGN.md
Authoritative stack and architecture baseline (C++17 CPU Ray Tracer).

> NOTE
> - This file is written in English for AI/tooling.
> - All generated docs and code comments MUST be Korean.

---

## 1) Target system
Single C++17 executable that:
- Renders an image offline (CPU) and writes to stdout or a file
- Primary output format: ASCII PPM (P3)
- Scene selection via CLI (later versions)

Non-goals (baseline):
- Real-time rendering
- GPU/CUDA/OptiX
- Complex file formats (EXR) as committed artifacts

---

## 2) Approved baseline stack
- Language: C++17
- Build: CMake 3.20+
- Tests: GoogleTest
- Optional (header-only, text-only) additions when scheduled:
  - stb_image.h for image texture loading

---

## 3) Core architecture (recommended modules)

### 3.1 Core math
- Vec3 / Point3 / Color
- Ray
- Interval / clamp utilities
- RNG (seedable, deterministic)

### 3.2 Rendering pipeline
- Camera (FOV, aspect, aperture/defocus)
- Sampler (multi-sampling, jitter)
- Integrator (rayColor / path tracing recursion)

### 3.3 Geometry & acceleration
- Hittable interface
- Sphere, Quad
- BVH
- Transform wrappers (Translate/Rotate)

### 3.4 Materials & textures
- Lambertian, Metal, Dielectric
- Emissive (DiffuseLight)
- Texture: solid, checker, noise, image
- Volume: ConstantMedium (later)

### 3.5 PDFs / importance sampling (later)
- CosinePDF, SpherePDF, HittablePDF, MixturePDF
- ONB

---

## 4) Determinism baseline
- Default build: single-thread rendering for easiest determinism.
- RNG must be explicit and seedable.
- For tests: render tiny images (e.g., 8x8) with fixed seed and assert exact output lines.

---

## 5) Repo layout (target)

```
ray-tracer/
  README.md
  AGENTS.md
  STACK_DESIGN.md
  PRODUCT_SPEC.md
  CODING_GUIDE.md
  VERSIONING.md
  VERSION_PROMPTS.md
  CLONE_GUIDE.md
  include/
  src/
    core/
    geometry/
    materials/
    textures/
    rendering/
    scenes/
    io/
    utils/
  tests/
    unit/
    integration/
  design/
    protocol/contract.md
    renderer/
  tools/
```
