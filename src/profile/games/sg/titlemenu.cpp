#include "titlemenu.h"
#include "../../../log.h"
#include "../../../window.h"
#include "../../profile_internal.h"

#include "../../ui/titlemenu.h"
#include "../../../game.h"
#include "../../../ui/ui.h"
#include "../../../games/sg/titlemenu.h"

namespace Impacto {
namespace Profile {
namespace SG {
namespace TitleMenu {

// Background itself
Sprite BackgroundSprite;
float BackgroundX;
float BackgroundY;

// "Press Enter" blinking prompt
float PressToStartX;
float PressToStartY;
float PressToStartAnimDurationIn;
float PressToStartAnimDurationOut;
float PressToStartAnimFastDurationIn;
float PressToStartAnimFastDurationOut;

// Main menu
float MenuX;
float MenuY;
float MenuEntriesSpacingY;
int MenuStartId;
int MenuLoadId;
int MenuExtraId;
int MenuConfigId;
int MenuHelpId;

void Configure() {
  BackgroundSprite = EnsureGetMemberSprite("BackgroundSprite");
  BackgroundX = EnsureGetMemberFloat("BackgroundX");
  BackgroundY = EnsureGetMemberFloat("BackgroundY");

  PressToStartX = EnsureGetMemberFloat("PressToStartX");
  PressToStartY = EnsureGetMemberFloat("PressToStartY");
  PressToStartAnimDurationIn =
      EnsureGetMemberFloat("PressToStartAnimDurationIn");
  PressToStartAnimDurationOut =
      EnsureGetMemberFloat("PressToStartAnimDurationOut");
  PressToStartAnimFastDurationIn =
      EnsureGetMemberFloat("PressToStartAnimFastDurationIn");
  PressToStartAnimFastDurationOut =
      EnsureGetMemberFloat("PressToStartAnimFastDurationOut");

  MenuX = EnsureGetMemberFloat("MenuX");
  MenuY = EnsureGetMemberFloat("MenuY");
  MenuEntriesSpacingY = EnsureGetMemberFloat("MenuEntriesSpacingY");
  MenuStartId = EnsureGetMemberInt("MenuStartId");
  MenuLoadId = EnsureGetMemberInt("MenuLoadId");
  MenuExtraId = EnsureGetMemberInt("MenuExtraId");
  MenuConfigId = EnsureGetMemberInt("MenuConfigId");
  MenuHelpId = EnsureGetMemberInt("MenuHelpId");

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