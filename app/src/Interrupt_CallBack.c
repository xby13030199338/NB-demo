#include "include.h"

void Gpio_IRQHandler(uint8_t u8Param)
{
  switch(u8Param)
  {
  case 0://PA
    {
      if(TRUE == Gpio_GetIrqStatus(GpioPortA, GpioPin3))
      {
        Gpio_ClearIrq(GpioPortA, GpioPin3);
        
        if(R_State(&SysState,Buttoning)==false )//�������������δ���� ������
        {
          W_State(&SysState,Buttoning,true);
          SCHTaskResume(ButtonTaskTcb);
        }
      }
    }
    break;
  case 1://PB
    {
    
    }
    break;
  case 2://PC
    {
    
    }
    break;
  case 3://PD
    {
    
    }
    break;
  default://����
    break;
  }

}    

void WdtCallback(void)
{
  NVIC_SystemReset();	
}

void RtcCycCb(void)
{
  SchedTicksInc();
//  Feed_Watch_Dog();
}

void RtcAlarmCb(void)
{
   
}

void AdcContIrqCallback(void)
{	
//  Adc_GetSglResult(&AdcTem);
//  Adc_SGL_Stop();
}

 
 

/*******************************************************************************
** Function name:           Tim0Int
** Descriptions:            Tim0�ص����� 
** input parameters:        void 
** output parameters:       void
** Returned value:          void   
		Tim0�����жϴ���һ������������֡�������
*******************************************************************************/
void Tim0Int(void)
{   
  if(TRUE == Bt_GetIntFlag(TIM0, BtUevIrq))
  {  
    Bt_ClearIntFlag(TIM0,BtUevIrq);		
    Bt_M0_Stop(TIM0);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralBTim, FALSE);
    //��������
    //��������ж�
  }
}

/*******************************************************************************
** Function name:           ****
** Descriptions:            ���ڻص����� 
** input parameters:        void 
** output parameters:       void
** Returned value:          void    
*******************************************************************************/
void TxIntCallback(void)
{
  Lpuart0_Tx();
}
void RxIntCallback(void)
{
  Lpuart0_Rx(LPUart_ReceiveData(LPUART0));
  Timer0_Init();
}
void ErrIntCallback(void)
{
}
void PErrIntCallBack(void)
{
}
void CtsIntCallBack(void)
{
}



/******************�͹��Ķ�ʱ��********************/
//0.125s
void LpTimIntCallback(void)
{ 
  if(TRUE == Lpt_GetIntFlag())
  {		
    Lpt_ClearIntFlag();
    
    Soft_Timer_Tick();//������ʱ�� �δ�
    Wdt_Feed(); //ι��
    WatchDogCount = 0 ;//����ι����־
  }
}




/*******************************************************************************
** Function name:           Tim1Int
** Descriptions:            ��ʱ��Tim1Pwc�ж� 
** input parameters:        void 
** output parameters:       void
** Returned value:          void    
*******************************************************************************/
uint32_t cap1,cap2=0;
void Tim1Int(void)
{
  //����ж� 
  if(TRUE == Bt_GetIntFlag(TIM1, BtUevIrq))
  {
	Bt_ClearIntFlag(TIM1,BtUevIrq);
  }
  //CA0�����ж� 
  if(TRUE == Bt_GetIntFlag(TIM1, BtCA0Irq))
  {								
	cap1  = Bt_M1_PWC_CapValueGet(TIM1);							//������Ȳ������ 
	Bt_ClearIntFlag(TIM1, BtCA0Irq);
	Pwc1_Stop();
  }
}
/*******************************************************************************
** Function name:           Tim2Int
** Descriptions:            ��ʱ��Tim2Pwc�ж� 
** input parameters:        void 
** output parameters:       void
** Returned value:          void    
*******************************************************************************/
void Tim2Int(void)
{
  if(TRUE == Bt_GetIntFlag(TIM2, BtUevIrq))
  {
	Bt_ClearIntFlag(TIM2,BtUevIrq);
  }
  if(TRUE == Bt_GetIntFlag(TIM2, BtCA0Irq))
  {
	cap2  = Bt_M1_PWC_CapValueGet(TIM2);
	Bt_ClearIntFlag(TIM2, BtCA0Irq);
	Pwc2_Stop();	
  }
}

/*******************************************************************************
** Function name:           Timer3
** Descriptions:            ��ʱ��Timer3��ʱģʽ 5ms
** input parameters:        void 
** output parameters:       void
** Returned value:          void    
*******************************************************************************/
void Tim3Int(void)
{
  if(TRUE == Tim3_GetIntFlag(Tim3UevIrq))
  {
	Tim3_ClearIntFlag(Tim3UevIrq);
  }
}




