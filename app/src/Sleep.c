#include "include.h"


///*进入高级低功耗模式*///
#define Sleep()       {  SCB->SCR &=~ 0x04;  __WFI();}
#define DeepSleep()   {  SCB->SCR = 0x04;  __WFI();}


SCH_TCB DeepSleepTaskTcb;

/*******************************************************************************
** Function name:           Sleep_Task
** Descriptions:            系统休眠任务 每个调度周期执行一次 注意 STM8L 唤醒之后 主频2M
** input parameters:        void 
** output parameters:       void
** Returned value:          void 
** Created by:              程卫玺
** Created Date:            2016-5-3   
*******************************************************************************/

void Deep_Sleep_Task(void)
{
  SCHTaskBegin();
  
  while (1)
  {
//	Gpio_ClrIO(GpioPortA,GpioPin15);
	DeepSleep();
//	Gpio_SetIO(GpioPortA,GpioPin15);
	SCHCurTaskDly( SCH_SYS_TICKS_MS / SCH_SYS_TICKS_MS ); 
  }
  
  SCHTaskEnd();
}
