#pragma once

#include <enum.h>
#include "../animation.h"

namespace Impacto {
namespace DelusionTrigger {

BETTER_ENUM(DelusionTriggerType, int, None, CHLCC)

class DelusionTriggerBase {
 public:
  enum DelusionState { DS_Neutral, DS_Positive, DS_Negative };
  enum UiState { Hidden, Showing, Shown, Hiding };
  DelusionTriggerBase(int& delusionState, UiState showState);
  virtual void Update(float dt) = 0;
  virtual void Render() = 0;
  virtual void Show() = 0;
  virtual void Hide() = 0;

  UiState State;
  int& DelusionState;
};

extern DelusionTriggerBase* Implementation;

void Init();
void Update(float dt);
void Show();
void Hide();
void Render();

}  // namespace DelusionTrigger
}  // namespace Impacto