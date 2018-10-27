#include "include.h"



/**************************8
Union_Lora_Protocol_MSG_Q msg;
        
Resolve_LoRa_Protocol_Frame(&LoraRx.buf[0],LoraRx.len,&msg);    //����lora��������֡ 
if(msg.Str_Lora_Protocol_MSG_Q.Validity == true)//������Ч��
{

    Str_Lora tx;
    
    Resolve_645(&msg.Str_Lora_Protocol_MSG_Q.Buf[0],msg.Str_Lora_Protocol_MSG_Q.BufLen,&tx);
            
    if(msg.Str_Lora_Protocol_MSG_Q.ControlCode.Str_LoRa_Ctrl.ack == true)
    {
      memcpy(&msg.Str_Lora_Protocol_MSG_Q.Buf[0],&tx.buf[0],tx.len);//�����ش��´�Ƽ�����֡ ��ȡ��Ч���� ��֯��645 
      msg.Str_Lora_Protocol_MSG_Q.BufLen = tx.len;
      Combine_LoRa_Protocol_Frame(&msg);
 
    }
}

  uint8_t tem[]={0x78,0x56,0x34,0x12,0x01,0x02,0x34,0xF3};
  Str_Lora  tx;

  memcpy(&msg.Str_Lora_Protocol_MSG_Q.Buf[0],&tem[0],8);//�������ߵ�����֡ 
  msg.Str_Lora_Protocol_MSG_Q.BufLen = 8;
  Resolve_645(&msg.Str_Lora_Protocol_MSG_Q.Buf[0],msg.Str_Lora_Protocol_MSG_Q.BufLen,&tx);
  memcpy(&msg.Str_Lora_Protocol_MSG_Q.Buf[0],&tx.buf[0],tx.len);//�����ش��´�Ƽ�����֡ ��ȡ��Ч���� ��֯��645 
  msg.Str_Lora_Protocol_MSG_Q.BufLen = tx.len;

  Combine_LoRa_Protocol_Frame(&msg);

***************************/


void Resolve_NB_Protocol_Frame(uint8_t *dat,uint8_t len,Union_NB_Protocol_MSG_Q *msg)
{
  uint8_t buf[128];
  
  if(len==0 || len>=128)
    msg->Str_NB_Protocol_MSG_Q.Validity = false;//����֡����
  else
    memcpy(&buf[0],&*dat,len);//�������ߵ�����֡ 
   
  if( (buf[0] == 0x68) && (Count_CheckSum(&buf[0],(len-1)) == buf[buf[1]+10]))
  {
    if((memcmp(&EquipmentID[0],&buf[4],6)==0) || (memcmp(&BroadcastLoraId[0],&buf[4],6)==0)) //ID�Ƿ���ȷ  
    { 
      msg->Str_NB_Protocol_MSG_Q.PollingAck = false;
      msg->Str_NB_Protocol_MSG_Q.Validity = false;//����֡��Ч
      msg->Str_NB_Protocol_MSG_Q.ControlCode.Byte = buf[2];    //��ȡ������
      msg->Str_NB_Protocol_MSG_Q.FrameNum   =  buf[3];
      switch(msg->Str_NB_Protocol_MSG_Q.ControlCode.Str_NB_Ctrl.FunctionCode) 
      {
      case NBProtocol_GateWayRegister: //��������������Ӧ��
        {
 
        }
        break;
      case NBProtocol_PingData: //����͸�� ��������  ����͸�� ��Ӧ���ڱ������У�
        {
          memcpy(&(msg->Str_NB_Protocol_MSG_Q.Buf[0]),&buf[10],buf[1]);//��������ȫ�������� �ȴ���һ������
          msg->Str_NB_Protocol_MSG_Q.BufLen = buf[1];
          msg->Str_NB_Protocol_MSG_Q.Validity = true;//����֡��Ч
        }
        break;
      case NBProtocol_PollingData:     //����͸�� �����ϱ�  ����͸�� ��Ӧ�����������У�
        {
          Str_Calendar   time; 
          
          msg->Str_NB_Protocol_MSG_Q.Validity = true;//����֡��Ч
          msg->Str_NB_Protocol_MSG_Q.FrameNum  = buf[3];
          time.Seconds    = buf[11];
          time.Minutes    = buf[12];
          time.Hours      = buf[13];
          time.DayOfMonth = buf[14];
          time.Month      = buf[15];
          time.Year       = 0x2000 + buf[16];
          RTC_Init_Calendar( time );
          msg->Str_NB_Protocol_MSG_Q.PollingAck = true;//polling��Ӧ��֡
        }
        break; 
      default:
        break;
      }
    }
    else
    {
      msg->Str_NB_Protocol_MSG_Q.Validity = false;//����֡����
    }
  }
  else
  {
    msg->Str_NB_Protocol_MSG_Q.Validity = false;//����֡����
  }
}


//��֯loraЭ�� ���ز�ѯ�����У� �����������У� ʧ����ѯ�����У� ���ݳ���������Ӧ�� polling�����У�
void Combine_NB_Protocol_Frame(Union_NB_Protocol_MSG_Q *msg,Str_Com *tx)
{

 switch(msg->Str_NB_Protocol_MSG_Q.ControlCode.Str_NB_Ctrl.FunctionCode)
  {
  case NBProtocol_GateWayRegister://�������� �ϱ�
    {
//      tx->len = 0;
//      tx->buf[tx->len++]=0x68;
//      tx->buf[tx->len++]=0x01;//���س���
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
  case NBProtocol_PingData://���ݳ��� �ϱ�Ӧ��
    {
      tx->len = 0;
      tx->buf[tx->len++]=0x68;
      tx->buf[tx->len++]=msg->Str_NB_Protocol_MSG_Q.BufLen;//���س���
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
  case NBProtocol_PollingData://polling�����ϱ�����͸��
    {
      tx->len = 0;
      tx->buf[tx->len++]=0x68;
      tx->buf[tx->len++]=msg->Str_NB_Protocol_MSG_Q.BufLen;//���س���
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




