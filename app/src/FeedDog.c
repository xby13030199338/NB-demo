#include "include.h"


uint8_t WatchDogCount = 0 ;

SCH_TCB  WatchDogTaskTcb;

/*******************************************************************************
** Function name:           Feed_Watch_Dog
** Descriptions:            
** input parameters:        void 
** output parameters:       void
** Returned value:          void 
** Created by:              ������
** Created Date:            2016-5-3  
���һ����û�и�λ���Ź�����������λ��Ƭ��������Ƭ��һ����������
1���������Ź���ʱ1.7s��λ��ι��ʱ��250ms
2���������Ķ������Ź�Ҳ��ȥ���ã���ôRTC�������һ�����룬�������ڿ��Ź����и�λ
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
** Descriptions:            ι��Ƭ���ڲ����Ź�����  250ms�����λ
** input parameters:        void 
** output parameters:       void
** Returned value:          void 
** Created by:              ������
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


