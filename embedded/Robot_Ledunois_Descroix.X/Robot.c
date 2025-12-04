#include "Robot.h"
#include "main.h"
volatile ROBOT_STATE_BITS robotState;
void HandleStartButton(void){
  if (robotState.isStarted == 0 && BOUTON_START == 0){
      robotState.isStarted = 1;
  }
}
