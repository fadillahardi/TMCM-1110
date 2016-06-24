//--------------------------------------------------------------
// File     : stm32_ub_pwm_tim5.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_PWM_TIM5_H
#define __STM32F4_UB_PWM_TIM5_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"


//--------------------------------------------------------------
// Liste aller PWM Pins
// (keine Nummer doppelt und von 0 beginnend)
//--------------------------------------------------------------
typedef enum 
{
  PWM_T5_PA2 = 0   // PWM per TIM5 an PA2
}PWM_TIM5_NAME_t;

#define  PWM_TIM5_ANZ   1 // Anzahl von PWM_TIM5_NAME_t


//--------------------------------------------------------------
// PWM Einstellungen (Frequenz und Aufl�sung)
// periode   = Aufl�sung (max = 0xFFFF => 16bit)
// prescaler = Frquenz
//
// Grundfrequenz = 2*APB1 (APB1=42MHz) => TIM_CLK=84MHz
// periode   : 0 bis 0xFFFF
// prescale  : 0 bis 0xFFFF
//
// PWM-Frq = TIM_CLK/(periode+1)/(vorteiler+1)
//--------------------------------------------------------------
#define  PWM_TIM5_PERIODE   0xFF // periode   (0xFF => 8bit)
#define  PWM_TIM5_PRESCALE   327 // prescaler (328 => 1kHz)


//--------------------------------------------------------------
// PWM Setting (Polarit�t)
//
// Hi => Hi-Impulse
// Lo => Lo-Impulse
//--------------------------------------------------------------
#define  PWM_TIM5_POLARITY  TIM_OCPolarity_High
//#define  PWM_TIM5_POLARITY  TIM_OCPolarity_Low


//--------------------------------------------------------------
// Struktur eines PWM Kanals
//--------------------------------------------------------------
typedef struct {
  PWM_TIM5_NAME_t PWM_NAME; // Name
  const uint8_t CHANNEL;    // Channel [1...4]
  GPIO_TypeDef* PWM_PORT;   // Port
  const uint16_t PWM_PIN;   // Pin
  const uint32_t PWM_CLK;   // Clock
  const uint8_t PWM_SOURCE; // Source
  const uint16_t PWM_INIT;  // Init
}PWM_TIM5_t;


//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void UB_PWM_TIM5_Init(void);
void UB_PWM_TIM5_SetPWM(PWM_TIM5_NAME_t pwm, uint16_t wert);



//--------------------------------------------------------------
#endif // __STM32F4_UB_PWM_TIM5_H