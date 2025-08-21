#include <cmath>
#include <string>
#include "managers/font-manager.h"
#include "raylib.h"
#include "terminal.h"
#include "wheel/spin-wheel.h"

// #define PLATFORM_WEB
#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

const int screen_width = 960;
const int screen_height = 700;

float background_animation = 0;
bool show_fps = false;

void Loop();

int main() {
  SetTraceLogLevel(LOG_WARNING);
  SetConfigFlags(FLAG_MSAA_4X_HINT);    // 启用 4x 多重采样抗锯齿
  SetConfigFlags(FLAG_WINDOW_HIGHDPI);  // HIDPI

  InitWindow(screen_width, screen_height, "game");

  // Game Loop
#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(Loop, 0, 1);
#else
  SetTargetFPS(120);
  while (!WindowShouldClose()) {
    Loop();
  }
#endif

  CloseWindow();
  return 0;
}

void Loop() {
  float dt = GetFrameTime();

  /*─────────────────────────────────────┐
  │                Update                │
  └──────────────────────────────────────*/
  background_animation += dt;
  SpinWheel::Get().Update(dt);
  Terminal::Get().Update(dt);

  /*─────────────────────────────────────┐
  │                Draw                  │
  └──────────────────────────────────────*/
  BeginDrawing();

  // 动态背景
  Color bg1 = {20, 30, 50, 255};
  Color bg2 = {40, 20, 60, 255};
  float bg_lerp = (sin(background_animation * 0.5f) + 1) * 0.5f;
  Color bg_color = {(unsigned char)(bg1.r + (bg2.r - bg1.r) * bg_lerp),
                    (unsigned char)(bg1.g + (bg2.g - bg1.g) * bg_lerp),
                    (unsigned char)(bg1.b + (bg2.b - bg1.b) * bg_lerp), 255};
  ClearBackground(bg_color);

  // 绘制标题
  DrawTextEx(FontManager::Get().Italic(), "Spin the wheel game", (Vector2){50, 50},
             static_cast<float>(SpinFontSize::kTitle), 2.0f, WHITE);
  DrawTextEx(FontManager::Get().Italic(), "Press [Space] to spin", (Vector2){50, 100},
             static_cast<float>(SpinFontSize::kSubtitle), 2.0f, LIGHTGRAY);
  DrawTextEx(FontManager::Get().Italic(), "Press [R] to reset", (Vector2){50, 130},
             static_cast<float>(SpinFontSize::kSubtitle), 2.0f, LIGHTGRAY);

  SpinWheel::Get().Draw();
  Terminal::Get().Draw();

  if (show_fps) {
    std::string fps_text = "FPS: " + std::to_string(GetFPS());
    DrawTextEx(FontManager::Get().Italic(), fps_text.c_str(), (Vector2){screen_width - 100, 20},
               static_cast<float>(SpinFontSize::kFps), 2.0f, GREEN);
  }

  EndDrawing();
}
