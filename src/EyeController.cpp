#include "EyeController.h"
#include <Arduino.h>
#include <cstdlib>

namespace M5Guruguru {

EyeController::EyeController(uint16_t blinkDurationMs, uint16_t minIntervalMs, uint16_t maxIntervalMs)
  : _blinkDurationMs(blinkDurationMs)
  , _minIntervalMs(minIntervalMs)
  , _maxIntervalMs(maxIntervalMs)
{
  _nextBlinkInterval = random(_minIntervalMs, _maxIntervalMs);
  _lastBlinkTime     = millis();
}

bool EyeController::shouldCloseEyes() {
  return _isCurrentlyClosed;
}

uint16_t EyeController::getBlinkDuration() {
  return _blinkDurationMs;
}

void EyeController::update() {
  unsigned long now = millis();
  if (!_isCurrentlyClosed) {
    if (now - _lastBlinkTime >= _nextBlinkInterval) {
      _isCurrentlyClosed = true;
      _lastBlinkTime     = now;
    }
  } else {
    if (now - _lastBlinkTime >= getBlinkDuration()) {
      _isCurrentlyClosed = false;
      _lastBlinkTime     = now;
      _nextBlinkInterval = random(_minIntervalMs, _maxIntervalMs);
    }
  }
}

} // namespace M5Guruguru
