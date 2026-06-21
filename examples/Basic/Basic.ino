#include <M5Unified.h>
#include <M5GuruguruAvatar.h>

M5GuruguruAvatar avatar;

uint8_t detectRotation(float ax, float ay) {
  if (fabsf(ax) > fabsf(ay)) {
    return (ax > 0) ? 2 : 0;  // Device portrait: normal or rotated 180
  }
  return M5.Display.getRotation();  // Device landscape: keep current rotation
}

void setup() {
  M5.begin();
  M5.Imu.begin();
  avatar.init(251, 240);
}

void loop() {
  M5.update();

  float ax, ay, az;
  if (M5.Imu.getAccel(&ax, &ay, &az)) {
    avatar.setRotation(detectRotation(ax, ay));
  }

  if (M5.Touch.getCount() > 0) {
    auto p = M5.Touch.getDetail(0);
    if (p.isPressed()) avatar.trackFace(p.x, p.y);
  }
}
