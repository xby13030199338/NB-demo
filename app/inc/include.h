#ifndef _INCLUDE_H
#define _INCLUDE_H

#ifdef __cplusplus
extern "C"
{
#endif
  



  
  
  
#include "stdbool.h"
#include "stdlib.h"
#include "string.h"
#include "intrinsics.h"
/*************OS*************/
#include "schedule.h"
#include "sch_cfg.h"
  
/********driver*************/ 
#include "ddl.h"
#include "bt.h"
#include "gpio.h"
#include "lpuart.h"
#include "lpm.h"
#include "vc.h"
#include "wdt.h"
#include "lpt.h"
#include "timer3.h"
#include "flash.h"
#include "adt.h"
#include "rtc.h"
#include "adc.h"
#include "rng.h"
#include "lcd.h"
  
  
#include "APP_IIC.h"
#include "APP_ADC.h"  

#include "APP_Lpuart0.H"
#include "APP_EEPROM.h"
#include "APP_RTC.h"
#include "APP_Random.h"
//#include "APP_NBiot_ZX.h"
#include "APP_LCD.h"
  
#include "Math_Lib.h"
  
/*****app*********/ 
#include "SoftwareTimer.h"
#include "Interrupt_CallBack.h"
#include "State.h"
#include "ZLG_Queue.h"
#include "sysinit.h"
#include "FeedDog.h" 
#include "Sleep.h"
#include "Variable.h"
#include "Buzzer.h"
#include "Valve.h"
#include "MechanicalButton.h"
#include "FlowMeasurement.h"
#include "LCD_JC_IC.h"
/*****ͨ��Э��*********/ 
#include "WM645.h" 
#include "NBProtocol.h"
#include "ModuleBoardProtocol.h"
  
  
  
  #define       BigValve

#ifdef          BigValve
  
  #define       Current_Threshold               100     // 320ma * 0.47 = 150mv
  #define       NumOfAttemptThreshold           3       //���Ŷ�תʱ�� ���Կ�������
  #define       ValveTaskExeNumThreshold        64      //2*32 = 64
  #define       NumOfMaxCurrentThreshold        6       //��תʱ �ﾢʱ�� 
  
#else
  
  #define       Current_Threshold           70      //���Ŷ�ת����220ma
  #define       ValveTrialTimes         5       //���Ŷ�תʱ�� ���Կ�������
  #define       ValveTaskExeNumThreshold   4*3     //8��Ϳ��Կ��ص�λ  ÿ���ж�4�� 
  
#endif
  
  
#define         Cash_Alarm_Threshold       100




  
  
#ifndef u8
#define u8 uint8_t
#endif
#ifndef u16
#define u16 uint16_t
#endif
#ifndef u32
#define u32 uint32_t
  
  
  
#endif
#endif
  
  