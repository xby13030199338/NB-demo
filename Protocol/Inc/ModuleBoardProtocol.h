#ifndef __ModuleBoardProtocol_H
#define __ModuleBoardProtocol_H

#ifdef __cplusplus
extern "C" {
#endif 
  
  
  /****
  1： 数据透传 
  2： 查询模块参数 
  ****/
  typedef  enum 
  {
    MBProtocol_TransparentTransmission  = 1,// 透传
    MBProtocol_QueryParameter           = 2,// 参数查询
    MBProtocol_Err                      = 0x08,
  }Enum_MB_CtrlCode;//lora网络状态
  /*******************NB数据帧 控制域 结构体************************/
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
  /*******************NB数据帧消息结构体************************/
  typedef union
  {
    uint8_t dat[135] ;
    struct
    { 
      Union_MB_Ctrl     ControlCode     ; //控制码 
      uint8_t           Validity        ; //有效性
      uint8_t           BufLen          ;         
      uint8_t           Buf[128]        ;
    }Str_MB_Protocol_MSG_Q              ; //polling 任务消息队列
  }Union_MB_Protocol_MSG_Q              ;
   

  void Resolve_MBoard_Frame(uint8_t *dat,uint8_t len,Union_MB_Protocol_MSG_Q *msg);
  void Combine_MB_Protocol_Frame(Union_MB_Protocol_MSG_Q *msg,Str_Com *tx);
  
  
  
#ifdef __cplusplus
}
#endif
#endif