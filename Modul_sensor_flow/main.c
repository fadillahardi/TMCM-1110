#include <math.h>
#include "stm32_ub_adc1_single.h"
#include "stm32f4xx_gpio.h"
#include "stm32_ub_dac.h"
#include "stm32_ub_systick.h"
#include "stm32_ub_icpwm_tim2.h"


uint16_t value;
uint16_t Flow;
uint32_t messwert=0,freuqenz=0,impuls=0;
//float convert,R_read,Bxlog,steinhart,inCelcius;
double convert,R_read,steinhart,inCelcius,Bxlog,Temp,x;


void Get_Temp(){

	//reading value from temp_sensor
	Temp = UB_ADC1_SINGLE_Read_MW(ADC_PC0);

	//conversion method
	convert = (4095/Temp)-1.0;
	R_read = 10000.0/convert;
	x = R_read/10000.0;

	//Getting temperature in celcius
	Bxlog = log(x);
	steinhart = 298.15*4268.2146/(4268.2146+(298.15*Bxlog));
	inCelcius = steinhart-273.15;
}

//reading freq of flow sensor
void Read_Flow(){

	messwert=UB_ICPWM_TIM2_ReadFRQ();
	if(messwert>0) {
	// wenn Messwert gueltig
	// frequenz ausrechnen
	freuqenz=(1000000/messwert);
	// impuls auslesen
	impuls=UB_ICPWM_TIM2_ReadDUTY();
	}
}

//code for speed control
void DAC_motorspeed(){
	for(value=0;value<2048;value++){
		Flow = UB_ADC1_SINGLE_Read_MW(ADC_PC1);
		UB_DAC_SetDAC1(value);
		UB_Systick_Pause_ms(20);
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
    	Get_Temp();
    	DAC_motorspeed();
    	Read_Flow();
	}

}
