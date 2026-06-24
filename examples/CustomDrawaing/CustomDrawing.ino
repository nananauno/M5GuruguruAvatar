#include <M5Unified.h>
#include <M5GuruguruAvatar.h>

// Rainbow color table (HSV hue steps)
static const uint16_t RAINBOW[] = {
  TFT_RED, TFT_ORANGE, TFT_YELLOW, TFT_GREEN, TFT_CYAN, TFT_BLUE, TFT_MAGENTA
};
static constexpr int RAINBOW_LEN = sizeof(RAINBOW) / sizeof(RAINBOW[0]);

class HelloAvatar : public M5GuruguruAvatar {
public:
  HelloAvatar() : _count(0), _frame(0) {}

  void incrementCount() { _count++; }

protected:
  void drawOverlay(M5Canvas* canvas) override {
    int cx = canvas->width()  / 2;
    int cy = canvas->height() / 2;

    // Line 1: "Hello!" in rainbow color cycling per frame
    uint16_t color = RAINBOW[(_frame / 10) % RAINBOW_LEN];
    canvas->setTextSize(3);
    canvas->setTextColor(color);
    canvas->drawCenterString("Hello!", cx, cy - 40);

    // Line 2: count-up display
    canvas->setTextSize(2);
    canvas->setTextColor(TFT_WHITE);
    canvas->drawCenterString(String("Count: ") + _count, cx, cy);

    // Line 3: simple bar that grows and resets
    int barWidth = (_frame % 100) * (canvas->width() - 20) / 100;
    canvas->fillRect(10, cy + 35, barWidth, 12, TFT_CYAN);
    canvas->drawRect(10, cy + 35, canvas->width() - 20, 12, TFT_WHITE);

    _frame++;
  }

private:
  int _count;
  int _frame;
};

HelloAvatar avatar;

uint8_t detectRotation(float ax, float ay) {
  if (fabsf(ax) > fabsf(ay)) {
    return (ax > 0) ? 2 : 0;
  }
  return M5.Display.getRotation();
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
    if (p.isPressed()) {
      avatar.trackFace(p.x, p.y);
      avatar.incrementCount();
    }
  }
}
