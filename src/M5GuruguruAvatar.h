#pragma once

#include <LittleFS.h>
#include <M5Unified.h>

// Future: replace bool init() with a DirectionMode enum, e.g.:
// enum class DirectionMode { Dir9, Dir18 };

static constexpr int AVATAR_NUM_DIR = 9;
static constexpr int AVATAR_CENTER_DIR = AVATAR_NUM_DIR / 2;  // 4

class M5GuruguruAvatar {
public:
  M5GuruguruAvatar() = default;
  ~M5GuruguruAvatar();

  // Initialize: load images from LittleFS and start the internal draw task (~30 fps).
  // imgWidth / imgHeight should match your PNG image dimensions.
  bool init(int imgWidth = 251, int imgHeight = 240);

  // Update the face direction based on a touch coordinate.
  void trackFace(int touchX, int touchY);

  // Stop the draw task and free all resources.
  void end();

private:
  static void drawTask(void* arg);
  int getDirection(int touchX, int touchY) const;

  int _imgWidth  = 251;
  int _imgHeight = 240;
  int _currentDir = AVATAR_CENTER_DIR;

  M5Canvas* _canvas[AVATAR_NUM_DIR] = {};
  TaskHandle_t _taskHandle = nullptr;
  bool _running = false;
};
