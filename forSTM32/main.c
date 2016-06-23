#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "delay.h"
#include "math.h"


GPIO_InitTypeDef gpio_InitDef;
void init_all()
{
	GPIO_ResetBits(GPIOD, GPIO_Pin_13);
	GPIO_ResetBits(GPIOD, GPIO_Pin_14);
}

void run_stepper_mod1(int steps, int delay)
{
	//this is code for running the stepper
	//full rotate (360 degree) = 12800 steps
	int doSteps;
	GPIO_ResetBits(GPIOD, GPIO_Pin_14);
	//delayUs(100);
	for(doSteps=0;doSteps<=steps;doSteps++)
	{
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
		delayUs(delay);
		GPIO_ResetBits(GPIOD, GPIO_Pin_13);
		delayUs(delay);
	}
	//GPIO_ResetBits(GPIOD, GPIO_Pin_14);
	//delayUs(100);
}

void TIMER_Init()
{
	TIM_TimeBaseInitTypeDef TIM_BaseStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_BaseStruct.TIM_Prescaler = 0;
	TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_BaseStruct.TIM_Period = 4199;
    TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseStruct.TIM_RepetitionCounter = 0;
    /* Initialize TIM4 */
    TIM_TimeBaseInit(TIM4, &TIM_BaseStruct);
    /* Start count on TIM4 */
    TIM_Cmd(TIM4, ENABLE);

}

void PWM_Init()
{
	TIM_OCInitTypeDef TIM_OCStruct;

	TIMER_Init();

	TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_High;

    //TIM_OCStruct.TIM_Pulse = 2099; /* 25% duty cycle */
    //TIM_OC1Init(TIM4, &TIM_OCStruct);
    //TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_OCStruct.TIM_Pulse = 1049; /* 50% duty cycle */
    TIM_OC2Init(TIM4, &TIM_OCStruct);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_OCStruct.TIM_Pulse = 4199; /* 75% duty cycle */
    TIM_OC3Init(TIM4, &TIM_OCStruct);
    TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);

    //TIM_OCStruct.TIM_Pulse = 8399; /* 100% duty cycle */
    //TIM_OC4Init(TIM4, &TIM_OCStruct);
    //TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
}
void main(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	//GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	//GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);

	gpio_InitDef.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14;
	gpio_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	gpio_InitDef.GPIO_OType = GPIO_OType_PP;
	gpio_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//gpio_InitDef.GPIO_Mode = GPIO_Mode_AF;
	gpio_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &gpio_InitDef);

	delayInit();
	init_all();
	//SystemInit();
	//PWM_Init();
	run_stepper_mod1(12800, 100);

	while(1)
    {
		//run_stepper_mod1(10, 100);
		//delayMs(10);

    }
}

