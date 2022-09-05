#pragma once

#include "../../animation.h"
#include "../../ui/menu.h"
#include "../../ui/widgets/group.h"
#include "../../ui/widgets/button.h"

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
  void DrawMainBackground(float opacity = 1.0f);
};

}  // namespace SG
}  // namespace UI
}  // namespace Impacto