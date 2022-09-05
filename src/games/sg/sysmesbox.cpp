#include "sysmesbox.h"

#include "../../impacto.h"
#include "../../renderer2d.h"
#include "../../game.h"
#include "../../mem.h"
#include "../../inputsystem.h"
#include "../../vm/interface/input.h"
#include "../../profile/scriptvars.h"
#include "../../profile/hud/sysmesbox.h"
#include "../../profile/games/sg/sysmesbox.h"
#include "../../profile/dialogue.h"

namespace Impacto {
namespace Sg {

using namespace Impacto::Profile::SysMesBox;
using namespace Impacto::Profile::Sg::SysMesBox;
using namespace Impacto::Profile::ScriptVars;

static float BoxAnimCount = 0.0f;
static float BoxTopY = 0.0f;
static float BoxBottomY = 0.0f;
static float LineLength = 0.0f;
static float BoxHeight = 0.0f;
static float BoxProgressCount = 0.0f;
static int TextStartCount = 0;

void SysMesBox::Show() { State = Showing; }
void SysMesBox::Hide() { State = Hiding; }

void SysMesBox::Update(float dt) {
  FadeAnimation.Update(dt);

  if (State == Hiding) {
    BoxAnimCount -= AnimationSpeed * dt;
    if (BoxAnimCount <= 0.0f) {
      BoxAnimCount = 0.0f;
      State = Hidden;
    }
  } else if (State == Showing) {
    BoxAnimCount += AnimationSpeed * dt;
    if (BoxAnimCount >= ScrWork[SW_SYSMESANIMCTF]) {
      BoxAnimCount = ScrWork[SW_SYSMESANIMCTF];
      State = Shown;
    }
  }

  ScrWork[SW_SYSMESANIMCTCUR] = std::floor(BoxAnimCount);

  if (State == Shown) {
    // Nice input
    if (Input::KeyboardButtonWentDown[SDL_SCANCODE_RIGHT]) {
      if (CurrentChoice == 255)
        CurrentChoice = 1;
      else {
        CurrentChoice++;
        if (CurrentChoice > 1) CurrentChoice = 0;
      }
    } else if (Input::KeyboardButtonWentDown[SDL_SCANCODE_LEFT]) {
      if (CurrentChoice == 255)
        CurrentChoice = 0;
      else {
        CurrentChoice--;
        if (CurrentChoice < 0) CurrentChoice = 1;
      }
    } else if (Vm::Interface::PAD1A & Vm::Interface::PADinputWentDown) {
      ChoiceMade = true;
    }
  }

  if (State != Hidden) {
    if (State == Showing && FadeAnimation.IsOut())
      FadeAnimation.StartIn();
    else if (State == Hiding && FadeAnimation.IsIn())
      FadeAnimation.StartOut();
  }
}

void SysMesBox::Render() {
  if (BoxOpacity) {
    glm::vec4 col(1.0f, 1.0f, 1.0f, FadeAnimation.Progress);

    float diff = 0.0f;
    float maxWidth = FLT_MIN;
    for (int i = 0; i < MessageCount; i++) {
      if (maxWidth < MessageWidths[i]) maxWidth = MessageWidths[i];
    }
    if (maxWidth < BoxMinimumWidth) maxWidth = BoxMinimumWidth;

    float leftStartX = BoxMiddleBaseX - (maxWidth / 2.0f);
    Renderer2D::DrawSprite(BoxPartLeft, glm::vec2(leftStartX, BoxY), col);

    BoxPartMiddle.BaseScale =
        glm::vec2((maxWidth - BoxRightBaseWidth) / BoxMiddleBaseWidth, 1.0f);
    Renderer2D::DrawSprite(
        BoxPartMiddle, glm::vec2(leftStartX + BoxRightRemainPad, BoxY), col);

    Renderer2D::DrawSprite(
        BoxPartRight,
        glm::vec2(leftStartX + maxWidth + BoxRightBaseWidth, BoxY), col);

    // TODO: Draw Yes/No/OK buttons here

    for (int i = 0; i < MessageCount; i++) {
      diff = Messages[i][0].DestRect.X - (TextX - (maxWidth / 2.0f));
      for (int j = 0; j < MessageLengths[i]; j++) {
        Messages[i][j].Colors = Profile::Dialogue::ColorTable[0];
        Messages[i][j].DestRect.X -= diff;
        Messages[i][j].DestRect.Y = TextMiddleY + (i * TextLineHeight);
      }
      Renderer2D::DrawProcessedText(Messages[i], MessageLengths[i],
                                    Profile::Dialogue::DialogueFont, col.a,
                                    true);
    }
  }
}

}  // namespace Sg
}  // namespace Impacto