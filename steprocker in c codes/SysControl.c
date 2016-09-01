/*******************************************************************************
  Project: stepRocker Mini-TMCL

  Module:  SysControl.c
           Motor monitoring (automatic current switching etc.)

   Copyright (C) 2011 TRINAMIC Motion Control GmbH & Co KG
                      Waterloohain 5
                      D - 22769 Hamburg, Germany
                      http://www.trinamic.com/

   This program is free software; you can redistribute it and/or modify it
   freely.

   This program is distributed "as is" in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
   or FITNESS FOR A PARTICULAR PURPOSE.
*******************************************************************************/

/**
  \file SysControl.c
  \author Trinamic Motion Control GmbH & Co KG
  \version 1.00

  \brief Motor monitoring

  This file contains the SystemControl function which does all necessary motor
  monitoring tasks.
*/

#include <limits.h>
#include <stdlib.h>
#include "csp.h"
#include "bits.h"
#include "stepRocker.h"
#include "Globals.h"
#include "IO.h"
#include "SysTick.h"
#include "TMC429.h"
#include "TMC262.h"


static UCHAR ActualAxis;                      //!< monitored axis
static UINT Delay;                            //!< Delay timer
static UCHAR StandbyTimerInitFlag;            //!< TRUE after the standby timers have been intialized
static UINT StandbyTimer[N_O_MOTORS];         //!< Standby timers
static UINT FreewheelingTimer[N_O_MOTORS];    //!< Freewheeling timers


/***************************************************************//**
   \fn SystemControl(void)
   \brief Motor monitoring

   This function must be called periodically from the main loop and
   does some monitoring tasks, e.g. lowering the current after the
   motor has not been moving for some time.
********************************************************************/
void SystemControl(void)
{
  int Velocity;
  UINT i;

  //Initialisierung der Standby-Timer (nur beim allerersten Durchlauf)
  if(!StandbyTimerInitFlag)
  {
    for(i=0; i<N_O_MOTORS; i++) StandbyTimer[i]=INT_MAX;
    StandbyTimerInitFlag=TRUE;
  }

  if(abs(GetSysTimer()-Delay)>10) //10ms-Tasks
  {
    Velocity=Read429Short(MOTOR_NUMBER(ActualAxis)<<5|IDX_VACTUAL);

    //Stromregelung (Umschaltung Run current / Stand by current)
    if(Velocity==0 && Read429Short(MOTOR_NUMBER(ActualAxis)<<5|IDX_VTARGET)==0)
    {
      if(abs(GetSysTimer()-StandbyTimer[ActualAxis])>MotorConfig[ActualAxis].SettingDelay*10)
      {
        SetMotorCurrent(ActualAxis, MotorConfig[ActualAxis].IStandby);
        StandbyFlag[ActualAxis]=TRUE;
      }
    }
    else
    {
      //Zus�tzliche Stromabsenkung bei niedriger Geschwindigkeit
      //(kann abgeschaltet werden indem man ThresholdSpeed auf 0 setzt).
      if(abs(Velocity)<CoolStepConfig[ActualAxis].ThresholdSpeed)
        SetMotorCurrent(ActualAxis, CoolStepConfig[ActualAxis].SlowRunCurrent);
      else
        SetMotorCurrent(ActualAxis, MotorConfig[ActualAxis].IRun);

      StandbyTimer[ActualAxis]=GetSysTimer();
      StandbyFlag[ActualAxis]=FALSE;
    }

      //Geschwindigkeitsabh�ngiges Ein- und Ausschalten von CoolStep
      //(nicht wenn ThresholdSpeed==0)
      if(abs(Velocity)<CoolStepConfig[ActualAxis].ThresholdSpeed)
      {
        //CoolStep ausschalten wenn nicht schon ausgeschaltet
        if(Get262SmartEnergyStallLevelMin(WHICH_262(ActualAxis))==CoolStepConfig[ActualAxis].HysteresisStart)
        {
          Set262SmartEnergyStallLevelMin(WHICH_262(ActualAxis), 0);
        }
      }
      else
      {
        //CoolStep einschalten wenn nicht bereits eingeschaltet (auf den mit SAP 172 eingestellten Wert
        //zur�cksetzen - wenn dieser 0 ist bleibt CoolStep nat�rlich ausgeschaltet).
        if(Get262SmartEnergyStallLevelMin(WHICH_262(ActualAxis))!=CoolStepConfig[ActualAxis].HysteresisStart)
        {
          Set262SmartEnergyStallLevelMin(WHICH_262(ActualAxis), CoolStepConfig[ActualAxis].HysteresisStart);
        }
      }

    //�berwachung der Treiber
    Read262State(WHICH_262(ActualAxis), NULL, NULL, &StallLevel[ActualAxis], &SmartEnergy[ActualAxis], &DriverFlags[ActualAxis]);

    //Stop bei Stall
    if(MotorConfig[ActualAxis].StallVMin>0 && GetStallState(ActualAxis))
    {
      if(abs(Velocity)>MotorConfig[ActualAxis].StallVMin)
      {
        HardStop(ActualAxis);
        StallFlag[ActualAxis]=TRUE;
      }
    }

    //Freewheeling Delay: Motor stromlos schalten nach angebener Stillstandzeit
    if((Velocity==0 && Read429Short(MOTOR_NUMBER(ActualAxis)<<5|IDX_VTARGET)==0) || MotorDisable[ActualAxis])
    {
      if(MotorConfig[ActualAxis].FreewheelingDelay>0 || MotorDisable[ActualAxis])
      {
        if(abs(GetSysTimer()-FreewheelingTimer[ActualAxis])>MotorConfig[ActualAxis].FreewheelingDelay*10)
        {
          if(!FreewheelingActive[ActualAxis])
          {
            Disable262(WHICH_262(ActualAxis));
            FreewheelingActive[ActualAxis]=TRUE;
          }
        }
      }
    }
    else FreewheelingTimer[ActualAxis]=GetSysTimer();

    //N�chste Achse
    ActualAxis++;
    if(ActualAxis>=N_O_MOTORS) ActualAxis=0;

    Delay=GetSysTimer();
  }
}
