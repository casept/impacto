#pragma once

#include "../../../spritesheet.h"
#include "../../../games/sg/sysmesbox.h"

namespace Impacto {
namespace Profile {
namespace SG {
namespace SysMesBox {

void Configure();

extern Sprite BoxPartLeft;
extern Sprite BoxPartRight;
extern Sprite BoxPartMiddle;
extern Sprite BoxDecoration;
extern Sprite SelectionHighlight;

extern float BoxX;
extern float BoxY;
extern float ChoicePadding;
extern float ChoiceY;
extern float ChoiceXBase;
extern float MinMaxMesWidth;
extern float BoxMinimumWidth;
extern float BoxMiddleBaseX;
extern float BoxMiddleBaseWidth;
extern float BoxRightBaseWidth;
extern float BoxRightRemainPad;

}  // namespace SysMesBox
}  // namespace SG
}  // namespace Profile
}  // namespace Impacto