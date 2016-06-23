//--------------------------------------------------------------
// File     : main.c
// Datum    : 20.08.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Funktion : Demo der Timer Library
// Hinweis  : Diese zwei Files muessen auf 8MHz stehen
//              "cmsis_boot/stm32f4xx.h"
//              "cmsis_boot/system_stm32f4xx.c"
//--------------------------------------------------------------

#include "main.h"
#include "stm32_ub_led.h"
#include "stm32_ub_tim2.h"

int main(void)
{
  SystemInit(); // Quarz Einstellungen aktivieren

  // Init der LEDs
  UB_Led_Init();

  // Init vom Timer2
  // mit f = 1 Hz
  UB_TIMER2_Init(8399,9999);

  // Timer2 starten
  UB_TIMER2_Start();

  while(1)
  {

  }
}

//--------------------------------------------------------------
// ISR von Timer2
//--------------------------------------------------------------
void UB_TIMER2_ISR_CallBack(void)
{
  UB_Led_Toggle(LED_GREEN);	// led blinken lassen
}
