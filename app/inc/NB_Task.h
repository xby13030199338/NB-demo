#ifndef __NB_Task_H
#define __NB_Task_H

#ifdef __cplusplus
extern "C" {
#endif 
 
  
  
  
extern SCH_TCB  NBTaskTcb;
  


  
  typedef enum 
  { 
    Enum_NB_Iot_Disable              = 0,//disable
    Enum_NB_Iot_Idle                 = 1,//空闲状态  
    Enum_NB_Iot_Reset                = 2,//复位
    Enum_NB_Iot_Activate_Network     = 3,//附着网络
    Enum_NB_Iot_Cell_Reselection     = 4,//小区重选
    Enum_NB_Iot_Polling              = 5,//主动上报
    Enum_NB_Iot_Polling_Ack          = 6,
    Enum_NB_Iot_Rx                   = 7,//接收数据帧
    Enum_NB_Iot_Ack                  = 8,//应答接收数据
  } Enum_NB_State_Machine;
 
  typedef union 
  {
    uint8_t Buf[16];
    struct 
    {
      Enum_NB_State_Machine StateMachine;
      uint8_t               NBResetDay  ;//NB复位天数
      uint8_t               NBFrameNum  ;//帧序号
      uint8_t               NBRegister  ;//网络注册 true注册成功
      uint8_t               NBPolling   ;
    }Str_NBParams;
  }Union_NBParams;

 
  
  
  
  
  
void NB_Iot_Task(void);
uint16_t NB_Iot_State_Machine(void);
  
  
  
  
  

#ifdef __cplusplus
}
#endif
#endif