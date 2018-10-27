#ifndef __NBProtocol_H
#define __NBProtocol_H


#ifdef __cplusplus
extern "C" {
#endif 
  
  
  /**
  0：入网请求 AccessGateway
  1：数据下发（唤醒 ping）
  2：数据上报（ polling）  
  **/
  typedef  enum 
  {    
    NBProtocol_GateWayRegister         = 0,//网关注册 
    NBProtocol_PingData                = 1,//时隙数据透传
    NBProtocol_PollingData             = 2,//上报数据透传
    NBProtocol_Err                     = 0x08,
  }Enum_NB_CtrlCode;//lora网络状态
 
  ///*******************NB数据帧 控制域 结构体************************/
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

  /*******************NB数据帧消息结构体************************/
  typedef union
  {
    uint8_t dat[135] ;
    struct
    { 
      Union_NB_Ctrl     ControlCode     ; //控制码 
      uint8_t           Validity        ; //有效性
      uint8_t           PollingAck      ; //polling应答有效
      uint8_t           FrameNum        ; //帧序号
      uint8_t           Register        ; //入网合法性
      uint8_t           SignalStrength  ; //信号强度
      uint8_t           BufLen          ;         
      uint8_t           Buf[128]        ;
    }Str_NB_Protocol_MSG_Q              ; //polling 任务消息队列
  }Union_NB_Protocol_MSG_Q              ;
  
 
  
void Resolve_NB_Protocol_Frame(uint8_t *spoint,uint8_t len,Union_NB_Protocol_MSG_Q *msg);
void Combine_NB_Protocol_Frame(Union_NB_Protocol_MSG_Q *msg,Str_Com *tx);
  
  
  

  #ifdef __cplusplus
}
#endif
#endif
