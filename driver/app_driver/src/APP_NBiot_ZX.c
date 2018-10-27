#include "include.h"

//中兴NB模块驱动程序



uint32_t NB_Iot_State = 0;

//基本命令集char *a = "abcdeaf“
//------------------------------------------------------------------
const char AT_AT[] = "AT";//At测试指令
char AT_CFUN_0[] = "AT+CFUN=0";//关闭射频
char AT_NCSEARFCN[] = "AT+NCSEARFCN";//清除频点
char AT_CELL_RESELECTION_1[] = "AT+NCONFIG=CELL_RESELECTION,TRUE";//小区重选开始
char AT_CFUN_1[] = "AT+CFUN=1";//开启射频
char AT_CGATT_1[] = "AT+CGATT=1";//执行搜网
char AT_CGATT_x[] = "AT+CGATT?";//确认入网
char AT_CELL_RESELECTION_0[] = "AT+NCONFIG=CELL_RESELECTION,TRUE";//关闭小区频点
char AT_IP[] = "AT+NCDP=180.101.147.115,5683";//设置IP地址

char AT_CIMI[] = "AT+CIMI";//查询sim卡号 Request international mobile subscriber identity (+CIMI)
 
char AT_CGSN_1[] = "AT+CGSN=1";//查询IMEI号

char AT_NRB[] = "AT+NRB";//软件重启
 
const char AT_NMGS[] = "AT+NMGS=";
const char AT_SETIMEI[] = "AT+NTSETID=1,";


/*
 
*/

/*
*********************************************************************************************************
*	函 数 名: SIM800_SendAT
*	功能说明: 向GSM模块发送AT命令。 本函数自动在AT字符串口增加<CR>字符
*	形    参: _Str : AT命令字符串，不包括末尾的回车<CR>. 以字符0结束
*	返 回 值: 无
\n = 换行
\r = 回车
*********************************************************************************************************
*/
void NB_iot_SendAT(char *Cmd)
{
  Uart_Rx_Init();	/* 清零串口接收缓冲区 */
  Uart_Tx_Init();       /* 清零串口发送缓冲区 */
  Padding_Uart_Tx_Buf((uint8_t *)Cmd, strlen(Cmd)); 
  Padding_Uart_Tx_Buf("\r", 1); 
  Padding_Uart_Tx_Buf("\n", 1); 
  Enable_Lpuart_Tx();
}

/*
*********************************************************************************************************
*	函 数 名: SIM800_WaitResponse
*	功能说明: 等待SIM800返回指定的应答字符串. 比如等待 OK
*	形    参: _pAckStr : 应答的字符串， 长度不得超过255
*			 _usTimeOut : 命令执行超时，0表示一直等待. >０表示超时时间，单位10ms
*	返 回 值: 1 表示成功  0 表示失败
*********************************************************************************************************
*/
uint8_t NB_iot_WaitResponse(char *reply,uint16_t TimeOut)
{
  char ucRxBuf[128]={0};
  uint8_t flag = 0;
  
  Sys_Lptim_Config();//启动低功耗定时器 
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
    
    /* 检查定时器0时间是否到 */
    if (bsp_CheckTimer(NB_Iot_BT_ID))
    {
      flag = 0;
      break;/* 3秒定时到后退出本状态 */
    }
  }
  
  /* 任务结束时，应该关闭定时器，因为他们会占用后台的资源 */
  bsp_StopTimer(NB_Iot_BT_ID);	 //单次定时器如果超时到过一次后，可以不必关闭
  Sys_Lptim_Disable();  
  return(flag);  
}

/*
*********************************************************************************************************
*	函 数 名: SIM800_PowerOn
*	功能说明: 模块上电. 函数内部先判断是否已经开机，如果已开机则直接返回1
*	形    参: 无
*	返 回 值: 1 表示上电成功  0: 表示异常
  if( NB_Iot_Reset() == true)
    {
      __no_operation( );//复位成功
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
  
  Sys_Lptim_Config();//启动低功耗定时器 
  bsp_StartTimer(NB_Iot_BT_ID, 500/10); /* 使用软件定时器3，作为超时控制 */
  
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
  
  NB_iot_SendAT("AT+CFUN=0");//关闭射频
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
  
  NB_iot_SendAT("AT+CFUN=1");//关闭射频
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
  
  NB_iot_SendAT("AT+CGATT=1");//执行搜网
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

  Sys_Lptim_Config();//启动低功耗定时器 
  bsp_StartTimer(NB_Iot_BT_ID, 30000/10); /*  */
  bsp_StartAutoTimer(NB_Iot_LT_ID, 3000/10);//3秒周期定时器	

  while (1)
  {	
    if (bsp_CheckTimer(NB_Iot_LT_ID))
    {
      NB_iot_SendAT("AT+CGATT?");//执行搜网
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
    /* 检查定时器0时间是否到 */
    if (bsp_CheckTimer(NB_Iot_BT_ID))
    {
      flag = 0;
      break;/* 3秒定时到后退出本状态 */
    }
  }
  
    /* 任务结束时，应该关闭定时器，因为他们会占用后台的资源 */
  bsp_StopTimer(NB_Iot_BT_ID);	 //单次定时器如果超时到过一次后，可以不必关闭
  bsp_StopTimer(NB_Iot_LT_ID);	 //单次定时器如果超时到过一次后，可以不必关闭
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
      if(NB_Iot_PowerUp() == true)//开启射频
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
读取信号强度
？？？？？？？？？？？？？？如果模块已经休眠 是否还有？

NB-IoT通信实验室有厂商做过测试，大于8就可以正常通信，但是不同的终端性能上可能有所差异
****/
uint8_t NB_Iot_Signal_Power(void)
{
  char ucRxBuf[128]={0};
  uint8_t len;
  uint8_t rssi = 0;
  
  NB_iot_SendAT("AT+CSQ");
 
  Sys_Lptim_Config();//启动低功耗定时器 
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
    /* 检查定时器0时间是否到 */
    if (bsp_CheckTimer(NB_Iot_BT_ID))
    {
      rssi = 0;
      break;/* 3秒定时到后退出本状态 */
    }
  }
  
  /* 任务结束时，应该关闭定时器，因为他们会占用后台的资源 */
  bsp_StopTimer(NB_Iot_BT_ID);	 //单次定时器如果超时到过一次后，可以不必关闭
  Sys_Lptim_Disable();  
 
  return(rssi);
}


/******
配置  TAU定时器 
TAU： Tracking Area Update， 
TAU定时器小于用户数据上报周期，会浪费一次空口过程。 
TAU定时器大于用户数据上报周期，则可节省一次空口过程，降低功耗。
TAU定时器0-310小时  默认12小时
AT+CPSMS=1,,,01000011,01000011
*******/

/******
查询IMEI 号 
[08:31:47.481]发→◇AT+CGSN=1
□
[08:31:47.497]收←◆
+CGSN:865352032912825

OK
*******/
void NB_Iot_Get_IMEI(uint8_t *imei)
{
  char *addr;
  char ucRxBuf[128]={0};
 
  NB_iot_SendAT("AT+CGSN=1");//查询 IMEI 号
 
  Sys_Lptim_Config();//启动低功耗定时器 
  bsp_StartTimer(NB_Iot_BT_ID, 300/10); /*  */
  
  while (1)
  {			
    if(R_State(&NB_Iot_State,RcvOK) == true)
    {
       Lpuart_Rx_Cpy(&ucRxBuf[0]);
      if(strstr(ucRxBuf,"OK") != NULL)//回复OK
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
    /* 检查定时器0时间是否到 */
    if (bsp_CheckTimer(NB_Iot_BT_ID))
    {
      imei = 0;
      break;/* 3秒定时到后退出本状态 */
    }
  }
  
  /* 任务结束时，应该关闭定时器，因为他们会占用后台的资源 */
  bsp_StopTimer(NB_Iot_BT_ID);	 //单次定时器如果超时到过一次后，可以不必关闭
  Sys_Lptim_Disable();  
}

/******
配置IMEI 号 
uint8_t tem[]={0x8,0x6,0x5,0x3,0x5,0x2,0x0,0x3,0x2,0x9,0x1,0x2,0x8,0x2,0x5};
NB_Iot_Set_IMEI(&tem[0]);

返回1 成功  返回0 不成功
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

  NB_iot_SendAT("AT+NNMI=1");//开启下发通知
  
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
查询sim卡是否正常  
已经测试AT+CIMI 发现 拔掉卡后 仍然会有OK应答
*******/

/******
配置 T3324(默认180秒)  T3421(默认12小时)
*******/

/******
发送 解析接收数据 
char AT_NMGS[128] = "AT+NMGS=";//发送数据AT+NMGS=14,6878563412000068010234F30E16

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
        len = Ascii_To_Hex( *(addr+ 6));//变量 = (* 指针) 指针 = &变量
        len = len * 10;
        len = len + Ascii_To_Hex( *(addr+ 7));//变量 = (* 指针) 指针 = &变量
        
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
//  Sys_Lptim_Config();//启动低功耗定时器 
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
//            len = Ascii_To_Hex( *(addr+ 6));//变量 = (* 指针) 指针 = &变量
//            len = len * 10;
//            len = len + Ascii_To_Hex( *(addr+ 7));//变量 = (* 指针) 指针 = &变量 
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
//    /* 检查定时器0时间是否到 */
//    if (bsp_CheckTimer(NB_Iot_BT_ID))
//    {
//      
//      break;/* 3秒定时到后退出本状态 */
//    }
//  }
//  
//  /* 任务结束时，应该关闭定时器，因为他们会占用后台的资源 */
//  bsp_StopTimer(NB_Iot_BT_ID);	 //单次定时器如果超时到过一次后，可以不必关闭
//  Sys_Lptim_Disable();  
//}








 

























