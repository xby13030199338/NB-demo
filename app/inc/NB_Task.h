#ifndef __NB_Task_H
#define __NB_Task_H

#ifdef __cplusplus
extern "C" {
#endif 
 
  
  
  
extern SCH_TCB  NBTaskTcb;
  


  
  typedef enum 
  { 
    Enum_NB_Iot_Disable              = 0,//disable
    Enum_NB_Iot_Idle                 = 1,//����״̬  
    Enum_NB_Iot_Reset                = 2,//��λ
    Enum_NB_Iot_Activate_Network     = 3,//��������
    Enum_NB_Iot_Cell_Reselection     = 4,//С����ѡ
    Enum_NB_Iot_Polling              = 5,//�����ϱ�
    Enum_NB_Iot_Polling_Ack          = 6,
    Enum_NB_Iot_Rx                   = 7,//��������֡
    Enum_NB_Iot_Ack                  = 8,//Ӧ���������
  } Enum_NB_State_Machine;
 
  typedef union 
  {
    uint8_t Buf[16];
    struct 
    {
      Enum_NB_State_Machine StateMachine;
      uint8_t               NBResetDay  ;//NB��λ����
      uint8_t               NBFrameNum  ;//֡���
      uint8_t               NBRegister  ;//����ע�� trueע��ɹ�
      uint8_t               NBPolling   ;
    }Str_NBParams;
  }Union_NBParams;

 
  
  
  
  
  
void NB_Iot_Task(void);
uint16_t NB_Iot_State_Machine(void);
  
  
  
  
  

#ifdef __cplusplus
}
#endif
#endif