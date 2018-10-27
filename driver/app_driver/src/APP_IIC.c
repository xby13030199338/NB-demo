/********************************************************************
                       VIIC_C51.C
  此程序是I2C操作平台（主方式的软件平台）的底层的C子程序,如发送数据
及接收数据,应答位发送,并提供了几个直接面对器件的操作函数，它很方便的
与用户程序连接并扩展.....  
  
    注意:函数是采用软件延时的方法产生SCL脉冲,固对高晶振频率要作 
一定的修改....(本例是1us机器周期,即晶振频率要小于12MHZ)

********************************************************************/
       
#include "APP_IIC.h"



void Eeprom_Wp_En(void)
{
  Gpio_SetIO(WP_PORT, WP_PIN);
}

void Eeprom_Wp_Ds(void)
{
  Gpio_ClrIO(WP_PORT, WP_PIN);
}

void Eeprom_Vcc_En(void)
{
  Gpio_SetIO(VCC_PORT, VCC_PIN);
}

void Eeprom_Vcc_Ds(void)
{
  Gpio_ClrIO(VCC_PORT, VCC_PIN);
}

uint8_t EEPROM_ReadSDA(void)
{
  uint8_t i = 0;
  i = Gpio_GetInputIO(SDA_PORT, SDA_PIN);
  return i;
}

void EEPROM_SCL_HIGH(void)
{
  Gpio_SetIO(SCL_PORT, SCL_PIN);
}

void EEPROM_SCL_LOW(void)
{
  Gpio_ClrIO(SCL_PORT, SCL_PIN);
}

void EEPROM_SDA_HIGH(void)
{
  Gpio_SetIO(SDA_PORT, SDA_PIN);
}

void EEPROM_SDA_LOW(void)
{
  Gpio_ClrIO(SDA_PORT, SDA_PIN);
}

/*******************************************************************
                     起动总线函数               
函数原型: void  Start_I2c();  
功能:       启动I2C总线,即发送I2C起始条件.
  
********************************************************************/
void EEPROM_Start_IIc(void)
{
  EEPROM_SDA_HIGH();        /*发送起始条件的数据信号*/
  Delay_us(5);
  EEPROM_SCL_HIGH();
  Delay_us(5);  /*起始条件建立时间大于4.7us,延时*/   
  EEPROM_SDA_LOW();        /*发送起始信号*/
  Delay_us(5);  /* 起始条件锁定时间大于4μs*/      
  EEPROM_SCL_LOW();        /*钳住I2C总线，准备发送或接收数据 */
  Delay_us(2);
}

/*******************************************************************
                      结束总线函数               
函数原型: void  Stop_I2c();  
功能:       结束I2C总线,即发送I2C结束条件.
  
********************************************************************/
void EEPROM_Stop_IIc(void)
{
  EEPROM_SDA_LOW();  /*发送结束条件的数据信号*/
  Delay_us(5);   /*发送结束条件的时钟信号*/
  EEPROM_SCL_HIGH();  /*结束条件建立时间大于4μs*/
  Delay_us(5);
  EEPROM_SDA_HIGH();  /*发送I2C总线结束信号*/
  Delay_us(5);
}

/*******************************************************************
                 字节数据传送函数               
函数原型: void  SendByte(uchar c);
功能:  将数据c发送出去,可以是地址,也可以是数据,发完后等待应答,并对
     此状态位进行操作.(不应答或非应答都使ack=0 假)     
     发送数据正常，ack=1; ack=0表示被控器无应答或损坏。
********************************************************************/
bool EEPROM_SendByte(uint8_t WriteData)
{
  uint8_t BitCnt;
  bool ack;
  
  for(BitCnt=0;BitCnt<8;BitCnt++)/*要传送的数据长度为8位*/
  {
    if(WriteData & 0x80)
    {
      EEPROM_SDA_HIGH(); /*判断发送位*/
    }
    else  
    {
      EEPROM_SDA_LOW();  
    }
    
    Delay_us(5);
    EEPROM_SCL_HIGH();                     /*置时钟线为高，通知被控器开始接收数据位*/
    Delay_us(5);              /*保证时钟高电平周期大于4μs*/                  
    EEPROM_SCL_LOW(); 
    WriteData=WriteData<<1;
  }
  EEPROM_SDA_HIGH();                      /*8位发送完后释放数据线，准备接收应答位*/
  Delay_us(5);  
  EEPROM_SCL_HIGH();
  Delay_us(5);
  if(EEPROM_ReadSDA()==1)
    ack=0;     
  else 
    ack=1;              /*判断是否接收到应答信号*/
  EEPROM_SCL_LOW();
  Delay_us(2);
  return(ack);
}

/*******************************************************************
                 字节数据传送函数               
函数原型: uchar  RcvByte();
功能:  用来接收从器件传来的数据,并判断总线错误(不发应答信号)，
     发完后请用应答函数。  
********************************************************************/	
uint8_t  EEPROM_RcvByte(void)
{
  uint8_t retc=0;
  uint8_t BitCnt;
  
  EEPROM_SDA_HIGH();             /*置数据线为输入方式*/
  Delay_us(5);   
  for(BitCnt=0;BitCnt<8;BitCnt++)
  {      
    EEPROM_SCL_HIGH();       /*置时钟线为高使数据线上数据有效*/
    Delay_us(5);
    retc=retc<<1;
    if(EEPROM_ReadSDA() == 1)
    {
      retc |= 0x01;//retc++; /*读数据位,接收的数据位放入retc中 */
    }
    EEPROM_SCL_LOW();    
    Delay_us(5);
  }
  return(retc);
}

/********************************************************************
                     应答子函数
原型:  void Ack_I2c(bit a);
 
功能:主控器进行应答信号,(可以是应答或非应答信号)
********************************************************************/
void EEPROM_Ack_IIc(const bool ack)
{
  if( ack == 0 )
    EEPROM_SDA_LOW();  /*在此发出应答或非应答信号 */
  else 
    EEPROM_SDA_HIGH();
  Delay_us(5);  
  EEPROM_SCL_HIGH();
  Delay_us(5);  /*时钟低电平周期大于4μs*/         
  EEPROM_SCL_LOW();    /*清时钟线，钳住I2C总线以便继续接收*/
  Delay_us(5);   
}

/*******************************************************************
                    向无子地址器件发送字节数据函数               
函数原型: bit  ISendByte(uchar sla,ucahr c);  
功能:     从启动总线到发送地址，数据，结束总线的全过程,从器件地址sla.
           如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
bool EEPROM_IIC_Send_Byte(const uint8_t sla,const uint8_t WriteData)
{
  bool ack;
  
  EEPROM_Start_IIc();               /*启动总线*/
  ack = EEPROM_SendByte(sla);            /*发送器件地址*/
  if(ack==0)return(0);
  ack = EEPROM_SendByte(WriteData);               /*发送数据*/
  if(ack==0)return(0);
  EEPROM_Stop_IIc();                 /*结束总线*/ 
  return(1);
}

/*******************************************************************
                    向无子地址器件读字节数据函数               
函数原型: bit  IRcvByte(uchar sla,ucahr *c);  
功能:     从启动总线到发送地址，读数据，结束总线的全过程,从器件地
          址sla，返回值在c.
           如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
bool EEPROM_IIC_Rcv_Byte(uint8_t sla,uint8_t *RcvData)
{
  bool ack;
  
  EEPROM_Start_IIc();                /*启动总线*/
  ack = EEPROM_SendByte(sla+1);           /*发送器件地址*/
  if(ack==0)return(0);
  *RcvData = EEPROM_RcvByte();               /*读取数据*/
  EEPROM_Ack_IIc(1);               /*发送非就答位*/
  EEPROM_Stop_IIc();                  /*结束总线*/ 
  return(1);
}

/*******************************************************************
                    向有子地址器件发送多字节数据函数               
函数原型: bit  ISendStr(uchar sla,uchar suba,ucahr *s,uchar no);  
功能:     从启动总线到发送地址，子地址,数据，结束总线的全过程,从器件
          地址sla，子地址suba，发送内容是s指向的内容，发送no个字节。
           如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
//void EEPROM_IIC_Send_Str(uint8_t sla,uint8_t suba,uint8_t *buf,uint8_t len)
//{
//  uint8_t i;
// 
//  
////  EEPROM_SCL_Port_Dir_In();
////  EEPROM_SDA_Port_Dir_In();
//  
//  EEPROM_Start_IIc();          /*启动总线*/
//  EEPROM_SendByte(sla);  /*发送器件地址*/
//  EEPROM_SendByte(suba); /*发送器件子地址*/
//
//  for(i=0;i<len;i++)
//  {   
//    EEPROM_SendByte(buf[i]); /*发送数据*/
//  } 
//  EEPROM_Stop_IIc();         /*结束总线*/ 
//}

/*******************************************************************
                    向有子地址器件读取多字节数据函数               
函数原型: bit  ISendStr(uchar sla,uchar suba,ucahr *s,uchar no);  
功能:     从启动总线到发送地址，子地址,读数据，结束总线的全过程,从器件
          地址sla，子地址suba，读出的内容放入s指向的存储区，读no个字节。
           如果返回1表示操作成功，否则操作有误。
注意：    使用前必须已结束总线。
********************************************************************/
//void EEPROM_IIC_Rcv_Str(uint8_t sla,uint8_t suba,uint8_t *buf,uint8_t len)
//{
//  uint8_t i;
// 
//  
//  EEPROM_Start_IIc();          /*启动总线*/
//  EEPROM_SendByte(sla);  /*发送器件地址*/
//  EEPROM_SendByte(suba); /*发送器件子地址*/
//  
//  EEPROM_Start_IIc();
//  EEPROM_SendByte(sla+1);
//  
//  for(i=0;i<len-1;i++)
//  {   
//    buf[i] = EEPROM_RcvByte();       /*发送数据*/
//    EEPROM_Ack_IIc(0);         /*发送就答位*/  
//  } 
//  buf[i] = EEPROM_RcvByte();
//  EEPROM_Ack_IIc(true);        /*发送非应位*/
//  EEPROM_Stop_IIc();           /*结束总线*/ 
//}


                        /*    完毕      */









