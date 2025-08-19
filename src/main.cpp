#include <cmath>
#include <string>
#include "managers/font-manager.h"
#include "raylib.h"
#include "wheel/spin-wheel.h"

int main() {
  SetTraceLogLevel(LOG_WARNING);
  SetConfigFlags(FLAG_MSAA_4X_HINT);    // 启用 4x 多重采样抗锯齿
  SetConfigFlags(FLAG_WINDOW_HIGHDPI);  // HIDPI

  const int screen_width = 1000;
  const int screen_height = 700;

  InitWindow(screen_width, screen_height, "game");
  SetTargetFPS(120);

  // 创建转盘
  Vector2 wheel_center = {screen_width / 2.0f, screen_height / 2.0f};
  SpinWheel wheel(wheel_center, 200.0f);

  float background_animation = 0;

  while (!WindowShouldClose()) {
    float delta_time = GetFrameTime();
    background_animation += delta_time;

    // 输入处理
    if (IsKeyPressed(KEY_SPACE)) {
      if (wheel.GetState() == SpinState::kIdle || wheel.IsShowingResult()) {
        wheel.Spin();
      }
    }

    if (IsKeyPressed(KEY_R)) {
      wheel.Reset();
    }

    // 更新游戏
    wheel.Update(delta_time);

    // 绘制
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

    // 绘制转盘
    wheel.Draw();

    // 绘制状态信息
    std::string state_text;
    switch (wheel.GetState()) {
      case SpinState::kIdle:
        state_text = "Ready";
        break;
      case SpinState::kSpinning:
        state_text = "Spinning";
        break;
      case SpinState::kSlowingDown:
        state_text = "Slowing down";
        break;
      case SpinState::kStopped:
        state_text = "Stopped";
        break;
    }
    DrawTextEx(FontManager::Get().Italic(), state_text.c_str(), (Vector2){50, screen_height - 100},
               static_cast<float>(SpinFontSize::kSubtitle), 2.0f, WHITE);

    // FPS 显示
    std::string fps_text = "FPS: " + std::to_string(GetFPS());
    DrawTextEx(FontManager::Get().Italic(), fps_text.c_str(), (Vector2){screen_width - 100, 20},
               static_cast<float>(SpinFontSize::kFps), 2.0f, GREEN);

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
