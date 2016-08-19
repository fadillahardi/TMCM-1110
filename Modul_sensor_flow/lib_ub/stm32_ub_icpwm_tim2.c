//--------------------------------------------------------------
// File     : stm32_ub_icpwm_tim2.c
// Datum    : 20.07.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.0
// Module   : GPIO, TIM, MISC
// Funktion : Input-Capture (PWM-Messung) per Timer2
//
// Hinweis  : mögliche Pinbelegungen
//            CH1 : [PA0, PA5]
//            CH2 : [PA1, PB3]
//
// Settings : Timer2-Frq steht auf 1MHz = 1us (siehe H-File)
//            Ein Messwert von 1     => 1us     = 1MHz
//            Ein Messwert von 10    => 10us    = 100kHz
//            Ein Messwert von 65535 => 65,53ms = 15 Hz

// This file has edited for further use by user 
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_icpwm_tim2.h"



//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
void P_ICPWM_InitIO(void);
void P_ICPWM_InitTIM(void); 
void P_ICPWM_InitNVIC(void);


//--------------------------------------------------------------
// Globale Variabeln
//--------------------------------------------------------------
ICPWM_TIM2_Var_t ICPWM_TIM2_Var;


//--------------------------------------------------------------
// Definition vom InputCapture Pin (zur PWM-Messung)
//
// Channel : [1...2]
//--------------------------------------------------------------
ICPWM_TIM2_t ICPWM_TIM2 = {
  // Name       ,Channel, PORT , PIN       , CLOCK               , Source
  //ICPWM_T2_PA0 ,1      ,GPIOA ,GPIO_Pin_0 ,RCC_AHB1Periph_GPIOA ,GPIO_PinSource0,
  //ICPWM_T2_PA5 ,1      ,GPIOA ,GPIO_Pin_5 ,RCC_AHB1Periph_GPIOA ,GPIO_PinSource5,
  ICPWM_T2_PA1 ,2      ,GPIOA ,GPIO_Pin_1 ,RCC_AHB1Periph_GPIOA ,GPIO_PinSource1,
  //ICPWM_T2_PB3 ,2      ,GPIOB ,GPIO_Pin_3 ,RCC_AHB1Periph_GPIOB ,GPIO_PinSource3,
};



//--------------------------------------------------------------
// init und start vom InputCapture (PWM-Messung) mit Timer2
//-------------------------------------------------------------- 
void UB_ICPWM_TIM2_Init(void)
{
  // init aller Variabeln
  ICPWM_TIM2_Var.freq=0;
  ICPWM_TIM2_Var.duty=0;

  // init der Funktionen
  P_ICPWM_InitIO();
  P_ICPWM_InitTIM();
  P_ICPWM_InitNVIC();
}


//--------------------------------------------------------------
// auslesen vom Input-Capture Wert (Frq)
//
// Return_Wert : 0         => Messwert nicht gültig
//               1...65535 => Messwert
// Formeln :
//  Mess_Frq = T2_Frq / Messwert
//  Messwert = T2_Frq / Mess_Frq
//--------------------------------------------------------------
uint16_t UB_ICPWM_TIM2_ReadFRQ(void)
{
  uint16_t read_val=0;
  
  read_val=ICPWM_TIM2_Var.freq;
  ICPWM_TIM2_Var.freq=0;

  return(read_val);
}


//--------------------------------------------------------------
// auslesen vom Input-Capture Wert (DutyCycle)
//
// Return_Wert : 0         => Messwert nicht gültig
//               1...65535 => Messwert
// Formeln :
//  Impuls   = 1/T2_Frq * Messwert
//  Messwert = T2_Frq * Impuls
//--------------------------------------------------------------
uint16_t UB_ICPWM_TIM2_ReadDUTY(void)
{
  uint16_t read_val=0;
 
  read_val=ICPWM_TIM2_Var.duty;
  ICPWM_TIM2_Var.duty=0;  

  return(read_val);
}


//--------------------------------------------------------------
// interne Funktion
// Init aller IO-Pins
//--------------------------------------------------------------
void P_ICPWM_InitIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // Clock Enable
  RCC_AHB1PeriphClockCmd(ICPWM_TIM2.IC_CLK, ENABLE);

  // Config des Pins als AF-Input
  GPIO_InitStructure.GPIO_Pin = ICPWM_TIM2.IC_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(ICPWM_TIM2.IC_PORT, &GPIO_InitStructure);

  // Alternative-Funktion mit dem IO-Pin verbinden
  GPIO_PinAFConfig(ICPWM_TIM2.IC_PORT, ICPWM_TIM2.IC_SOURCE, GPIO_AF_TIM2);
}



//--------------------------------------------------------------
// interne Funktion
// Init vom Timer
//--------------------------------------------------------------
void P_ICPWM_InitTIM(void)
{
  TIM_ICInitTypeDef  TIM_ICInitStructure;

  // Clock enable
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 

  // Vorteiler einstellen
  TIM_PrescalerConfig(TIM2, ICPWM_TIM2_PRESCALE, TIM_PSCReloadMode_Immediate);
  
  if(ICPWM_TIM2.CHANNEL==1) {
    // Channel 1
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_PWMIConfig(TIM2, &TIM_ICInitStructure);

    // input Trigger
    TIM_SelectInputTrigger(TIM2, TIM_TS_TI1FP1);      
  }
  if(ICPWM_TIM2.CHANNEL==2) {
    // Channel 2
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_PWMIConfig(TIM2, &TIM_ICInitStructure);

    // input Trigger
    TIM_SelectInputTrigger(TIM2, TIM_TS_TI2FP2);
  }

  // Slave-Mode (Reset)
  TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);
  TIM_SelectMasterSlaveMode(TIM2,TIM_MasterSlaveMode_Enable);

  // Timer enable
  TIM_Cmd(TIM2, ENABLE);
}


//--------------------------------------------------------------
// interne Funktion
// Init vom NVIC
//--------------------------------------------------------------
void P_ICPWM_InitNVIC(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  // NVIC init
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  if(ICPWM_TIM2.CHANNEL==1) {
    // Channel 1
    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
  }
  if(ICPWM_TIM2.CHANNEL==2) {
    // Channel 2
    TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
  }
}




//--------------------------------------------------------------
// ISR von Timer2
// wird bei einem Capture-Event aufgerufen
//--------------------------------------------------------------
void TIM2_IRQHandler(void)
{ 
  if(TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET) {
    // Interrupt Flags loeschen
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

    // aktuellen Frq-Wert auslesen
    ICPWM_TIM2_Var.freq = TIM_GetCapture1(TIM2);

    // aktuellen DutyCycle auslesen
    ICPWM_TIM2_Var.duty = TIM_GetCapture2(TIM2);    
  }

  if(TIM_GetITStatus(TIM2, TIM_IT_CC2) == SET) {
    // Interrupt Flags loeschen
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);

    // aktuellen Frq-Wert auslesen
    ICPWM_TIM2_Var.freq = TIM_GetCapture2(TIM2);

    // aktuellen DutyCycle auslesen
    ICPWM_TIM2_Var.duty = TIM_GetCapture1(TIM2);    
  }
}
