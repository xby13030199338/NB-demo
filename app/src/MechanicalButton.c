
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
        ����ֵ 1�������а������� ���۳���
               0�������ް�������
50ms����һ��  �̰���ҪС��2�루40�Σ�  ������ ����3�루60�Σ�  
---------------|      |-----------------
               |______|
                   |    | 
               |   |    |
                   |    |
---------------|                              |-----------------
               |______________________________|
                   |    |      |      |      |     |
               |   |    |      |      |      |     |
                  С��2S�ĵ͵�ƽ���ô�����
                         С��2S�ĸߵ�ƽ��Ϊ�Ƕ̰�����
                                      ����3S�ĵ͵�ƽ��Ϊ�ǳ�����   
                                               ����3S�ĸߵ�ƽ������
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
  if(LowLevelNum <= 5)//2������
  {
    if (Gpio_GetInputIO(Button_Port, Button_Pin) == 1)
    {
        LowLevelNum = 0; 
        ButtonDat = Short_Button;//�̰���
        Padding_Buzzer_Q(BuzzerShort);SCHTaskResume(BuzzerTaskTcb);SCHTaskResume(LCDTaskTcb);
        delay = 0xffff; 
    }
  }
  else if(LowLevelNum <= 7)//2��3��
  {
    if (Gpio_GetInputIO(Button_Port, Button_Pin) == 1)
    {
        LowLevelNum = 0; 
        ButtonDat = Null_Button;//�̰���
        delay = 0xffff; 
    }
  }
  else//����3��
  {
    LowLevelNum = 0; 
    ButtonDat = Long_Button;//�̰���
    Padding_Buzzer_Q(BuzzerLong);SCHTaskResume(BuzzerTaskTcb);
    delay = 0xffff; 
  }
  
  return(delay);
}
