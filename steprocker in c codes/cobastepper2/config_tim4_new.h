
#ifndef __STM32F4_UB_PWM_TIM4_H
#define __STM32F4_UB_PWM_TIM4_H

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"


typedef enum
{
  PWM_T4_PD12 = 0,
  PWM_T4_PD13 = 1,
  PWM_T4_PD14 = 2,
  PWM_T4_PD15 = 3
}PWM_TIM4_NAME_t;

#define  PWM_TIM4_ANZ  4


//--------------------------------------------------------------
// PWM Einstellungen (Frequenz und Auflösung)
// periode   = Auflösung (max = 0xFFFF => 16bit)
// prescaler = Frquenz
//
// Grundfrequenz = 2*APB1 (APB1=42MHz) => TIM_CLK=84MHz
// periode   : 0 bis 0xFFFF
// prescale  : 0 bis 0xFFFF
//
// PWM-Frq = TIM_CLK/(periode+1)/(vorteiler+1)
//--------------------------------------------------------------
#define  PWM_TIM4_PERIODE   0xFFFF // periode   (0xFF => 8bit)
#define  PWM_TIM4_PRESCALE   119 // prescaler (328 => 1kHz)


//--------------------------------------------------------------
// PWM Setting (Polarität)
//
// Hi => Hi-Impulse
// Lo => Lo-Impulse
//--------------------------------------------------------------
#define  PWM_TIM4_POLARITY  TIM_OCPolarity_High
//#define  PWM_TIM4_POLARITY  TIM_OCPolarity_Low


//--------------------------------------------------------------
// Struktur eines PWM Kanals
//--------------------------------------------------------------
//typedef struct {
//  PWM_TIM4_NAME_t PWM_NAME; // Name
//  const uint8_t CHANNEL;    // Channel [1...4]
//  GPIO_TypeDef* PWM_PORT;   // Port
//  const uint16_t PWM_PIN;   // Pin
//  const uint32_t PWM_CLK;   // Clock
//  const uint8_t PWM_SOURCE; // Source
//  const uint16_t PWM_INIT;  // Init
//}PWM_TIM4_t;


//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void UB_PWM_TIM4_Init(void);
void UB_PWM_TIM4_SetPWM(PWM_TIM4_NAME_t pwm, uint16_t wert);



//--------------------------------------------------------------
#endif // __STM32F4_UB_PWM_TIM4_H
