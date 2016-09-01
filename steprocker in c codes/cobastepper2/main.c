#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32_ub_systick.h"
#include "stm32_ub_pwm_tim4.h"

GPIO_InitTypeDef gpio_InitDef;

void run_stepper(long del_us)
{

	GPIO_ResetBits(GPIOD, GPIO_Pin_14);
	GPIO_SetBits(GPIOD, GPIO_Pin_13);
	UB_Systick_Pause_us(del_us);
	GPIO_ResetBits(GPIOD, GPIO_Pin_13);
	//UB_Systick_Pause_us(del_us);
	//GPIO_ResetBits(GPIOD, GPIO_Pin_14);

}

void runUseL298(long del_us)
{
	//step1
	GPIO_SetBits(GPIOE, GPIO_Pin_11);
	GPIO_ResetBits(GPIOE, GPIO_Pin_12);
	GPIO_SetBits(GPIOE, GPIO_Pin_13);
	GPIO_ResetBits(GPIOE, GPIO_Pin_14);
	UB_Systick_Pause_us(del_us);

	//step2
	GPIO_ResetBits(GPIOE, GPIO_Pin_11);
	GPIO_SetBits(GPIOE, GPIO_Pin_12);
	GPIO_SetBits(GPIOE, GPIO_Pin_13);
	GPIO_ResetBits(GPIOE, GPIO_Pin_14);
	UB_Systick_Pause_us(del_us);

	//step3
	GPIO_ResetBits(GPIOE, GPIO_Pin_11);
	GPIO_SetBits(GPIOE, GPIO_Pin_12);
	GPIO_ResetBits(GPIOE, GPIO_Pin_13);
	GPIO_SetBits(GPIOE, GPIO_Pin_14);
	UB_Systick_Pause_us(del_us);

	//step4
	GPIO_SetBits(GPIOE, GPIO_Pin_11);
	GPIO_ResetBits(GPIOE, GPIO_Pin_12);
	GPIO_ResetBits(GPIOE, GPIO_Pin_13);
	GPIO_SetBits(GPIOE, GPIO_Pin_14);
	UB_Systick_Pause_us(del_us);
}

void runCoil1(long del_us)
{
	//coil1(pin E11, E12)
	GPIO_SetBits(GPIOE, GPIO_Pin_11);
	GPIO_ResetBits(GPIOE, GPIO_Pin_12);
	GPIO_SetBits(GPIOE, GPIO_Pin_13);
	GPIO_ResetBits(GPIOE, GPIO_Pin_14);
	UB_Systick_Pause_us(del_us);
	GPIO_ResetBits(GPIOE, GPIO_Pin_11);
	GPIO_SetBits(GPIOE, GPIO_Pin_12);
	GPIO_SetBits(GPIOE, GPIO_Pin_13);
	GPIO_ResetBits(GPIOE, GPIO_Pin_14);
	UB_Systick_Pause_us(del_us);
}

void runCoil2(long del_us)
{
	//coil2(pin E13, E14)
	GPIO_SetBits(GPIOE, GPIO_Pin_13);
	GPIO_ResetBits(GPIOE, GPIO_Pin_14);
	UB_Systick_Pause_us(0.5*(del_us));
	GPIO_ResetBits(GPIOE, GPIO_Pin_13);
	GPIO_SetBits(GPIOE, GPIO_Pin_14);
	UB_Systick_Pause_us(del_us);
}

void main()
{
	SystemInit();
	UB_Systick_Init();
	//UB_PWM_TIM4_Init();
	//init GPIOA
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	gpio_InitDef.GPIO_Pin = GPIO_Pin_0;
	gpio_InitDef.GPIO_Mode = GPIO_Mode_IN;
	gpio_InitDef.GPIO_OType = GPIO_OType_PP;
	gpio_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &gpio_InitDef);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	gpio_InitDef.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14;
	gpio_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	gpio_InitDef.GPIO_OType = GPIO_OType_PP;
	gpio_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOE, &gpio_InitDef);

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

	int x,step,rotate;
	long y=0 ,newDelay=0;
	int flag=0;



    while(1)
    {
    	flag = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
    	if(flag ==1)
    	{
    		runUseL298(50000);
    		//runCoil1(10000);
    		//UB_Systick_Pause_us(5000);
    		//runCoil2(10000);

    	}
    	/*if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)==1)
    		{
    			//for(x=0;x<12800;x++)
    			//{
    			run_stepper(40);
    			//}
    		}
    	flag = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
    	if(flag ==1)
    	{
    		for(y=150;y>10;y--)
    		{
    			for(x=0;x<100;x++)
    			{
    				run_stepper(y);
    			}
    			if(y==25)
    			{
    				flag = 2;
    				newDelay=y;
    				break;


    			}
    		}
    	}
    	if(flag==2)
    	{
    		for(rotate=0;rotate<10;rotate++)
    		{
    			for(step=0;step<12800;step++)
    			    		{
    			    			run_stepper(newDelay);
    			    		}
    		}
    	}*/


    }
}
