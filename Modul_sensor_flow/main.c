#include <math.h>
#include "stm32_ub_adc1_single.h"
#include "stm32f4xx_gpio.h"
#include "stm32_ub_dac.h"
#include "stm32_ub_systick.h"
#include "stm32_ub_icpwm_tim2.h"


uint16_t value;
//uint16_t Flow;
uint32_t messwert=0,freuqenz=0,impuls=0,litre,lpm;
//float litre,lpm;
//float convert,R_read,Bxlog,steinhart,inCelcius;
double convert,R_read,steinhart,inCelcius,Bxlog,Temp,x;


void Get_Temp(){

	//reading value from temp_sensor
	Temp = (UB_ADC1_SINGLE_Read_MW(ADC_PC0));

	//conversion method
	convert = (4096/Temp)-1.0;
	R_read = 5000.0/convert;
	x = R_read/10000.0;

	//Getting temperature in celcius
	Bxlog = log(x);
	//steinhart = 298.15*3892.04/(3892.04+(298.15*Bxlog));
	steinhart = 298.15*4268.2146/(4268.2146+(298.15*Bxlog));
	//double T = 1/(0.001125161025848 +(0.000234721098632*log(R_read))+(0.000000085877049*log(R_read)*log(R_read)*log(R_read)));
	inCelcius = steinhart-273.15;

}

//reading freq of flow sensor
void Read_Flow(){

	messwert=UB_ICPWM_TIM2_ReadFRQ();
	if(messwert>0) {
	// if messwert valid
	// read frequency
	freuqenz=(100000/messwert);
	litre = freuqenz/36000.0;
	lpm = litre*60.0;
	// impuls auslesen
	impuls=UB_ICPWM_TIM2_ReadDUTY();
	}
}

//code for speed control
void DAC_motorspeed(){
	for(value=2048;value<4096;value++){
		//Flow = UB_ADC1_SINGLE_Read_MW(ADC_PC1);
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
    	//int r;
    	//for(r=0;r<1000;r++){
    		//Get_Temp();
    		DAC_motorspeed();
    		//Read_Flow();
    		//UB_Systick_Pause_ms(10);
    	//}
	}

}
