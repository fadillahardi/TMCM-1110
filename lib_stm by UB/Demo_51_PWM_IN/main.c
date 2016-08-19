//--------------------------------------------------------------
// File     : main.c
// Datum    : 20.07.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Funktion : Demo der InputCapture PWM-Library
// Hinweis  : Diese zwei Files muessen auf 8MHz stehen
//              "cmsis_boot/stm32f4xx.h"
//              "cmsis_boot/system_stm32f4xx.c"
//--------------------------------------------------------------

#include "main.h"
#include "stm32_ub_icpwm_tim2.h"

uint32_t messwert=0,freuqenz=0,impuls=0;
int main(void)
{
  //uint32_t messwert=0,freuqenz=0,impuls=0;

  SystemInit(); // Quarz Einstellungen aktivieren

  // Init vom Timer2 zur PWM-Messung
  UB_ICPWM_TIM2_Init();

  while(1)
  {
    // gemessene FRQ an PA1 auslesen
    messwert=UB_ICPWM_TIM2_ReadFRQ();
    if(messwert>0) {
      // wenn Messwert gueltig
      // frequenz ausrechnen
      freuqenz=(1000000/messwert);
      // impuls auslesen
      impuls=UB_ICPWM_TIM2_ReadDUTY();
    }
  }
}

