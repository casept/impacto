#pragma once

#include "../../animation.h"
#include "../../ui/menu.h"
#include "../../ui/widgets/group.h"
#include "../../ui/widgets/button.h"
#include "../../ui/widgets/sg/titlebutton.h"

using Impacto::UI::Widgets::SG::TitleButton;

namespace Impacto {
namespace UI {
namespace SG {

class TitleMenu : public Menu {
 public:
  TitleMenu();

  void Show();
  void Hide();
  void UpdateInput();
  void Update(float dt);
  void Render();

 private:
  Animation PressToStartAnimation;

  Widgets::Group* MainItems;
  TitleButton* Start;
  TitleButton* Load;
  TitleButton* Extra;
  TitleButton* Config;
  TitleButton* Help;

  void DrawMainBackground();
  void DrawStartButton();

  void MenuButtonOnClick(Widgets::Button* target);
};

}  // namespace SG
}  // namespace UI
}  // namespace Impacto