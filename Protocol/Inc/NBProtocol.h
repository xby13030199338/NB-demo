#ifndef __NBProtocol_H
#define __NBProtocol_H


#ifdef __cplusplus
extern "C" {
#endif 
  
  
  /**
  0���������� AccessGateway
  1�������·������� ping��
  2�������ϱ��� polling��  
  **/
  typedef  enum 
  {    
    NBProtocol_GateWayRegister         = 0,//����ע�� 
    NBProtocol_PingData                = 1,//ʱ϶����͸��
    NBProtocol_PollingData             = 2,//�ϱ�����͸��
    NBProtocol_Err                     = 0x08,
  }Enum_NB_CtrlCode;//lora����״̬
 
  ///*******************NB����֡ ������ �ṹ��************************/
  typedef union 
  {
    uint8_t Byte;
    struct          
    {
      Enum_NB_CtrlCode FunctionCode:  4;    
      bool protocol_type:         1;
      bool follow_up:             1;
      bool ack:                   1;
      bool direction:             1;
    }Str_NB_Ctrl;
  }Union_NB_Ctrl;

  /*******************NB����֡��Ϣ�ṹ��************************/
  typedef union
  {
    uint8_t dat[135] ;
    struct
    { 
      Union_NB_Ctrl     ControlCode     ; //������ 
      uint8_t           Validity        ; //��Ч��
      uint8_t           PollingAck      ; //pollingӦ����Ч
      uint8_t           FrameNum        ; //֡���
      uint8_t           Register        ; //�����Ϸ���
      uint8_t           SignalStrength  ; //�ź�ǿ��
      uint8_t           BufLen          ;         
      uint8_t           Buf[128]        ;
    }Str_NB_Protocol_MSG_Q              ; //polling ������Ϣ����
  }Union_NB_Protocol_MSG_Q              ;
  
 
  
void Resolve_NB_Protocol_Frame(uint8_t *spoint,uint8_t len,Union_NB_Protocol_MSG_Q *msg);
void Combine_NB_Protocol_Frame(Union_NB_Protocol_MSG_Q *msg,Str_Com *tx);
  
  
  

  #ifdef __cplusplus
}
#endif
#endif