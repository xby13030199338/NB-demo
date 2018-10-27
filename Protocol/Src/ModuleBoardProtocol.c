#include "include.h"


/*************解析 电路板模块之间协议*****************/
void Resolve_MBoard_Frame(uint8_t *dat,uint8_t len,Union_MB_Protocol_MSG_Q *msg)
{
  
  uint8_t buf[128];
  
  if(len==0 || len>=128)
    msg->Str_MB_Protocol_MSG_Q.Validity = false;//数据帧错误
  else
    memcpy(&buf[0],&*dat,len);//接收无线的数据帧 
  
  if( (buf[0] == 0x68) && (Count_CheckSum(&buf[0],(len-1)) == buf[buf[1]+10]))
  { 
    msg->Str_MB_Protocol_MSG_Q.ControlCode.Byte = buf[2];    //提取控制码
    msg->Str_MB_Protocol_MSG_Q.Validity = false;//数据帧有效

    switch(msg->Str_MB_Protocol_MSG_Q.ControlCode.Str_MB_Ctrl.FunctionCode) 
    {
    case MBProtocol_TransparentTransmission:    //模块间透明传输
      {
        memcpy(&(msg->Str_MB_Protocol_MSG_Q.Buf[0]),&buf[4],buf[1]);//把数据域全部读出来 等待下一步解析
        msg->Str_MB_Protocol_MSG_Q.BufLen = buf[1];
        msg->Str_MB_Protocol_MSG_Q.Validity = true;//数据帧有效
      }
      break;
    case MBProtocol_QueryParameter:             //查询NB模块参数
      {
        
        msg->Str_MB_Protocol_MSG_Q.Validity = true;//数据帧有效
      }
      break;
    default:
      {}
      break;
    }
  }
  else
  {
    msg->Str_MB_Protocol_MSG_Q.Validity = false;//数据帧错误
  }
  
}



//组织模块间通信协议
void Combine_MB_Protocol_Frame(Union_MB_Protocol_MSG_Q *msg,Str_Com *tx)
{

 switch(msg->Str_MB_Protocol_MSG_Q.ControlCode.Str_MB_Ctrl.FunctionCode)
  {
  case MBProtocol_TransparentTransmission://入网请求 上报
    {

    }
    break;
  case MBProtocol_QueryParameter://数据抄读 上报应答
    {

    }
    break;
  default:
    break;
  }

  tx->buf[tx->len  ] = Count_CheckSum(&(tx->buf[0]),tx->len);
  tx->len++;
 
}







