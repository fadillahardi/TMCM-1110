#include "Read_ADC1.h"

uint16_t nilaiRead;

int main(void)
{
	ADC_PORT_CONFIG();
	ADC_FEATURE_CONFIG();

    while(1)
    {
    	nilaiRead = read_adc1(0);
    }
}
