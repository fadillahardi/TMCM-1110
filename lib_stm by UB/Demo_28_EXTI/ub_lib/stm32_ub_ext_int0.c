//--------------------------------------------------------------
// File     : stm32_ub_ext_int0.c
// Datum    : 01.04.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : GPIO, EXTI, SYSCFG, MISC
// Funktion : External Interrupt0
//
// Hinweis  : mögliche Pinbelegungen
//            EXT_INT0 :
//               [PA0, PB0, PC0, PD0, PE0, PF0, PG0, PH0, PI0]
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_ext_int0.h"
// ab hier eigene includes
#include "stm32_ub_led.h"



//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
void P_EXT_INT0_LoFlanke(void);
void P_EXT_INT0_HiFlanke(void);



//--------------------------------------------------------------
// Definition vom EXT_INT0 Pin
//
// Widerstand : [GPIO_PuPd_NOPULL,GPIO_PuPd_UP,GPIO_PuPd_DOWN]
// Trigger    : [EXTI_Trigger_Rising,EXTI_Trigger_Falling,EXTI_Trigger_Rising_Falling]
//--------------------------------------------------------------
EXT_INT0_t EXT_INT0 = {
  // PORT , PIN       , CLOCK          ,Source,             ,Widerstand      , Trigger
  GPIOA,GPIO_Pin_0,RCC_AHB1Periph_GPIOA,EXTI_PortSourceGPIOA,GPIO_PuPd_NOPULL,EXTI_Trigger_Rising   // EXT_Int-Pin = PA0
};



//--------------------------------------------------------------
// Init vom external Interrupt 0
//--------------------------------------------------------------
void UB_Ext_INT0_Init(void)
{
  GPIO_InitTypeDef   GPIO_InitStructure;
  EXTI_InitTypeDef   EXTI_InitStructure;
  NVIC_InitTypeDef   NVIC_InitStructure;

  // Clock enable (GPIO)
  RCC_AHB1PeriphClockCmd(EXT_INT0.INT_CLK, ENABLE);

  // Config als Digital-Eingang
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = EXT_INT0.INT_R;
  GPIO_InitStructure.GPIO_Pin = EXT_INT0.INT_PIN;
  GPIO_Init(EXT_INT0.INT_PORT, &GPIO_InitStructure);

  // Clock enable (SYSCONFIG)
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  // EXT_INT0 mit Pin verbinden
  SYSCFG_EXTILineConfig(EXT_INT0.INT_SOURCE, EXTI_PinSource0);

  // EXT_INT0 config
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXT_INT0.INT_TRG;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  // NVIC config
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


//--------------------------------------------------------------
// diese Funktion wird aufgerufen,
// bei einer Lo-Flanke am EXT-INT0
// wenn [EXTI_Trigger_Falling oder EXTI_Trigger_Rising_Falling]
//--------------------------------------------------------------
void P_EXT_INT0_LoFlanke(void)
{
  // hier eigenen Code eintragen

}


//--------------------------------------------------------------
// diese Funktion wird aufgerufen,
// bei einer Hi-Flanke am EXT-INT0
// wenn [EXTI_Trigger_Rising oder EXTI_Trigger_Rising_Falling]
//--------------------------------------------------------------
void P_EXT_INT0_HiFlanke(void)
{
  // hier eigenen Code eintragen
  UB_Led_On(LED_GREEN);
}




//--------------------------------------------------------------
// external Interrupt-0
// wird je nach einstellung der Triggerflanke
// entweder bei Hi- oder Lo- oder beiden Flanken ausgelöst
//--------------------------------------------------------------
void EXTI0_IRQHandler(void)
{
  uint32_t wert;

  if(EXTI_GetITStatus(EXTI_Line0) != RESET)
  {
    // wenn Interrupt aufgetreten
    EXTI_ClearITPendingBit(EXTI_Line0);

    wert=GPIO_ReadInputDataBit(EXT_INT0.INT_PORT, EXT_INT0.INT_PIN);
    if(wert==Bit_RESET) {
      P_EXT_INT0_LoFlanke();
    }
    else {
      P_EXT_INT0_HiFlanke();
    }
  }
}

