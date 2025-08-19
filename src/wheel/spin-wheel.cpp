#include "wheel/spin-wheel.h"
#include <raylib.h>
#include <format>
#include <string>
#include "managers/font-manager.h"
#include "utilities/ease.h"

void SpinWheel::InitializeOptions() {
  std::vector<std::string> texts = {"Big", "Small", "Miss", "Again", "Secret", "Double", "Again", "SSR Huge"};
  std::vector<Color> colors = {RED, BLUE, GREEN, YELLOW, PURPLE, ORANGE, PINK, SKYBLUE};
  std::vector<int> weights = {2, 3, 1, 2, 1, 2, 2, 1};

  float angle_step = 2 * PI / texts.size();

  int total_weights = 0;
  for (int weight : weights) {
    total_weights += weight;
  }

  float pivot = 0;
  for (int i = texts.size() - 1; i >= 0; i--) {
    float angle_span = static_cast<float>(weights[i]) / total_weights * 2 * PI;
    options_.emplace_back(WheelOption{.text = texts[i],
                                      .color = colors[i % colors.size()],
                                      .weight = weights[i],
                                      .start_angle = pivot,
                                      .end_angle = pivot + angle_span});
    pivot += angle_span;
  }
}

void SpinWheel::Spin() {
  if (state_ == SpinState::kIdle || state_ == SpinState::kStopped) {
    std::uniform_real_distribution<float> dist(0.0f, 2 * PI);
    std::uniform_int_distribution<int> rotations(8, 15);

    int extra_rotations = rotations(rng_);

    float random_angle = dist(rng_);
    target_angle_ = current_angle_ + extra_rotations * 2 * PI + random_angle;

    state_ = SpinState::kSpinning;
    animation_time_ = 0;
    show_result_ = false;
    result_display_time_ = 0;
  }
}

void SpinWheel::Update(float delta_time) {
  HandleInput();
  switch (state_) {
    case SpinState::kSpinning:
    case SpinState::kSlowingDown: {
      animation_time_ += delta_time;
      float progress = animation_time_ / total_animation_time_;

      if (progress >= 1.0f) {
        progress = 1.0f;
        state_ = SpinState::kStopped;
        current_angle_ = target_angle_;
        CalculateSelectedOption();
        show_result_ = true;
      } else {
        // 使用缓动函数实现平滑动画
        float eased_progress = EaseOutCubic(progress);
        current_angle_ = LerpAngle(start_angle_, target_angle_, eased_progress);
      }
      break;
    }
    case SpinState::kStopped:
      start_angle_ = current_angle_;
      if (show_result_) {
        result_display_time_ += delta_time;
      }
      break;
    case SpinState::kIdle:
      break;
  }

  // 标准化角度
  while (current_angle_ >= 2 * PI) current_angle_ -= 2 * PI;
  while (current_angle_ < 0) current_angle_ += 2 * PI;
}

void SpinWheel::Draw() {
  int count = 1;  // used for numbering options
  for (const auto& option : options_) {
    // because the pointer is at the top, we need to adjust the display angles
    float start_angle = option.start_angle + current_angle_ - PI * 0.5f;
    float end_angle = option.end_angle + current_angle_ - PI * 0.5f;

    DrawCircleSector(center_, radius_, start_angle * RAD2DEG, end_angle * RAD2DEG, 256, option.color);
    DrawCircleSectorLines(center_, radius_, start_angle * RAD2DEG, end_angle * RAD2DEG, 256, BLACK);

    // Draw option text
    float text_angle = (start_angle + end_angle) / 2.0f;
    Vector2 text_pos = {center_.x + cos(text_angle) * (radius_ * 0.7f), center_.y + sin(text_angle) * (radius_ * 0.7f)};
    Vector2 text_size = MeasureTextEx(FontManager::Get().Italic(), option.text.c_str(),
                                      static_cast<float>(SpinFontSize::kWheelText), 2.0f);

    text_pos.x -= text_size.x / 2;
    text_pos.y -= text_size.y / 2;

    std::string display_text = std::to_string(count++) + " " + option.text;
    DrawTextEx(FontManager::Get().Italic(), display_text.c_str(), text_pos,
               static_cast<float>(SpinFontSize::kWheelText), 2.0f, WHITE);
  }

  // Debug current angle
  std::string debug_angle_text = std::format("Current Angle: {:.1f}", current_angle_ * RAD2DEG);
  DrawTextEx(FontManager::Get().Italic(), debug_angle_text.c_str(),
             (Vector2){50, static_cast<float>(GetScreenHeight() - 70)}, static_cast<float>(SpinFontSize::kSubtitle),
             2.0f, WHITE);

  // 绘制外圈
  // DrawCircleLines(center_.x, center_.y, radius_, BLACK);
  // DrawCircleLines(center_.x, center_.y, radius_ + 2, DARKGRAY);

  // 绘制中心圆
  float center_radius = 30;
  DrawCircle(center_.x, center_.y, center_radius, DARKGRAY);
  DrawCircleLines(center_.x, center_.y, center_radius, BLACK);

  // 绘制三角形指针
  Vector2 pointer1 = {center_.x - 10, center_.y - radius_ - 10};
  Vector2 pointer2 = {center_.x, center_.y - radius_ + 15};
  Vector2 pointer3 = {center_.x + 10, center_.y - radius_ - 10};

  DrawTriangle(pointer1, pointer2, pointer3, RED);
  DrawTriangleLines(pointer1, pointer2, pointer3, DARKBROWN);

  // 绘制轮盘状态
  std::string state_text;
  switch (state_) {
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
  DrawTextEx(FontManager::Get().Italic(), state_text.c_str(),
             (Vector2){50, static_cast<float>(GetScreenHeight()) - 100}, static_cast<float>(SpinFontSize::kSubtitle),
             2.0f, WHITE);

  // 绘制结果
  if (show_result_ && selected_index_ >= 0) {
    float result_alpha = fminf(1.0f, result_display_time_ * 2.0f);

    // 背景
    Color bg_color = {0, 0, 0, (unsigned char)(150 * result_alpha)};
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), bg_color);

    // 结果文字
    std::string result_text = "Result: " + options_[selected_index_].text;
    Vector2 text_size = MeasureTextEx(FontManager::Get().Italic(), result_text.c_str(),
                                      static_cast<float>(SpinFontSize::kResult), 2.0f);
    Vector2 text_pos = {(GetScreenWidth() - text_size.x) / 2, GetScreenHeight() / 2 - text_size.y / 2};

    // 文字阴影
    Color shadow_color = {0, 0, 0, (unsigned char)(255 * result_alpha)};
    DrawTextEx(FontManager::Get().Italic(), result_text.c_str(), (Vector2){text_pos.x + 3, text_pos.y + 3},
               static_cast<float>(SpinFontSize::kResult), 2.0f, shadow_color);

    // 主文字
    Color text_color = {255, 255, 255, (unsigned char)(255 * result_alpha)};
    DrawTextEx(FontManager::Get().Italic(), result_text.c_str(), text_pos, static_cast<float>(SpinFontSize::kResult),
               2.0f, text_color);

    // 提示文字
    if (result_display_time_ > 2.0f) {
      std::string hint_text = "Press [Space] to spin again";
      Vector2 hint_size = MeasureTextEx(FontManager::Get().Italic(), hint_text.c_str(),
                                        static_cast<float>(SpinFontSize::kSubtitle), 2.0f);
      Vector2 hint_pos = {(GetScreenWidth() - hint_size.x) / 2, text_pos.y + text_size.y + 40};

      Color hint_color = {200, 200, 200, (unsigned char)(255 * result_alpha)};
      DrawTextEx(FontManager::Get().Italic(), hint_text.c_str(), hint_pos, static_cast<float>(SpinFontSize::kSubtitle),
                 2.0f, hint_color);
    }
  }
}

void SpinWheel::CalculateSelectedOption() {
  float normalized_angle = fmod(-current_angle_, 2 * PI);
  if (normalized_angle < 0) normalized_angle += 2 * PI;

  for (size_t i = 0; i < options_.size(); i++) {
    if (normalized_angle >= options_[i].start_angle && normalized_angle < options_[i].end_angle) {
      selected_index_ = i;
      break;
    }
  }
}

void SpinWheel::Reset() {
  state_ = SpinState::kIdle;
  current_angle_ = 0;
  target_angle_ = 0;
  animation_time_ = 0;
  selected_index_ = -1;
  show_result_ = false;
  result_display_time_ = 0;
}

void SpinWheel::HandleInput() {
  if (IsKeyPressed(KEY_SPACE)) {
    if (state_ == SpinState::kIdle || IsShowingResult()) {
      Spin();
    }
  }

  if (IsKeyPressed(KEY_R)) {
    Reset();
  }
}
