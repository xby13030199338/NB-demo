#include "include.h"

uint32_t SysState=0;
Union_Protocol_MSG_Q msg;
uint8_t t5557buf[28]={0};
SCH_TCB T5557TaskTcb;

void T5557_Task(void)
{
  SCHTaskBegin();
  
  while (1)
  {
	Resolve_Protocol_Frame(&UartRx.buf[0],UartRx.len,&msg);
	Uart_Rx_Init();
	if(msg.Str_Protocol_MSG_Q.Validity == true)
	{
	  if(msg.Str_Protocol_MSG_Q.ControlCode == 0x01)//读卡片
	  {
		memset(&t5557buf[0],0,28);
		Read_T5567_Card(&t5557buf[0]);         //读取卡片信息
		memcpy(&msg.Str_Protocol_MSG_Q.Buf[0],&t5557buf[0],28); 
	  }
	  else if(msg.Str_Protocol_MSG_Q.ControlCode == 0x04)//写卡片
	  {
		Write_T5567_Card(&msg.Str_Protocol_MSG_Q.Buf[0]);
		memset(&t5557buf[0],0,28);
		Read_T5567_Card(&t5557buf[0]);         //读取卡片信息
		//比较要写入数据和读出来的数据是否一致
		if( memcmp(&msg.Str_Protocol_MSG_Q.Buf[0],&t5557buf[0],28) == 0)
		{
		  msg.Str_Protocol_MSG_Q.Validity = true;
		}
		else
		{
		  msg.Str_Protocol_MSG_Q.Validity = false;
		}
	  }
	  Combine_Protocol_Frame(&msg);
	  //组织数据回传帧
	}
	SCHCurTaskPend( );     
  }             
  SCHTaskEnd();
}

uint8_t Count_CheckSum(uint8_t *dat,uint8_t len)
{
  uint8_t num,comsum=0;
  
  for(num=0;num<len;num++)
  {
    comsum+=dat[num];
  }
  return(comsum);
}
void Resolve_Protocol_Frame(uint8_t *dat,uint8_t len,Union_Protocol_MSG_Q *msg)
{
  uint8_t buf[64];
  
  if(len==0 || len>=64)
  {
    msg->Str_Protocol_MSG_Q.Validity = false;//数据帧错误
  }
  else
  {
    memcpy(&buf[0],&*dat,len); 
  }
  
  if( (buf[0] == 0x68) && (Count_CheckSum(&buf[0],(len-1)) == buf[buf[1]+3]))
  {
    switch( buf[2] )
    {
    case 0x01://读卡
      {
        msg->Str_Protocol_MSG_Q.Validity = true;
        msg->Str_Protocol_MSG_Q.ControlCode = 0x01;//读卡数据
      }
      break;
    case 0x04://写卡
      {
        msg->Str_Protocol_MSG_Q.ControlCode = 0x04;//读卡数据
        if(buf[1] == 28)
        {
          msg->Str_Protocol_MSG_Q.Validity = true;
          memcpy(&(msg->Str_Protocol_MSG_Q.Buf[0]),&buf[3],buf[1]);
        }
        else
        {
          msg->Str_Protocol_MSG_Q.Validity = false;
        }
      }
      break;
    default:
      break;
    }
  }
}
void Combine_Protocol_Frame(Union_Protocol_MSG_Q *msg)
{
  Str_Com tx;
  
  switch(msg->Str_Protocol_MSG_Q.ControlCode)
  {
  case 0x01://读卡
    {// 
      tx.len = 0;
      tx.buf[tx.len++]=0x68;
      tx.buf[tx.len++]=0x1c;
      tx.buf[tx.len++]=0x81;
      memcpy(&tx.buf[tx.len],&(msg->Str_Protocol_MSG_Q.Buf[0]),28);
      tx.len = 31;

    }
    break;
  case 0x04://写卡
    {//68 01 84 00 ED
      tx.len = 0;
      tx.buf[tx.len++]=0x68;
      tx.buf[tx.len++]=0x01;
      tx.buf[tx.len++]=0x84;
      if(msg->Str_Protocol_MSG_Q.Validity == true)
      {
        tx.buf[tx.len++]=0x00;
      }
      else
      {
        tx.buf[tx.len++]=0x01;
      }
    }
    break;
  default:
    break;
  }
  tx.buf[ tx.len ] = Count_CheckSum(&(tx.buf[0]),tx.len);
  tx.len++;
  Padding_Uart_Tx_Buf(&tx.buf[0],tx.len);
  Enable_Lpuart_Tx();
}

 
 



