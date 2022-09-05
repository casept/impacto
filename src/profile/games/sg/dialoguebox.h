#pragma once

#include "../../../spritesheet.h"

namespace Impacto {
namespace Profile {
namespace SG {
namespace DialogueBox {

void Configure();

int const NamePlateCountMax = 80;

extern Sprite ADVBoxMask;
extern Sprite NamePlateMainSprites[NamePlateCountMax];
extern Sprite NamePlateLabelSprites[NamePlateCountMax];

extern glm::vec2 ADVBoxNamePlateMainPos;
extern glm::vec2 ADVBoxNamePlateLabelPos;

extern float ADVBoxEffectDuration;

}  // namespace DialogueBox
}  // namespace SG
}  // namespace Profile
}  // namespace Impacto