
#include "include.h"


SCH_TCB ButtonTaskTcb;

static Enum_Button_Name ButtonDat = Null_Button;



Enum_Button_Name Get_Button(void)
{
  return(ButtonDat);
}

void Set_Button(Enum_Button_Name dat)
{
  ButtonDat = dat;
}
/****************************************************************************
        返回值 1；代表有按键发生 无论长短
               0：代表无按键发生
50ms调用一次  短按键要小于2秒（40次）  长按键 等于3秒（60次）  
---------------|      |-----------------
               |______|
                   |    | 
               |   |    |
                   |    |
---------------|                              |-----------------
               |______________________________|
                   |    |      |      |      |     |
               |   |    |      |      |      |     |
                  小于2S的低电平不用处理，
                         小于2S的高电平认为是短按键，
                                      大于3S的低电平认为是长按键   
                                               大于3S的高电平不处理
****************************************************************************/


void Button_Task(void)
{
  static uint16_t ButtonTaskDelay = 0xffff;
  SCHTaskBegin();
  
  while (1)
  {
    
    ButtonTaskDelay = Button();
    if(ButtonTaskDelay == 0xffff)
    {
        W_State(&SysState,Buttoning,false);
        SCHCurTaskPend();
    }
    else
    {
      SCHCurTaskDly( 500 / SCH_HW_TIM_MS );  //delay 250ms 
    }
  }
  
  SCHTaskEnd();
}



uint16_t Button(void)
{
  static uint8_t LowLevelNum=0;
  uint16_t delay;
  
  delay = 500;

  LowLevelNum++;
  if(LowLevelNum <= 5)//2秒以内
  {
    if (Gpio_GetInputIO(Button_Port, Button_Pin) == 1)
    {
        LowLevelNum = 0; 
        ButtonDat = Short_Button;//短按键
        Padding_Buzzer_Q(BuzzerShort);SCHTaskResume(BuzzerTaskTcb);SCHTaskResume(LCDTaskTcb);
        delay = 0xffff; 
    }
  }
  else if(LowLevelNum <= 7)//2到3秒
  {
    if (Gpio_GetInputIO(Button_Port, Button_Pin) == 1)
    {
        LowLevelNum = 0; 
        ButtonDat = Null_Button;//短按键
        delay = 0xffff; 
    }
  }
  else//大于3秒
  {
    LowLevelNum = 0; 
    ButtonDat = Long_Button;//短按键
    Padding_Buzzer_Q(BuzzerLong);SCHTaskResume(BuzzerTaskTcb);
    delay = 0xffff; 
  }
  
  return(delay);
}

