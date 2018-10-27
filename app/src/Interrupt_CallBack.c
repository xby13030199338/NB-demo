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
        
        if(R_State(&SysState,Buttoning)==false )//如果按键检查程序未运行 则启动
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
  default://错误
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
** Descriptions:            Tim0回调函数 
** input parameters:        void 
** output parameters:       void
** Returned value:          void   
		Tim0进入中断代表一个完整的数据帧接收完成
*******************************************************************************/
void Tim0Int(void)
{   
  if(TRUE == Bt_GetIntFlag(TIM0, BtUevIrq))
  {  
    Bt_ClearIntFlag(TIM0,BtUevIrq);		
    Bt_M0_Stop(TIM0);
    Sysctrl_SetPeripheralGate(SysctrlPeripheralBTim, FALSE);
    //开启休眠
    //接收完成中断
  }
}

/*******************************************************************************
** Function name:           ****
** Descriptions:            串口回调函数 
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



/******************低功耗定时器********************/
//0.125s
void LpTimIntCallback(void)
{ 
  if(TRUE == Lpt_GetIntFlag())
  {		
    Lpt_ClearIntFlag();
    
    Soft_Timer_Tick();//软件定时器 滴答
    Wdt_Feed(); //喂狗
    WatchDogCount = 0 ;//软件喂狗标志
  }
}




/*******************************************************************************
** Function name:           Tim1Int
** Descriptions:            定时器Tim1Pwc中断 
** input parameters:        void 
** output parameters:       void
** Returned value:          void    
*******************************************************************************/
uint32_t cap1,cap2=0;
void Tim1Int(void)
{
  //溢出中断 
  if(TRUE == Bt_GetIntFlag(TIM1, BtUevIrq))
  {
	Bt_ClearIntFlag(TIM1,BtUevIrq);
  }
  //CA0捕获中断 
  if(TRUE == Bt_GetIntFlag(TIM1, BtCA0Irq))
  {								
	cap1  = Bt_M1_PWC_CapValueGet(TIM1);							//脉冲宽度测量结果 
	Bt_ClearIntFlag(TIM1, BtCA0Irq);
	Pwc1_Stop();
  }
}
/*******************************************************************************
** Function name:           Tim2Int
** Descriptions:            定时器Tim2Pwc中断 
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
** Descriptions:            定时器Timer3定时模式 5ms
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





