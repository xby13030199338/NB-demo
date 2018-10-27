#include "include.h"

//����NBģ����������



uint32_t NB_Iot_State = 0;

//�������char *a = "abcdeaf��
//------------------------------------------------------------------
const char AT_AT[] = "AT";//At����ָ��
char AT_CFUN_0[] = "AT+CFUN=0";//�ر���Ƶ
char AT_NCSEARFCN[] = "AT+NCSEARFCN";//���Ƶ��
char AT_CELL_RESELECTION_1[] = "AT+NCONFIG=CELL_RESELECTION,TRUE";//С����ѡ��ʼ
char AT_CFUN_1[] = "AT+CFUN=1";//������Ƶ
char AT_CGATT_1[] = "AT+CGATT=1";//ִ������
char AT_CGATT_x[] = "AT+CGATT?";//ȷ������
char AT_CELL_RESELECTION_0[] = "AT+NCONFIG=CELL_RESELECTION,TRUE";//�ر�С��Ƶ��
char AT_IP[] = "AT+NCDP=180.101.147.115,5683";//����IP��ַ

char AT_CIMI[] = "AT+CIMI";//��ѯsim���� Request international mobile subscriber identity (+CIMI)
 
char AT_CGSN_1[] = "AT+CGSN=1";//��ѯIMEI��

char AT_NRB[] = "AT+NRB";//��������
 
const char AT_NMGS[] = "AT+NMGS=";
const char AT_SETIMEI[] = "AT+NTSETID=1,";


/*
 
*/

/*
*********************************************************************************************************
*	�� �� ��: SIM800_SendAT
*	����˵��: ��GSMģ�鷢��AT��� �������Զ���AT�ַ���������<CR>�ַ�
*	��    ��: _Str : AT�����ַ�����������ĩβ�Ļس�<CR>. ���ַ�0����
*	�� �� ֵ: ��
\n = ����
\r = �س�
*********************************************************************************************************
*/
void NB_iot_SendAT(char *Cmd)
{
  Uart_Rx_Init();	/* ���㴮�ڽ��ջ����� */
  Uart_Tx_Init();       /* ���㴮�ڷ��ͻ����� */
  Padding_Uart_Tx_Buf((uint8_t *)Cmd, strlen(Cmd)); 
  Padding_Uart_Tx_Buf("\r", 1); 
  Padding_Uart_Tx_Buf("\n", 1); 
  Enable_Lpuart_Tx();
}

/*
*********************************************************************************************************
*	�� �� ��: SIM800_WaitResponse
*	����˵��: �ȴ�SIM800����ָ����Ӧ���ַ���. ����ȴ� OK
*	��    ��: _pAckStr : Ӧ����ַ����� ���Ȳ��ó���255
*			 _usTimeOut : ����ִ�г�ʱ��0��ʾһֱ�ȴ�. >����ʾ��ʱʱ�䣬��λ10ms
*	�� �� ֵ: 1 ��ʾ�ɹ�  0 ��ʾʧ��
*********************************************************************************************************
*/
uint8_t NB_iot_WaitResponse(char *reply,uint16_t TimeOut)
{
  char ucRxBuf[128]={0};
  uint8_t flag = 0;
  
  Sys_Lptim_Config();//�����͹��Ķ�ʱ�� 
  bsp_StartTimer(NB_Iot_BT_ID, TimeOut); /*  */
  
  while (1)
  {			
    if(R_State(&NB_Iot_State,RcvOK) == true)
    {
      Lpuart_Rx_Cpy(&ucRxBuf[0]);
      
      if(strstr(&ucRxBuf[0],reply) != NULL)
      {
        flag = 1;
        break;
      }
    }
    
    /* ��鶨ʱ��0ʱ���Ƿ� */
    if (bsp_CheckTimer(NB_Iot_BT_ID))
    {
      flag = 0;
      break;/* 3�붨ʱ�����˳���״̬ */
    }
  }
  
  /* �������ʱ��Ӧ�ùرն�ʱ������Ϊ���ǻ�ռ�ú�̨����Դ */
  bsp_StopTimer(NB_Iot_BT_ID);	 //���ζ�ʱ�������ʱ����һ�κ󣬿��Բ��عر�
  Sys_Lptim_Disable();  
  return(flag);  
}

/*
*********************************************************************************************************
*	�� �� ��: SIM800_PowerOn
*	����˵��: ģ���ϵ�. �����ڲ����ж��Ƿ��Ѿ�����������ѿ�����ֱ�ӷ���1
*	��    ��: ��
*	�� �� ֵ: 1 ��ʾ�ϵ�ɹ�  0: ��ʾ�쳣
  if( NB_Iot_Reset() == true)
    {
      __no_operation( );//��λ�ɹ�
    }
    else
    {
      __no_operation( );
    }
*********************************************************************************************************
*/
uint8_t NB_Iot_Reset(void)
{
  uint8_t flag = 0;
  
  Sys_Lptim_Config();//�����͹��Ķ�ʱ�� 
  bsp_StartTimer(NB_Iot_BT_ID, 500/10); /* ʹ��������ʱ��3����Ϊ��ʱ���� */
  
  NB_Iot_Reset_En() ;
  while(1)
  {
    if (bsp_CheckTimer(NB_Iot_BT_ID))
    {
       NB_Iot_Reset_Ds();
      break;
    }
  }
  
  bsp_StopTimer(NB_Iot_BT_ID);
  Sys_Lptim_Disable();
  
  if( NB_iot_WaitResponse("OK",8000/10) == true)
  {
    flag = 1;
  }
  else
  {
    flag = 0;
  }
  return(flag);
}

uint8_t NB_Iot_PowerDown(void)
{
  uint8_t flag = 0;
  
  NB_iot_SendAT("AT+CFUN=0");//�ر���Ƶ
  if( NB_iot_WaitResponse("OK",300/10) == true)
  {
    flag = 1;
  }
  else
  {
    flag = 0;
  }
  return(flag);
}

uint8_t NB_Iot_PowerUp(void)
{
  uint8_t flag = 0;
  
  NB_iot_SendAT("AT+CFUN=1");//�ر���Ƶ
  if( NB_iot_WaitResponse("OK",3000/10) == true)
  {
    flag = 1;
  }
  else
  {
    flag = 0;
  }
  return(flag);
}

uint8_t NB_Iot_Set_IP(void)
{
  uint8_t flag = 0;
  
  NB_iot_SendAT("AT+NCDP=180.101.147.115,5683");
  if( NB_iot_WaitResponse("OK",500/10) == true)
  {
    flag = 1;
  }
  else
  {
    flag = 0;
  }
  return(flag);
}

uint8_t NB_Iot_Attach_NetWork(void)
{
  uint8_t flag = 0;
  
  NB_iot_SendAT("AT+CGATT=1");//ִ������
  if( NB_iot_WaitResponse("OK",1000/10) == true)
  {
    flag = 1;
  }
  else
  {
    flag = 0;
  }
  return(flag);
}

//Query whether network is activated,
uint8_t NB_Iot_Query_Activate_Network(void)
{
  char ucRxBuf[128]={0};
  uint8_t flag = 0;

  Sys_Lptim_Config();//�����͹��Ķ�ʱ�� 
  bsp_StartTimer(NB_Iot_BT_ID, 30000/10); /*  */
  bsp_StartAutoTimer(NB_Iot_LT_ID, 3000/10);//3�����ڶ�ʱ��	

  while (1)
  {	
    if (bsp_CheckTimer(NB_Iot_LT_ID))
    {
      NB_iot_SendAT("AT+CGATT?");//ִ������
    }
    
    if(R_State(&NB_Iot_State,RcvOK) == true)
    {
      Lpuart_Rx_Cpy(&ucRxBuf[0]);
      
      if(strstr(&ucRxBuf[0],"+CGATT:1") != NULL)
      {
        flag = 1;
        break;
      }
    }
    /* ��鶨ʱ��0ʱ���Ƿ� */
    if (bsp_CheckTimer(NB_Iot_BT_ID))
    {
      flag = 0;
      break;/* 3�붨ʱ�����˳���״̬ */
    }
  }
  
    /* �������ʱ��Ӧ�ùرն�ʱ������Ϊ���ǻ�ռ�ú�̨����Դ */
  bsp_StopTimer(NB_Iot_BT_ID);	 //���ζ�ʱ�������ʱ����һ�κ󣬿��Բ��عر�
  bsp_StopTimer(NB_Iot_LT_ID);	 //���ζ�ʱ�������ʱ����һ�κ󣬿��Բ��عر�
  Sys_Lptim_Disable();  
  return(flag);  
}

uint8_t NB_Iot_Activate_Network(void)
{
  uint8_t flag=0;
  
  if( NB_Iot_PowerDown() == true)
  {
    if(NB_Iot_Set_IP() == true)
    {
      if(NB_Iot_PowerUp() == true)//������Ƶ
      {
        if(NB_Iot_Attach_NetWork() == true)
        {
          if(NB_Iot_Query_Activate_Network() == true)
          {
            flag = true;
          }
          else
          {
            flag = false;
          }
        }
        else
        {
          flag = false;
        }
      }
      else
      {
        flag = false;
      }
    }
    else
    {
      flag = false;
    }
  }
  else
  {
    flag = false;
  }
  
  return(flag);
}

/****
��ȡ�ź�ǿ��
�������������������������������ģ���Ѿ����� �Ƿ��У�

NB-IoTͨ��ʵ�����г����������ԣ�����8�Ϳ�������ͨ�ţ����ǲ�ͬ���ն������Ͽ�����������
****/
uint8_t NB_Iot_Signal_Power(void)
{
  char ucRxBuf[128]={0};
  uint8_t len;
  uint8_t rssi = 0;
  
  NB_iot_SendAT("AT+CSQ");
 
  Sys_Lptim_Config();//�����͹��Ķ�ʱ�� 
  bsp_StartTimer(NB_Iot_BT_ID, 300/10); /*  */
  
  while (1)
  {			
    if(R_State(&NB_Iot_State,RcvOK) == true)
    {
      len = Lpuart_Rx_Cpy(&ucRxBuf[0]);
      if( len>0)
      {
        if( ucRxBuf[16]==0x4F && ucRxBuf[17]==0x4B )
        {
          rssi = Ascii_To_Hex(ucRxBuf[7]);
          rssi = rssi * 10;
          rssi = rssi + Ascii_To_Hex(ucRxBuf[8]);
          break;
        }
      }
      
    }
    /* ��鶨ʱ��0ʱ���Ƿ� */
    if (bsp_CheckTimer(NB_Iot_BT_ID))
    {
      rssi = 0;
      break;/* 3�붨ʱ�����˳���״̬ */
    }
  }
  
  /* �������ʱ��Ӧ�ùرն�ʱ������Ϊ���ǻ�ռ�ú�̨����Դ */
  bsp_StopTimer(NB_Iot_BT_ID);	 //���ζ�ʱ�������ʱ����һ�κ󣬿��Բ��عر�
  Sys_Lptim_Disable();  
 
  return(rssi);
}


/******
����  TAU��ʱ�� 
TAU�� Tracking Area Update�� 
TAU��ʱ��С���û������ϱ����ڣ����˷�һ�οտڹ��̡� 
TAU��ʱ�������û������ϱ����ڣ���ɽ�ʡһ�οտڹ��̣����͹��ġ�
TAU��ʱ��0-310Сʱ  Ĭ��12Сʱ
AT+CPSMS=1,,,01000011,01000011
*******/

/******
��ѯIMEI �� 
[08:31:47.481]������AT+CGSN=1
��
[08:31:47.497]�ա���
+CGSN:865352032912825

OK
*******/
void NB_Iot_Get_IMEI(uint8_t *imei)
{
  char *addr;
  char ucRxBuf[128]={0};
 
  NB_iot_SendAT("AT+CGSN=1");//��ѯ IMEI ��
 
  Sys_Lptim_Config();//�����͹��Ķ�ʱ�� 
  bsp_StartTimer(NB_Iot_BT_ID, 300/10); /*  */
  
  while (1)
  {			
    if(R_State(&NB_Iot_State,RcvOK) == true)
    {
       Lpuart_Rx_Cpy(&ucRxBuf[0]);
      if(strstr(ucRxBuf,"OK") != NULL)//�ظ�OK
      {
        addr = strstr(ucRxBuf,"+CGSN:");
        for(uint8_t num=0;num<15;num++)
        {
          *(imei + num) = Ascii_To_Hex(*(addr+ 6 + num));
        }  
        break;
      }
      else
      {
        for(uint8_t num=0;num<15;num++)
        {
          *(imei + num) = 0x00;
        }  
        break;
      }
    }
    /* ��鶨ʱ��0ʱ���Ƿ� */
    if (bsp_CheckTimer(NB_Iot_BT_ID))
    {
      imei = 0;
      break;/* 3�붨ʱ�����˳���״̬ */
    }
  }
  
  /* �������ʱ��Ӧ�ùرն�ʱ������Ϊ���ǻ�ռ�ú�̨����Դ */
  bsp_StopTimer(NB_Iot_BT_ID);	 //���ζ�ʱ�������ʱ����һ�κ󣬿��Բ��عر�
  Sys_Lptim_Disable();  
}

/******
����IMEI �� 
uint8_t tem[]={0x8,0x6,0x5,0x3,0x5,0x2,0x0,0x3,0x2,0x9,0x1,0x2,0x8,0x2,0x5};
NB_Iot_Set_IMEI(&tem[0]);

����1 �ɹ�  ����0 ���ɹ�
*******/
uint8_t NB_Iot_Set_IMEI(uint8_t *imei)
{
  char string[128]={0};
  uint8_t offset=0,flag=0;
  
  strcat(string,AT_SETIMEI);
  offset = strlen(string);
  for(uint8_t num=0;num<15;num++)
  {
    string[offset+num]  = Hex_To_Ascii(*(imei+num));
  }
  NB_iot_SendAT(&string[0]);
  
  if( NB_iot_WaitResponse("OK",500/10) == true)
  {
    flag = 1;
  }
  else
  {
    flag = 0;
  }
  return(flag);
}

uint8_t NB_Iot_Tx(uint8_t *dat,uint8_t len)
{
  uint8_t flag=0;
  
  char string[256]={0};
  
  strcat(string,AT_NMGS); 
  Hex_To_String(dat,&string[0],len,strlen(AT_NMGS));

  NB_iot_SendAT("AT+NNMI=1");//�����·�֪ͨ
  
  if( NB_iot_WaitResponse("OK",500/10) == true)
  {
    NB_iot_SendAT(string);
    if( NB_iot_WaitResponse("OK",5000/10) == true)
    {
      flag = 1;
    }
    else
    {
      flag = 0;
    } 
  }
  else
  {
    flag = 0;
  }
  
  return(flag);
}

/******
��ѯsim���Ƿ�����  
�Ѿ�����AT+CIMI ���� �ε����� ��Ȼ����OKӦ��
*******/

/******
���� T3324(Ĭ��180��)  T3421(Ĭ��12Сʱ)
*******/

/******
���� ������������ 
char AT_NMGS[128] = "AT+NMGS=";//��������AT+NMGS=14,6878563412000068010234F30E16

static uint8_t tem[]={
  0x68,0x78,0x56,0x34,0x12,0x00,0x00,0x68,0x81,0x16,
  0x34,0xF3,0x45,0x45,0x45,0x45,0x4A,0x43,0x43,0x43,
  0x43,0x4B,0x33,0x54,0x75,0x36,0x45,0x58,0x36,0x4A,
  0x33,0x33,0x0C,0x16};
static char string[128];

strcat(string,AT_NMGS); 
Hex_To_String(&tem[0],&string[0],sizeof(tem),strlen(AT_NMGS));
NB_iot_SendAT(string);
*******/
void Hex_To_String(uint8_t *buf , char *str,uint8_t len,uint8_t stroffset)
{
  uint8_t dat=0;

  dat = len / 10 ;
  *(str+0+stroffset) = Hex_To_Ascii(dat);
  dat = len % 10;
  *(str+1+stroffset) = Hex_To_Ascii(dat);
  *(str+2+stroffset) = ',';
  
  for(uint8_t num=0;num<len;num++)
  {
    dat = *(buf+num) & 0xf0 ;
    dat = dat >> 4;
    *(str + 2*num + 3+stroffset)  = Hex_To_Ascii(dat);
    dat = *(buf+num) & 0x0f;
    *(str + 2*num + 4+stroffset) = Hex_To_Ascii(dat);
  }
 
}



uint8_t String_To_Hex(uint8_t *dat )
{
  char ucRxBuf[128]={0};
  uint8_t len,tem,num;
  char *addr;
  
  if( Lpuart_Rx_Cpy(&ucRxBuf[0])>0)
  {
    addr = strstr(ucRxBuf,"+NNMI:");
    
    if(addr != NULL)
    {  
      if( *(addr+ 7) != ',')
      {
        len = Ascii_To_Hex( *(addr+ 6));//���� = (* ָ��) ָ�� = &����
        len = len * 10;
        len = len + Ascii_To_Hex( *(addr+ 7));//���� = (* ָ��) ָ�� = &����
        
        for( num =0;num<len;num++)
        {     
          tem = Ascii_To_Hex(*(addr + 9 + 2*num));
          tem = tem <<4;
          tem = tem + Ascii_To_Hex(*(addr + 10 + 2*num));
          *(dat+num) = tem;
        }
      }
      else
      {
        len = 0;
      }
    }
    else
    {
      len = 0;
    }
  }
  else
  {
    len = 0;
  }
  return(len);
}


//void rxtest(void)
//{
//  char ucRxBuf[128]={0};
//  uint8_t buf[128];
//  char *addr;
//  uint8_t len,tem;
//  uint8_t num =0;
//  Sys_Lptim_Config();//�����͹��Ķ�ʱ�� 
//  bsp_StartTimer(NB_Iot_BT_ID, 6000/10); /*  */
//  
//  while (1)
//  {			 
//    if(R_State(&NB_Iot_State,RcvOK) == true)
//    {
//      W_State(&NB_Iot_State,RcvOK,false);
//      if( Lpuart_Rx_Cpy(&ucRxBuf[0])>0)
//      {
//        addr = strstr(ucRxBuf,"+NNMI:");
//        
//        if(addr != NULL)
//        {  
//          if( *(addr+ 7) != ',')
//          {
//            len = Ascii_To_Hex( *(addr+ 6));//���� = (* ָ��) ָ�� = &����
//            len = len * 10;
//            len = len + Ascii_To_Hex( *(addr+ 7));//���� = (* ָ��) ָ�� = &���� 
//            if(len > 11)
//            {
//              for( num =0;num<len;num++)
//              {     
//                tem = Ascii_To_Hex(*(addr+ 9 +2*num));
//                tem = tem <<4;
//                tem = tem + Ascii_To_Hex(*(addr+ 10 +2*num));
//            
//                buf[num] = tem;
//              } 
//            }
//          } 
//        }  
//      }
//    }
//    /* ��鶨ʱ��0ʱ���Ƿ� */
//    if (bsp_CheckTimer(NB_Iot_BT_ID))
//    {
//      
//      break;/* 3�붨ʱ�����˳���״̬ */
//    }
//  }
//  
//  /* �������ʱ��Ӧ�ùرն�ʱ������Ϊ���ǻ�ռ�ú�̨����Դ */
//  bsp_StopTimer(NB_Iot_BT_ID);	 //���ζ�ʱ�������ʱ����һ�κ󣬿��Բ��عر�
//  Sys_Lptim_Disable();  
//}








 
























