//--------------------------------------------------------------
// File     : main.c
// Datum    : 07.12.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.4
// GCC      : 4.7 2012q4
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Funktion : Demo der L298-Stepper-Library
// Hinweis  : Diese zwei Files muessen auf 8MHz stehen
//              "cmsis_boot/stm32f4xx.h"
//              "cmsis_boot/system_stm32f4xx.c"
//--------------------------------------------------------------

#include "main.h"
#include "stm32_ub_l298_stepper.h"

int main(void)
{
  SystemInit(); // Quarz Einstellungen aktivieren

  // init vom L298 im FullStep-Mode
  UB_L298_Init(L298_FULL_STEP);

  while(1)
  {
    // 1sec warten
    UB_L298_Pause_us(1000000);
    // Schrittmotor 30 Schritte nach rechts
    // mit 80% Speed
    UB_L298_StepRight(35,1000);
    // 1sec warten
    UB_L298_Pause_us(1000000);
    // Schrittmotor 30 Schritte nach links
    // mit 35% Speed
    UB_L298_StepLeft(35,1000);
  }
}

