#include "include.h"


/*************���� ��·��ģ��֮��Э��*****************/
void Resolve_MBoard_Frame(uint8_t *dat,uint8_t len,Union_MB_Protocol_MSG_Q *msg)
{
  
  uint8_t buf[128];
  
  if(len==0 || len>=128)
    msg->Str_MB_Protocol_MSG_Q.Validity = false;//����֡����
  else
    memcpy(&buf[0],&*dat,len);//�������ߵ�����֡ 
  
  if( (buf[0] == 0x68) && (Count_CheckSum(&buf[0],(len-1)) == buf[buf[1]+10]))
  { 
    msg->Str_MB_Protocol_MSG_Q.ControlCode.Byte = buf[2];    //��ȡ������
    msg->Str_MB_Protocol_MSG_Q.Validity = false;//����֡��Ч

    switch(msg->Str_MB_Protocol_MSG_Q.ControlCode.Str_MB_Ctrl.FunctionCode) 
    {
    case MBProtocol_TransparentTransmission:    //ģ���͸������
      {
        memcpy(&(msg->Str_MB_Protocol_MSG_Q.Buf[0]),&buf[4],buf[1]);//��������ȫ�������� �ȴ���һ������
        msg->Str_MB_Protocol_MSG_Q.BufLen = buf[1];
        msg->Str_MB_Protocol_MSG_Q.Validity = true;//����֡��Ч
      }
      break;
    case MBProtocol_QueryParameter:             //��ѯNBģ�����
      {
        
        msg->Str_MB_Protocol_MSG_Q.Validity = true;//����֡��Ч
      }
      break;
    default:
      {}
      break;
    }
  }
  else
  {
    msg->Str_MB_Protocol_MSG_Q.Validity = false;//����֡����
  }
  
}



//��֯ģ���ͨ��Э��
void Combine_MB_Protocol_Frame(Union_MB_Protocol_MSG_Q *msg,Str_Com *tx)
{

 switch(msg->Str_MB_Protocol_MSG_Q.ControlCode.Str_MB_Ctrl.FunctionCode)
  {
  case MBProtocol_TransparentTransmission://�������� �ϱ�
    {

    }
    break;
  case MBProtocol_QueryParameter://���ݳ��� �ϱ�Ӧ��
    {

    }
    break;
  default:
    break;
  }

  tx->buf[tx->len  ] = Count_CheckSum(&(tx->buf[0]),tx->len);
  tx->len++;
 
}






