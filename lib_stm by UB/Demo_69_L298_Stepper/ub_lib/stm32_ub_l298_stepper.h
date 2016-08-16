//--------------------------------------------------------------
// File     : stm32_ub_l298_stepper.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_L298_STEPPER_H
#define __STM32F4_UB_L298_STEPPER_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "misc.h"
#include <stdbool.h>


//--------------------------------------------------------------
// Motor Settings
// kleinstes Delay mit dem der Motor noch funktioniert
// und keine Schritte überspringt oder verschluckt
// => groesste Geschwindigkeit
// alle Werte in us
//--------------------------------------------------------------
#define  L298_MIN_DELAY_HS    1400  // fullspeed = 1,4ms bei HalfStep
#define  L298_MIN_DELAY_FS    2700  // fullspeed = 2,7ms bei FullStep
#define  L298_MIN_DELAY_WS    4000  // fullspeed = 4,0ms bei WaveStep


//--------------------------------------------------------------
// Motor Settings
// grösstes Delay das benutzt werden soll
// => kleinste Geschwindigkeit
// alle Werte in us
//--------------------------------------------------------------
#define  L298_MAX_DELAY_HS    250000  // lowspeed = 250ms bei HalfStep
#define  L298_MAX_DELAY_FS    500000  // lowspeed = 500ms bei FullStep
#define  L298_MAX_DELAY_WS    600000  // lowspeed = 600ms bei WaveStep


//--------------------------------------------------------------
// Defines fuer die Geschwindigkeitsangabe
//--------------------------------------------------------------
#define  L298_MAX_SPEED       100     // Geschwindigkeit als Prozentwert


//--------------------------------------------------------------
// Defines fuer die Positionsangabe
//--------------------------------------------------------------
#define  L298_MAX_POS         1000


//--------------------------------------------------------------
// L298 Port-Pins
//--------------------------------------------------------------
#define L298_IN1_PIN          GPIO_Pin_4
#define L298_IN1_PORT         GPIOE
#define L298_IN1_CLK          RCC_AHB1Periph_GPIOE

#define L298_IN2_PIN          GPIO_Pin_5
#define L298_IN2_PORT         GPIOE
#define L298_IN2_CLK          RCC_AHB1Periph_GPIOE

#define L298_IN3_PIN          GPIO_Pin_6
#define L298_IN3_PORT         GPIOE
#define L298_IN3_CLK          RCC_AHB1Periph_GPIOE

#define L298_IN4_PIN          GPIO_Pin_7
#define L298_IN4_PORT         GPIOE
#define L298_IN4_CLK          RCC_AHB1Periph_GPIOE

#define L298_ENA_PIN          GPIO_Pin_8
#define L298_ENA_PORT         GPIOE
#define L298_ENA_CLK          RCC_AHB1Periph_GPIOE

#define L298_ENB_PIN          GPIO_Pin_9
#define L298_ENB_PORT         GPIOE
#define L298_ENB_CLK          RCC_AHB1Periph_GPIOE


//--------------------------------------------------------------
// Timer definitionen
// F_TIM = 84 MHz / (prescaler+1) / (periode+1)
// F_TIM = 10us
// Hinweis : nicht kleiner als 10us einstellen !!
//--------------------------------------------------------------
#define L298_TIMER            TIM2
#define L298_TIMER_CLK        RCC_APB1Periph_TIM2
#define L298_TIMER_IRQ        TIM2_IRQn
#define L298_TIM_PERIODE      19
#define L298_TIM_PRESCALER    41
#define L298_TIM_ISR_HANDLER  TIM2_IRQHandler


//--------------------------------------------------------------
typedef enum {
  L298_HALF_STEP = 0,  // Halb-Schritt-Mode
  L298_FULL_STEP,      // Voll-Schritt-Mode
  L298_WAVE_STEP       // Wave-Mode
}L298_Mode_t;


//--------------------------------------------------------------
typedef enum {
  L298_STOP  = 0,      // Schrittmotor steht
  L298_RIGHT,          // Schrittmotor dreht rechts
  L298_LEFT,           // Schrittmotor dreht links
  L298_HALT            // Schrittmotor wird angehalten
}L298_Status_t;


//--------------------------------------------------------------
// interne Struktur von L298
//--------------------------------------------------------------
typedef struct {
  L298_Mode_t mode;
  L298_Status_t status;
  uint32_t akt_delay;
  uint32_t soll_delay;
  uint32_t step_cycle;
  uint32_t soll_step;
  uint32_t akt_pos;
  uint32_t pause_soll;
  uint32_t pause_ist;
}L298_t;
L298_t L298;



//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
void UB_L298_Init(L298_Mode_t mode);
void UB_L298_StepRight(uint32_t speed, uint32_t cnt);
void UB_L298_StepLeft(uint32_t speed, uint32_t cnt);
void UB_L298_StepTo(uint32_t speed, uint32_t pos);
void UB_L298_Stop(void);
bool UB_L298_StopCheck(void);
void UB_L298_SetZeroPos(void);
void UB_L298_Pause_us(uint32_t cnt);





//--------------------------------------------------------------
#endif // __STM32F4_UB_L298_STEPPER_H
