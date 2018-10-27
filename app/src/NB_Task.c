#include "Driverlib.h"








SCH_TCB  NBTaskTcb;





void NB_Iot_Task(void)
{
  static uint16_t NBTaskDelay = 0;
  
  SCHTaskBegin();
  
  while (1)
  {
    NBTaskDelay = NB_Iot_State_Machine();
    if( NBTaskDelay == Task_Suspend )
    {
      SCHCurTaskPend();               //����CAD����
    }
    else
    {
      SCHCurTaskDly( NBTaskDelay / SCH_SYS_TICKS_MS );      //2����һ��
    }
  }
  
  SCHTaskEnd();
}


uint16_t NB_Iot_State_Machine(void)
{
  static uint8_t NB_Polling_Loop = 0;
  Str_Com tx ;
  
  uint16_t delay = 0;
  Union_NB_Protocol_MSG_Q msg;
  
  switch(NBParams.Str_NBParams.StateMachine)
  {
  case Enum_NB_Iot_Disable:     //NBiot ��һ���ϵ�֮���������
    { 
      if(NB_Iot_PowerDown() == true)
      {
        //NB����
        
      }
      else
      {
        //NB����
        
      }
      NBParams.Str_NBParams.StateMachine = Enum_NB_Iot_Disable;
      delay = Task_Suspend;
    }
    break;
  case Enum_NB_Iot_Idle:        //NB����״̬  
    {
      
      NBParams.Str_NBParams.StateMachine = Enum_NB_Iot_Idle;
      delay = Task_Suspend;
    }
    break;
  case Enum_NB_Iot_Reset:       //NBiot  ��λ
    {
      if(NB_Iot_Reset() == true)// ��λ֮���ܷ������� �����������總��
      {//NB����
        NBParams.Str_NBParams.StateMachine = Enum_NB_Iot_Activate_Network;
      }
      else
      {//NB����
        NBParams.Str_NBParams.StateMachine = Enum_NB_Iot_Disable;
      }
      delay = 1000;
    }
    break;
  case Enum_NB_Iot_Activate_Network://NBiot  ���總��
    {
      if( NB_Iot_Activate_Network() == true)
      {
        //NBParams.Str_NBParams.StateMachine = Enum_NB_Iot_Idle;
        NBParams.Str_NBParams.StateMachine = Enum_NB_Iot_Polling;
      }
      else
      {
        NBParams.Str_NBParams.StateMachine = Enum_NB_Iot_Disable;
      }
      delay = 250; 
    }
    break;
  case Enum_NB_Iot_Cell_Reselection://NBiot С����ѡ
    {  
      
      delay = Task_Suspend;
      NBParams.Str_NBParams.StateMachine = Enum_NB_Iot_Idle;
      delay = Task_Suspend;
    }   
    break;
  case Enum_NB_Iot_Polling:          //NBiot �ϱ�����
    {
      uint8_t  tem[]={ 0x68,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0x68,0x01,0x02,0x34,0xF3,0xF6,0x16 };
      Str_Com tx;
      
      
      Resolve_645(&tem[0],sizeof(tem),&tx);
      memcpy(&msg.Str_NB_Protocol_MSG_Q.Buf[0],&tx.buf[0],tx.len);//�����ش��´�Ƽ�����֡ ��ȡ��Ч���� ��֯��645 
      msg.Str_NB_Protocol_MSG_Q.BufLen = tx.len;
      msg.Str_NB_Protocol_MSG_Q.ControlCode.Str_NB_Ctrl.FunctionCode = NetProtocol_PollingData;
      Combine_NB_Protocol_Frame(&msg,&tx);
      
      if( NB_Iot_Tx(&tx.buf[0],tx.len) == true)
      {
        NB_Polling_Loop = 0;
        NBParams.Str_NBParams.StateMachine = Enum_NB_Iot_Rx;
        delay = 20000;
      }
      else
      { 
        if(NB_Polling_Loop++ >= 3)
        {
          NB_Polling_Loop = 0;
          NBParams.Str_NBParams.StateMachine = Enum_NB_Iot_Reset;
          delay = 1000;
        }
        else
        {
          NBParams.Str_NBParams.StateMachine = Enum_NB_Iot_Polling;
          delay = rand() % 128 ;                                                   //����ȴ�ʱ��
          delay = delay  * 256;//1��Ϊ���
          delay = delay  * 64 ;//���256��λ��
        }
      }
    }
    break; 
  case Enum_NB_Iot_Rx:          //NBiot ��������
    { 
      uint8_t buf[128]={0},len;
      
      tx.len = 0;
      if(R_State(&NB_Iot_State,RcvOK)==true)
      {
        tx.len = 0;
        W_State(&NB_Iot_State,RcvOK,false);//�����־λ
        
        msg.Str_NB_Protocol_MSG_Q.ControlCode.Str_NB_Ctrl.FunctionCode = NetProtocol_Err;
        //�յ��ͽ��н���
        len  = String_To_Hex(&buf[0]);
        
        
        Resolve_NB_Protocol_Frame(&buf[0],len,&msg);    //����lora��������֡ 
        
        switch(msg.Str_NB_Protocol_MSG_Q.ControlCode.Str_NB_Ctrl.FunctionCode)
        {
        case NetProtocol_GateWayRegister:
          {
            NBParams.Str_NBParams.NBRegister = msg.Str_NB_Protocol_MSG_Q.Register;
            //�洢NBParams
            NBParams.Str_NBParams.StateMachine = Enum_NB_Iot_Rx;
            delay = 20000;
          }
          break;
        case NetProtocol_PingData:
          {
            Resolve_645(&msg.Str_NB_Protocol_MSG_Q.Buf[0],msg.Str_NB_Protocol_MSG_Q.BufLen,&tx);
            if(msg.Str_NB_Protocol_MSG_Q.ControlCode.Str_NB_Ctrl.ack == true)
            {    
              memcpy(&msg.Str_NB_Protocol_MSG_Q.Buf[0],&tx.buf[0],tx.len);//�����ش��´�Ƽ�����֡ ��ȡ��Ч���� ��֯��645 
              msg.Str_NB_Protocol_MSG_Q.BufLen = tx.len;
              Combine_NB_Protocol_Frame(&msg,&tx);
              if( NB_Iot_Tx(&tx.buf[0],tx.len) == true)
              {
                NBParams.Str_NBParams.StateMachine = Enum_NB_Iot_Rx;
                delay = 20000;
              }
              else
              {
                NBParams.Str_NBParams.StateMachine = Enum_NB_Iot_Rx;
                delay = 20000;
              }
            }
            else
            { 
              NBParams.Str_NBParams.StateMachine = Enum_NB_Iot_Rx;
              delay = 20000;
            }
          }
          break;
        case NetProtocol_PollingData:
          {
            NBParams.Str_NBParams.NBPolling = msg.Str_NB_Protocol_MSG_Q.PollingAck;
            //�洢NBParams
            NBParams.Str_NBParams.StateMachine = Enum_NB_Iot_Rx;
            delay = 20000;
          }
          break;
        default:
          {
            NBParams.Str_NBParams.StateMachine = Enum_NB_Iot_Rx;
            delay = 20000;
          }
          break;
        }
      }
      else//���ճ�ʱ���  �������̬
      {
        NBParams.Str_NBParams.StateMachine = Enum_NB_Iot_Idle;
        delay = 250; 
      }
    }
    break;
  default:
    {
      NBParams.Str_NBParams.StateMachine = Enum_NB_Iot_Rx;
      delay = 20000;
    }
    break;
  }
  return(delay);
}














































