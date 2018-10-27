/*******************************************************************************
**--------------File Info-------------------------------------------------------
** File name:           Flux.c
** Last modified Date:  2015-04-24
** Last Version:        V1.00
** Descriptions:        �������������
**          
��5000L/H���㣬��Ӧ 1.3888L/S 100L��Ҫ72s���ܹ����֡� ����1S���һ���㹻�ˡ�
ÿ�����һ������������
ÿ�뿪��һ��ʹ�ܣ�1ms�����Ӧ�ܽ�
11 01 11 10 11 01 11 10 11
��������
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
** Descriptions:            �����������
** input parameters:        void
** output parameters:       void
** Returned value:          void 
** Created by:              ������
** Created Date:            2015-4-25   
100k 1nf ��Ҫ500us�ȶ�ʱ�䣬1ms�㹻���
*******************************************************************************/
void Flux_Measure(void)
{
  
  static uint8_t Loop_Num = 0;//�����һ��IO�ڵ�״ֵ̬��
  static uint8_t MagnetNum= 0;
  uint8_t LLIO_Dat=0;
  
  LLIO_Dat = Gpio_GetInputIO(LLIO_One_Port, LLIO_One_Pin) << 1;
  LLIO_Dat = LLIO_Dat + Gpio_GetInputIO(LLIO_Two_Port, LLIO_Two_Pin);

  /*����Ƿ��дŹ�������*/
  if(LLIO_Dat == B0000_0000) //�����ɻɹܶ�������˵���������⡣�����ǴŹ���
  {
    if(++MagnetNum>=3)
    {
      MagnetNum=0;
      Padding_Valve_Comm(Close , true , 99);
    }
  }
  else                       //û�з����Ź���
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
          Accounting_Management();//������100L  ��������������Ϣ������
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
** Descriptions:            �����ۼ� �Ʒѹ���
** input parameters:        void
** output parameters:       void
** Returned value:          void 
** Created by:              ������
** Created Date:            2015-5-28  
ע�⣺�·����������������׵ģ��������ķֱ�����0.1m3�����з��õ����⡣Ƿ������Ӧ���Ǹ���
*******************************************************************************/

void Accounting_Management(void)
{
  WMData.Str_WMData.Sum++;       
  WMData.Str_WMData.YearSum++;   
  WMData.Str_WMData.DaySum++;     
  /***********************ˮ�����Ϳ۷Ѳ���**************************/
  WMData.Str_WMData.Cash =WMData.Str_WMData.Cash - Multistep_Fee(WMData.Str_WMData.YearSum);

  if(WMData.Str_WMData.Cash <= Cash_Alarm_Threshold)
  {
    Padding_Valve_Comm(Open , true , 30);
  }
  else if(WMData.Str_WMData.Cash <= 0)
  {
    Padding_Valve_Comm(Close, true , 99);
  }
  W_WMData();//����ˮ����������
  //����ˮ������״̬��
}

/*******************************************************************************
** Function name:           Multistep_Fee
** Descriptions:            ����˰�Ѽ���
** input parameters:        void
** output parameters:       void
** Returned value:          void 
** Created by:              ������
** Created Date:            2015-5-28  
����Э�飬�������1����0����0������1���ֵ�ˮ��Ϊ0.
ֻ֧�ֵ������ݡ�
����1��2 ֮���÷���1 
����2��3 ֮���÷���2
����3���� �÷���3 
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















