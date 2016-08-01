//--------------------------------------------------------------
// File     : main.c
// Datum    : 22.02.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Funktion : Demo der Systick-Library
// Hinweis  : Diese zwei Files muessen auf 8MHz stehen
//              "cmsis_boot/stm32f4xx.h"
//              "cmsis_boot/system_stm32f4xx.c"
//--------------------------------------------------------------

#include "main.h"
#include "stm32_ub_systick.h"
#include "stm32_ub_led.h"

int main(void)
{
  SystemInit(); // Quarz Einstellungen aktivieren

  UB_Systick_Init(); // init vom System-Counter

  UB_Led_Init(); // init der LEDs


  while(1)
  {
    // 200ms Pause (per Systick)
    UB_Systick_Pause_us(50);
    // LED toggeln
    UB_Led_Toggle(LED_GREEN);

    // Test ob Systick-Timer1 abgelaufen ist
    //if(UB_Systick_Timer1(TIMER_CHECK,0)==TIMER_HOLD) {
      // LED toggeln
     // UB_Led_Toggle(LED_RED);
      // Timer wieder starten (mit 800 ms)
     // UB_Systick_Timer1(TIMER_START_ms,200);
 //   }
  }
}

