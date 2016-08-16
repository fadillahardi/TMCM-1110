#include "Read_ADC.h"

uint16_t Nilai_ADC;

void ADC_PORT_CONFIG(void){
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);

	//karena menggunakan ADC 1 , bisa dipakai IN10 dan IN11 , baca datasheet
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2
			| GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN; //pakai mode Analog input
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // tidak pakai pull up atau pull down
GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void ADC_FEATURE_CONFIG(void){
	//Setting konfigurasi ADC1
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;

	ADC_CommonInit(&ADC_CommonInitStructure);

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion = 1;

	ADC_StructInit(&ADC_InitStructure);
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_EOCOnEachRegularChannelCmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE); //The ADC is powered on by setting the ADON bit in the ADC_CR2 register.
//When the ADON bit is set for the first time, it wakes up the ADC from the Power-down mode.
}

uint16_t read_adc1(uint8_t channel) {
	uint16_t ADC_Value;
	ADC_RegularChannelConfig(ADC1, channel, 1, (uint8_t)0x0F); //setting Channel ADC yg akan diolah
	ADC_SoftwareStartConv(ADC1); //start capture data
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
		; //tunggu konversi selesai
	ADC_Value = ADC_GetConversionValue(ADC1); // get data
	return ADC_Value;
}