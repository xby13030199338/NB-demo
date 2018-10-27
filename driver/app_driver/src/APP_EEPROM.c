#include "include.h"

/*
  EEPROM ��ʼ���ݾ�Ϊ0xFF  
  ʵ�� ����д128�ֽ� ��Լ��Ҫ20----30ms
  Memory array:
�C 16384 bit (2 Kbytes) of EEPROM
�C Page size: 16 bytes
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
** Descriptions:            M24M01д����ֽ�����  ��ַ��֪
** input parameters:        addr  M24M01�ڲ���ַ
*buff Ҫд�����������
lengh ����
** output parameters:       true  �ɹ�
false ʧ��
** Returned value:          void 
** Created by:              ������
** Created Date:            2016-6-12     
*******************************************************************************/
bool M24C16_Array_Write(uint16_t addr,uint8_t *buff,uint8_t lengh)
{
  bool ack;
  uint8_t tem;
  
  __disable_interrupt();
  Eeprom_Vcc_En();
  Eeprom_Wp_Ds();//���д����
//  Delay_us(100)   ;//��ʱ���ȶ�IO״̬
  
  EEPROM_Start_IIc()   ;
  
  if(addr > 0xff)
  {
    tem = addr >> 8;
    tem = tem  << 1;
    tem = 0xA0 | tem;
    ack = EEPROM_SendByte(tem);      /*����������ַ*/
  }
  else
  {
    ack = EEPROM_SendByte(0xA0);      /*����������ַ*/
  }
  if(ack==false)return(false);
  
  
  
  
  if(addr > 0xff)
  {
    tem = addr & 0xff;
    ack = EEPROM_SendByte(tem);             /*���������ӵ�ַ*/
  }
  else
  {
    ack = EEPROM_SendByte(addr);             /*���������ӵ�ַ*/
  }
  if(ack==false)return(false);

  
  
  
  for(uint16_t i=0;i<lengh;i++)
  {   
    ack = EEPROM_SendByte(*buff);                 /*��������*/
    if(ack==false)return(false);
    buff++;
  } 
  EEPROM_Stop_IIc();                           /*��������*/ 
  
  Delay_ms(10);	                        //Ҫ��EEPROM������д���ʱ�� 
  
  Eeprom_Wp_En();  
  Eeprom_Vcc_Ds();  //��д����  	
  __enable_interrupt();
  return(true);
}

/*******************************************************************************
** Function name:           M24M01_Array_Read
** Descriptions:            M24M01�������ֽ�����  ��ַ��֪
** input parameters:        addr  M24M01�ڲ���ַ
*buff Ҫд�����������
lengh ����
** output parameters:       true  �ɹ�
false ʧ��
** Returned value:          void 
** Created by:              ������
** Created Date:            2016-6-12     
*******************************************************************************/
bool M24C16_Array_Read(uint16_t addr,uint8_t *buff,uint8_t lengh)
{
  bool ack;
  uint8_t tem=0;
  
  __disable_interrupt();
  
  Eeprom_Vcc_En();
//  Delay_us(100) ;

  EEPROM_Start_IIc();          /*��������*/
  if(addr > 0xff)
  {
    tem = addr >> 8;
    tem = tem  << 1;
    tem = 0xA0 | tem;
    ack = EEPROM_SendByte(tem);      /*����������ַ*/
  }
  else
  {
    ack = EEPROM_SendByte(0xA0);      /*����������ַ*/
  }
  if(ack==false)return(false);
  
  if(addr > 0xff)
  {
    tem = addr & 0xff;
    ack = EEPROM_SendByte(tem);             /*���������ӵ�ַ*/
  }
  else
  {
    ack = EEPROM_SendByte(addr);             /*���������ӵ�ַ*/
  }
  if(ack==false)return(false);


  EEPROM_Start_IIc();          /*��������*/
  if(addr > 0xff)
  {
    tem = addr >> 8;
    tem = tem  << 1;
    tem = 0xA0 | tem;
    ack = EEPROM_SendByte(tem | 0x01);      /*����������ַ*/
  }
  else
  {
    ack = EEPROM_SendByte(0xA0 | 0x01);      /*����������ַ*/
  }
  if(ack==false)return(false);
  

  for(uint16_t i=0;i<lengh-1;i++)
  {   
    *buff = EEPROM_RcvByte();       /*��������*/
    EEPROM_Ack_IIc(false);         /*���;ʹ�λ*/  
    buff++;
  } 
  *buff = EEPROM_RcvByte();
  
  EEPROM_Ack_IIc(true);        /*���ͷ�Ӧλ*/
  
  EEPROM_Stop_IIc();           /*��������*/ 
  
  Delay_ms(5);
  
  Eeprom_Vcc_Ds();
  __enable_interrupt();
  return(true);
}



/*******************************************************************************
** Function name:           M24M01_Array_Write
** Descriptions:            M24M01д����ֽ�����  ��ַ��֪
** input parameters:        addr  M24M01�ڲ���ַ
*buff Ҫд�����������
lengh ����
** output parameters:       true  �ɹ�
false ʧ��
** Returned value:          void 
** Created by:              ������
** Created Date:            2016-6-12     
*******************************************************************************/
bool M24M01_Array_Write(uint32_t addr,uint8_t *buff,uint16_t lengh)
{
  bool ack;
  
  __disable_interrupt();
  Eeprom_Vcc_En();
  Eeprom_Wp_Ds();//���д����
  Delay_us(500)   ;//��ʱ���ȶ�IO״̬
  
  EEPROM_Start_IIc()   ;
  
  if(addr>=0xffff)
  {
    ack = EEPROM_SendByte(DeviceAddrHigh);     /*����������ַ*/
  }
  else
  {
    ack = EEPROM_SendByte(DeviceAddrLow);      /*����������ַ*/
  }
  if(ack==false)
    return(false);
  
  ack = EEPROM_SendByte(addr/256);             /*���������ӵ�ַ*/
  if(ack==false)return(false);
  ack = EEPROM_SendByte(addr%256);             /*���������ӵ�ַ*/
  if(ack==false)return(false);
  
  for(uint16_t i=0;i<lengh;i++)
  {   
    ack = EEPROM_SendByte(*buff);                 /*��������*/
    if(ack==false)return(false);
    buff++;
  } 
  EEPROM_Stop_IIc();                           /*��������*/ 
  
  Delay_ms(5);	                        //Ҫ��EEPROM������д���ʱ�� 
  
  Eeprom_Wp_En();  
  Eeprom_Vcc_Ds();  //��д����  	
  __enable_interrupt();
  return(true);
}

/*******************************************************************************
** Function name:           M24M01_Array_Read
** Descriptions:            M24M01�������ֽ�����  ��ַ��֪
** input parameters:        addr  M24M01�ڲ���ַ
*buff Ҫд�����������
lengh ����
** output parameters:       true  �ɹ�
false ʧ��
** Returned value:          void 
** Created by:              ������
** Created Date:            2016-6-12     
*******************************************************************************/
bool M24M01_Array_Read(uint32_t addr,uint8_t *buff,uint16_t lengh)
{
  bool ack;
  
  __disable_interrupt();
  
  Eeprom_Vcc_En();
  Delay_us(500) ;

  EEPROM_Start_IIc();          /*��������*/
  if(addr>=0xffff)
  {
    ack = EEPROM_SendByte(DeviceAddrHigh);     /*����������ַ*/
  }
  else
  {
    ack = EEPROM_SendByte(DeviceAddrLow);      /*����������ַ*/
  }
  if(ack==false)return(false);
  
  ack = EEPROM_SendByte(addr/256);             /*���������ӵ�ַ*/
  if(ack==false)return(false);
  ack = EEPROM_SendByte(addr%256);             /*���������ӵ�ַ*/
  if(ack==false)return(false);
  
  EEPROM_Start_IIc();	
  if(addr>=0xffff)
  {
    ack = EEPROM_SendByte(DeviceAddrHigh|0x01);     /*����������ַ*/
  }
  else
  {
    ack = EEPROM_SendByte(DeviceAddrLow|0x01);      /*����������ַ*/
  }
  if(ack==false)return(false);
  
  for(uint16_t i=0;i<lengh-1;i++)
  {   
    *buff = EEPROM_RcvByte();       /*��������*/
    EEPROM_Ack_IIc(false);         /*���;ʹ�λ*/  
    buff++;
  } 
  *buff = EEPROM_RcvByte();
  
  EEPROM_Ack_IIc(true);        /*���ͷ�Ӧλ*/
  
  EEPROM_Stop_IIc();           /*��������*/ 
  
  Delay_ms(5);
  
  Eeprom_Vcc_Ds();
  __enable_interrupt();
  return(true);
}






















