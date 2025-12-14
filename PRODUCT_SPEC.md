# PRODUCT_SPEC.md
What to build (portfolio-oriented C++17 CPU ray tracer).

> NOTE
> - This file is written in English for AI/tooling.
> - All generated docs and code comments MUST be Korean.

---

## 1) Product goal (portfolio)
Demonstrate capability in:
- 3D math implementation (Vec3, transforms)
- Geometry intersection routines
- Monte Carlo sampling basics and variance reduction (later versions)
- Clean modular C++17 design + tests

This is an offline educational renderer inspired by the "Ray Tracing in One Weekend" series.

---

## 2) Functional scope (intentionally staged)

### Included (core)
- PPM(P3) output
- Vec3/Ray/Camera fundamentals
- Sphere intersection
- Anti-aliasing (multi-sampling)
- Materials: Lambertian / Metal / Dielectric

### Included (advanced, scheduled by VERSIONING)
- Defocus blur (depth of field)
- Motion blur (moving sphere)
- BVH acceleration
- Textures (checker, Perlin noise, image)
- Quads + area lights
- Volumes (smoke/fog)
- Importance sampling + PDFs + Mixture sampling

### Explicitly excluded
- GPU backends
- Real-time rasterization pipeline
- Image assets committed to repo (no committed PNG/JPG outputs)

---

## 3) Non-functional requirements (must be demonstrable)
- Determinism:
  - fixed seed => same PPM output for test scenes
- Testability:
  - unit tests for math/geometry
  - integration tests for small deterministic renders
- Performance:
  - basic profiling/benchmarks are OK, but keep outputs text-only

---

## 4) Definition of Done (per version)
A version is DONE only if:
- VERSIONING.md status updated
- contract.md matches implementation
- tests are green
- CLONE_GUIDE.md contains exact reproduction steps
