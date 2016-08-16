#ifndef _READ_ADC1_H_
#define _READ_ADC1_H_

//#include <stdint.h>
//#include <stdbool.h>

void ADC_PORT_CONFIG(void);
void ADC_FEATURE_CONFIG(void);
uint16_t read_adc1(uint8_t channel);
//uint8_t warna();
//int isLine();
//int isDepan();
//bool cekGaris();

#endif