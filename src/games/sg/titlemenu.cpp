#include "titlemenu.h"

#include "../../texture/texture.h"
#include "../../spritesheet.h"
#include "../../io/vfs.h"

#include "../../profile/ui/titlemenu.h"
#include "../../profile/games/sg/titlemenu.h"
#include "../../renderer2d.h"
#include "../../texture/texture.h"
#include "../../spritesheet.h"
#include "../../audio/audiosystem.h"
#include "../../audio/audiostream.h"
#include "../../audio/audiochannel.h"
#include "../../io/vfs.h"
#include "../../mem.h"
#include "../../profile/scriptvars.h"
#include "../../profile/game.h"
#include "../../background2d.h"
#include "../../inputsystem.h"
#include "../../ui/widgets/label.h"
#include "../../vm/interface/input.h"

namespace Impacto {
namespace UI {
namespace SG {

using namespace Impacto::Profile::TitleMenu;
using namespace Impacto::Profile::SG::TitleMenu;
using namespace Impacto::Profile::ScriptVars;

using namespace Impacto::Vm::Interface;

TitleMenu::TitleMenu() {
  Sprite nullSprite = Sprite();
  nullSprite.Bounds = RectF(0.0f, 0.0f, 0.0f, 0.0f);
}

void TitleMenu::Show() {
  if (State != Shown) {
    State = Shown;
    if (UI::FocusedMenu != 0) {
      LastFocusedMenu = UI::FocusedMenu;
      LastFocusedMenu->IsFocused = false;
    }
    IsFocused = true;
    UI::FocusedMenu = this;
    AllowsScriptInput = true;
  }
}
void TitleMenu::Hide() {
  if (State != Hidden) {
    State = Hidden;
    if (LastFocusedMenu != 0) {
      UI::FocusedMenu = LastFocusedMenu;
      LastFocusedMenu->IsFocused = true;
    } else {
      UI::FocusedMenu = 0;
    }
    IsFocused = false;
    AllowsScriptInput = true;
  }
}

void TitleMenu::UpdateInput() { Menu::UpdateInput(); }

void TitleMenu::Update(float dt) {
  UpdateInput();

  if (GetFlag(SF_TITLEMODE)) {
    Show();
  } else {
    Hide();
  }
}

void TitleMenu::Render() { DrawMainBackground(true); }

inline void TitleMenu::DrawMainBackground(float opacity) {
  glm::vec4 col = glm::vec4(1.0f);
  col.a = opacity;
  Renderer2D::DrawSprite(BackgroundSprite, glm::vec2(BackgroundX, BackgroundY),
                         col);
}

}  // namespace SG
}  // namespace UI
}  // namespace Impacto