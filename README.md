# M5GuruguruAvatar
A cute animated character library that follows your finger (touch) on the M5Stack StopWatch's round AMOLED display.
- 9-direction head tracking
- Automatic blinking (random interval, built-in by default)

Inspired by @rotejin's tomari-guruguru project.

## Demo Video

https://x.com/nananauno/status/2067257510378274823

## Requirements
- M5Unified

## Tested Devices
- [M5Stack StopWatch](https://shop.m5stack.com/products/m5stack-stopwatch-dev-kit-esp32-s3) (ESP32-S3 + 1.75" round AMOLED touch)

## How to Use

### Preparing Character Images
Image preparation: Please refer to @rotejin's [tomari-guruguru](https://github.com/rotejin/tomari-guruguru) repository for how to create the sprite sheets.

1. Create a sprite sheet.
2. See [here](https://github.com/nananauno/M5-guruguru/tree/main/tools) and open `tools/sprite-splitter.html` in your browser.
3. Upload your sprite sheet and adjust the grid settings.
4. Click **Split Sprites** to generate individual images.
5. Download the sliced images and rename them as `dir0.png` to `dir8.png`.
6. Prepare closed-eye versions of each image and rename them as `dir0_close.png` to `dir8_close.png`. If a close image is missing, the open image is used as a fallback.

*Image mapping:*
- `dir0` / `dir0_close`: Top-Left
- `dir1` / `dir1_close`: Top
- `dir2` / `dir2_close`: Top-Right
- `dir3` / `dir3_close`: Left
- `dir4` / `dir4_close`: Center (Front) ← Default
- `dir5` / `dir5_close`: Right
- `dir6` / `dir6_close`: Bottom-Left
- `dir7` / `dir7_close`: Bottom
- `dir8` / `dir8_close`: Bottom-Right

### Flashing to Device
1. Place your character images (`dir0.png` to `dir8.png` and `dir0_close.png` to `dir8_close.png`) in the `data/` folder of the project.
2. Upload the firmware **and** the LittleFS filesystem (images) to your M5StopWatch.
   - **PlatformIO**: `Upload` and then `Upload Filesystem Image`
   - **Arduino IDE**: Use the [Arduino LittleFS Uploader](https://github.com/earlephilhower/arduino-littlefs-upload) plugin
3. Power on the device and touch the screen — the character will follow your finger!

### Sample code

```cpp
#include <M5Unified.h>
#include <M5GuruguruAvatar.h>

M5GuruguruAvatar avatar;

void setup() {
  M5.begin();
  avatar.init(251, 240);  // imgWidth=251px, imgHeight=240px — adjust to your images
}

void loop() {
  M5.update();
  if (M5.Touch.getCount() > 0) {
    auto p = M5.Touch.getDetail(0);
    if (p.isPressed()) avatar.trackFace(p.x, p.y);
  }
}
```

### platformio.ini

```ini
[env]
platform = https://github.com/pioarduino/platform-espressif32/releases/download/stable/platform-espressif32.zip
framework = arduino

[env:m5stack-stopwatch]
board = esp32s3box
board_build.partitions = default_16MB.csv
board_upload.flash_size = 16MB
board_upload.maximum_size = 16777216
board_build.filesystem = littlefs
board_build.arduino.memory_type = qio_opi
monitor_speed = 115200
build_flags =
    -DESP32S3
    -DBOARD_HAS_PSRAM
    -DCORE_DEBUG_LEVEL=5
    -DARDUINO_USB_CDC_ON_BOOT=1
    -DARDUINO_USB_MODE=1
lib_deps =
    M5Unified = https://github.com/m5stack/M5Unified
    https://github.com/nananauno/M5GuruguruAvatar.git
```

## API

| Method | Description |
|--------|-------------|
| `bool init(int imgWidth, int imgHeight)` | Mount LittleFS, load PNG images, and start the internal draw task. Automatic blinking is enabled by default. Returns `false` if LittleFS fails to mount. |
| `void trackFace(int x, int y)` | Update face direction from a touch coordinate. Call this every time a touch event is received. |
| `void setRotation(uint8_t r)` | Rotate the display. Uses the same convention as `M5.Display.setRotation()`: 0=0°, 1=90°, 2=180°, 3=270°. |
| `void setEyeClose(bool close)` | Manually force eyes open (`false`) or closed (`true`). Ignored when an `EyeController` is active. |
| `void setEyeController(M5Guruguru::EyeController* controller)` | Replace the built-in blink controller with a custom one. Pass `nullptr` to disable blinking entirely. |
| `void end()` | Stop the draw task and free all resources. Called automatically by the destructor. |

### Custom Overlay

You can subclass `M5GuruguruAvatar` and override `drawOverlay()` to draw anything on top of the avatar each frame. The `canvas` is a screen-sized sprite, so what you draw here is composited with the avatar before being pushed to the display in a single transfer — no flickering.

```cpp
class MyAvatar : public M5GuruguruAvatar {
protected:
  void drawOverlay(M5Canvas* canvas) override {
    canvas->setTextColor(TFT_WHITE);
    canvas->setTextSize(2);
    canvas->drawCenterString("Hello!", canvas->width() / 2, canvas->height() / 2);
  }
};
```

If you need full control over the rendering pipeline, override `drawFrame()` instead.

### Custom Blink Controller

You can subclass `M5Guruguru::EyeController` to implement custom blink patterns (e.g. forced close, double blink).

```cpp
#include <EyeController.h>

class AlwaysClosedEyes : public M5Guruguru::EyeController {
public:
  bool shouldCloseEyes() override { return true; }
  void update() override {}
};

AlwaysClosedEyes myEyes;
avatar.setEyeController(&myEyes);
```

Pass `nullptr` to disable blinking entirely:

```cpp
avatar.setEyeController(nullptr);
```

## License

This project is licensed under the **MIT License** — see the [LICENSE](LICENSE) file for details.

Character assets (images) follow their own license. Please respect the original creators.

## Acknowledgments

- Huge thanks to [@rotejin](https://github.com/rotejin) for the wonderful `tomari-guruguru` idea and open resources!
- M5Stack team for the amazing StopWatch hardware.
- [@lovyan03](https://github.com/lovyan03) for the [M5Unified](https://github.com/m5stack/M5Unified) library.