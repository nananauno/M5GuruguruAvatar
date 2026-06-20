#include <M5Unified.h>
#include <M5GuruguruAvatar.h>

M5GuruguruAvatar avatar;

void setup() {
  M5.begin();
  avatar.init(251, 240);  // imgWidth, imgHeight — adjust to your images
}

void loop() {
  M5.update();
  if (M5.Touch.getCount() > 0) {
    auto p = M5.Touch.getDetail(0);
    if (p.isPressed()) avatar.trackFace(p.x, p.y);
  }
}
