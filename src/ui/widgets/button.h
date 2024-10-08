#pragma once

#include <functional>

#include "../widget.h"
#include "../../spritesheet.h"
#include "../../text.h"
#include "../../renderer/renderer.h"

namespace Impacto {
namespace UI {
namespace Widgets {

class Button : public Widget {
 public:
  Button() {}
  Button(int id, Sprite const& norm, Sprite const& focused,
         Sprite const& highlight, glm::vec2 pos);

  virtual void UpdateInput() override;
  virtual void Render() override;
  virtual void Move(glm::vec2 relativePosition) override;
  virtual void Move(glm::vec2 relativePosition, float duration) override;
  virtual void MoveTo(glm::vec2 pos) override;
  virtual void MoveTo(glm::vec2 pos, float duration) override;

  void SetText(uint8_t* str, float fontSize, RendererOutlineMode outlineMode,
               int colorIndex = 10);
  void SetText(ProcessedTextGlyph* str, int textLength, float textWidth,
               float fontSize, RendererOutlineMode outlineMode);

  int Id;
  Sprite NormalSprite;
  Sprite FocusedSprite;
  Sprite HighlightSprite;
  Sprite DisabledSprite;
  Sprite LockedSprite;

  bool IsLocked = false;

  std::function<void(Button*)> OnClickHandler;

  glm::vec2 HighlightOffset = glm::vec2(0.0f, 3.0f);

 protected:
  bool HasText = false;
  ProcessedTextGlyph* Text = nullptr;
  int TextLength = 0;
  float TextWidth = 0.0f;
  RendererOutlineMode OutlineMode;
};

}  // namespace Widgets
}  // namespace UI
}  // namespace Impacto