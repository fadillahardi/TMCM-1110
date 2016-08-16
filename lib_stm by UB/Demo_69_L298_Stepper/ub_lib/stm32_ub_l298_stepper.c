//--------------------------------------------------------------
// File     : stm32_ub_l298_stepper.c
// Datum    : 07.12.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.4
// GCC      : 4.7 2012q4
// Module   : GPIO, TIM, MISC
// Funktion : Schrittmotor Ansteuerung per L298N-IC
//
// Hinweis  : PE4  ->  L298_IN1 (Pin5)
//            PE5  ->  L298_IN2 (Pin7)
//            PE6  ->  L298_IN3 (Pin10)
//            PE7  ->  L298_IN4 (Pin12)
//            PE8  ->  L298_ENA (Pin6)
//            PE9  ->  L298_ENB (Pin11)
//
// L298N ist kompatibel mit 6pol Unipolar-Schrittmotor
// oder 4pol Bipolar-Schrittmotor
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_l298_stepper.h"


//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
void P_L298_InitIO(void);
void P_L298_SetIO(uint8_t wert);
void P_L298_InitTIM(void);
void P_L298_InitNVIC(void);
void P_L298_MakeOneStep(void);
uint8_t P_L298_CalcNextStep(uint32_t mode);
uint32_t P_L298_CalcDelay(uint32_t speed);


//--------------------------------------------------------------
// HalfStep-Mode
// [B5=IN1,B4=IN2,B3=IN3,B2=IN4,B1=ENA,B0=ENB] 
//--------------------------------------------------------------
const uint8_t L298_HalfStep [] = {
  0x17,	// 0 = 0b010111
  0x05,	// 1 = 0b000101
  0x27,	// 2 = 0b100111
  0x22,	// 3 = 0b100010
  0x2B,	// 4 = 0b101011
  0x09,	// 5 = 0b001001
  0x1B,	// 6 = 0b011011
  0x12 	// 7 = 0b010010
};


//--------------------------------------------------------------
// FullStep-Mode 
// [B5=IN1,B4=IN2,B3=IN3,B2=IN4,B1=ENA,B0=ENB] 
//--------------------------------------------------------------
const uint8_t L298_FullStep [] = {
  0x17,	// 0 = 0b010111
  0x27,	// 1 = 0b100111
  0x2B,	// 2 = 0b101011
  0x1B 	// 3 = 0b011011
};


//--------------------------------------------------------------
// Wave-Mode 
// [B5=IN1,B4=IN2,B3=IN3,B2=IN4,B1=ENA,B0=ENB] 
//--------------------------------------------------------------
const uint8_t L298_WaveStep [] = {
  0x05,	// 0 = 0b000101
  0x22,	// 1 = 0b100010
  0x09,	// 2 = 0b001001
  0x12 	// 3 = 0b010010
};


//--------------------------------------------------------------
// init des Moduls als Schrittmotor Ansteuerung
// mode : [L298_HALF_STEP, L298_FULL_STEP, L298_WAVE_STEP]
//--------------------------------------------------------------
void UB_L298_Init(L298_Mode_t mode)
{
  // init der IO-Pins
  P_L298_InitIO();
  // default-Pegel einstellen
  P_L298_SetIO(L298_FullStep[0]);
  // init vom Timer
  P_L298_InitTIM();
  // init vom NVIC
  P_L298_InitNVIC();

  // init der Struktur
  L298.mode=L298_FULL_STEP;
  if(mode==L298_HALF_STEP) L298.mode=L298_HALF_STEP;
  if(mode==L298_WAVE_STEP) L298.mode=L298_WAVE_STEP;
  L298.status=L298_STOP;
  L298.akt_delay=0;
  L298.soll_delay=P_L298_CalcDelay(1); // slow
  L298.step_cycle=0;
  L298.soll_step=0;
  L298.akt_pos=0;
  L298.pause_soll=0;
  L298.pause_ist=0;

  // Timer enable
  TIM_Cmd(L298_TIMER, ENABLE); 
}


//--------------------------------------------------------------
// startet den Schrittmotor nach rechts
// mit einer Geschwindigkeit und einer Anzahl von Schritten
// speed : [1...L298_MAX_SPEED]
// cnt   : Anzahl der Schritte -> 0 = unendlich
//--------------------------------------------------------------
void UB_L298_StepRight(uint32_t speed, uint32_t cnt)
{
  // Motor zuerst stoppen
  L298.soll_delay=P_L298_CalcDelay(L298_MAX_SPEED);
  L298.status=L298_HALT;
  while(L298.status!=L298_STOP);
  // neue Werte einstellen
  L298.soll_delay=P_L298_CalcDelay(speed);
  L298.soll_step=cnt;
  // Motor starten
  L298.status=L298_RIGHT;
}


//--------------------------------------------------------------
// startet den Schrittmotor nach links
// mit einer Geschwindigkeit und einer Anzahl von Schritten
// speed : [1...L298_MAX_SPEED]
// cnt   : Anzahl der Schritte -> 0 = unendlich
//--------------------------------------------------------------
void UB_L298_StepLeft(uint32_t speed, uint32_t cnt)
{
  // Motor zuerst stoppen
  L298.soll_delay=P_L298_CalcDelay(L298_MAX_SPEED);
  L298.status=L298_HALT;
  while(L298.status!=L298_STOP);
  // neue Werte einstellen
  L298.soll_delay=P_L298_CalcDelay(speed);
  L298.soll_step=cnt;
  // Motor starten
  L298.status=L298_LEFT;
}


//--------------------------------------------------------------
// startet den Schrittmotor um eine Position anzufahren
// mit einer Geschwindigkeit
// speed : [1...L298_MAX_SPEED]
// pos : Absolutwert der Position [0...L298_MAX_POS]
//--------------------------------------------------------------
void UB_L298_StepTo(uint32_t speed, uint32_t pos)
{
  if(pos>L298_MAX_POS) pos=L298_MAX_POS;

  // Motor zuerst stoppen
  L298.soll_delay=P_L298_CalcDelay(L298_MAX_SPEED);
  L298.status=L298_HALT;
  while(L298.status!=L298_STOP);
  // zielpos ausrechnen
  if(pos>L298.akt_pos) {
    // neue Werte einstellen
    L298.soll_delay=P_L298_CalcDelay(speed);
    L298.soll_step=pos-L298.akt_pos;
    // Motor starten
    L298.status=L298_RIGHT;
  }
  else if(pos<L298.akt_pos) {
    // neue Werte einstellen
    L298.soll_delay=P_L298_CalcDelay(speed);
    L298.soll_step=L298.akt_pos-pos;
    // Motor starten
    L298.status=L298_LEFT;
  }
}


//--------------------------------------------------------------
// stoppt den Schrittmotor
//--------------------------------------------------------------
void UB_L298_Stop(void)
{
  // Motor stoppen
  L298.soll_delay=P_L298_CalcDelay(L298_MAX_SPEED);
  L298.status=L298_HALT;
  while(L298.status!=L298_STOP);
}


//--------------------------------------------------------------
// testet ob Schrittmotor steht
// diese Funktion kann benutzt werden um zu prüfen
// ob der Schrittmotor seine Zielposition erreicht hat
//
// return_wert :
//   false = Schrittmotor steht nicht
//   true  = Schrittmotor steht
//--------------------------------------------------------------
bool UB_L298_StopCheck(void)
{
  if(L298.status==L298_STOP) return(true);
  return(false);
}


//--------------------------------------------------------------
// setzt die aktuelle Schrittmotor-Position als 0-Referenz
// (geht nur, wenn Motor steht)
//--------------------------------------------------------------
void UB_L298_SetZeroPos(void)
{
  if(L298.status==L298_STOP) {
    L298.akt_pos=0;
  }
}


//--------------------------------------------------------------
// Pausenfunktion
// wartet eine Zeit (in us)
//--------------------------------------------------------------
void UB_L298_Pause_us(uint32_t cnt)
{
  L298.pause_soll=cnt;
  while(L298.pause_ist<L298.pause_soll) {

  }
  L298.pause_soll=0;
}


//--------------------------------------------------------------
// interne Funktion
// init der IO-Pins
//--------------------------------------------------------------
void P_L298_InitIO(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // Init von IN1-Pin
  RCC_AHB1PeriphClockCmd(L298_IN1_CLK,ENABLE);

  GPIO_InitStructure.GPIO_Pin = L298_IN1_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(L298_IN1_PORT, &GPIO_InitStructure);

  // Init von IN2-Pin
  RCC_AHB1PeriphClockCmd(L298_IN2_CLK,ENABLE);

  GPIO_InitStructure.GPIO_Pin = L298_IN2_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(L298_IN2_PORT, &GPIO_InitStructure);

  // Init von IN3-Pin
  RCC_AHB1PeriphClockCmd(L298_IN3_CLK,ENABLE);

  GPIO_InitStructure.GPIO_Pin = L298_IN3_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(L298_IN3_PORT, &GPIO_InitStructure);

  // Init von IN4-Pin
  RCC_AHB1PeriphClockCmd(L298_IN4_CLK,ENABLE);

  GPIO_InitStructure.GPIO_Pin = L298_IN4_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(L298_IN4_PORT, &GPIO_InitStructure);

  // Init von ENA-Pin
  RCC_AHB1PeriphClockCmd(L298_ENA_CLK,ENABLE);

  GPIO_InitStructure.GPIO_Pin = L298_ENA_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(L298_ENA_PORT, &GPIO_InitStructure);

  // Init von ENB-Pin
  RCC_AHB1PeriphClockCmd(L298_ENB_CLK,ENABLE);

  GPIO_InitStructure.GPIO_Pin = L298_ENB_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(L298_ENB_PORT, &GPIO_InitStructure);
}


//--------------------------------------------------------------
// interne Funktion
// Pegel der IO-Leitungen einstellen
// 0b000000 bis 0b111111
// [B5=IN1,B4=IN2,B3=IN3,B2=IN4,B1=ENA,B0=ENB] 
//--------------------------------------------------------------
void P_L298_SetIO(uint8_t wert) {
  // IN1
  if((wert&0x20)==0) {
    L298_IN1_PORT->BSRRH = L298_IN1_PIN;
  }
  else {
    L298_IN1_PORT->BSRRL = L298_IN1_PIN;
  }
  // IN2
  if((wert&0x10)==0) {
    L298_IN2_PORT->BSRRH = L298_IN2_PIN;
  }
  else {
    L298_IN2_PORT->BSRRL = L298_IN2_PIN;
  }
  // IN3
  if((wert&0x08)==0) {
    L298_IN3_PORT->BSRRH = L298_IN3_PIN;
  }
  else {
    L298_IN3_PORT->BSRRL = L298_IN3_PIN;
  }
  // IN4
  if((wert&0x04)==0) {
    L298_IN4_PORT->BSRRH = L298_IN4_PIN;
  }
  else {
    L298_IN4_PORT->BSRRL = L298_IN4_PIN;
  }
  // ENA
  if((wert&0x02)==0) {
    L298_ENA_PORT->BSRRH = L298_ENA_PIN;
  }
  else {
    L298_ENA_PORT->BSRRL = L298_ENA_PIN;
  }
  // ENB
  if((wert&0x01)==0) {
    L298_ENB_PORT->BSRRH = L298_ENB_PIN;
  }
  else {
    L298_ENB_PORT->BSRRL = L298_ENB_PIN;
  }
}


//--------------------------------------------------------------
// interne Funktion
// init vom Timer
//--------------------------------------------------------------
void P_L298_InitTIM(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

  // Clock enable
  RCC_APB1PeriphClockCmd(L298_TIMER_CLK, ENABLE);

  // Timer disable
  TIM_Cmd(L298_TIMER, DISABLE);

  // Timer init
  TIM_TimeBaseStructure.TIM_Period =  L298_TIM_PERIODE;
  TIM_TimeBaseStructure.TIM_Prescaler = L298_TIM_PRESCALER;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(L298_TIMER, &TIM_TimeBaseStructure);

  // Timer preload enable
  TIM_ARRPreloadConfig(L298_TIMER, ENABLE);
}


//--------------------------------------------------------------
// interne Funktion
// init vom NVIC
//--------------------------------------------------------------
void P_L298_InitNVIC(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  // Update Interrupt enable
  TIM_ITConfig(L298_TIMER,TIM_IT_Update,ENABLE);

  // NVIC konfig
  NVIC_InitStructure.NVIC_IRQChannel = L298_TIMER_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);  
}


//--------------------------------------------------------------
// interne Funktion
// einen Schritt vom Schrittmotor
//--------------------------------------------------------------
void P_L298_MakeOneStep(void)
{
  uint8_t wert;

  switch(L298.status) {
    case L298_STOP :
      // keinen neuen Schritt ausgeben
    break;
    case L298_RIGHT :
      wert=P_L298_CalcNextStep(0);
      P_L298_SetIO(wert);
      if(L298.soll_step>0) {
        L298.soll_step--;
        if(L298.soll_step==0) {
          L298.soll_delay=P_L298_CalcDelay(L298_MAX_SPEED);
          L298.status=L298_HALT;
        }
        if(L298.akt_pos<L298_MAX_POS) L298.akt_pos++;
      }
    break;
    case L298_LEFT :
      wert=P_L298_CalcNextStep(1);
      P_L298_SetIO(wert);
      if(L298.soll_step>0) {
        L298.soll_step--;
        if(L298.soll_step==0) {
          L298.soll_delay=P_L298_CalcDelay(L298_MAX_SPEED);
          L298.status=L298_HALT;
        }
        if(L298.akt_pos>0) L298.akt_pos--;
      }
    break;
    case L298_HALT :
      L298.soll_delay=P_L298_CalcDelay(L298_MAX_SPEED);
      L298.status=L298_STOP;
    break;
  }
}


//--------------------------------------------------------------
// interne Funktion
// berechnet die Bit-Pegel für den naechsten Schritt
// 0=rechtsrum , 1=linksrum
//--------------------------------------------------------------
uint8_t P_L298_CalcNextStep(uint32_t mode) {
  uint8_t ret_wert=0;

  if(mode==0) {
    if(L298.mode==L298_HALF_STEP) {
      L298.step_cycle++;
      if(L298.step_cycle>7) L298.step_cycle=0;
      ret_wert=L298_HalfStep[L298.step_cycle];
    }
    else if(L298.mode==L298_FULL_STEP) {
      L298.step_cycle++;
      if(L298.step_cycle>3) L298.step_cycle=0;
      ret_wert=L298_FullStep[L298.step_cycle];
    }
    else {
      L298.step_cycle++;
      if(L298.step_cycle>3) L298.step_cycle=0;
      ret_wert=L298_WaveStep[L298.step_cycle];
    }
  }
  else {
    if(L298.mode==L298_HALF_STEP) {
      if(L298.step_cycle==0) L298.step_cycle=8;
      L298.step_cycle--;
      ret_wert=L298_HalfStep[L298.step_cycle];
    }
    else if(L298.mode==L298_FULL_STEP) {
      if(L298.step_cycle==0) L298.step_cycle=4;
      L298.step_cycle--;
      ret_wert=L298_FullStep[L298.step_cycle];
    }
    else {
      if(L298.step_cycle==0) L298.step_cycle=4;
      L298.step_cycle--;
      ret_wert=L298_WaveStep[L298.step_cycle];
    }
  }

  return(ret_wert);
}


//--------------------------------------------------------------
// interne Funktion
// berrechnet aus der Geschwindigkeit das Delay
//--------------------------------------------------------------
uint32_t P_L298_CalcDelay(uint32_t speed)
{
  uint32_t ret_wert=0;
  float f;

  if(speed>L298_MAX_SPEED) speed=L298_MAX_SPEED;
  if(speed<1) speed=1;

  if(L298.mode==L298_HALF_STEP) {
    f=(float)(L298_MAX_SPEED)/(float)(speed);
    f=f*(float)(L298_MIN_DELAY_HS);

    ret_wert=(uint32_t)(f);
    if(ret_wert>L298_MAX_DELAY_HS) ret_wert=L298_MAX_DELAY_HS;
    if(ret_wert<L298_MIN_DELAY_HS) ret_wert=L298_MIN_DELAY_HS;
  }
  else if(L298.mode==L298_FULL_STEP) {
    f=(float)(L298_MAX_SPEED)/(float)(speed);
    f=f*(float)(L298_MIN_DELAY_FS);

    ret_wert=(uint32_t)(f);
    if(ret_wert>L298_MAX_DELAY_FS) ret_wert=L298_MAX_DELAY_FS;
    if(ret_wert<L298_MIN_DELAY_FS) ret_wert=L298_MIN_DELAY_FS;
  }
  else {
    f=(float)(L298_MAX_SPEED)/(float)(speed);
    f=f*(float)(L298_MIN_DELAY_WS);

    ret_wert=(uint32_t)(f);
    if(ret_wert>L298_MAX_DELAY_WS) ret_wert=L298_MAX_DELAY_WS;
    if(ret_wert<L298_MIN_DELAY_WS) ret_wert=L298_MIN_DELAY_WS;
  }

  return(ret_wert);
}


//--------------------------------------------------------------
// interne Funktion
// Timer-Interrupt
// wird alle 10us aufgerufen
//--------------------------------------------------------------
void L298_TIM_ISR_HANDLER(void)
{
  if (TIM_GetITStatus(L298_TIMER, TIM_IT_Update) != RESET) {
    // wenn Interrupt aufgetreten
    TIM_ClearITPendingBit(L298_TIMER, TIM_IT_Update);
    
    if(L298.pause_soll>0) {
      L298.pause_ist+=10;
    }
    else {
      L298.pause_ist=0;
    }


    L298.akt_delay+=10;
    if(L298.akt_delay>=L298.soll_delay) {
      L298.akt_delay=0;
      P_L298_MakeOneStep();
    }
  }
}
