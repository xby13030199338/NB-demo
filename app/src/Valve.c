#include "include.h"


SCH_TCB         ValveTaskTcb             ;
static uint8_t  ValveCommandPassing = 0 ; //�����Ź���ʱ��ŷ��ŵ�ָ�� ����䴫��
static uint8_t  ValveAnglePassing = 0   ;   // ���ݷ��ſ���             ����䴫��
static uint8_t  CtrValve[32]={0}        ; 


void Close_Valve(void)
{
  Motor_Close_H();
  Motor_Open_L();
}

void Open_Valve(void)
{
  Motor_Close_L();
  Motor_Open_H();
}

void Disable_Valve(void)
{
  Motor_Close_L();
  Motor_Open_L();
}


//���Ŷ��г�ʼ��
void Valve_Q_Init(void)
{
  queueCreate(CtrValve,sizeof(CtrValve),0,0); 
}
//��ȡ���Ŷ�������
uint8_t Get_Valve_Buf_Num(void)
{
  return(queueNData(CtrValve));
}


/*
comm ���ط������� Open  Close
enable �����Լ쿪��  TRUE ֱ����俪������  FALSE ����Ѿ����� ����俪������  ����Ѿ��ر� �����ر�����
BIT7        ��λ������������  ��������ر�����
BIT0---BIT6 �������ؽǶ�

Padding_Valve_Comm(Open , true , 60);

*/
void Padding_Valve_Comm(uint8_t comm,uint8_t enable,uint8_t angle)
{
  if( comm == Open )
  {
    if(enable == TRUE)
    {
      queueWrite(CtrValve,(angle|BIT7));//bit7 ��λ �������� 
    }
    else
    {
      if(ValvePara.Str_ValveState.State != EnumValveOpen)
      {
        queueWrite(CtrValve,(angle|BIT7));//bit7 ��λ �������� 
      }
    }
  }
  else if( comm == Close )
  {
    if(enable == TRUE)
    {
      queueWrite(CtrValve,angle);//bit7 ���� �رշ��� 
    }
    else
    {
      if(ValvePara.Str_ValveState.State != EnumValveClose)
      {
        queueWrite(CtrValve,angle);//bit7 ���� �رշ��� 
      }
    }
  }
}






void Valve_Task(void)
{
  static uint8_t ValveComm = 0;
  
  SCHTaskBegin();
  
  while(1)
  {
    if(R_State(&SysState,Valveing) == false)// �������û���ڹ���
    {
      if(queueNData(CtrValve)>0) //��������������Ƿ��п���ָ��
      {
        queueRead(CtrValve,&ValveComm);
        if(ValveComm & BIT7)//����ǿ���������
        {
          Open_Valve();
          ValveCommandPassing = Open;//����ȫ������
          ValveAnglePassing   = ValveComm & 0x7f ;
          W_State(&SysState,Valveing,true);
        }
        else
        {
          Close_Valve(); 
          ValveCommandPassing = Close;//����ȫ������
          W_State(&SysState,Valveing,true);
        }
      }
      SCHCurTaskDly( 3000 / SCH_HW_TIM_MS);
    }
    else
    {
      Perform_Valve_Task();
      SCHCurTaskDly( 500 / SCH_HW_TIM_MS); 
    }
  }
  
  SCHTaskEnd();
}

/*******************************************************************************
** Function name:            
** Descriptions:             
** input parameters:        void
** output parameters:       void
** Returned value:          void 
** Created by:              ������
** Created Date:            2015-4-25   
���Ŷ�ת�����������á�
ÿ�ν����ж�ʱ����Ϊ250ms��Ӧ��ע�⣬��ص��������ʱ���Ƿ񻹿��������ĵ���
������������������п��ܻ�ﲻ������
*******************************************************************************/
void Perform_Valve_Task(void)
{
  static Enum_Valve_Rotation_Status_Name  ValveRotationStatus = Forward_Rotation_State; //����״̬ ������TRUE�� �� ��ת��FALSE��
  static uint8_t        ValveTaskExeNum=0;      //����ִ�д���
  static uint8_t        NumOfMaxCurrent = 0;   //���ŵ����������������
  static uint8_t        NumOfAttempt=0;      //�����Դ���
  uint16_t              adcdat ;

 
  adcdat = Valve_Current_Measure();
 
  ValveTaskExeNum++;     //����ִ�д����ۼ�
  
  switch(ValveRotationStatus) //�жϷ���ת��״̬
  {
  case Forward_Rotation_State://����ת��
    {
      if(ValveTaskExeNum < (ValveTaskExeNumThreshold / 2))      //16������
      {
        if( adcdat > ( Current_Threshold / 2 ) )                //�൱��160ma
        {/***********������ǰ��γ��ִ����ʱ Ҫ�ﾢ���� ������**********/
          if(++NumOfMaxCurrent >= NumOfMaxCurrentThreshold)                    //�ﾢ3��
          { 
            NumOfMaxCurrent  = 0;
            //���� ��ת״̬��������
            if(++NumOfAttempt >= NumOfAttemptThreshold)            //�Ѿ����Թ�3�� ���ٳ��� ����
            {
              Disable_Valve();
              ValveAnglePassing         = 0;
              ValveCommandPassing       = 0;
              ValveRotationStatus       = Forward_Rotation_State;
              ValveTaskExeNum    = 0; 
              NumOfMaxCurrent   = 0;
              NumOfAttempt = 0;
              W_State(&SysState,Valveing,FALSE);
              ValvePara.Str_ValveState.State = EnumValveErr;
              W_Valve_State();
            }
            else                                                //�����������  ������ת��
            {
              if(ValveCommandPassing == Open)
              {
                Close_Valve();
                ValveTaskExeNum=0;
                ValveRotationStatus = Reversal_Rotation_State;
              }
              else if(ValveCommandPassing == Close)
              {
                Open_Valve();
                ValveTaskExeNum=0;
                ValveRotationStatus = Reversal_Rotation_State;
              }
              else
              {
                Disable_Valve();
                ValveAnglePassing         = 0;
                ValveCommandPassing       = 0;
                ValveRotationStatus       = Forward_Rotation_State;
                ValveTaskExeNum    = 0; 
                NumOfMaxCurrent   = 0;
                NumOfAttempt = 0;
                W_State(&SysState,Valveing,FALSE);
                
                ValvePara.Str_ValveState.State = EnumValveErr;
                W_Valve_State();
              }
            }
          }
        }
        else
        {
          NumOfMaxCurrent = 0;
        }
      }
      else if(ValveTaskExeNum < ValveTaskExeNumThreshold*2)   //Ӧע�� ��ص�ѹ�½�֮�� ����ֿ���ʱ�������� 
      {                                                        // ����Ҫ�ӳ�ִ��ʱ�� ��֤���еķ��Ŷ��������
        NumOfMaxCurrent = 0;
        if(adcdat > (Current_Threshold/2))                          //�൱��80ma
        {
          if( ValveCommandPassing == Open)                      //����λ
          {    
            uint16_t tem = 0;
            
            Disable_Valve();
            ValvePara.Str_ValveState.Angle = ValveAnglePassing;
            tem = ValveTaskExeNumThreshold * (100 - ValveAnglePassing);
            ValveAnglePassing = tem / 100;
            if(ValveAnglePassing >= ValveTaskExeNumThreshold)
            {
              ValveAnglePassing = ValveTaskExeNumThreshold;
            }
            ValveCommandPassing       = 0;
            NumOfAttempt = 0;
            ValveTaskExeNum = 0;
            NumOfMaxCurrent   = 0;
            ValveRotationStatus = Angle_Rotation_State ;        //��ת������״̬
            Close_Valve();
          }
          else if(ValveCommandPassing == Close)                 //�ص�λ
          {
            Disable_Valve();
            ValveAnglePassing         = 0;
            ValveCommandPassing       = 0;
            ValveRotationStatus       = Forward_Rotation_State;
            ValveTaskExeNum    = 0; 
            NumOfMaxCurrent   = 0;
            NumOfAttempt = 0;
            W_State(&SysState,Valveing,FALSE);
            
            ValvePara.Str_ValveState.State = EnumValveClose;
            W_Valve_State();
          }
          else
          {
            Disable_Valve();
            ValveAnglePassing         = 0;
            ValveCommandPassing       = 0;
            ValveRotationStatus       = Forward_Rotation_State;
            ValveTaskExeNum    = 0; 
            NumOfMaxCurrent   = 0;
            NumOfAttempt = 0;
            W_State(&SysState,Valveing,FALSE);
            
            ValvePara.Str_ValveState.State = EnumValveErr;
            W_Valve_State();
          }
        }
      }
      else//��ʱ��
      {
        Disable_Valve();
        ValveCommandPassing = 0;
        ValveRotationStatus = Forward_Rotation_State;
        ValveTaskExeNum =0; 
        NumOfAttempt = 0;
        
        ValvePara.Str_ValveState.State = EnumValveErr;
        W_Valve_State();
        W_State(&SysState,Valveing,FALSE);
      }
    }
    break;
  case Reversal_Rotation_State://����ת��  
    {
      if( adcdat >= ( Current_Threshold / 2 ) )          //����ADC��ֵ �������
      {
        if(ValveCommandPassing == Open)
          Open_Valve();
        else
          Close_Valve();
        
        NumOfMaxCurrent = 0;
        ValveRotationStatus     = Forward_Rotation_State;
        ValveTaskExeNum  = 0;
      }
      else //����û�д��ڶ�ת����   �п��������� �п��ܳ�ʱ��û����Ӧ
      {
        if( ValveTaskExeNum > ValveTaskExeNumThreshold )//���ų�ʱ��û�е�ѹ��ת 
        {
          Disable_Valve();
          ValveCommandPassing = 0;
          ValveRotationStatus = Forward_Rotation_State;
          ValveTaskExeNum =0; 
          NumOfAttempt = 0;
          NumOfMaxCurrent = 0;
          
          ValvePara.Str_ValveState.State = EnumValveErr;
          W_Valve_State();

          W_State(&SysState,Valveing,FALSE);
        }
      } 
      
    }
    break;
  case Angle_Rotation_State:  //���ȹ���
    {
      if( ValveTaskExeNum >= ValveAnglePassing)
      {
        Disable_Valve();
        ValvePara.Str_ValveState.State = EnumValveOpen; //д��־λ
        W_Valve_State();

        ValveAnglePassing = 0;
        ValveCommandPassing = 0;
        ValveRotationStatus = Forward_Rotation_State;
        ValveTaskExeNum =0; 
        NumOfAttempt = 0;
        W_State(&SysState,Valveing,FALSE);
      }
    }
    break;
  default:
    break;
  }

}
















