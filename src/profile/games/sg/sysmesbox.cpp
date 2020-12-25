#include "sysmesbox.h"
#include "../../profile_internal.h"

namespace Impacto {
namespace Profile {
namespace Sg {
namespace SysMesBox {

Sprite BoxPartLeft;
Sprite BoxPartRight;
Sprite BoxPartMiddle;
Sprite BoxDecoration;

float BoxX;
float BoxY;
float BoxMinimumWidth;
float BoxMiddleBaseX;
float BoxMiddleBaseWidth;
float BoxRightBaseWidth;
float BoxRightRemainPad;

Impacto::SysMesBox::SysMesBoxBase* Configure() {
  Impacto::Sg::SysMesBox* result = new Impacto::Sg::SysMesBox();

  BoxPartLeft = EnsureGetMemberSprite("BoxPartLeft");
  BoxPartRight = EnsureGetMemberSprite("BoxPartRight");
  BoxPartMiddle = EnsureGetMemberSprite("BoxPartMiddle");
  BoxDecoration = EnsureGetMemberSprite("BoxDecoration");

  BoxX = EnsureGetMemberFloat("BoxX");
  BoxY = EnsureGetMemberFloat("BoxY");
  BoxMinimumWidth = EnsureGetMemberFloat("BoxMinimumWidth");
  BoxMiddleBaseX = EnsureGetMemberFloat("BoxMiddleBaseX");
  BoxMiddleBaseWidth = EnsureGetMemberFloat("BoxMiddleBaseWidth");
  BoxRightBaseWidth = EnsureGetMemberFloat("BoxRightBaseWidth");
  BoxRightRemainPad = EnsureGetMemberFloat("BoxRightRemainPad");

  return result;
}

}  // namespace SysMesBox
}  // namespace Sg
}  // namespace Profile
}  // namespace Impacto