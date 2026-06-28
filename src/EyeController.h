#pragma once

#include <cstdint>

namespace M5Guruguru {

// Controls eye open/close state. Override to implement custom blink patterns.
class EyeController {
public:
  EyeController(uint16_t blinkDurationMs = 80,
                uint16_t minIntervalMs   = 500,
                uint16_t maxIntervalMs   = 4000);
  virtual ~EyeController() = default;

  virtual bool shouldCloseEyes();
  virtual uint16_t getBlinkDuration();  // blink duration in ms
  virtual void update();                // called every frame

protected:
  unsigned long _lastBlinkTime     = 0;
  bool          _isCurrentlyClosed = false;
  unsigned long _nextBlinkInterval = 0;
  uint16_t      _blinkDurationMs;
  uint16_t      _minIntervalMs;
  uint16_t      _maxIntervalMs;
};

} // namespace M5Guruguru
