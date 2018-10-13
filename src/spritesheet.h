#pragma once

#include "util.h"

namespace Impacto {

struct SpriteSheet {
  SpriteSheet() {}
  SpriteSheet(float width, float height)
      : DesignWidth(width), DesignHeight(height) {}

  float DesignWidth;
  float DesignHeight;

  GLuint Texture = 0;
};

struct Sprite {
  Sprite() {}
  Sprite(SpriteSheet const& sheet, float x, float y, float width, float height)
      : Sheet(sheet), Bounds(x, y, width, height) {}

  SpriteSheet Sheet;
  RectF Bounds;
};

struct Font {
  Font() {}
  Font(float width, float height, uint8_t columns, uint8_t rows)
      : Sheet(width, height), Columns(columns), Rows(rows) {}

  SpriteSheet Sheet;
  uint8_t Columns;
  uint8_t Rows;

  float RowHeight() const { return Sheet.DesignHeight / (float)Rows; }
  float ColWidth() const { return Sheet.DesignWidth / (float)Columns; }

  Sprite Glyph(uint8_t row, uint8_t col) {
    return Sprite(Sheet, col * ColWidth(), row * RowHeight(), ColWidth(),
                  RowHeight());
  }

  Sprite Glyph(uint16_t id) {
    uint8_t row = id / Columns;
    uint8_t col = id % Columns;
    return Glyph(row, col);
  }
};

}  // namespace Impacto