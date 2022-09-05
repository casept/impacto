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
  PressToStartAnimation = Animation();
  PressToStartAnimation.DurationIn =
      Profile::TitleMenu::PressToStartAnimDurationIn;
  PressToStartAnimation.DurationOut =
      Profile::TitleMenu::PressToStartAnimDurationOut;
  PressToStartAnimation.LoopMode = ALM_ReverseDirection;
  // TODO: Remove
  PressToStartAnimation.StartIn();
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
    if (PressToStartAnimation.State == AS_Stopped) {
      PressToStartAnimation.StartIn();
    }
  }
}
void TitleMenu::Hide() {
  if (State != Hidden) {
    State = Hidden;
    if (LastFocusedMenu != nullptr) {
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

  PressToStartAnimation.Update(dt);

  if (GetFlag(SF_TITLEMODE)) {
    Show();
  } else {
    Hide();
  }

  // These are copied from C;C, unsure if correct
  if (State != Hidden && GetFlag(SF_TITLEMODE)) {
    switch (ScrWork[SW_TITLEMODE]) {
      case 1: {  // Press to start
        PressToStartAnimation.DurationIn = PressToStartAnimDurationIn;
        PressToStartAnimation.DurationOut = PressToStartAnimDurationOut;
      } break;
      case 2: {  // Transition between Press to start and menus
        PressToStartAnimation.DurationIn = PressToStartAnimFastDurationIn;
        PressToStartAnimation.DurationOut = PressToStartAnimFastDurationOut;
      } break;
      default: {
        ImpLog(LL_Warning, LC_Render, "Unknown SW_TITLEMODE %i", SW_TITLEMODE);
      } break;
    }
  }
}

void TitleMenu::Render() {
  DrawMainBackground();
  DrawStartButton();
}

inline void TitleMenu::DrawMainBackground() {
  glm::vec4 col = glm::vec4(1.0f);
  col.a = 1.0f;
  Renderer2D::DrawSprite(BackgroundSprite, glm::vec2(BackgroundX, BackgroundY),
                         col);
}

inline void TitleMenu::DrawStartButton() {
  glm::vec4 col = glm::vec4(1.0f);
  col.a = glm::smoothstep(0.0f, 1.0f, PressToStartAnimation.Progress);
  Renderer2D::DrawSprite(PressToStartSprite,
                         glm::vec2(PressToStartX, PressToStartY), col);
}

}  // namespace SG
}  // namespace UI
}  // namespace Impacto