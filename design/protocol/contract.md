# design/protocol/contract.md

## 목적
v0.4.0에서 외부와 맞닿는 인터페이스를 고정한다. Lambertian/Metal/Dielectric 재질, 재귀 rayColor, 감마 보정, CLI 규약, PPM 출력 형식, 결정성 규칙을 본 문서에 정의하며 구현과 테스트는 본 규약을 기준으로 한다.

## 대상 버전
- 버전: v0.4.0
- 범위: 다중 구 장면을 Lambertian/Metal/Dielectric로 렌더링하는 CLI (카메라 FOV/종횡비 + 멀티샘플링 + 재귀 반사/굴절 + 감마 보정)

## CLI 규약
- 실행 파일: `raytracer`
- 입력이 없어도 동작하며 기본값으로 렌더링한다.
- 지원 옵션(순서 무관):
  - `--width <정수>`: 이미지 너비(픽셀). 기본값 256. 1 이상 정수만 허용.
  - `--height <정수>`: 이미지 높이(픽셀). 기본값 256. 1 이상 정수만 허용.
  - `--spp <정수>`: 픽셀당 샘플 수(samples per pixel). 기본값 10. 1 이상 정수만 허용.
  - `--max-depth <정수>`: rayColor 재귀 최대 깊이. 기본값 10. 1 이상 정수만 허용하며 0 이하면 오류로 처리한다.
  - `--seed <정수>`: 난수 시드. 기본값 1. 0 이상 32비트 정수만 허용하며 동일 시드는 동일 결과를 보장한다.
  - `--output <경로>`: 출력 대상. 기본값 `-` 이며, `-`는 표준 출력으로 기록한다. 파일 경로가 주어지면 동일 경로에 덮어쓴다.
- 잘못된 옵션이나 값(예: 누락된 파라미터, 허용 범위 밖 값) 입력 시:
  - 표준 오류로 한국어 오류 메시지를 한 줄 출력하고 종료 코드 1을 반환한다.
  - 어떠한 부분 출력도 생성하지 않는다.

## 카메라 및 샘플링 규약
- 카메라
  - 원점(origin): `(0, 0, 0)`
  - 초점 거리(focal length): `1.0`
  - 수직 시야각(vertical FOV): `90도`
  - 종횡비(aspect ratio): `width / height`
  - 뷰포트 높이: `2 * tan(FOV/2)`
  - 뷰포트 너비: `aspect_ratio * viewport_height`
  - 수평 벡터: `(viewport_width, 0, 0)`
  - 수직 벡터: `(0, viewport_height, 0)`
  - 좌하단 코너: `origin - horizontal/2 - vertical/2 - (0, 0, focal_length)`
- 픽셀 좌표계
  - 입력 픽셀 좌표 `(x, y)`에서 `y=0`은 상단 행을 의미한다.
- 멀티샘플링
  - 픽셀당 `samples_per_pixel`번 랜덤 샘플링한다.
  - 각 샘플에 대해 정규화 좌표를 계산한다.
    - `u = (width == 1) ? 0.5 : (x + rand01) / (width - 1)`
    - `v = (height == 1) ? 0.5 : (height - 1 - y + rand01) / (height - 1)`
    - `rand01`은 `[0, 1)` 범위 균일 분포 난수다.
  - 레이 방향: `dir = lower_left_corner + u * horizontal + v * vertical - origin`
  - 색상 샘플을 모두 합산한 뒤 `samples_per_pixel`로 나누어 평균을 만든다.

## RNG 및 결정성 규칙
- 난수 생성: `std::mt19937` + `std::uniform_real_distribution<double>(0.0, 1.0)`을 사용한다.
- 재질 산란에서도 동일 분포를 사용하며, 모든 난수는 하나의 생성기 인스턴스에서 직렬화되어 소비된다.
- 초기 시드: `--seed` 값으로 생성자를 초기화한다.
- 동일한 입력(옵션, 시드)에서는 항상 동일한 PPM 문자열을 생성한다.
- 테스트는 동일 시드 2회 실행 결과 문자열을 비교한다.

## rayColor 재귀 규약
- `max_depth`는 CLI/옵션으로 입력받는다. 재귀 깊이가 0 이하가 되면 `(0,0,0)`을 반환하여 추가 기여를 차단한다.
- 히트 시: 재질의 `Scatter`가 새로운 레이와 감쇠(attenuation)를 반환하면 `attenuation * rayColor(scattered, depth-1)`을 반환한다. 산란 실패 시 `(0,0,0)`을 반환한다.
- 미히트 시: 배경색은 기존 그라디언트 `c = (1.0 - t) * (1,1,1) + t * (0.5, 0.7, 1.0)`을 사용한다(단, `t = 0.5 * (unit_dir.y + 1.0)`).

## 재질 규약
- 공통: `Scatter`는 입력 레이, 교차 정보, RNG를 받아 산란 레이와 감쇠 색을 결정한다.
- Lambertian(diffuse):
  - 감쇠: 고정 알베도 색상.
  - 산란 방향: 법선 + 단위 구 내부 임의 벡터. 방향이 거의 0에 가까우면 법선을 사용한다.
- Metal:
  - 감쇠: 금속 알베도 색상.
  - 산란 방향: 입력 레이를 법선에 대해 반사한 뒤 `fuzz`(0~1) 배의 단위 구 무작위 벡터를 더한다. 산란 방향이 법선과 같은 쪽을 향하지 않으면 산란 실패로 간주한다.
- Dielectric:
  - 파라미터: 굴절률 `refraction_index`.
  - 법선 방향에 따라 굴절률 비율을 `eta = front_face ? (1.0 / refraction_index) : refraction_index`로 사용한다.
  - 전반사 조건 `eta * sin(theta) > 1`이면 반사만 수행한다.
  - 슐릭 근사 `R(theta) = r0 + (1-r0)*(1-cos(theta))^5`, `r0=((1-eta)/(1+eta))^2`를 사용해 반사/굴절을 선택한다.
  - 감쇠는 `(1,1,1)`을 사용한다.

## PPM(P3) 출력 규약
- 포맷: ASCII PPM (P3)
- 헤더:
  1. 첫 줄: `P3`
  2. 둘째 줄: `<width> <height>` (공백 하나 구분)
  3. 셋째 줄: `255`
- 본문:
  - 픽셀 순서: 위에서 아래로, 각 행은 왼쪽에서 오른쪽으로 진행한다.
  - 각 픽셀은 `R G B` 세 값으로 이루어진 한 줄로 기록하며, 값 사이에는 공백 하나만 둔다.
  - 모든 줄 끝에는 개행(\n)을 넣는다. 마지막 픽셀 뒤에도 개행을 유지한다.
- 색상 생성 규칙(결정적):
  - 장면: 고정된 4개 구를 사용한다.
    - `Sphere(center=(0, -100.5, -1), radius=100, material=Lambertian(albedo=(0.8, 0.8, 0.0)))`
    - `Sphere(center=(0, 0, -1), radius=0.5, material=Lambertian(albedo=(0.1, 0.2, 0.5)))`
    - `Sphere(center=(-1, 0, -1), radius=0.5, material=Dielectric(refraction_index=1.5))`
    - `Sphere(center=(1, 0, -1), radius=0.5, material=Metal(albedo=(0.8, 0.6, 0.2), fuzz=0.0))`
  - 교차 테스트: 각 구는 기존 판별식 기반 2차 방정식으로 교차를 판정하며, `t_min=0.001`, `t_max=+∞`를 사용한다.
  - 표면 방향: `front_face`는 레이 방향과 법선 내적 부호로 결정하며, 내부에서 나가는 경우 법선을 뒤집는다.
  - 배경색: `unit_dir = dir / |dir|`, `t = 0.5 * (unit_dir.y + 1.0)`, `background = (1.0 - t) * (1,1,1) + t * (0.5, 0.7, 1.0)`
  - rayColor: 앞서 정의한 재질 규약과 재귀 규칙을 따른다.
  - 감마 보정: 픽셀 평균 색상 `c`에 대해 각 채널을 `gamma=2.0`으로 보정한다(`corrected = sqrt(c)`), 이후 [0, 0.999]로 클램프한다.
  - 최종 채널 값: `channel = round(255 * corrected)`이며 `round`는 `std::lround`를 사용한다.

## 출력/파일 정책
- `--output -` 또는 미지정 시 표준 출력으로만 기록한다.
- 파일로 기록 시 ASCII 텍스트만 생성하며 바이너리 파일은 생성하지 않는다.
- 렌더 산출물(.ppm 등)은 저장할 수 있으나 저장한 파일은 커밋 금지이다.

## 종료 코드
- 정상 종료: 0
- 입력 오류 등 사용법 위반: 1
- 파일 기록 실패: 2 (에러 메시지 후 종료)
