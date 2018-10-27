#include "include.h"

static char Lpuart0Tx[128]  ;   //蜂鸣器命令数据队列
static char Lpuart0Rx[128]  ;   //蜂鸣器命令数据队列

void Lpuart0_Q_Init(void)
{
  queueCreate(Lpuart0Tx,sizeof(Lpuart0Tx),0,0); 
  queueCreate(Lpuart0Rx,sizeof(Lpuart0Rx),0,0); 
}

uint8_t Lpuart0_Rx_Cpy(char *dest)
{
  uint8_t dat=0,num=0,len = 0;
  
  len = queueNData(Lpuart0Rx);
  for(num = 0;num<len;num++)
  {
    dat = 0;
    queueRead(Lpuart0Rx,&dat);
    *(dest + num) = dat;
  }
  Lpuart0_Rx_Init();
  return(len);
}

/*******************************************************************************
串口接收初始化      
*******************************************************************************/
void Lpuart0_Rx_Init(void)
{
  queueFlush(Lpuart0Rx);
}

/*******************************************************************************
串口发送初始化      
*******************************************************************************/
void Lpuart0_Tx_Init(void)
{
  queueFlush(Lpuart0Tx); 
}

void Padding_Lpuart0_Tx_Buf(uint8_t *buf,uint8_t len)
{
  for(uint8_t num=0;num<len;num++)
  {
    queueWrite(Lpuart0Tx,*(buf+num));
  }
}

void Enable_Lpuart0_Tx(void)
{
  Lpuart0_Tx(); //启动发送
}
/*******************************************************************************
串口接收底层驱动 用于把串口数据缓存到RXbuf中     
*******************************************************************************/
void Lpuart0_Rx(uint8_t val)
{
  queueWrite(Lpuart0Rx,val);
}

uint8_t Get_Lpuart0_RxBuf_Num(void)
{
  return(queueNData(Lpuart0Rx));
}

uint8_t Get_Lpuart0_Rx_Data(void)
{
  uint8_t dat=0;
  
  queueRead(Lpuart0Rx,&dat);
  return(dat);
}

/*******************************************************************************
启动串口发送 中断中连续把buf数据发送出去  在串口发送中断中调用
*******************************************************************************/
void Lpuart0_Tx(void)
{
  uint8_t dat=0;
  
  if(queueNData(Lpuart0Tx) > 0)
  {
    queueRead(Lpuart0Tx,&dat);
    M0P_LPUART0->SBUF = dat;
  }
  else
  {
    Lpuart0_Tx_Init();
    LPUart_ClrStatus(LPUART0,LPUartTC);
  }
}









