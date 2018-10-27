/********************************************************************
                       VIIC_C51.C
  �˳�����I2C����ƽ̨������ʽ������ƽ̨���ĵײ��C�ӳ���,�緢������
����������,Ӧ��λ����,���ṩ�˼���ֱ����������Ĳ������������ܷ����
���û��������Ӳ���չ.....  
  
    ע��:�����ǲ���������ʱ�ķ�������SCL����,�̶Ը߾���Ƶ��Ҫ�� 
һ�����޸�....(������1us��������,������Ƶ��ҪС��12MHZ)

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
                     �����ߺ���               
����ԭ��: void  Start_I2c();  
����:       ����I2C����,������I2C��ʼ����.
  
********************************************************************/
void EEPROM_Start_IIc(void)
{
  EEPROM_SDA_HIGH();        /*������ʼ�����������ź�*/
  Delay_us(5);
  EEPROM_SCL_HIGH();
  Delay_us(5);  /*��ʼ��������ʱ�����4.7us,��ʱ*/   
  EEPROM_SDA_LOW();        /*������ʼ�ź�*/
  Delay_us(5);  /* ��ʼ��������ʱ�����4��s*/      
  EEPROM_SCL_LOW();        /*ǯסI2C���ߣ�׼�����ͻ�������� */
  Delay_us(2);
}

/*******************************************************************
                      �������ߺ���               
����ԭ��: void  Stop_I2c();  
����:       ����I2C����,������I2C��������.
  
********************************************************************/
void EEPROM_Stop_IIc(void)
{
  EEPROM_SDA_LOW();  /*���ͽ��������������ź�*/
  Delay_us(5);   /*���ͽ���������ʱ���ź�*/
  EEPROM_SCL_HIGH();  /*������������ʱ�����4��s*/
  Delay_us(5);
  EEPROM_SDA_HIGH();  /*����I2C���߽����ź�*/
  Delay_us(5);
}

/*******************************************************************
                 �ֽ����ݴ��ͺ���               
����ԭ��: void  SendByte(uchar c);
����:  ������c���ͳ�ȥ,�����ǵ�ַ,Ҳ����������,�����ȴ�Ӧ��,����
     ��״̬λ���в���.(��Ӧ����Ӧ��ʹack=0 ��)     
     ��������������ack=1; ack=0��ʾ��������Ӧ����𻵡�
********************************************************************/
bool EEPROM_SendByte(uint8_t WriteData)
{
  uint8_t BitCnt;
  bool ack;
  
  for(BitCnt=0;BitCnt<8;BitCnt++)/*Ҫ���͵����ݳ���Ϊ8λ*/
  {
    if(WriteData & 0x80)
    {
      EEPROM_SDA_HIGH(); /*�жϷ���λ*/
    }
    else  
    {
      EEPROM_SDA_LOW();  
    }
    
    Delay_us(5);
    EEPROM_SCL_HIGH();                     /*��ʱ����Ϊ�ߣ�֪ͨ��������ʼ��������λ*/
    Delay_us(5);              /*��֤ʱ�Ӹߵ�ƽ���ڴ���4��s*/                  
    EEPROM_SCL_LOW(); 
    WriteData=WriteData<<1;
  }
  EEPROM_SDA_HIGH();                      /*8λ��������ͷ������ߣ�׼������Ӧ��λ*/
  Delay_us(5);  
  EEPROM_SCL_HIGH();
  Delay_us(5);
  if(EEPROM_ReadSDA()==1)
    ack=0;     
  else 
    ack=1;              /*�ж��Ƿ���յ�Ӧ���ź�*/
  EEPROM_SCL_LOW();
  Delay_us(2);
  return(ack);
}

/*******************************************************************
                 �ֽ����ݴ��ͺ���               
����ԭ��: uchar  RcvByte();
����:  �������մ���������������,���ж����ߴ���(����Ӧ���ź�)��
     ���������Ӧ������  
********************************************************************/	
uint8_t  EEPROM_RcvByte(void)
{
  uint8_t retc=0;
  uint8_t BitCnt;
  
  EEPROM_SDA_HIGH();             /*��������Ϊ���뷽ʽ*/
  Delay_us(5);   
  for(BitCnt=0;BitCnt<8;BitCnt++)
  {      
    EEPROM_SCL_HIGH();       /*��ʱ����Ϊ��ʹ��������������Ч*/
    Delay_us(5);
    retc=retc<<1;
    if(EEPROM_ReadSDA() == 1)
    {
      retc |= 0x01;//retc++; /*������λ,���յ�����λ����retc�� */
    }
    EEPROM_SCL_LOW();    
    Delay_us(5);
  }
  return(retc);
}

/********************************************************************
                     Ӧ���Ӻ���
ԭ��:  void Ack_I2c(bit a);
 
����:����������Ӧ���ź�,(������Ӧ����Ӧ���ź�)
********************************************************************/
void EEPROM_Ack_IIc(const bool ack)
{
  if( ack == 0 )
    EEPROM_SDA_LOW();  /*�ڴ˷���Ӧ����Ӧ���ź� */
  else 
    EEPROM_SDA_HIGH();
  Delay_us(5);  
  EEPROM_SCL_HIGH();
  Delay_us(5);  /*ʱ�ӵ͵�ƽ���ڴ���4��s*/         
  EEPROM_SCL_LOW();    /*��ʱ���ߣ�ǯסI2C�����Ա��������*/
  Delay_us(5);   
}

/*******************************************************************
                    �����ӵ�ַ���������ֽ����ݺ���               
����ԭ��: bit  ISendByte(uchar sla,ucahr c);  
����:     ���������ߵ����͵�ַ�����ݣ��������ߵ�ȫ����,��������ַsla.
           �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
bool EEPROM_IIC_Send_Byte(const uint8_t sla,const uint8_t WriteData)
{
  bool ack;
  
  EEPROM_Start_IIc();               /*��������*/
  ack = EEPROM_SendByte(sla);            /*����������ַ*/
  if(ack==0)return(0);
  ack = EEPROM_SendByte(WriteData);               /*��������*/
  if(ack==0)return(0);
  EEPROM_Stop_IIc();                 /*��������*/ 
  return(1);
}

/*******************************************************************
                    �����ӵ�ַ�������ֽ����ݺ���               
����ԭ��: bit  IRcvByte(uchar sla,ucahr *c);  
����:     ���������ߵ����͵�ַ�������ݣ��������ߵ�ȫ����,��������
          ַsla������ֵ��c.
           �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
bool EEPROM_IIC_Rcv_Byte(uint8_t sla,uint8_t *RcvData)
{
  bool ack;
  
  EEPROM_Start_IIc();                /*��������*/
  ack = EEPROM_SendByte(sla+1);           /*����������ַ*/
  if(ack==0)return(0);
  *RcvData = EEPROM_RcvByte();               /*��ȡ����*/
  EEPROM_Ack_IIc(1);               /*���ͷǾʹ�λ*/
  EEPROM_Stop_IIc();                  /*��������*/ 
  return(1);
}

/*******************************************************************
                    �����ӵ�ַ�������Ͷ��ֽ����ݺ���               
����ԭ��: bit  ISendStr(uchar sla,uchar suba,ucahr *s,uchar no);  
����:     ���������ߵ����͵�ַ���ӵ�ַ,���ݣ��������ߵ�ȫ����,������
          ��ַsla���ӵ�ַsuba������������sָ������ݣ�����no���ֽڡ�
           �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
//void EEPROM_IIC_Send_Str(uint8_t sla,uint8_t suba,uint8_t *buf,uint8_t len)
//{
//  uint8_t i;
// 
//  
////  EEPROM_SCL_Port_Dir_In();
////  EEPROM_SDA_Port_Dir_In();
//  
//  EEPROM_Start_IIc();          /*��������*/
//  EEPROM_SendByte(sla);  /*����������ַ*/
//  EEPROM_SendByte(suba); /*���������ӵ�ַ*/
//
//  for(i=0;i<len;i++)
//  {   
//    EEPROM_SendByte(buf[i]); /*��������*/
//  } 
//  EEPROM_Stop_IIc();         /*��������*/ 
//}

/*******************************************************************
                    �����ӵ�ַ������ȡ���ֽ����ݺ���               
����ԭ��: bit  ISendStr(uchar sla,uchar suba,ucahr *s,uchar no);  
����:     ���������ߵ����͵�ַ���ӵ�ַ,�����ݣ��������ߵ�ȫ����,������
          ��ַsla���ӵ�ַsuba�����������ݷ���sָ��Ĵ洢������no���ֽڡ�
           �������1��ʾ�����ɹ��������������
ע�⣺    ʹ��ǰ�����ѽ������ߡ�
********************************************************************/
//void EEPROM_IIC_Rcv_Str(uint8_t sla,uint8_t suba,uint8_t *buf,uint8_t len)
//{
//  uint8_t i;
// 
//  
//  EEPROM_Start_IIc();          /*��������*/
//  EEPROM_SendByte(sla);  /*����������ַ*/
//  EEPROM_SendByte(suba); /*���������ӵ�ַ*/
//  
//  EEPROM_Start_IIc();
//  EEPROM_SendByte(sla+1);
//  
//  for(i=0;i<len-1;i++)
//  {   
//    buf[i] = EEPROM_RcvByte();       /*��������*/
//    EEPROM_Ack_IIc(0);         /*���;ʹ�λ*/  
//  } 
//  buf[i] = EEPROM_RcvByte();
//  EEPROM_Ack_IIc(true);        /*���ͷ�Ӧλ*/
//  EEPROM_Stop_IIc();           /*��������*/ 
//}


                        /*    ���      */








