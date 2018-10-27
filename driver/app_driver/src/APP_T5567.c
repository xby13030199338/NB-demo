#include "T5567.h"
/*
TIM2 发125k方波 PWM 
TIM3 定时器工作方式 
*/
void Rf_Cfe_Set(void)
{
  Sysctrl_SetPeripheralGate(SysctrlPeripheralAdvTim, TRUE);
}
void Rf_Cfe_Clr(void)
{
  Sysctrl_SetPeripheralGate(SysctrlPeripheralAdvTim, FALSE);
}
void Rf_Power_En(void)
{
  Gpio_SetIO(GpioPortA, GpioPin15);
}
void Rf_Power_Ds(void)
{
  Gpio_ClrIO(GpioPortA, GpioPin15);
}
uint32_t Rf_Data(void)
{
  return Gpio_GetInputIO(GpioPortA, GpioPin12);
}

void InitRF(void)
{
  CloseRF();
}
void Open_125K(void)
{
  en_adt_unit_t             enAdt;
  en_adt_compare_t          enAdtCompareA;
  en_adt_compare_t          enAdtCompareB;
  stc_adt_basecnt_cfg_t     stcAdtBaseCntCfg;
  stc_adt_CHxX_port_cfg_t   stcAdtTIM4ACfg;
  //    stc_adt_CHxX_port_cfg_t   stcAdtTIM4BCfg;
  
  DDL_ZERO_STRUCT(enAdt);
  DDL_ZERO_STRUCT(enAdtCompareA);
  DDL_ZERO_STRUCT(enAdtCompareB);
  DDL_ZERO_STRUCT(stcAdtBaseCntCfg);
  DDL_ZERO_STRUCT(stcAdtTIM4ACfg);
  
  Sysctrl_SetPeripheralGate(SysctrlPeripheralAdvTim, TRUE);
  
  enAdt = AdtTIM4;
  
  stcAdtBaseCntCfg.enCntMode = AdtSawtoothMode;
  stcAdtBaseCntCfg.enCntDir = AdtCntUp;
  stcAdtBaseCntCfg.enCntClkDiv = AdtClkPClk0Div16;
  
  Adt_Init(enAdt, &stcAdtBaseCntCfg);                      
  
  Adt_SetPeriod(enAdt, 0x07);                        
  
  enAdtCompareA = AdtCompareA;
  Adt_SetCompareValue(enAdt, enAdtCompareA, 0x03); 
  
  enAdtCompareB = AdtCompareB;
  Adt_SetCompareValue(enAdt, enAdtCompareB, 0x03);  
  
  stcAdtTIM4ACfg.enCap = AdtCHxCompareOutput;          
  stcAdtTIM4ACfg.bOutEn = TRUE;                          
  stcAdtTIM4ACfg.enPerc = AdtCHxPeriodLow;              
  stcAdtTIM4ACfg.enCmpc = AdtCHxCompareHigh;             
  stcAdtTIM4ACfg.enStaStp = AdtCHxStateSelSS;            
  stcAdtTIM4ACfg.enStaOut = AdtCHxPortOutLow;            
  stcAdtTIM4ACfg.enStpOut = AdtCHxPortOutLow;            
  Adt_CHxXPortConfig(enAdt, AdtCHxA, &stcAdtTIM4ACfg);  
  
  //	stcAdtTIM4BCfg.enCap = AdtCHxCompareOutput;
  //	stcAdtTIM4BCfg.bOutEn = TRUE;
  //	stcAdtTIM4BCfg.enPerc = AdtCHxPeriodLow;
  //	stcAdtTIM4BCfg.enCmpc = AdtCHxCompareHigh;
  //	stcAdtTIM4BCfg.enStaStp = AdtCHxStateSelSS;
  //	stcAdtTIM4BCfg.enStaOut = AdtCHxPortOutLow;
  //	stcAdtTIM4BCfg.enStpOut = AdtCHxPortOutLow;
  //	Adt_CHxXPortConfig(enAdt, AdtCHxB, &stcAdtTIM4BCfg); 
  
  Adt_StartCount(enAdt);
}
void Close_125K(void)
{
  Sysctrl_SetPeripheralGate(SysctrlPeripheralAdvTim, FALSE);
  Gpio_ClrIO(GpioPortA, GpioPin8);
  Gpio_ClrIO(GpioPortA, GpioPin15);
}

void TIM3_SetCounter(uint16_t Coun)
{
  Tim3_M0_ARRSet(Coun);                           
  Tim3_M0_Cnt16Set(Coun);                        
  Tim3_M0_Run();
}

uint16_t TIM3_GetCounter(void)
{
  return Tim3_M0_Cnt16Get();
}

void Enable_Timer(void)
{
  stc_tim3_mode0_config_t     stcTim3BaseCfg;
  DDL_ZERO_STRUCT(stcTim3BaseCfg);
  
  Sysctrl_SetPeripheralGate(SysctrlPeripheralTim3, TRUE); 
  
  stcTim3BaseCfg.enWorkMode = Tim3WorkMode0;              
  stcTim3BaseCfg.enCT       = Tim3Timer;                  
  stcTim3BaseCfg.enPRS      = Tim3PCLKDiv16;             
  stcTim3BaseCfg.enCntMode  = Tim316bitArrMode;          
  stcTim3BaseCfg.bEnTog     = FALSE;
  stcTim3BaseCfg.bEnGate    = FALSE;
  stcTim3BaseCfg.enGateP    = Tim3GatePositive;                  
  
  Tim3_Mode0_Init(&stcTim3BaseCfg);                       

  Tim3_Mode0_DisableIrq();    
}
void Disable_Timer(void)
{
  Sysctrl_SetPeripheralGate(SysctrlPeripheralTim3,FALSE);
}
void OpenRF(void)
{
  Rf_Power_En();
  Open_125K();
  Enable_Timer();
}
void CloseRF(void)
{
  Rf_Power_Ds();
  Close_125K();
  Disable_Timer();
}
bool Read(unsigned char m,unsigned char *r_buf)
{
  unsigned char temp=0;
  unsigned char BitCounter=8;
  unsigned char ByteCounter=0;
  unsigned int  ReadTimer=0;
  unsigned int  BitTimer=0;
  unsigned char OldIn=0;   
  unsigned char NewIn=0; 
  bool           CardErr = true;
  //错误
Error:
  temp=0;
  BitCounter=8;
  ByteCounter=0;
  OldIn=0;
  BitTimer=0;
  //开始
Start:
  TIM3_SetCounter(0);
  do{;}while((Rf_Data())==1);     //等待高电平
  
  ReadTimer=ReadTimer+1;
  if(ReadTimer>1000)            //读数据次数限制
  {
    temp=0;
    BitCounter=8;
    ByteCounter=0;
    OldIn=0;
    BitTimer=0;
    goto End;
  }
  OldIn=0;                    //指定要判断的电平"0"
  do
  {
    NewIn=Rf_Data();            //读取外部输入电平
    BitTimer = TIM3_GetCounter();///////////////////////////////////////////
    if(BitTimer>READ_TIME_OUT)
    {
      goto End;
    }
  }
  while(NewIn==OldIn);
  OldIn=NewIn;                //指定要判断的电平"1"
  TIM3_SetCounter(0);         ///////////////////////////////////////////
  do
  {
    NewIn=Rf_Data();            //读取外部输入电平
    BitTimer = TIM3_GetCounter(); ///////////////////////////////////////////
    if(BitTimer>READ_TIME_OUT)
    {
      goto End;
    }
  }
  while(NewIn==OldIn);
  OldIn=NewIn;                //指定要判断的电平"0"
  BitTimer = TIM3_GetCounter();///////////////////////////////////////////
  if(BitTimer>=READ_15P_TIME)      //判断时间>=READ_1.5P_TIME  1 
  {
    TIM3_SetCounter(0);
    do
    {
      NewIn=Rf_Data();          //读取外部输入电平
      
    }
    while(NewIn==OldIn);
    OldIn=NewIn;             //指定要判断的电平"1"
    BitTimer=TIM3_GetCounter();
    if(BitTimer<=READ_05P_TIME)  //判断时间<=READ_05P_TIME  2
    {
      TIM3_SetCounter(0);
      do
      {
        NewIn=Rf_Data();       //读取外部输入电平
      }
      while(NewIn==OldIn);
      OldIn=NewIn;                   //指定要判断的电平"0"
      BitTimer=TIM3_GetCounter();
      if(BitTimer>=READ_05P_TIME)         //判断时间>=READ_05P_TIME 3
      {
        TIM3_SetCounter(0);
        if(BitTimer<=READ_10P_TIME ) //判断时间<=READ_READ_10P_TIME 为"0" 否则为"1"  4
        {        
        num:
          do
          {
            NewIn=Rf_Data();          //读取外部输入电平
          }
          while(NewIn==OldIn);
          OldIn=NewIn;              //指定要判断的电平“1”
          BitTimer=TIM3_GetCounter();
          
          if(BitTimer<=READ_05P_TIME)    //判断时间<=READ_05P_TIME  
          {
            goto pddp;
          }
          else goto Error; 
        }
        else   
        {
          goto pddp;
        }
      }
      else
      {
        goto Start; 
      }
      //接收数据判断保存
    pddp:
      if(OldIn==0)   
      {
        temp=temp|0x01;
      }
      else if(OldIn==1)
      {
        temp=temp&0xfe;
      }
      BitCounter--;
      if(BitCounter!=0)  
      {
        temp=temp<<1;
      }
      else
      {
        BitCounter=8;
        ByteCounter++;
        r_buf[ByteCounter-1]=temp;
        temp=0;
        BitTimer=0;
        if(ByteCounter==m)         //判断读取数据数量与制定读取数量是否相等
        {
          CardErr = false ;
          goto End;
        }
      }
      TIM3_SetCounter(0);
      do
      {
        NewIn=Rf_Data();            //读取外部输入电平
      }
      while(NewIn==OldIn);
      OldIn=NewIn;
      BitTimer = TIM3_GetCounter();
      if(BitTimer>=READ_05P_TIME)      //判断时间>=READ_05P_TIME 为"1" 否则为"0"
      {
        goto pddp;
      }
      else   
      {
        TIM3_SetCounter(0);
        goto num;
      }
    }
    else goto Start;  
  }
  else goto Start;   
  //结束
End:
  temp=0;
  BitCounter=8;
  ByteCounter=0;
  OldIn=0;
  BitTimer=0;
  ReadTimer=0;
  return (CardErr);
}
void Rf_Send_Zero(void)
{
  unsigned int cnt=0,i=1;
  Rf_Cfe_Set();
  TIM3_SetCounter(0);
  while(i)
  {
    cnt = TIM3_GetCounter();
    if(cnt>SEND_ZERO_TIME)//128//118
    {
      i=0;
    }
  }
}
void Rf_Send_One(void)
{
  
  unsigned int cnt=0,i=1;
  Rf_Cfe_Set();
  TIM3_SetCounter(0);
  while(i)
  {
    cnt = TIM3_GetCounter();
    if(cnt>SEND_ONE_TIME)//384//354
    {
      i=0;
    }
  }
}
void Rf_Send_Gap(void)
{ 
  unsigned int cnt=0,i=1;
  Rf_Cfe_Clr();
  TIM3_SetCounter(0);
  while(i)
  {
    cnt = TIM3_GetCounter();
    if(cnt>SEND_GAP_TIME)//256//234
    {
      i=0;
    }
  }
}

void RF_Start(void)
{
  unsigned int cnt=0,i=1; 
  Rf_Cfe_Clr();
  TIM3_SetCounter(0);
  while(i)
  {
    cnt = TIM3_GetCounter();
    if(cnt> START_TIME)//256//236
    {
      i=0;
    }
  }
}
void RF_Stop(void)
{
  unsigned int cnt=0,i=1,j=0;
  Rf_Cfe_Set();
  for (j=0;j<100;j++)
  {
    TIM3_SetCounter(0);
    while(i)
    {
      cnt = TIM3_GetCounter();
      if(cnt>STOP_TIME)//256//236
      {
        i=0;
      }
    }
  }
}
void RF_Reast(void)
{
  RF_Start(); 
  
  Rf_Send_Zero();            /*1-"0"*/
  Rf_Send_Gap();
  
  Rf_Send_Zero();            /*2_"0"*/
  Rf_Send_Gap();
  
  RF_Stop();
}
void Rf_Send_Opcode(unsigned char a,unsigned char b)
{
  if (a==1)
  {
    Rf_Send_One();             /*1-"1"*/
    Rf_Send_Gap();
  }
  else
  {
    Rf_Send_Zero();            /*1-"0"*/
    Rf_Send_Gap();
  }
  
  if (b==1)
  {
    Rf_Send_One();             /*2-"1"*/
    Rf_Send_Gap();
  }
  else
  {
    Rf_Send_Zero();            /*2-"0"*/
    Rf_Send_Gap();
  }
}
void Rf_Send_Lockbit(unsigned char a)
{
  if (a==1)
  {
    Rf_Send_One();             /*1-"1"*/
    Rf_Send_Gap();
  }
  else
  {
    Rf_Send_Zero();            /*1-"0"*/
    Rf_Send_Gap();
  }
}
void Rf_Send_Adress(unsigned char a)
{
  unsigned char temp,i;
  temp=a<<5;
  for (i=0;i<3;i++)
  {
    if((temp&0x80)==0x80)
    {
      Rf_Send_One();             /*"1"*/
      Rf_Send_Gap();
    }
    else
    {
      Rf_Send_Zero();            /*"0"*/
      Rf_Send_Gap();
    }
    temp=temp<<1;
  }
}
void Rf_Send_Word(unsigned char temp)
{
  unsigned char i;
  for (i=0;i<8;i++)
  {
    if((temp&0x80)==0x80)
    {
      Rf_Send_One();             /*"1"*/
      Rf_Send_Gap();
    }
    else
    {
      Rf_Send_Zero();            /*"0"*/
      Rf_Send_Gap();
    }
    temp=temp<<1;
  }
}
void Rf_Send_Data(unsigned char m,unsigned char*w_buf)
{
  unsigned char i;
  for(i=m;i<m+4;i++)
  {
    Rf_Send_Word(w_buf[i]);
  }
}
void Read_T5567_Card(unsigned char *buf)
{
  OpenRF();                     //打开RF
  RF_Reast();                   //卡复位      
  RF_Start();                   //卡开始
  Rf_Send_Gap();                //发送两个"GAP"
  Rf_Send_Gap();                //发送两个"GAP"       
  Rf_Send_Opcode(1,0);          //指定页
  RF_Stop();                    //卡结束
  Read(28,buf);                 //指定数据数量    //读数据（page-数据页,number-数据数量）
  CloseRF();                    //关闭RF
}
void Write_T5567_Card(unsigned char *buf)
{
	unsigned char i=0;
  OpenRF();                                   //打开RF 
  
  for(i=0;i<7;i++)                     //写数据（1-7）
  {
    RF_Reast();            //卡复位      
    RF_Start();            //卡开始
    Rf_Send_Opcode(1,0);   //发送操作码
    Rf_Send_Lockbit(0);    //发送锁位码
    Rf_Send_Data(i*4,buf);   //发送指定的4个字节数据
    Rf_Send_Adress(i+1);//发送要写入的卡数据区 
    RF_Stop();             //卡结束
  }
  CloseRF();                                  //关闭RF
}




