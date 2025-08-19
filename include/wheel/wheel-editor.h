#pragma once

#include <raylib.h>
#include <string>
#include <vector>
#include "wheel/spin-wheel.h"

class WheelEditor {
public:
  WheelEditor(SpinWheel& wheel)
      : wheel_(wheel) {
          // 初始化编辑器
        };
  virtual ~WheelEditor();

  void Update(float delta_time);
  void Draw();
  void Toggle() {
    is_open_ = !is_open_;
    TraceLog(LOG_WARNING, "WheelEditor toggled: %s", is_open_ ? "Open" : "Closed");
  }
  bool IsOpen() const { return is_open_; }

private:
  SpinWheel wheel_;

  /* UI */
  float panel_width_{300.0f};
  float current_panel_width{0.0f};
  float animation_speed_ = 1200.0f;  // pixels per second

  /* State */
  bool is_open_{false};
  std::vector<WheelOption> tmep_options_;
  int selected_row_ = -1;
  bool editing_text_ = false;
  bool editing_weight_ = false;
  std::string input_buffer_;

  void InitializeOptions();
  void AddNewOption();
  void DeleteOpotion(int index);
  void ApplyChanges();

  void DrawOptionRow(int index, Rectangle rect);
  void HandleInput();
};
