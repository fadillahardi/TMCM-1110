//--------------------------------------------------------------
// File     : main.c
// Datum    : 26.03.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Funktion : Demo der PWM-Library
// Hinweis  : Diese zwei Files muessen auf 8MHz stehen
//              "cmsis_boot/stm32f4xx.h"
//              "cmsis_boot/system_stm32f4xx.c"
//--------------------------------------------------------------

#include "main.h"
#include "stm32_ub_pwm_tim4.h"
#include "stm32_ub_adc1_single.h"

int main(void)
{
  uint16_t ad_wert;

  SystemInit(); // Quarz Einstellungen aktivieren

  // init vom ADC
  UB_ADC1_SINGLE_Init();

  // Init vom PWM mit Timer4
  // Settings vom H-File :
  //   Auflösung = 8bit (0...255)
  //   Frequenz = 1kHz
  //   Kanal1 an PD12
  //   Kanal2 an PD14
  UB_PWM_TIM4_Init();

  // PD12 auf 25% einstellen
  UB_PWM_TIM4_SetPWM(PWM_T4_PD12,64);

  // PD14 auf 50% einstellen
  UB_PWM_TIM4_SetPWM(PWM_T4_PD14,128);

  while(1)
  {
    // AD-Wert an PA3 lesen
	ad_wert=UB_ADC1_SINGLE_Read_MW(ADC_PA3);
	// von 12bit in 8bit umwandeln
	ad_wert=(ad_wert>>4);
	// als PWM Wert an PD12 ausgeben;
	UB_PWM_TIM4_SetPWM(PWM_T4_PD12,ad_wert);
  }
}

