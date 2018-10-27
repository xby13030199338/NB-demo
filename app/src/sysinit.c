#include "include.h" 



 

/*******************************************************************************
** Function name:           SysClkInit
** Descriptions:            时钟初始化 内部16MHz高频时钟 
** input parameters:        void 
** output parameters:       void
** Returned value:          void    
*******************************************************************************/
void SysClk_Init(void)
{
  stc_sysctrl_clk_config_t stcCfg;

  ///< 因要使用的时钟源HCLK小于24M：此处设置FLASH 读等待周期为0 cycle(默认值也为0 cycle)
  Flash_WaitCycle(FlashWaitCycle0);
  ///< 时钟初始化前，优先设置要使用的时钟源：此处设置RCH为4MHz（默认值为4MHz）
  Sysctrl_SetRCHTrim(SysctrlRchFreq16MHz);
  Sysctrl_SetRCLTrim(SysctrlRclFreq38400);
  Sysctrl_ClkSourceEnable(SysctrlClkXTL,TRUE);
  Sysctrl_ClkSourceEnable(SysctrlClkRCL, TRUE);
  ///< 选择内部RCH作为HCLK时钟源;
  stcCfg.enClkSrc    = SysctrlClkRCH;
  ///< HCLK SYSCLK/1
  stcCfg.enHClkDiv   = SysctrlHclkDiv1;
  ///< PCLK 为HCLK/1
  stcCfg.enPClkDiv   = SysctrlPclkDiv1;
  ///< 系统时钟初始化
  Sysctrl_ClkInit(&stcCfg);//配置内核系统时钟
    
  //开启外设时钟
  Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);//GPIO外设时钟打开
  Sysctrl_SetPeripheralGate(SysctrlPeripheralRtc,TRUE);//RTC模块时钟打开
  Sysctrl_SetPeripheralGate(SysctrlPeripheralWdt,TRUE);//RTC模块时钟打开
  Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr, TRUE); //ADC模块外设时钟打开
  Sysctrl_SetPeripheralGate(SysctrlPeripheralLpUart0,TRUE); //低功耗串口0
  Sysctrl_SetPeripheralGate(SysctrlPeripheralRng, TRUE);  //打开随机数发生器RNG模块时钟门控
  Sysctrl_SetPeripheralGate(SysctrlPeripheralLcd,TRUE);

    
}


void GPIO_Init(void)
{ 
  stc_gpio_config_t stcGpioCfg;
  DDL_ZERO_STRUCT(stcGpioCfg);

  stcGpioCfg.enDir = GpioDirOut;
  stcGpioCfg.enDrv = GpioDrvH;//GPIO低驱动能力 
  stcGpioCfg.enPuPd= GpioNoPuPd;//GPIO无上下拉
  stcGpioCfg.enOD  = GpioOdDisable;
  //空IO口
  Gpio_Init(GpioPortA,GpioPin9,&stcGpioCfg);
  Gpio_ClrIO(GpioPortA,GpioPin9);
  Gpio_Init(GpioPortB,GpioPin3,&stcGpioCfg);
  Gpio_SetIO(GpioPortB,GpioPin3);
  
  
 /****************** IIC GPIO初始化 ******************************************/ 
  
    ///< 端口方向配置->输出
  stcGpioCfg.enDir = GpioDirOut;
  ///< 端口驱动能力配置->高驱动能力
  stcGpioCfg.enDrv = GpioDrvH;
  ///< 端口上下拉配置->无上下拉
  stcGpioCfg.enPuPd = GpioPu;
  ///< 端口开漏输出配置->开漏输出关闭
  stcGpioCfg.enOD = GpioOdEnable;

  Gpio_Init(GpioPortB, GpioPin5, &stcGpioCfg);//EEPROM_SCL
  Gpio_Init(GpioPortB, GpioPin4, &stcGpioCfg);//EEPROM_SDA
  
      ///< 端口方向配置->输出
  stcGpioCfg.enDir = GpioDirOut;
  ///< 端口驱动能力配置->高驱动能力
  stcGpioCfg.enDrv = GpioDrvH;
  ///< 端口上下拉配置->无上下拉
  stcGpioCfg.enPuPd = GpioNoPuPd;
  ///< 端口开漏输出配置->开漏输出关闭
  stcGpioCfg.enOD = GpioOdDisable;
  Gpio_Init(GpioPortB, GpioPin6, &stcGpioCfg);//EEPROM_WP
  Gpio_Init(GpioPortB, GpioPin7, &stcGpioCfg);//EEPROM_VCC
  
 /****************** 电机驱动 GPIO初始化 *************************************/ 
        ///< 端口方向配置->输出
  stcGpioCfg.enDir = GpioDirOut;
  ///< 端口驱动能力配置->高驱动能力
  stcGpioCfg.enDrv = GpioDrvH;
  ///< 端口上下拉配置->无上下拉
  stcGpioCfg.enPuPd = GpioNoPuPd;
  ///< 端口开漏输出配置->开漏输出关闭
  stcGpioCfg.enOD = GpioOdDisable;
  Gpio_Init(GpioPortD, GpioPin2 , &stcGpioCfg);// 
  Gpio_Init(GpioPortC, GpioPin12, &stcGpioCfg);// 
  Gpio_ClrIO(GpioPortD, GpioPin2);
  Gpio_ClrIO(GpioPortC, GpioPin12);
  /******************按键GPIO初始化 ******************************************/ 

 
    
    ///< 端口方向配置->输入
    stcGpioCfg.enDir = GpioDirIn;
    ///< 端口驱动能力配置->高驱动能力
    stcGpioCfg.enDrv = GpioDrvL;
    ///< 端口上下拉配置->上拉
    stcGpioCfg.enPuPd = GpioPu;
    ///< 端口开漏输出配置->开漏输出关闭
    stcGpioCfg.enOD = GpioOdDisable;
    ///< GPIO IO PD04初始化(PD04在STK上外接KEY(USER))
    Gpio_Init(GpioPortA, GpioPin3, &stcGpioCfg);
    
    ///< 使能端口PORTD系统中断
    EnableNvic(PORTA_IRQn, IrqLevel3, TRUE);
    ///< 打开并配置PD04为下降沿中断
    Gpio_EnableIrq(GpioPortA, GpioPin3, GpioIrqFalling);
    
  
  /**************低功耗串口0 GPIO初始化 ***********************/ 
  
  stcGpioCfg.enDir      = GpioDirOut;
  stcGpioCfg.enDrv      = GpioDrvH;
  stcGpioCfg.enPuPd     = GpioNoPuPd;
  stcGpioCfg.enOD       = GpioOdDisable;
    
  Gpio_Init(GpioPortB, GpioPin0, &stcGpioCfg);
  Gpio_ClrIO(GpioPortB, GpioPin0);
  Gpio_Init(GpioPortB, GpioPin11, &stcGpioCfg);
  Gpio_ClrIO(GpioPortB, GpioPin11);
  Gpio_SetAfMode(GpioPortB,GpioPin0,GpioAf3);
  Gpio_SetAfMode(GpioPortB,GpioPin11,GpioAf3);
  
  /**************ADC GPIO初始化 ******************************/ 
  
  Gpio_SetAnalogMode(GpioPortC, GpioPin0);        //PA00 (AIN10)
  stcGpioCfg.enDir = GpioDirIn;
  Gpio_Init(GpioPortC, GpioPin0, &stcGpioCfg);
  
  /**************流量GPIO初始化 ******************************/ 

    ///< 端口方向配置->输入
  stcGpioCfg.enDir = GpioDirIn;
  ///< 端口驱动能力配置->高驱动能力
  stcGpioCfg.enDrv = GpioDrvL;
  ///< 端口上下拉配置->上拉
  stcGpioCfg.enPuPd = GpioPu;
  ///< 端口开漏输出配置->开漏输出关闭
  stcGpioCfg.enOD = GpioOdDisable;
  
  Gpio_Init(GpioPortD, GpioPin4, &stcGpioCfg);
  Gpio_Init(GpioPortD, GpioPin5, &stcGpioCfg);
//  ///< 使能端口PORTD系统中断
//  EnableNvic(PORTD_IRQn, IrqLevel3, TRUE);
//  ///< 打开并配置PD04为下降沿中断
//  Gpio_EnableIrq(GpioPortD, GpioPin4, GpioIrqFalling);
//  Gpio_EnableIrq(GpioPortD, GpioPin5, GpioIrqFalling);
  
  
  
    /****************** 蜂鸣器 GPIO初始化 *************************************/ 
    ///< 端口方向配置->输出
    stcGpioCfg.enDir = GpioDirOut;
    ///< 端口驱动能力配置->高驱动能力
    stcGpioCfg.enDrv = GpioDrvH;
    ///< 端口上下拉配置->无上下拉
    stcGpioCfg.enPuPd = GpioNoPuPd;
    ///< 端口开漏输出配置->开漏输出关闭
    stcGpioCfg.enOD = GpioOdDisable;
    Gpio_Init(GpioPortD, GpioPin6, &stcGpioCfg);//EEPROM_WP
}


void RTC_Init(void)
{
  stc_rtc_config_t stcRtcConfig; 
  stc_rtc_irq_cb_t stcIrqCb;
  stc_rtc_time_t  stcTime;
  stc_rtc_alarmset_t stcAlarm;
  stc_rtc_cyc_sel_t   stcCycSel;
  
  DDL_ZERO_STRUCT(stcRtcConfig);
  DDL_ZERO_STRUCT(stcIrqCb);
  DDL_ZERO_STRUCT(stcAlarm);
  DDL_ZERO_STRUCT(stcTime);
  DDL_ZERO_STRUCT(stcCycSel);
  
  Sysctrl_SetPeripheralGate(SysctrlPeripheralRtc,TRUE);//RTC模块时钟打开
  
  stcRtcConfig.enClkSel = RtcClk32768;//RtcClkHxt1024;//RtcClk32;//
  stcRtcConfig.enAmpmSel = Rtc24h;//Rtc12h;//
  
  stcCycSel.enCyc_sel = RtcPrads;
  stcCycSel.enPrds_sel = Rtc_05S;
  
  stcRtcConfig.pstcCycSel = &stcCycSel;
  
  Rtc_DisableFunc(RtcCount);
  stcAlarm.u8Minute = 0x59;
  stcAlarm.u8Hour = 0x10;
  stcAlarm.u8Week = 0x02;
  Rtc_DisableFunc(RtcAlarmEn);
  Rtc_EnAlarmIrq(Rtc_AlarmInt_Enable);
  Rtc_SetAlarmTime(&stcAlarm);
  Rtc_EnableFunc(RtcAlarmEn);
  
  //  stcTime.u8Year = 0x18;
  //  stcTime.u8Month = 0x04;
  //  stcTime.u8Day = 0x16;
  //  stcTime.u8Hour = 0x10;
  //  stcTime.u8Minute = 0x58;
  //  stcTime.u8Second = 0x55;
  //  stcTime.u8DayOfWeek = Rtc_CalWeek(&stcTime.u8Day);
  
  stcRtcConfig.pstcTimeDate = NULL;
  
  stcIrqCb.pfnAlarmIrqCb = RtcAlarmCb;
  stcIrqCb.pfnTimerIrqCb = RtcCycCb;
  stcRtcConfig.pstcIrqCb = &stcIrqCb;
  stcRtcConfig.bTouchNvic = TRUE;
  
  Rtc_DisableFunc(RtcCount);
  Rtc_Init(&stcRtcConfig); 
  
  Rtc_EnableFunc(RtcCount);
  
}

 
/*******************************************************************************
** Function name:           Lpuart0_Init
** Descriptions:            串口初始化函数
** input parameters:        void 
** output parameters:       void
** Returned value:          void    
*******************************************************************************/
void Lpuart0_Init(void)
{
  uint16_t                      u16Scnt = 0;
  stc_lpuart_sclk_sel_t 	stcSclk;
  stc_lpuart_config_t  		stcConfig;
  stc_lpuart_irq_cb_t 		stcLPUartIrqCb;
  stc_lpuart_multimode_t 	stcMulti;
  stc_lpuart_baud_t 		stcBaud;
  
  DDL_ZERO_STRUCT(stcConfig);
  DDL_ZERO_STRUCT(stcLPUartIrqCb);
  DDL_ZERO_STRUCT(stcMulti);
  DDL_ZERO_STRUCT(stcBaud);
  DDL_ZERO_STRUCT(stcSclk);

  stcLPUartIrqCb.pfnRxIrqCb   = RxIntCallback;
  stcLPUartIrqCb.pfnTxIrqCb   = TxIntCallback;
  stcLPUartIrqCb.pfnRxFEIrqCb = ErrIntCallback;
  stcLPUartIrqCb.pfnPEIrqCb   = PErrIntCallBack;
  stcLPUartIrqCb.pfnCtsIrqCb  = CtsIntCallBack;
  
  stcConfig.pstcIrqCb = &stcLPUartIrqCb;
  stcConfig.bTouchNvic = TRUE;
  stcConfig.enStopBit = LPUart1bit;
  stcConfig.enRunMode = LPUartMode3;
  
  stcSclk.enSclk_Prs  = LPUart4Or8Div;
  stcSclk.enSclk_sel  = LPUart_Pclk;
  stcConfig.pstcLpuart_clk = &stcSclk;  
  
  stcMulti.enMulti_mode = LPUartNormal;
  stcConfig.pstcMultiMode = &stcMulti;
  
  LPUart_SetMMDOrCk(LPUART0,LPUartEven);
  LPUart_Init(LPUART0, &stcConfig);
  
  LPUart_SetClkDiv(LPUART0,LPUart4Or8Div);
  stcBaud.u32Sclk = LPUart_GetSclk(LPUART0);
  stcBaud.enRunMode = LPUartMode3;
  stcBaud.u16Baud = 9600;
  u16Scnt = LPUart_CalScnt(LPUART0,stcBaud);
  LPUart_SetBaud(LPUART0,u16Scnt);
  
  LPUart_EnableIrq(LPUART0 ,LPUartTxIrq);
  LPUart_EnableIrq(LPUART0 ,LPUartRxIrq);
  LPUart_ClrStatus(LPUART0 ,LPUartRC);
  LPUart_EnableFunc(LPUART0,LPUartRx); 
}


/*****************低功耗定时器********************
******************低功耗定时器********************/
void Sys_Lptim_Config(void)
{
  stc_lpt_config_t	stcConfig;
  DDL_ZERO_STRUCT(stcConfig);
  Sysctrl_SetPeripheralGate(SysctrlPeripheralLpTim, TRUE); //使能低功耗模块时钟
  
  stcConfig.pfnLpTimCb	= LpTimIntCallback;	//中断函数入口
  stcConfig.enGate      = LptGateDisable; 	//Gate Disable
  stcConfig.enGateP     = LptActLowLevel; 	//Gate_P 低电平有效
  stcConfig.enTckSel 	= LptIRC32K;            //LPT时钟选择RCL 内部低频38000
  stcConfig.enTog       = LptTogDisable;        //TOG功能Disable
  stcConfig.enCT        = LptTimer;             //定时功能
  stcConfig.enMD        = Lpt16bitArrMode;      //16位重载模式 模式2 
  Lpt_Init(&stcConfig);                         //LPT初始化
  //设置重载值，启动运行
  Lpt_ARRSet(65155);                            //(65535-60785=4750)*(1/38000)=0.125s
  Lpt_ClearIntFlag();                           //清除中断标志位
  Lpt_EnableIrq(); 	                        // 使能中断
  EnableNvic(LPTIM_IRQn, IrqLevel3, TRUE);      //使能中断。
  Lpt_Run();                                    //启动定时器
}

void Sys_Lptim_Disable(void)
{
  Lpt_ClearIntFlag(); //清除中断标志位
  Lpt_DisableIrq();
  Lpt_Stop();
  Sysctrl_SetPeripheralGate(SysctrlPeripheralLpTim, FALSE); //使能低功耗模块时钟
}

/*******************************************************************************
** Function name:           Timer0Init
** Descriptions:            Timer0初始化 
** input parameters:        void 
** output parameters:       void
** Returned value:          void    
	Timer0  10ms产生中断，用于判断接收数据帧是否完成，在RxIntCallback中调用
*******************************************************************************/
void Timer0_Init(void)
{
  uint16_t                  u16ArrValue;
  uint16_t                  u16CntValue;
  stc_bt_mode0_config_t     stcBtBaseCfg;
  
  DDL_ZERO_STRUCT(stcBtBaseCfg);

  Sysctrl_SetPeripheralGate(SysctrlPeripheralBTim, TRUE); 
  
  stcBtBaseCfg.enWorkMode = BtWorkMode0;                  
  stcBtBaseCfg.enCT       = BtTimer;                      
  stcBtBaseCfg.enPRS      = BtPCLKDiv16;                 
  stcBtBaseCfg.enCntMode  = Bt16bitArrMode;              
  stcBtBaseCfg.bEnTog     = FALSE;
  stcBtBaseCfg.bEnGate    = FALSE;
  stcBtBaseCfg.enGateP    = BtGatePositive;
  stcBtBaseCfg.pfnTim0Cb  = Tim0Int;                     
  Bt_Mode0_Init(TIM0, &stcBtBaseCfg);                    
  
  Bt_ClearIntFlag(TIM0,BtUevIrq);                    
  EnableNvic(TIM0_IRQn, IrqLevel3, TRUE);              
  Bt_Mode0_EnableIrq(TIM0);  
  
  u16ArrValue = 55535;
  Bt_M0_ARRSet(TIM0, u16ArrValue);                   
  u16CntValue = 55535;
  Bt_M0_Cnt16Set(TIM0, u16CntValue);                      
  Bt_M0_Run(TIM0);                                        
}



void LCD_Config(void)
{
  uint8_t i;
  
  stc_lcd_config_t stcLcdCfg;
  stc_lcd_segcompara_t stcSegComPara;
  stc_lcd_segcom_t stcLcdSegCom;  
  
  DDL_ZERO_STRUCT(stcLcdCfg);
  DDL_ZERO_STRUCT(stcSegComPara);
  DDL_ZERO_STRUCT(stcLcdSegCom);
  
  /****************** LCD GPIO初始化 *************************************/ 
  Gpio_SetAnalogMode(GpioPortA, GpioPin9);//COM0
  Gpio_SetAnalogMode(GpioPortA, GpioPin10);//COM1
  Gpio_SetAnalogMode(GpioPortA, GpioPin11);//COM2
  Gpio_SetAnalogMode(GpioPortA, GpioPin12);//COM3
  
  Gpio_SetAnalogMode(GpioPortA, GpioPin8);//SEG0
  Gpio_SetAnalogMode(GpioPortC, GpioPin9);//SEG1
  Gpio_SetAnalogMode(GpioPortC, GpioPin8);//SEG2
  Gpio_SetAnalogMode(GpioPortC, GpioPin7);//SEG3
  Gpio_SetAnalogMode(GpioPortC, GpioPin6);//SEG4
  Gpio_SetAnalogMode(GpioPortB, GpioPin15);//SEG5
  Gpio_SetAnalogMode(GpioPortB, GpioPin14);//SEG6
  Gpio_SetAnalogMode(GpioPortB, GpioPin13);//SEG7
  Gpio_SetAnalogMode(GpioPortB, GpioPin12);//SEG8
  Gpio_SetAnalogMode(GpioPortB, GpioPin11);//SEG9
  Gpio_SetAnalogMode(GpioPortB, GpioPin10);//SEG10
  Gpio_SetAnalogMode(GpioPortB, GpioPin2);//SEG11
  Gpio_SetAnalogMode(GpioPortB, GpioPin1);//SEG12
  Gpio_SetAnalogMode(GpioPortB, GpioPin0);//SEG13
  Gpio_SetAnalogMode(GpioPortC, GpioPin5);//SEG14
  Gpio_SetAnalogMode(GpioPortC, GpioPin4);//SEG15
  Gpio_SetAnalogMode(GpioPortA, GpioPin7);//SEG16
  Gpio_SetAnalogMode(GpioPortA, GpioPin6);//SEG17
  Gpio_SetAnalogMode(GpioPortA, GpioPin5);//SEG18
  
  Gpio_SetAnalogMode(GpioPortB, GpioPin3);//VLCDH
  Gpio_SetAnalogMode(GpioPortB, GpioPin4);//VLCD3
  Gpio_SetAnalogMode(GpioPortB, GpioPin5);//VLCD2
  Gpio_SetAnalogMode(GpioPortB, GpioPin6);//VLCD1
  /*******************和LCD相关端口都要配置为模拟端口*****************/
  
  stcSegComPara.enBiasSrc = LcdInRes_Low;
  stcSegComPara.enRunMode = Lcd_4DutyMode;
  stcSegComPara.u8MaxSeg = 19;
  LCD_GetSegCom(stcSegComPara,&stcLcdSegCom);//根据DUTY及SRC获取COM端口配置
  for(i=0;i<stcSegComPara.u8MaxSeg;i++)
  {
    stcLcdSegCom.u32Seg0_31 |=1<<i;
  }
  stcLcdSegCom.u32Seg0_31 = ~stcLcdSegCom.u32Seg0_31;
  LCD_SetSegCom(stcLcdSegCom);//comseg口配置
  stcLcdCfg.enDispMode = LcdMode1;
  stcLcdCfg.enBias = LcdBias3;
  stcLcdCfg.enBiasSrc = LcdInRes_Low;
  stcLcdCfg.enClk = LcdRCL;
  stcLcdCfg.enDuty = LcdDuty4;
  stcLcdCfg.enCpClk = LcdClk2k;
  stcLcdCfg.enScanClk = LcdClk256hz;
  LCD_Init(stcLcdCfg);
  LCD_EnFunc(LcdEn,TRUE);
  LCD_ClearDisp();
//  LCD_FullDisp();
}














/*****************Pwc部分********************
******************Pwc部分********************
******************Pwc部分********************/


/*******************************************************************************
** Function name:           Tim_Pwc
** Descriptions:            定时器Tim1_CHA  PWC模式 （测量脉宽） 
** input parameters:        void 
** output parameters:       void
** Returned value:          void    
	Tim1_CHA测量脉宽，测量结果保存在函数Tim1Int
		中断函数的u32PwcCapValue_TIM1变量中
*******************************************************************************/
void Pwc_Init(void)
{
  stc_bt_mode1_config_t      stcBtBaseCfg;
  stc_bt_pwc_input_config_t  stcBtPwcInCfg;
  stc_gpio_config_t          stcTIM0APort;
  
  DDL_ZERO_STRUCT(stcBtBaseCfg);
  DDL_ZERO_STRUCT(stcBtPwcInCfg);
  DDL_ZERO_STRUCT(stcTIM0APort);

  Sysctrl_SetPeripheralGate(SysctrlPeripheralBTim, TRUE); //Base Timer外设时钟使能

  stcBtBaseCfg.enWorkMode = BtWorkMode1;                  //定时器模式
  stcBtBaseCfg.enCT       = BtTimer;                      //定时器功能，计数时钟为内部PCLK
  stcBtBaseCfg.enPRS      = BtPCLKDiv4;                  //PCLK
  stcBtBaseCfg.enOneShot  = BtPwcOneShotDetect;           //PWC循环检测
  stcBtBaseCfg.pfnTim1Cb  = Tim1Int;                      //中断函数入口                   
  Bt_Mode1_Init(TIM1, &stcBtBaseCfg);						//TIM1的PWC模式功能初始化
  
  stcBtPwcInCfg.enTsSel  = BtTs6IAFP;                     //PWC输入选择CHA
  stcBtPwcInCfg.enIA0Sel = BtIA0Input;                    //CHA选择IA0
  stcBtPwcInCfg.enFltIA0 = BtFltPCLKDiv4Cnt3;            //PCLK/4  3个连续有效
  Bt_M1_Input_Config(TIM1, &stcBtPwcInCfg);				//PWC输入设置
  
  Bt_M1_PWC_Edge_Sel(TIM1, BtPwcRiseToFall);				//上升沿到下降沿捕获
 
  Bt_ClearIntFlag(TIM1,BtUevIrq);                		 	//清Uev中断标志
  Bt_ClearIntFlag(TIM1,BtCA0Irq); 						//清捕捉中断标志
  EnableNvic(TIM1_IRQn, IrqLevel2, TRUE); 				//TIM2中断使能       
  Bt_Mode1_EnableIrq(TIM1, BtUevIrq);             		//使能TIM2溢出中断
  Bt_Mode1_EnableIrq(TIM1, BtCA0Irq); 					//使能TIM2捕获中断
  Pwc1_Stop();

  stcBtBaseCfg.pfnTim2Cb  = Tim2Int;                      //中断函数入口                   
  Bt_Mode1_Init(TIM2, &stcBtBaseCfg);						//TIM2的PWC模式功能初始化
  
  Bt_M1_Input_Config(TIM2, &stcBtPwcInCfg);				//PWC输入设置
  
  Bt_M1_PWC_Edge_Sel(TIM2, BtPwcRiseToFall);				//上升沿到下降沿捕获
  
  Bt_ClearIntFlag(TIM2,BtUevIrq);                		 	//清Uev中断标志
  Bt_ClearIntFlag(TIM2,BtCA0Irq); 						//清捕捉中断标志
  
  EnableNvic(TIM2_IRQn, IrqLevel3, TRUE); 				//TIM2中断使能       
  Bt_Mode1_EnableIrq(TIM2, BtUevIrq);             		//使能TIM2溢出中断
  Bt_Mode1_EnableIrq(TIM2, BtCA0Irq); 					//使能TIM2捕获中断
  Pwc2_Stop();
}
/*******************************************************************************
** Function name:           Pwc_Run
** Descriptions:            没什么可说的
** input parameters:        void 
** output parameters:       void
** Returned value:          void    

*******************************************************************************/
void Pwc_Run(void)
{
  Sysctrl_SetPeripheralGate(SysctrlPeripheralBTim, TRUE); //Base Timer外设时钟使能  
  
  Bt_M1_Cnt16Set(TIM1, 0); 						//设置计数初值
  Bt_M1_Run(TIM1);                               //TIM1 运行。
  
  Bt_M1_Cnt16Set(TIM2, 0); 						//设置计数初值
  Bt_M1_Run(TIM2);                                        //TIM2 运行。
} 
/*******************************************************************************
** Function name:           Pwc_Closed
** Descriptions:            关闭PWC
** input parameters:        void 
** output parameters:       void
** Returned value:          void    
*******************************************************************************/
void Pwc2_Stop(void)
{
  Bt_ClearIntFlag(TIM2,BtUevIrq);
  Bt_ClearIntFlag(TIM2, BtCA0Irq);
  Bt_M1_Stop(TIM2);
}

void Pwc1_Stop(void)
{
  Bt_ClearIntFlag(TIM1,BtUevIrq);
  Bt_ClearIntFlag(TIM1, BtCA0Irq);
  Bt_M1_Stop(TIM1);
}



/*******************************************************************************
** Function name:           Timer3
** Descriptions:            定时器Timer3定时模式
** input parameters:        void 
** output parameters:       void
** Returned value:          void    
*******************************************************************************/
void Timer3Init(void)
{
  stc_tim3_mode0_config_t     stcTim3BaseCfg;
  DDL_ZERO_STRUCT(stcTim3BaseCfg);
  
  Sysctrl_SetPeripheralGate(SysctrlPeripheralTim3, TRUE); 
  
  stcTim3BaseCfg.enWorkMode = Tim3WorkMode0;             
  stcTim3BaseCfg.enCT       = Tim3Timer;                 
  stcTim3BaseCfg.enPRS      = Tim3PCLKDiv4;            
  stcTim3BaseCfg.enCntMode  = Tim316bitArrMode;          
  stcTim3BaseCfg.bEnTog     = FALSE;
  stcTim3BaseCfg.bEnGate    = FALSE;
  stcTim3BaseCfg.enGateP    = Tim3GatePositive;
  stcTim3BaseCfg.pfnTim3Cb  = Tim3Int;                    
  
  Tim3_Mode0_Init(&stcTim3BaseCfg);                      
  
  Tim3_ClearIntFlag(Tim3UevIrq);                         
  EnableNvic(TIM3_IRQn, IrqLevel3, TRUE);                 
  Tim3_Mode0_EnableIrq();
  /* Set the Counter Register value */
  uint16_t              u16ArrValue;
  uint16_t              u16CntValue;
  u16CntValue = 65335;
  u16ArrValue = 65035;
  
  Tim3_M0_Cnt16Set(u16CntValue);  
  Tim3_M0_ARRSet(u16ArrValue);                                   
  Tim3_M0_Run();
}

/*******************************************************************************
** Function name:           Timer3_Closed
** Descriptions:            关闭定时器Timer3
** input parameters:        void 
** output parameters:       void
** Returned value:          void    
*******************************************************************************/
void Timer3_Closed(void)
{
  Sysctrl_SetPeripheralGate(SysctrlPeripheralTim3, TRUE); 
  Tim3_ClearIntFlag(Tim3UevIrq); 
  Tim3_Mode0_DisableIrq();
  Tim3_M0_Stop();
  Sysctrl_SetPeripheralGate(SysctrlPeripheralTim3, FALSE);
}

/*****************vcc部分********************
******************vcc部分********************
******************vcc部分********************/
/*******************************************************************************
** Function name:           Vc_Init
** Descriptions:            Vc启动
** input parameters:        void 
** output parameters:       void
** Returned value:          void
*******************************************************************************/
void Vc_Init(void)
{
  stc_vc_channel_config_t stcChannelConfig;
  
  DDL_ZERO_STRUCT(stcChannelConfig);//变量清0
  Sysctrl_SetPeripheralGate(SysctrlPeripheralVcLvd, TRUE);
  
  stcChannelConfig.enVcChannel = VcChannel0;// VC0
  stcChannelConfig.enVcCmpDly  = VcDelayoff;
  stcChannelConfig.enVcFilterTime = VcFilter7us;
  stcChannelConfig.enVcInPin_P = VcInPCh14;//通道选择
  stcChannelConfig.enVcBiasCurrent = VcBias20ua;
  stcChannelConfig.enVcInPin_N = ResDivOut;
  stcChannelConfig.enVcIrqSel = VcIrqNone;
  stcChannelConfig.enVcOutConfig = VcOutDisable;
  Vc_EnableFilter(VcChannel0);
  Vc_ChannelInit(VcChannel0,&stcChannelConfig);
  
  stcChannelConfig.enVcChannel = VcChannel1;//  VC1
  stcChannelConfig.enVcInPin_P = VcInPCh13;//通道选择
  Vc_EnableFilter(VcChannel1);
  Vc_ChannelInit(VcChannel1,&stcChannelConfig);
}
/*******************************************************************************
** Function name:           Vc_Run
** Descriptions:            Vc启动
** input parameters:        void 
** output parameters:       void
** Returned value:          void
*******************************************************************************/
void Vc_Run(void)
{
  stc_vc_dac_config_t stcdacconfig;
  
  DDL_ZERO_STRUCT(stcdacconfig);
  
  Sysctrl_SetPeripheralGate(SysctrlPeripheralVcLvd, TRUE);
  Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr, TRUE);
  
  M0P_BGR->CR_f.BGR_EN = 0x1u;                 //BGR必须使能
  M0P_BGR->CR_f.TS_EN = 0x0u;
  delay10us(5);
  
  Vc_EnableChannel(VcChannel0);
  Vc_EnableChannel(VcChannel1);
  
  stcdacconfig.bDivEn = 1;
  stcdacconfig.enDivVref = VcDivVrefAvcc;
  stcdacconfig.u8DivVal = 31;
  
  Vc_DACInit(&stcdacconfig);
}

/*******************************************************************************
** Function name:           VC_Stop
** Descriptions:            Vc 停止
** input parameters:        void 
** output parameters:       void
** Returned value:          void
*******************************************************************************/
void VC_Stop(void)
{
  M0P_BGR->CR_f.BGR_EN = 0x0u; //BGR必须使能
  M0P_BGR->CR_f.TS_EN = 0x0u;
  delay10us(5);

  Sysctrl_SetPeripheralGate(SysctrlPeripheralVcLvd, FALSE);
  Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr,FALSE);
}


/*****************低功耗配置********************
******************低功耗配置********************
******************低功耗配置********************/

/*************************** 
当进入低功耗模式后，中断执行后返回主程序执行。
****************************/
void LpConfig(void)
{
  //低功耗模式配置
  stc_lpm_config_t stcConfig;
  
  DDL_ZERO_STRUCT(stcConfig);
  
  stcConfig.enSEVONPEND   = SevPndDisable;
  stcConfig.enSLEEPDEEP   = SlpDpEnable;
  stcConfig.enSLEEPONEXIT = SlpExtDisable; //这里是使能后，深度睡眠模式唤醒后，不会返回主程序。
  Lpm_Config(&stcConfig);	
}










/*****************看门狗********************

******************看门狗********************/

void WatchDog_Config(void)
{
  stc_wdt_config_t  stcWdt_Config;
  
  DDL_ZERO_STRUCT(stcWdt_Config);
  
  stcWdt_Config.u8LoadValue = 0x0b;//3.2s
  stcWdt_Config.enResetEnable = WRESET_EN;//WINT_EN;//WRESET_EN;////
  stcWdt_Config.pfnWdtIrqCb = WdtCallback;
  Wdt_Init(&stcWdt_Config);
  Wdt_Start();  
}



/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
