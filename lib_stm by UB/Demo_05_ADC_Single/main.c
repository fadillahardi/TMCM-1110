//--------------------------------------------------------------
// File     : main.c
// Datum    : 17.02.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Funktion : Demo der ADC-Single-Library
// Hinweis  : Diese zwei Files muessen auf 8MHz stehen
//              "cmsis_boot/stm32f4xx.h"
//              "cmsis_boot/system_stm32f4xx.c"
//--------------------------------------------------------------

#include "main.h"
#include "stm32_ub_adc1_single.h"
#include "stm32_ub_led.h"

int main(void)
{
  uint16_t adc_wert;

  SystemInit(); // Quarz Einstellungen aktivieren

  UB_ADC1_SINGLE_Init(); // Init vom ADC1
  UB_Led_Init();         // Init der LEDs


  while(1)
  {
	// ADC-Kanal an PA3 messen und 3 LEDs entsprechend schalten
    adc_wert=UB_ADC1_SINGLE_Read_MW(ADC_PA3);
    if(adc_wert>1024) UB_Led_On(LED_GREEN); else UB_Led_Off(LED_GREEN);
    if(adc_wert>2048) UB_Led_On(LED_ORANGE); else UB_Led_Off(LED_ORANGE);
    if(adc_wert>3072) UB_Led_On(LED_RED); else UB_Led_Off(LED_RED);

    // ADC-Kanal an PC4 messen und die Blaue LED entsprechend schalten
    adc_wert=UB_ADC1_SINGLE_Read_MW(ADC_PC4);
    if(adc_wert>2048) UB_Led_On(LED_BLUE); else UB_Led_Off(LED_BLUE);
  }
}
