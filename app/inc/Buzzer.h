#ifndef __Buzzer_H
#define __Buzzer_H

 

#define Buzzer_S        0x11
#define Buzzer_L        0x22


#define Buzzer_En_Port GpioPortD
#define Buzzer_En_Pin  GpioPin6
#define Buzzer_En()    Gpio_ClrIO(Buzzer_En_Port, Buzzer_En_Pin)  
#define Buzzer_Ds()    Gpio_SetIO(Buzzer_En_Port, Buzzer_En_Pin)                    



typedef enum//水表状态变量 
{
  BuzzerShort   = 1,//       
  BuzzerLong    = 2,//        
  BuzzerErr     = 0,// 
} Enum_Buzzer_Name;



extern SCH_TCB  BuzzerTaskTcb;

void Padding_Buzzer_Q(Enum_Buzzer_Name dat);
void Buzzer_Q_Init(void);
void Buzzer_Task(void);

#endif

