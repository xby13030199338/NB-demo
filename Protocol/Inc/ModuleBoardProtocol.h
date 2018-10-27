#ifndef __ModuleBoardProtocol_H
#define __ModuleBoardProtocol_H

#ifdef __cplusplus
extern "C" {
#endif 
  
  
  /****
  1�� ����͸�� 
  2�� ��ѯģ����� 
  ****/
  typedef  enum 
  {
    MBProtocol_TransparentTransmission  = 1,// ͸��
    MBProtocol_QueryParameter           = 2,// ������ѯ
    MBProtocol_Err                      = 0x08,
  }Enum_MB_CtrlCode;//lora����״̬
  /*******************NB����֡ ������ �ṹ��************************/
  typedef union 
  {
    uint8_t Byte;
    struct          
    {
      Enum_MB_CtrlCode FunctionCode:  4;    
      bool protocol_type:         1;
      bool follow_up:             1;
      bool ack:                   1;
      bool direction:             1;
    }Str_MB_Ctrl;
  }Union_MB_Ctrl;
  /*******************NB����֡��Ϣ�ṹ��************************/
  typedef union
  {
    uint8_t dat[135] ;
    struct
    { 
      Union_MB_Ctrl     ControlCode     ; //������ 
      uint8_t           Validity        ; //��Ч��
      uint8_t           BufLen          ;         
      uint8_t           Buf[128]        ;
    }Str_MB_Protocol_MSG_Q              ; //polling ������Ϣ����
  }Union_MB_Protocol_MSG_Q              ;
   

  void Resolve_MBoard_Frame(uint8_t *dat,uint8_t len,Union_MB_Protocol_MSG_Q *msg);
  void Combine_MB_Protocol_Frame(Union_MB_Protocol_MSG_Q *msg,Str_Com *tx);
  
  
  
#ifdef __cplusplus
}
#endif
#endif