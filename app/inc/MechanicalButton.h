
#ifndef __MechanicalButton_H
#define __MechanicalButton_H


#ifdef __cplusplus
extern "C" {
#endif 


typedef enum
{
  Long_Button              = 1, 
  Short_Button             = 2, 
  Null_Button              = 0, 
} Enum_Button_Name;

  
#define Button_Port  GpioPortA
#define Button_Pin   GpioPin3
 
 
 
  
extern SCH_TCB ButtonTaskTcb;

uint16_t Button(void);
void Button_Task(void);
void Set_Button(Enum_Button_Name dat);
Enum_Button_Name Get_Button(void);




#ifdef __cplusplus
}
#endif
#endif




