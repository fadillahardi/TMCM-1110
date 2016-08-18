#include <math.h>
#include "stm32_ub_adc1_single.h"
#include "stm32f4xx_gpio.h"
#include "stm32_ub_dac.h"
#include "stm32_ub_systick.h"

GPIO_InitTypeDef gpio_InitDef;
uint16_t value;
uint16_t Flow;
//float convert,R_read,Bxlog,steinhart,inCelcius;

/*void Get_Temp(){

	//reading value from temp_sensor
	float Temp = UB_ADC1_SINGLE_Read_MW(ADC_PC0);

	//conversion method
	convert = (4095/Temp)-1.0;
	R_read = 10000/convert;
	float x = R_read/10000;

	//Getting temperature in celcius
	Bxlog = logf(1/((4095/Temp)-1));
	steinhart = 298.15*4268.2146/(4268.2146+(298.15*Bxlog));
	inCelcius = steinhart-273.15;
}*/
int main(void)
{
	double convert,R_read,steinhart,inCelcius,Temp;
	//SystemInit();
    UB_ADC1_SINGLE_Init();
    //UB_DAC_Init(SINGLE_DAC1);
    UB_Systick_Init();
    double Bxlog, x =0;

    while(1)
    {
    	//reading value from temp_sensor
    		Temp = UB_ADC1_SINGLE_Read_MW(ADC_PC0);

    		//conversion method
    		//convert = (4095/Temp)-1.0;
    		//R_read = 10000/convert;
    		//x = R_read/10000;

    		//Getting temperature in celcius
    		x = 1.3;
    		Bxlog = log(x);
    		//steinhart = 298.15*4268.2146/(4268.2146+(298.15*Bxlog));
    		//inCelcius = steinhart-273.15;


    	//this is dac code for pin PA4, 0 to 3.3V
    	/*for(value = 0;value<4096;value++){
    		UB_DAC_SetDAC1(value);
    		UB_Systick_Pause_ms(20);

    	}*/
    }
}
