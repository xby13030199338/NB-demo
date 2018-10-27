#include "include.h"



/**************************8
Union_Lora_Protocol_MSG_Q msg;
        
Resolve_LoRa_Protocol_Frame(&LoraRx.buf[0],LoraRx.len,&msg);    //解析lora组网数据帧 
if(msg.Str_Lora_Protocol_MSG_Q.Validity == true)//数据有效性
{

    Str_Lora tx;
    
    Resolve_645(&msg.Str_Lora_Protocol_MSG_Q.Buf[0],msg.Str_Lora_Protocol_MSG_Q.BufLen,&tx);
            
    if(msg.Str_Lora_Protocol_MSG_Q.ControlCode.Str_LoRa_Ctrl.ack == true)
    {
      memcpy(&msg.Str_Lora_Protocol_MSG_Q.Buf[0],&tx.buf[0],tx.len);//解析回传新达科技数据帧 提取有效部分 组织成645 
      msg.Str_Lora_Protocol_MSG_Q.BufLen = tx.len;
      Combine_LoRa_Protocol_Frame(&msg);
 
    }
}

  uint8_t tem[]={0x78,0x56,0x34,0x12,0x01,0x02,0x34,0xF3};
  Str_Lora  tx;

  memcpy(&msg.Str_Lora_Protocol_MSG_Q.Buf[0],&tem[0],8);//接收无线的数据帧 
  msg.Str_Lora_Protocol_MSG_Q.BufLen = 8;
  Resolve_645(&msg.Str_Lora_Protocol_MSG_Q.Buf[0],msg.Str_Lora_Protocol_MSG_Q.BufLen,&tx);
  memcpy(&msg.Str_Lora_Protocol_MSG_Q.Buf[0],&tx.buf[0],tx.len);//解析回传新达科技数据帧 提取有效部分 组织成645 
  msg.Str_Lora_Protocol_MSG_Q.BufLen = tx.len;

  Combine_LoRa_Protocol_Frame(&msg);

***************************/


void Resolve_NB_Protocol_Frame(uint8_t *dat,uint8_t len,Union_NB_Protocol_MSG_Q *msg)
{
  uint8_t buf[128];
  
  if(len==0 || len>=128)
    msg->Str_NB_Protocol_MSG_Q.Validity = false;//数据帧错误
  else
    memcpy(&buf[0],&*dat,len);//接收无线的数据帧 
   
  if( (buf[0] == 0x68) && (Count_CheckSum(&buf[0],(len-1)) == buf[buf[1]+10]))
  {
    if((memcmp(&EquipmentID[0],&buf[4],6)==0) || (memcmp(&BroadcastLoraId[0],&buf[4],6)==0)) //ID是否正确  
    { 
      msg->Str_NB_Protocol_MSG_Q.PollingAck = false;
      msg->Str_NB_Protocol_MSG_Q.Validity = false;//数据帧有效
      msg->Str_NB_Protocol_MSG_Q.ControlCode.Byte = buf[2];    //提取控制码
      msg->Str_NB_Protocol_MSG_Q.FrameNum   =  buf[3];
      switch(msg->Str_NB_Protocol_MSG_Q.ControlCode.Str_NB_Ctrl.FunctionCode) 
      {
      case NBProtocol_GateWayRegister: //集中器入网请求应答
        {
 
        }
        break;
      case NBProtocol_PingData: //数据透传 被动唤醒  数据透传 （应用于被动下行）
        {
          memcpy(&(msg->Str_NB_Protocol_MSG_Q.Buf[0]),&buf[10],buf[1]);//把数据域全部读出来 等待下一步解析
          msg->Str_NB_Protocol_MSG_Q.BufLen = buf[1];
          msg->Str_NB_Protocol_MSG_Q.Validity = true;//数据帧有效
        }
        break;
      case NBProtocol_PollingData:     //数据透传 主动上报  数据透传 （应用于主动上行）
        {
          Str_Calendar   time; 
          
          msg->Str_NB_Protocol_MSG_Q.Validity = true;//数据帧有效
          msg->Str_NB_Protocol_MSG_Q.FrameNum  = buf[3];
          time.Seconds    = buf[11];
          time.Minutes    = buf[12];
          time.Hours      = buf[13];
          time.DayOfMonth = buf[14];
          time.Month      = buf[15];
          time.Year       = 0x2000 + buf[16];
          RTC_Init_Calendar( time );
          msg->Str_NB_Protocol_MSG_Q.PollingAck = true;//polling的应答帧
        }
        break; 
      default:
        break;
      }
    }
    else
    {
      msg->Str_NB_Protocol_MSG_Q.Validity = false;//数据帧错误
    }
  }
  else
  {
    msg->Str_NB_Protocol_MSG_Q.Validity = false;//数据帧错误
  }
}


//组织lora协议 网关查询（上行） 入网请求（上行） 失步查询（上行） 数据抄读（上行应答） polling（上行）
void Combine_NB_Protocol_Frame(Union_NB_Protocol_MSG_Q *msg,Str_Com *tx)
{

 switch(msg->Str_NB_Protocol_MSG_Q.ControlCode.Str_NB_Ctrl.FunctionCode)
  {
  case NBProtocol_GateWayRegister://入网请求 上报
    {
//      tx->len = 0;
//      tx->buf[tx->len++]=0x68;
//      tx->buf[tx->len++]=0x01;//负载长度
//      tx->buf[tx->len++]=0xC0;
//      tx->buf[tx->len++]=msg->Str_NB_Protocol_MSG_Q.FrameNum;
//      tx->buf[tx->len++]=EquipmentID[0];
//      tx->buf[tx->len++]=EquipmentID[1];
//      tx->buf[tx->len++]=EquipmentID[2];
//      tx->buf[tx->len++]=EquipmentID[3];
//      tx->buf[tx->len++]=EquipmentID[4];
//      tx->buf[tx->len++]=EquipmentID[5];
//      tx->buf[tx->len++]=Node_Type_Code;
    }
    break;
  case NBProtocol_PingData://数据抄读 上报应答
    {
      tx->len = 0;
      tx->buf[tx->len++]=0x68;
      tx->buf[tx->len++]=msg->Str_NB_Protocol_MSG_Q.BufLen;//负载长度
      tx->buf[tx->len++]=0x81;
      tx->buf[tx->len++]=msg->Str_NB_Protocol_MSG_Q.FrameNum;
      tx->buf[tx->len++]=EquipmentID[0];
      tx->buf[tx->len++]=EquipmentID[1];
      tx->buf[tx->len++]=EquipmentID[2];
      tx->buf[tx->len++]=EquipmentID[3];
      tx->buf[tx->len++]=EquipmentID[4];
      tx->buf[tx->len++]=EquipmentID[5];
      memcpy(&tx->buf[tx->len],&(msg->Str_NB_Protocol_MSG_Q.Buf[0]),msg->Str_NB_Protocol_MSG_Q.BufLen);
      tx->len = tx->len + msg->Str_NB_Protocol_MSG_Q.BufLen;
    }
    break;
  case NBProtocol_PollingData://polling主动上报数据透传
    {
      tx->len = 0;
      tx->buf[tx->len++]=0x68;
      tx->buf[tx->len++]=msg->Str_NB_Protocol_MSG_Q.BufLen;//负载长度
      tx->buf[tx->len++]=0xC2;
      tx->buf[tx->len++]=msg->Str_NB_Protocol_MSG_Q.FrameNum;
      tx->buf[tx->len++]=EquipmentID[0];
      tx->buf[tx->len++]=EquipmentID[1];
      tx->buf[tx->len++]=EquipmentID[2];
      tx->buf[tx->len++]=EquipmentID[3];
      tx->buf[tx->len++]=EquipmentID[4];
      tx->buf[tx->len++]=EquipmentID[5];
      memcpy(&tx->buf[tx->len],&(msg->Str_NB_Protocol_MSG_Q.Buf[0]),msg->Str_NB_Protocol_MSG_Q.BufLen);
      tx->len = tx->len + msg->Str_NB_Protocol_MSG_Q.BufLen;
    }
    break;
  default:
    break;
  }

  tx->buf[tx->len  ] = Count_CheckSum(&(tx->buf[0]),tx->len);
  tx->len++;
 
}





