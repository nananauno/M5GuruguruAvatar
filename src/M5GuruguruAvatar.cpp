#include "M5GuruguruAvatar.h"
#include <math.h>

bool M5GuruguruAvatar::init(int imgWidth, int imgHeight) {
  _imgWidth   = imgWidth;
  _imgHeight  = imgHeight;
  _currentDir = AVATAR_CENTER_DIR;

  if (!LittleFS.begin(true)) {
    Serial.println("[Avatar] LittleFS mount failed");
    return false;
  }

  for (int i = 0; i < AVATAR_NUM_DIR; i++) {
    _canvas[i] = new M5Canvas(&M5.Display);
    _canvas[i]->setColorDepth(16);
    _canvas[i]->createSprite(_imgWidth, _imgHeight);

    String path = String("/dir") + i + ".png";
    if (_canvas[i]->drawPngFile(LittleFS, path.c_str())) {
      Serial.printf("[Avatar] Loaded: %s\n", path.c_str());
    } else {
      Serial.printf("[Avatar] Missing: %s  (placeholder)\n", path.c_str());
      _canvas[i]->fillSprite(TFT_BLACK);
      _canvas[i]->setTextColor(TFT_WHITE, TFT_BLACK);
      _canvas[i]->drawCenterString(String("Dir ") + i, _imgWidth / 2, _imgHeight / 2);
    }
  }

  _running = true;
  xTaskCreatePinnedToCore(drawTask, "AvatarDraw", 4096, this, 1, &_taskHandle, 1);
  return true;
}

void M5GuruguruAvatar::trackFace(int touchX, int touchY) {
  _currentDir = getDirection(touchX, touchY);
}

void M5GuruguruAvatar::end() {
  _running = false;
  if (_taskHandle) {
    vTaskDelete(_taskHandle);
    _taskHandle = nullptr;
  }
  for (int i = 0; i < AVATAR_NUM_DIR; i++) {
    delete _canvas[i];
    _canvas[i] = nullptr;
  }
}

M5GuruguruAvatar::~M5GuruguruAvatar() {
  end();
}

// static
void M5GuruguruAvatar::drawTask(void* arg) {
  auto* self = static_cast<M5GuruguruAvatar*>(arg);
  while (self->_running) {
    float zx = (float)M5.Display.width()  / self->_imgWidth;
    float zy = (float)M5.Display.height() / self->_imgHeight;
    M5.Display.startWrite();
    self->_canvas[self->_currentDir]->pushRotateZoom(
      M5.Display.width()  / 2,
      M5.Display.height() / 2,
      0.0f, zx, zy
    );
    M5.Display.endWrite();
    vTaskDelay(pdMS_TO_TICKS(33));  // ~30 fps
  }
  vTaskDelete(nullptr);
}

int M5GuruguruAvatar::getDirection(int touchX, int touchY) const {
  int cx = M5.Display.width()  / 2;
  int cy = M5.Display.height() / 2;
  float dx = touchX - cx;
  float dy = touchY - cy;

  if (sqrtf(dx * dx + dy * dy) < 50) return AVATAR_CENTER_DIR;

  float angle = atan2f(dy, dx) * 180.0f / M_PI;

  if (angle < -157.5f || angle > 157.5f) return 3;  // Left
  else if (angle < -112.5f)              return 0;  // Upper-Left
  else if (angle < -67.5f)               return 1;  // Up
  else if (angle < -22.5f)               return 2;  // Upper-Right
  else if (angle < 22.5f)                return 5;  // Right
  else if (angle < 67.5f)                return 8;  // Lower-Right
  else if (angle < 112.5f)               return 7;  // Down
  else if (angle < 157.5f)               return 6;  // Lower-Left
  return 3;
}
