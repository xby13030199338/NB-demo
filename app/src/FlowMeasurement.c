/*******************************************************************************
**--------------File Info-------------------------------------------------------
** File name:           Flux.c
** Last modified Date:  2015-04-24
** Last Version:        V1.00
** Descriptions:        流量测量程序库
**          
以5000L/H计算，对应 1.3888L/S 100L需要72s才能够出现。 所以1S检测一次足够了。
每秒调用一次流量检测程序，
每秒开启一次使能，1ms后检测对应管脚
11 01 11 10 11 01 11 10 11
流量流程
*******************************************************************************/

#include "include.h"



SCH_TCB FluxMeasureTcb;


void Flux_Measure_Task(void)
{
  SCHTaskBegin();
  
  while (1)
  {
    Flux_Measure();
    SCHCurTaskDly( 1000 / SCH_HW_TIM_MS );  //delay 250ms 
  }
  
  SCHTaskEnd();
}
 


/*******************************************************************************
** Function name:           Flux_Task
** Descriptions:            流量检测任务
** input parameters:        void
** output parameters:       void
** Returned value:          void 
** Created by:              程卫玺
** Created Date:            2015-4-25   
100k 1nf 需要500us稳定时间，1ms足够检测
*******************************************************************************/
void Flux_Measure(void)
{
  
  static uint8_t Loop_Num = 0;//存放上一次IO口的状态值。
  static uint8_t MagnetNum= 0;
  uint8_t LLIO_Dat=0;
  
  LLIO_Dat = Gpio_GetInputIO(LLIO_One_Port, LLIO_One_Pin) << 1;
  LLIO_Dat = LLIO_Dat + Gpio_GetInputIO(LLIO_Two_Port, LLIO_Two_Pin);

  /*检查是否有磁攻击产生*/
  if(LLIO_Dat == B0000_0000) //两个干簧管都吸合则说明出现问题。或者是磁攻击
  {
    if(++MagnetNum>=3)
    {
      MagnetNum=0;
      Padding_Valve_Comm(Close , true , 99);
    }
  }
  else                       //没有发生磁攻击
  {  
    switch(FluxStateMachine)
    {
    case B0000_0000:
      if(LLIO_Dat == B0000_0001)
      {
        if(++Loop_Num>=2)
        {
          Loop_Num=0;
          FluxStateMachine++;
        }
      }
      else
        Loop_Num=0;
      break;
    case B0000_0001:
      if(LLIO_Dat == B0000_0011)
      {
        if(++Loop_Num>=2)
        {
          Loop_Num=0;
          FluxStateMachine++;
        }
      }
      else
        Loop_Num=0;
      break;
    case B0000_0010:
      if(LLIO_Dat == B0000_0010)
      {
        if(++Loop_Num>=2)
        {
          Loop_Num=0;
          FluxStateMachine++;
        }
      }
      else
        Loop_Num=0;
      break;
    case B0000_0011:
      if(LLIO_Dat == B0000_0011)
      {
        if(++Loop_Num>=2)
        {
          Loop_Num=0;
          FluxStateMachine = B0000_0000;
          Accounting_Management();//流量加100L  处理流量数据信息，计算
        }
      }
      else
        Loop_Num=0;
      break;
    default:
      break;
    }
  }
}


/*******************************************************************************
** Function name:           Accounting_Management
** Descriptions:            流量累加 计费管理
** input parameters:        void
** output parameters:       void
** Returned value:          void 
** Created by:              程卫玺
** Created Date:            2015-5-28  
注意：下发的流量都是立方米的，而计量的分辨率是0.1m3，还有费用的问题。欠费门限应该是负数
*******************************************************************************/

void Accounting_Management(void)
{
  WMData.Str_WMData.Sum++;       
  WMData.Str_WMData.YearSum++;   
  WMData.Str_WMData.DaySum++;     
  /***********************水计量和扣费部分**************************/
  WMData.Str_WMData.Cash =WMData.Str_WMData.Cash - Multistep_Fee(WMData.Str_WMData.YearSum);

  if(WMData.Str_WMData.Cash <= Cash_Alarm_Threshold)
  {
    Padding_Valve_Comm(Open , true , 30);
  }
  else if(WMData.Str_WMData.Cash <= 0)
  {
    Padding_Valve_Comm(Close, true , 99);
  }
  W_WMData();//保存水表流量数据
  //保存水表流量状态机
}

/*******************************************************************************
** Function name:           Multistep_Fee
** Descriptions:            阶梯税费计算
** input parameters:        void
** output parameters:       void
** Returned value:          void 
** Created by:              程卫玺
** Created Date:            2015-5-28  
根据协议，如果阶梯1大于0，则0到阶梯1部分的水费为0.
只支持递增阶梯。
阶梯1到2 之间用费率1 
阶梯2到3 之间用费率2
阶梯3以上 用费率3 
*******************************************************************************/
uint32_t  Multistep_Fee( uint32_t dat)
{
  uint32_t tem=0;

  if(dat <= MultiRate.Str_MultiRate.One)
    return(0);
  else
  {
    if(MultiRate.Str_MultiRate.Two == 0 || tem <= MultiRate.Str_MultiRate.Two)
       tem = MultiRate.Str_MultiRate.RateOne;
    else
    {
      if(MultiRate.Str_MultiRate.Three == 0 || tem <= MultiRate.Str_MultiRate.Three)
         tem = MultiRate.Str_MultiRate.RateTwo;
      else
      {
        if(tem>MultiRate.Str_MultiRate.Three)
         tem = MultiRate.Str_MultiRate.RateThree;
      }
    }
  }
  return(tem);
}
















