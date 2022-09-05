#include "titlemenu.h"
#include "../../../log.h"
#include "../../../window.h"
#include "../../profile_internal.h"

#include "../../ui/titlemenu.h"
#include "../../../game.h"
#include "../../../ui/ui.h"
#include "../../../games/chlcc/titlemenu.h"

namespace Impacto {
namespace Profile {
namespace SG {
namespace TitleMenu {

Sprite BackgroundSprite;
float BackgroundX;
float BackgroundY;

void Configure() {
  BackgroundSprite = EnsureGetMemberSprite("BackgroundSprite");
  BackgroundX = EnsureGetMemberFloat("BackgroundX");
  BackgroundY = EnsureGetMemberFloat("BackgroundY");

  UI::SG::TitleMenu* menu = new UI::SG::TitleMenu();

  UI::TitleMenuPtr = menu;

  auto drawType = Game::DrawComponentType::_from_integral_unchecked(
      EnsureGetMemberInt("DrawType"));

  UI::Menus[drawType].push_back(UI::TitleMenuPtr);
}

}  // namespace TitleMenu
}  // namespace SG
}  // namespace Profile
}  // namespace Impacto