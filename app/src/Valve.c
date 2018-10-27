#include "include.h"


SCH_TCB         ValveTaskTcb             ;
static uint8_t  ValveCommandPassing = 0 ; //当阀门工作时存放阀门的指令 任务间传递
static uint8_t  ValveAnglePassing = 0   ;   // 传递阀门开度             任务间传递
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


//阀门队列初始化
void Valve_Q_Init(void)
{
  queueCreate(CtrValve,sizeof(CtrValve),0,0); 
}
//获取阀门队列数量
uint8_t Get_Valve_Buf_Num(void)
{
  return(queueNData(CtrValve));
}


/*
comm 开关阀门命令 Open  Close
enable 阀门自检开关  TRUE 直接填充开关命令  FALSE 如果已经开阀 则不填充开阀命令  如果已经关闭 则不填充关闭命令
BIT7        置位代表开阀命令  清零代表关闭命令
BIT0---BIT6 代表开关角度

Padding_Valve_Comm(Open , true , 60);

*/
void Padding_Valve_Comm(uint8_t comm,uint8_t enable,uint8_t angle)
{
  if( comm == Open )
  {
    if(enable == TRUE)
    {
      queueWrite(CtrValve,(angle|BIT7));//bit7 置位 开启阀门 
    }
    else
    {
      if(ValvePara.Str_ValveState.State != EnumValveOpen)
      {
        queueWrite(CtrValve,(angle|BIT7));//bit7 置位 开启阀门 
      }
    }
  }
  else if( comm == Close )
  {
    if(enable == TRUE)
    {
      queueWrite(CtrValve,angle);//bit7 清零 关闭阀门 
    }
    else
    {
      if(ValvePara.Str_ValveState.State != EnumValveClose)
      {
        queueWrite(CtrValve,angle);//bit7 清零 关闭阀门 
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
    if(R_State(&SysState,Valveing) == false)// 阀门如果没有在工作
    {
      if(queueNData(CtrValve)>0) //阀门命令队列中是否有开关指令
      {
        queueRead(CtrValve,&ValveComm);
        if(ValveComm & BIT7)//如果是开阀门命令
        {
          Open_Valve();
          ValveCommandPassing = Open;//传递全局命令
          ValveAnglePassing   = ValveComm & 0x7f ;
          W_State(&SysState,Valveing,true);
        }
        else
        {
          Close_Valve(); 
          ValveCommandPassing = Close;//传递全局命令
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
** Created by:              程卫玺
** Created Date:            2015-4-25   
阀门堵转电流可以设置。
每次进入中断时间间隔为250ms。应当注意，电池电量不足的时候是否还可以输出大的电流
，如果电流输出能力差，有可能会达不到门限
*******************************************************************************/
void Perform_Valve_Task(void)
{
  static Enum_Valve_Rotation_Status_Name  ValveRotationStatus = Forward_Rotation_State; //阀门状态 正常（TRUE） 和 堵转（FALSE）
  static uint8_t        ValveTaskExeNum=0;      //任务执行次数
  static uint8_t        NumOfMaxCurrent = 0;   //阀门电机最大电流持续次数
  static uint8_t        NumOfAttempt=0;      //任务尝试次数
  uint16_t              adcdat ;

 
  adcdat = Valve_Current_Measure();
 
  ValveTaskExeNum++;     //任务执行次数累加
  
  switch(ValveRotationStatus) //判断阀门转动状态
  {
  case Forward_Rotation_State://正向转动
    {
      if(ValveTaskExeNum < (ValveTaskExeNumThreshold / 2))      //16秒以内
      {
        if( adcdat > ( Current_Threshold / 2 ) )                //相当于160ma
        {/***********当开启前半段出现大电流时 要憋劲几秒 尽量打开**********/
          if(++NumOfMaxCurrent >= NumOfMaxCurrentThreshold)                    //憋劲3秒
          { 
            NumOfMaxCurrent  = 0;
            //反向 跳转状态机到反向
            if(++NumOfAttempt >= NumOfAttemptThreshold)            //已经尝试过3次 不再尝试 故障
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
            else                                                //清除缓存数据  反向阀门转动
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
      else if(ValveTaskExeNum < ValveTaskExeNumThreshold*2)   //应注意 电池电压下降之后 会出现开启时间过长情况 
      {                                                        // 所以要加长执行时间 保证所有的阀门都可以完成
        NumOfMaxCurrent = 0;
        if(adcdat > (Current_Threshold/2))                          //相当于80ma
        {
          if( ValveCommandPassing == Open)                      //开到位
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
            ValveRotationStatus = Angle_Rotation_State ;        //跳转到开度状态
            Close_Valve();
          }
          else if(ValveCommandPassing == Close)                 //关到位
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
      else//超时了
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
  case Reversal_Rotation_State://反向转动  
    {
      if( adcdat >= ( Current_Threshold / 2 ) )          //大于ADC阀值 清除数据
      {
        if(ValveCommandPassing == Open)
          Open_Valve();
        else
          Close_Valve();
        
        NumOfMaxCurrent = 0;
        ValveRotationStatus     = Forward_Rotation_State;
        ValveTaskExeNum  = 0;
      }
      else //电流没有大于堵转电流   有可能在运行 有可能长时间没有响应
      {
        if( ValveTaskExeNum > ValveTaskExeNumThreshold )//阀门长时间没有电压堵转 
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
  case Angle_Rotation_State:  //开度管理
    {
      if( ValveTaskExeNum >= ValveAnglePassing)
      {
        Disable_Valve();
        ValvePara.Str_ValveState.State = EnumValveOpen; //写标志位
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

















