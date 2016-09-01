#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "delay.h"
//#include "misc.h"
GPIO_InitTypeDef GPIO_InitStruct;
void TIM4_Init()
{
	//int value;
	TIM_TimeBaseInitTypeDef TIM_Struct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_Struct.TIM_Prescaler = 6;
	TIM_Struct.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_Struct.TIM_Period = 599;
	TIM_Struct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_Struct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_Struct);
	//TIM_Cmd(TIM4, ENABLE);
}

void PWM_Init_TIM4_GPIO()
{
	TIM_OCInitTypeDef TIM_OCStruct;
	//GPIO_InitTypeDef GPIO_InitStruct;

    // Common settings

    // PWM mode 2 = Clear on compare match
    // PWM mode 1 = Set on compare match
    TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;


   // To get proper duty cycle, you have simple equation

   // pulse_length = ((TIM_Period + 1) * DutyCycle) / 100 - 1

   // where DutyCycle is in percent, between 0 and 100%

   // 25% duty cycle:     pulse_length = ((8399 + 1) * 25) / 100 - 1 = 2099
   // 50% duty cycle:     pulse_length = ((8399 + 1) * 50) / 100 - 1 = 4199
   // 75% duty cycle:     pulse_length = ((8399 + 1) * 75) / 100 - 1 = 6299
   // 100% duty cycle:    pulse_length = ((8399 + 1) * 100) / 100 - 1 = 8399

    TIM_OCStruct.TIM_Pulse = 299; // 50% duty cycle
    TIM_OC2Init(TIM4, &TIM_OCStruct);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

    // Clock for GPIOD
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

    // Alternating functions for pins
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);

    // Set pins
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOD, &GPIO_InitStruct);
}

int main()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	SystemInit();
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*if((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))==1)
    {
    	TIM4_Init();
    	PWM_Init_TIM4_GPIO();
    }*/
    TIM4_Init();
    	PWM_Init_TIM4_GPIO();

	while(1);
	{

	}
}
