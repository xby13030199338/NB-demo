#include "include.h"


uint8_t WatchDogCount = 0 ;

SCH_TCB  WatchDogTaskTcb;

/*******************************************************************************
** Function name:           Feed_Watch_Dog
** Descriptions:            
** input parameters:        void 
** output parameters:       void
** Returned value:          void 
** Created by:              程卫玺
** Created Date:            2016-5-3  
如果一秒内没有复位看门狗计数器，则复位单片机，给单片机一定的余量。
1：独立看门狗定时1.7s复位。喂狗时间250ms
2：如果上面的独立看门狗也是去作用，那么RTC程序会在一秒后进入，开启窗口看门狗进行复位
*******************************************************************************/
void Feed_Watch_Dog(void)
{
 
  if(++WatchDogCount >= 3)              //250 * 6 = ms
  {
    while(1)
    {
      NVIC_SystemReset();
    }
  }
  else
  {
      Wdt_Feed(); 
  }
}

/*******************************************************************************
** Function name:           Watch_Dog_Task
** Descriptions:            喂单片机内部看门狗任务  250ms溢出复位
** input parameters:        void 
** output parameters:       void
** Returned value:          void 
** Created by:              程卫玺
** Created Date:            2016-5-3   
*******************************************************************************/
void Watch_Dog_Task(void)
{
  SCHTaskBegin();
  
  while (1)
  { 
 
    WatchDogCount = 0;
    SCHCurTaskDly(500 / SCH_SYS_TICKS_MS );  
  }
  
  SCHTaskEnd();
}



