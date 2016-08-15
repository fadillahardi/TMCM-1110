//--------------------------------------------------------------
// File     : main.c
// Datum    : 23.03.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Funktion : Demo der DAC-Library
// Hinweis  : Diese zwei Files muessen auf 8MHz stehen
//              "cmsis_boot/stm32f4xx.h"
//              "cmsis_boot/system_stm32f4xx.c"
//--------------------------------------------------------------

#include "main.h"
#include "stm32_ub_dac.h"

int main(void)
{
  uint16_t wert;

  SystemInit(); // Quarz Einstellungen aktivieren

  // init vom DAC (nur DAC-1)
  UB_DAC_Init(SINGLE_DAC1);

  while(1)
  {
	  // erzeugen eines Sägezahn-Signals
	  // von 0 bis 3,3V an PA4
	  // mit einer Periodendauer von ca T=2ms (f=ca 500Hz)
	  for(wert=0;wert<4096;wert++) {
		  UB_DAC_SetDAC1(wert);
	  }
  }
}

