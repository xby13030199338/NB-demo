#include "include.h"


///*����߼��͹���ģʽ*///
#define Sleep()       {  SCB->SCR &=~ 0x04;  __WFI();}
#define DeepSleep()   {  SCB->SCR = 0x04;  __WFI();}


SCH_TCB DeepSleepTaskTcb;

/*******************************************************************************
** Function name:           Sleep_Task
** Descriptions:            ϵͳ�������� ÿ����������ִ��һ�� ע�� STM8L ����֮�� ��Ƶ2M
** input parameters:        void 
** output parameters:       void
** Returned value:          void 
** Created by:              ������
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