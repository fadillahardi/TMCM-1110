//--------------------------------------------------------------
// File     : stm32_ub_icpwm_tim2.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_ICPWM_TIM2_H
#define __STM32F4_UB_ICPWM_TIM2_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"


//--------------------------------------------------------------
// InputCapture Pin (PWM-Messung)
// der Name dient hier nur zur leichteren Lesbarkeit
//--------------------------------------------------------------
//#define ICPWM_T2_PA0  0   // PWM-Messung per TIM2 an PA1
//#define ICPWM_T2_PA5  1
#define ICPWM_T2_PA1  2
//#define ICPWM_T2_PB3  3


//--------------------------------------------------------------
// Input-Capture Settings (Timer 2 frequency)
//
// Frequency_max = 2*APB1 (APB1=42MHz) => TIM_CLK=84MHz
// prescale  : 0 bis 0xFFFF
//
// T2_Frq = TIM_CLK/(prescale+1)
//--------------------------------------------------------------
#define  ICPWM_TIM2_PRESCALE  839     // prescaler => (83 => T2_Frq = 1 MHz)



//--------------------------------------------------------------
// Struktur vom InputCapture Kanals (zur PWM-Messung)
//--------------------------------------------------------------
typedef struct {
  const uint8_t IC_NAME;       // Name
  const uint8_t CHANNEL;       // Channel [1...2]
  GPIO_TypeDef* IC_PORT;       // Port
  const uint16_t IC_PIN;       // Pin
  const uint32_t IC_CLK;       // Clock
  const uint8_t IC_SOURCE;     // Source
}ICPWM_TIM2_t;


//--------------------------------------------------------------
// interne Struktur
//--------------------------------------------------------------
typedef struct {
  uint16_t freq;       // FRQ-Wert
  uint16_t duty;      // DutyCycle
}ICPWM_TIM2_Var_t;


//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void UB_ICPWM_TIM2_Init(void);
uint16_t UB_ICPWM_TIM2_ReadFRQ(void);
uint16_t UB_ICPWM_TIM2_ReadDUTY(void);




//--------------------------------------------------------------
#endif // __STM32F4_UB_ICPWM_TIM2_H
