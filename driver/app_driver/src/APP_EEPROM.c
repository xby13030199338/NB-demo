#include "include.h"

/*
  EEPROM 初始数据均为0xFF  
  实测 读、写128字节 大约需要20----30ms
  Memory array:
– 16384 bit (2 Kbytes) of EEPROM
– Page size: 16 bytes
*/



void Init_M24C16(void)
{
//  uint8_t buf[16]={0};
//  uint32_t addr=0;
  
  __disable_interrupt();
  
//  
//  for(uint32_t page = 0;page < 500;page++)
//  {     
//    addr = page << 8;
//    M24M01_Array_Write(addr,&buf[0],256);
//  }
//  
  
  __enable_interrupt();
}






/*******************************************************************************
** Function name:           M24M01_Array_Write
** Descriptions:            M24M01写入多字节数据  地址已知
** input parameters:        addr  M24M01内部地址
*buff 要写入的数据数组
lengh 长度
** output parameters:       true  成功
false 失败
** Returned value:          void 
** Created by:              程卫玺
** Created Date:            2016-6-12     
*******************************************************************************/
bool M24C16_Array_Write(uint16_t addr,uint8_t *buff,uint8_t lengh)
{
  bool ack;
  uint8_t tem;
  
  __disable_interrupt();
  Eeprom_Vcc_En();
  Eeprom_Wp_Ds();//解除写保护
//  Delay_us(100)   ;//延时，稳定IO状态
  
  EEPROM_Start_IIc()   ;
  
  if(addr > 0xff)
  {
    tem = addr >> 8;
    tem = tem  << 1;
    tem = 0xA0 | tem;
    ack = EEPROM_SendByte(tem);      /*发送器件地址*/
  }
  else
  {
    ack = EEPROM_SendByte(0xA0);      /*发送器件地址*/
  }
  if(ack==false)return(false);
  
  
  
  
  if(addr > 0xff)
  {
    tem = addr & 0xff;
    ack = EEPROM_SendByte(tem);             /*发送器件子地址*/
  }
  else
  {
    ack = EEPROM_SendByte(addr);             /*发送器件子地址*/
  }
  if(ack==false)return(false);

  
  
  
  for(uint16_t i=0;i<lengh;i++)
  {   
    ack = EEPROM_SendByte(*buff);                 /*发送数据*/
    if(ack==false)return(false);
    buff++;
  } 
  EEPROM_Stop_IIc();                           /*结束总线*/ 
  
  Delay_ms(10);	                        //要给EEPROM留出来写入的时间 
  
  Eeprom_Wp_En();  
  Eeprom_Vcc_Ds();  //打开写保护  	
  __enable_interrupt();
  return(true);
}

/*******************************************************************************
** Function name:           M24M01_Array_Read
** Descriptions:            M24M01读出多字节数据  地址已知
** input parameters:        addr  M24M01内部地址
*buff 要写入的数据数组
lengh 长度
** output parameters:       true  成功
false 失败
** Returned value:          void 
** Created by:              程卫玺
** Created Date:            2016-6-12     
*******************************************************************************/
bool M24C16_Array_Read(uint16_t addr,uint8_t *buff,uint8_t lengh)
{
  bool ack;
  uint8_t tem=0;
  
  __disable_interrupt();
  
  Eeprom_Vcc_En();
//  Delay_us(100) ;

  EEPROM_Start_IIc();          /*启动总线*/
  if(addr > 0xff)
  {
    tem = addr >> 8;
    tem = tem  << 1;
    tem = 0xA0 | tem;
    ack = EEPROM_SendByte(tem);      /*发送器件地址*/
  }
  else
  {
    ack = EEPROM_SendByte(0xA0);      /*发送器件地址*/
  }
  if(ack==false)return(false);
  
  if(addr > 0xff)
  {
    tem = addr & 0xff;
    ack = EEPROM_SendByte(tem);             /*发送器件子地址*/
  }
  else
  {
    ack = EEPROM_SendByte(addr);             /*发送器件子地址*/
  }
  if(ack==false)return(false);


  EEPROM_Start_IIc();          /*启动总线*/
  if(addr > 0xff)
  {
    tem = addr >> 8;
    tem = tem  << 1;
    tem = 0xA0 | tem;
    ack = EEPROM_SendByte(tem | 0x01);      /*发送器件地址*/
  }
  else
  {
    ack = EEPROM_SendByte(0xA0 | 0x01);      /*发送器件地址*/
  }
  if(ack==false)return(false);
  

  for(uint16_t i=0;i<lengh-1;i++)
  {   
    *buff = EEPROM_RcvByte();       /*发送数据*/
    EEPROM_Ack_IIc(false);         /*发送就答位*/  
    buff++;
  } 
  *buff = EEPROM_RcvByte();
  
  EEPROM_Ack_IIc(true);        /*发送非应位*/
  
  EEPROM_Stop_IIc();           /*结束总线*/ 
  
  Delay_ms(5);
  
  Eeprom_Vcc_Ds();
  __enable_interrupt();
  return(true);
}



/*******************************************************************************
** Function name:           M24M01_Array_Write
** Descriptions:            M24M01写入多字节数据  地址已知
** input parameters:        addr  M24M01内部地址
*buff 要写入的数据数组
lengh 长度
** output parameters:       true  成功
false 失败
** Returned value:          void 
** Created by:              程卫玺
** Created Date:            2016-6-12     
*******************************************************************************/
bool M24M01_Array_Write(uint32_t addr,uint8_t *buff,uint16_t lengh)
{
  bool ack;
  
  __disable_interrupt();
  Eeprom_Vcc_En();
  Eeprom_Wp_Ds();//解除写保护
  Delay_us(500)   ;//延时，稳定IO状态
  
  EEPROM_Start_IIc()   ;
  
  if(addr>=0xffff)
  {
    ack = EEPROM_SendByte(DeviceAddrHigh);     /*发送器件地址*/
  }
  else
  {
    ack = EEPROM_SendByte(DeviceAddrLow);      /*发送器件地址*/
  }
  if(ack==false)
    return(false);
  
  ack = EEPROM_SendByte(addr/256);             /*发送器件子地址*/
  if(ack==false)return(false);
  ack = EEPROM_SendByte(addr%256);             /*发送器件子地址*/
  if(ack==false)return(false);
  
  for(uint16_t i=0;i<lengh;i++)
  {   
    ack = EEPROM_SendByte(*buff);                 /*发送数据*/
    if(ack==false)return(false);
    buff++;
  } 
  EEPROM_Stop_IIc();                           /*结束总线*/ 
  
  Delay_ms(5);	                        //要给EEPROM留出来写入的时间 
  
  Eeprom_Wp_En();  
  Eeprom_Vcc_Ds();  //打开写保护  	
  __enable_interrupt();
  return(true);
}

/*******************************************************************************
** Function name:           M24M01_Array_Read
** Descriptions:            M24M01读出多字节数据  地址已知
** input parameters:        addr  M24M01内部地址
*buff 要写入的数据数组
lengh 长度
** output parameters:       true  成功
false 失败
** Returned value:          void 
** Created by:              程卫玺
** Created Date:            2016-6-12     
*******************************************************************************/
bool M24M01_Array_Read(uint32_t addr,uint8_t *buff,uint16_t lengh)
{
  bool ack;
  
  __disable_interrupt();
  
  Eeprom_Vcc_En();
  Delay_us(500) ;

  EEPROM_Start_IIc();          /*启动总线*/
  if(addr>=0xffff)
  {
    ack = EEPROM_SendByte(DeviceAddrHigh);     /*发送器件地址*/
  }
  else
  {
    ack = EEPROM_SendByte(DeviceAddrLow);      /*发送器件地址*/
  }
  if(ack==false)return(false);
  
  ack = EEPROM_SendByte(addr/256);             /*发送器件子地址*/
  if(ack==false)return(false);
  ack = EEPROM_SendByte(addr%256);             /*发送器件子地址*/
  if(ack==false)return(false);
  
  EEPROM_Start_IIc();	
  if(addr>=0xffff)
  {
    ack = EEPROM_SendByte(DeviceAddrHigh|0x01);     /*发送器件地址*/
  }
  else
  {
    ack = EEPROM_SendByte(DeviceAddrLow|0x01);      /*发送器件地址*/
  }
  if(ack==false)return(false);
  
  for(uint16_t i=0;i<lengh-1;i++)
  {   
    *buff = EEPROM_RcvByte();       /*发送数据*/
    EEPROM_Ack_IIc(false);         /*发送就答位*/  
    buff++;
  } 
  *buff = EEPROM_RcvByte();
  
  EEPROM_Ack_IIc(true);        /*发送非应位*/
  
  EEPROM_Stop_IIc();           /*结束总线*/ 
  
  Delay_ms(5);
  
  Eeprom_Vcc_Ds();
  __enable_interrupt();
  return(true);
}























