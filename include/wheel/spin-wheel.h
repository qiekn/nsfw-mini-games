#pragma once

#include <raylib.h>
#include <random>
#include <string>
#include <vector>
#include "utilities/singleton.h"

enum class SpinState { kIdle, kSpinning, kSlowingDown, kStopped };

enum class SpinFontSize { kTitle = 36, kSubtitle = 24, kWheelText = 20, kResult = 64, kFps = 20 };

struct WheelOption {
  std::string text;
  Color color;
  int weight;
  float start_angle;
  float end_angle;
};

class SpinWheel {
public:
  MAKE_SINGLETON(SpinWheel);
  void Update(float delta_time);
  void Draw();

  std::vector<WheelOption>& GetOptions() { return options_; }
  void UpdateOptions();  // Reset Options' color and angles

  SpinState GetState() const { return state_; }
  bool IsShowingResult() const { return show_result_; }

private:
  SpinWheel() { InitializeOptions(); }
  std::vector<WheelOption> options_;
  Vector2 center_{GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f};
  float radius_{200.0f};
  float start_angle_{0};
  float current_angle_{0};
  float target_angle_{0};
  float spin_speed_{0};
  float max_spin_speed_{20.0f};
  SpinState state_{SpinState::kIdle};
  float animation_time_{0};
  float total_animation_time_{6.0f};
  int selected_index_{-1};

  // 视觉效果
  std::vector<Color> colors_;
  float pulse_animation_;
  bool show_result_;
  float result_display_time_;

  // 随机数生成器
  std::mt19937 rng_{std::random_device{}()};

  void InitializeOptions();
  void HandleInput();
  void Spin();
  void Reset();
  void CalculateSelectedOption();
};
