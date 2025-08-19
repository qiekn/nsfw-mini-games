#include "wheel/wheel-editor.h"
#include <raylib.h>

WheelEditor::~WheelEditor() {}

void WheelEditor::Update(float delta_time) { HandleInput(); }

void WheelEditor::Draw() {}

void WheelEditor::InitializeOptions() {}

void WheelEditor::AddNewOption() {}

void WheelEditor::DeleteOpotion(int index) {}

void WheelEditor::ApplyChanges() {}

void WheelEditor::DrawOptionRow(int index, Rectangle rect) {}

void WheelEditor::HandleInput() {
  if (IsKeyPressed(KEY_BACKSLASH)) {
    Toggle();
  }
}
