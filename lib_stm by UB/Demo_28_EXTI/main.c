//--------------------------------------------------------------
// File     : main.c
// Datum    : 01.04.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Funktion : Demo der EXT-Interrupt-Library
// Hinweis  : Diese zwei Files muessen auf 8MHz stehen
//              "cmsis_boot/stm32f4xx.h"
//              "cmsis_boot/system_stm32f4xx.c"
//--------------------------------------------------------------

#include "main.h"
#include "stm32_ub_led.h"
#include "stm32_ub_ext_int0.h"

int main(void)
{
  SystemInit(); // Quarz Einstellungen aktivieren

  // init der LEDs
  UB_Led_Init();

  // init vom Ext-Interrupt-0
  UB_Ext_INT0_Init();

  while(1)
  {
    // Wenn der User-Button gedrückt wird (Hi-Flanke an PA0),
	// wird per ext-Int0 die grüne LED eingeschaltet
  }
}
