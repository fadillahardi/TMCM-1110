
#include "stm32_ub_adc1_single.h"
#include "stm32f4xx_gpio.h"
#include "stm32_ub_dac.h"
#include "stm32_ub_systick.h"

GPIO_InitTypeDef gpio_InitDef;
uint16_t value;
uint16_t Flow;
uint16_t Temp;


int main(void)
{

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	//GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
	//GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
	gpio_InitDef.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	gpio_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	gpio_InitDef.GPIO_OType = GPIO_OType_PP;
	gpio_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	//gpio_InitDef.GPIO_Mode = GPIO_Mode_AF;
	gpio_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &gpio_InitDef);


    SystemInit();
    UB_ADC1_SINGLE_Init();
    UB_DAC_Init(SINGLE_DAC1);
    UB_Systick_Init();

    while(1)
    {
    	//reading sensors
    	//Temp = UB_ADC1_SINGLE_Read_MW(ADC_PC0);
    	Flow = UB_ADC1_SINGLE_Read(ADC_PC1);

    	//this is dac code for pin PA4, 0 to 3.3V
    	for(value = 0;value<4096;value++){
    		UB_DAC_SetDAC1(value);
    		Temp = UB_ADC1_SINGLE_Read_MW(ADC_PC0);
    		UB_Systick_Pause_ms(20);

    	}
    }
}
