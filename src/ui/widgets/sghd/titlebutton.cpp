#include "titlebutton.h"

#include "../../../renderer/renderer.h"
#include "../../../profile/games/sghd/titlemenu.h"

namespace Impacto {
namespace UI {
namespace Widgets {
namespace SGHD {

using namespace Impacto::Profile::SGHD::TitleMenu;

void TitleButton::Render() {
  /*
  if (HasFocus) {
    if (!IsSubButton) {  // Main buttons
      Renderer->DrawSprite(HighlightSprite,
                           glm::vec2(Bounds.X - ItemHighlightOffsetX,
                                     Bounds.Y - ItemHighlightOffsetY),
                           Tint);
      Renderer->DrawSprite(
          ItemHighlightPointerSprite,
          glm::vec2(Bounds.X - ItemHighlightPointerY, Bounds.Y), Tint);
      Renderer->DrawSprite(FocusedSprite, glm::vec2(Bounds.X, Bounds.Y), Tint);
    } else {  // Sub buttons
      Renderer->DrawSprite(HighlightSprite, glm::vec2(Bounds.X, Bounds.Y),
                           Tint);
      Renderer->DrawSprite(FocusedSprite, glm::vec2(Bounds.X, Bounds.Y), Tint);
    }
  } else {
    if (Enabled) {
      Renderer->DrawSprite(NormalSprite, glm::vec2(Bounds.X, Bounds.Y), Tint);
    } else {
      Renderer->DrawSprite(DisabledSprite, glm::vec2(Bounds.X, Bounds.Y), Tint);
    }
  }
  */
}

}  // namespace SGHD
}  // namespace Widgets
}  // namespace UI
}  // namespace Impacto