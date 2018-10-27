
#include "include.h"



SCH_TCB  BuzzerTaskTcb;



static uint8_t CtrBuzzer[32]={0} ;   //蜂鸣器命令数据队列

//蜂鸣器队列初始化
void Buzzer_Q_Init(void)
{
  queueCreate(CtrBuzzer,sizeof(CtrBuzzer),0,0); 
}


void Padding_Buzzer_Q(Enum_Buzzer_Name dat)
{
  if( dat <= 2 )
  {
    queueWrite(CtrBuzzer,dat);
    SCHTaskDly(BuzzerTaskTcb,0);
  }
}


void Buzzer_Task(void)
{
  static uint8_t CtrBuzzerData;
  
  SCHTaskBegin();
 
  while(1)
  {
    if(queueNData(CtrBuzzer)>0)
    {
      queueRead(CtrBuzzer,&CtrBuzzerData);
      
      if(CtrBuzzerData == BuzzerShort)//如果是短 
      {
        Buzzer_En();
        SCHCurTaskDly(500 / SCH_SYS_TICKS_MS);
      }
      else                        //如果是长      
      {
        Buzzer_En();
        SCHCurTaskDly(1000 / SCH_SYS_TICKS_MS);
      }
      Buzzer_Ds();
    }
    else
    {
      Buzzer_Ds();
      SCHCurTaskPend();
    } 
  }
 
  
  SCHTaskEnd();
}
