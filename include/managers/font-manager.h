#pragma once

#include <raylib.h>
#include <cstddef>
#include "utilities/singleton.h"

class FontManager {
  MAKE_SINGLETON(FontManager)
public:
  const Font& Regular() const { return normal_font_; }
  const Font& Italic() const { return itlaic_font_; }

private:
  FontManager()
      : normal_font_(LoadFontEx("assets/fonts/noto-regular.ttf", 128, NULL, 0)),
        itlaic_font_(LoadFontEx("assets/fonts/noto-italic.ttf", 128, NULL, 0)) {}

  ~FontManager() {
    UnloadFont(normal_font_);
    UnloadFont(itlaic_font_);
  }

  Font normal_font_;
  Font itlaic_font_;
};
