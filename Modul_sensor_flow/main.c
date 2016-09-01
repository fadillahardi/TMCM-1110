#include <math.h>
#include "stm32_ub_adc1_single.h"
#include "stm32f4xx_gpio.h"
#include "stm32_ub_dac.h"
#include "stm32_ub_systick.h"
#include "stm32_ub_icpwm_tim2.h"


uint16_t value;

//flowmeter variables
uint16_t frq_read=0,frq_get=0,impuls=0,litre,rate = 0;

//temperature sensor variables
double convert,R_read,steinhart,inCelcius,Bxlog,Temp,x;
float lpm;

void Get_Temp(){
	//reading value from sensor
	Temp = (UB_ADC1_SINGLE_Read_MW(ADC_PC0));

	//conversion method
	convert = (4096/Temp)-1.0;
	R_read = 5000.0/convert;
	x = R_read/10000.0;

	//Getting temperature in celcius
	Bxlog = log(x);
	//steinhart = 298.15*3892.04/(3892.04+(298.15*Bxlog));
	steinhart = 298.15*4268.2146/(4268.2146+(298.15*Bxlog));
	inCelcius = steinhart-273.15;
}

//reading freq of flow sensor
void Read_Flow(){
	frq_read=UB_ICPWM_TIM2_ReadFRQ();
	if(frq_read>0) {
	// if reading valid
	// read frequency
	frq_get=(100000/frq_read);
	litre = frq_get/36000.0;
	//lpm = frq_get*60.0/k_fact;
	impuls=UB_ICPWM_TIM2_ReadDUTY();
	if(impuls>0){
		rate = 100000/impuls;
		lpm = impuls/600.0;
	}
	}
}

//code for speed control
void DAC_motorspeed(){
	for(value=1024;value<3000;value++){
		Get_Temp();
		Read_Flow();
		UB_DAC_SetDAC1(value);
		UB_Systick_Pause_ms(10);
	}
}
int main(void)
{
	SystemInit();
    UB_ADC1_SINGLE_Init();
    UB_Systick_Init();
    UB_DAC_Init(SINGLE_DAC1);
    UB_ICPWM_TIM2_Init();

    while(1)
    {
    	DAC_motorspeed();
	}

}
