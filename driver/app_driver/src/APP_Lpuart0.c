#include "include.h"

static char Lpuart0Tx[128]  ;   //�������������ݶ���
static char Lpuart0Rx[128]  ;   //�������������ݶ���

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
���ڽ��ճ�ʼ��      
*******************************************************************************/
void Lpuart0_Rx_Init(void)
{
  queueFlush(Lpuart0Rx);
}

/*******************************************************************************
���ڷ��ͳ�ʼ��      
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
  Lpuart0_Tx(); //��������
}
/*******************************************************************************
���ڽ��յײ����� ���ڰѴ������ݻ��浽RXbuf��     
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
�������ڷ��� �ж���������buf���ݷ��ͳ�ȥ  �ڴ��ڷ����ж��е���
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








