#ifndef __Valve_H
#define __Valve_H


#ifdef __cplusplus
extern "C" {
#endif 
  
  

#define Close            0x55  //管阀门命令
#define Open             0x99  //开阀门命令
  
#define Valve_Open_Port  GpioPortC
#define Valve_Open_Pin   GpioPin12
#define Valve_Close_Port GpioPortD
#define Valve_Close_Pin  GpioPin2 
  
#define Motor_Close_H()  Gpio_SetIO(Valve_Close_Port, Valve_Close_Pin) 
#define Motor_Close_L()  Gpio_ClrIO(Valve_Close_Port, Valve_Close_Pin)
  
#define Motor_Open_H()   Gpio_SetIO(Valve_Open_Port, Valve_Open_Pin)
#define Motor_Open_L()   Gpio_ClrIO(Valve_Open_Port, Valve_Open_Pin)

  typedef enum//水表状态变量 
  {
    Forward_Rotation_State        = 0,//正向转动状态
    Reversal_Rotation_State       = 1,//反向转动状态
    Angle_Rotation_State          = 2,//开度转动状态
  } Enum_Valve_Rotation_Status_Name;
  
 extern SCH_TCB ValveTaskTcb            ;
extern SCH_TCB ValveSelfCheckTask      ; 





void Close_Valve(void);
void Open_Valve(void);
void Disable_Valve(void);

void Valve_Q_Init(void);
uint8_t Get_Valve_Buf_Num(void);
void Padding_Valve_Comm(uint8_t comm,uint8_t enable,uint8_t angle);
void Valve_Task(void); 
void Perform_Valve_Task(void);
  
#ifdef __cplusplus
}
#endif
#endif

