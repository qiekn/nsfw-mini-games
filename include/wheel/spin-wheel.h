#pragma once

#include <raylib.h>
#include <random>
#include <string>
#include <vector>

enum class SpinState { kIdle, kSpinning, kSlowingDown, kStopped };

enum class SpinFontSize { kTitle = 36, kSubtitle = 24, kWheelText = 20, kResult = 64, kFps = 20 };

struct WheelOption {
  std::string text;
  Color color;
  float start_angle;
  float end_angle;
};

class SpinWheel {
public:
  SpinWheel(Vector2 pos, float r) : center_(pos), radius_(r) { InitializeOptions(); }

  void InitializeOptions();

  void Update(float delta_time);
  void Draw();

  void Spin();
  void Reset();
  void CalculateSelectedOption();

  SpinState GetState() const { return state_; }
  bool IsShowingResult() const { return show_result_; }

private:
  std::vector<WheelOption> options_;
  Vector2 center_;
  float radius_;
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
  float pulse_animation_;
  bool show_result_;
  float result_display_time_;

  // 随机数生成器
  std::mt19937 rng_{std::random_device{}()};
};
