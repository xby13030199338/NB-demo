#include "include.h"





SCH_TCB LCDTaskTcb;
static Str_Lcd_State LcdState;


/*****************************************************************************************************************************************
显示程序任务
         
A0界面显示内容                     P5界面显示内容               A3界面显示内容           A2界面显示内容           A1界面显示内容

1 ：XXXXXXXX  KWH                    0： P---[0]            
2 ：XXXXX.XXX WH                     1： P---[1]     --长按键----------------------------------------------------->>   1：最近一月的数据
3 ：XXXXXX.XX M3   <<---长按键--->>  2： P---[2]    --长按键---------------------------->>  1: DN--20                  2：上个月的数据
4 ：XXXXX.XXX M3/H                   3：P---[3]   --长按键------>> 1:xxxx.xxxx kwh         2: Err--x                  3：。。。。。。。
5 ：进会水温度                                                      2:xxx.xxxxx M3          3: 故障时间 xxxxxxh 
6 ：温差                                                            3:xxx.xxxx M3/H         4: 版本号   U xx.xx
7 ：XX-xx-xx年月日                                                  4:进会水温度            5：表号
8 : XX-xx-xx时分秒                                                  5:温差 
9 ：累计工作时间                                                    6:表号  ---长按键--->>  调试菜单
10：表号                                                                                    当前时间差 

国标 :
  使用模式下：
  累计流量 DN15---DN25  0.01M3
          DN32---DN400 0.1M3
  累计热量 1kwh  1mwh  1GJ 
  温度 0.1℃  
    
  检定模式下：
  累计流量 DN15---DN25  0.00001M3
          DN32---DN100 0.0001M3
         DN125--DN400 0.001M3
  累计热量 0.001kwh 或 0.001MWH
  温度 0.01℃  

进入A3界面时BT中断时间改为125ms测量一次。
在进入每一个界面后（不包括A0），开计时功能，计时满足10分钟后就要退出，A3检定界面定时1小时。但进入A0界面时要首先清除A0下的计时时间，重新开始
当进入子界面时 可以通过长按键推出到A0界面
*******************************************************************************************************************************************/
void LCD_Task(void)
{
  SCHTaskBegin();
  
  while (1)
  {
    LCD_Loop(); 
    SCHCurTaskDly( 1000 / SCH_HW_TIM_MS );  //delay 250ms 
  }
  
  SCHTaskEnd();
}


void LCD_Loop(void)
{
  switch(LcdState.Menu)
  {
    case 0:// 表数据显示界面
      {
          LCD_Page_0();
      }
      break;
      
    case 1://表资料显示界面
      {
          LCD_Page_1();
      }
      break;
      
      
      
    default:
      break;
  }
  Batt_Vol = 25;
  if(Batt_Vol <= 26)
  {
      LCD_Alarm_State();
  }
  

}

void LCD_Alarm_State(void)
{
    static uint8_t num = 0;
    if(num++ % 2)
    {
        SegmentLCD_Pic(SET_BATTERY1, true);
    }
    else 
    {
        SegmentLCD_Pic(SET_BATTERY1, false);
    }

    if (num == 255)
    {
        num = 0;
    }
}

// 数值轮显界面
void LCD_Page_0(void)
{
    /****************************短按键处理部分**********************************/ 
    uint32_t tem = 0;
    static uint8_t LCD_Page0_time = 0;
    Str_Calendar rtc;
    
    if(Get_Button() == Short_Button)
    {
        LCD_Page0_time = 0;
        Set_Button( Null_Button ) ; 
        if(++LcdState.Page_0 >= 7)
        {
            LcdState.Page_0 = 1 ; 
        }
    }
    LCD_ClearDisp();
    
    switch(LcdState.Page_0)
    {
      case 1: // 累计用水量  x.xxx M3
        {
            WMData.Str_WMData.DaySum = 2111;
            Lcd_Dis_Num(WMData.Str_WMData.Sum,3,false);
            SegmentLCD_Char(SET_SUM, true);
            SegmentLCD_Pic(SET_UNITM3, true);
        }
        break;
      case 2: // 当月用水量  x.xxx M3 
        {  
            Lcd_Dis_Num(WMData.Str_WMData.DaySum, 3, false);
            SegmentLCD_Char(SET_MON, true);
            SegmentLCD_Pic(SET_UNITM3, true);
        }
        break;  
      case 3: // 当月金额用度  x.xxxx kw
        {  
            Lcd_Dis_Num(WMData.Str_WMData.Cash, 2, false);
            SegmentLCD_Char(SET_NOW, true);
            SegmentLCD_Char(SET_UNIT, true);
            SegmentLCD_Char(SET_REMAIN, true);
        }
        break;
      case 4:  //ID号 
        {
            Lcd_Displaydigit(EquipmentID[0] & 0x0f, 7);
            Lcd_Displaydigit((EquipmentID[0] & 0xf0) >> 4, 6);
            
            Lcd_Displaydigit(EquipmentID[1] & 0x0f, 5);
            Lcd_Displaydigit((EquipmentID[1] & 0xf0) >> 4, 4);
            
            Lcd_Displaydigit(EquipmentID[2] & 0x0f, 3);
            Lcd_Displaydigit((EquipmentID[2] & 0xf0) >> 4, 2);
            
            Lcd_Displaydigit(EquipmentID[3] & 0x0f, 1);
            Lcd_Displaydigit((EquipmentID[3] & 0xf0) >> 4, 0);
            
            SegmentLCD_Char(SET_METERID, true);
        }
        break;
      case 5: // 年月日
        {
            RTC_Get_Calendar(&rtc);
            
            LCD_Dis_num(Bcd_To_Bin_Bit32(rtc.Year), 6);
            if (Bcd_To_Bin_Bit32(rtc.Year) < 10)
                LCD_Dis_num(0, 7);
            
            Lcd_Displaydigit(10,5);
            
            LCD_Dis_num(Bcd_To_Bin_Bit32(rtc.Month), 3);
            if (Bcd_To_Bin_Bit32(rtc.Month) < 10)
                LCD_Dis_num(0, 4);
            
            Lcd_Displaydigit(10,2);
            
            LCD_Dis_num(Bcd_To_Bin_Bit32(rtc.DayOfMonth), 0);
            if (Bcd_To_Bin_Bit32(rtc.DayOfMonth) < 10)
                LCD_Dis_num(0, 1);
        }
        break;
      case 6:   //时分秒
        {
            RTC_Get_Calendar(&rtc);
            
            LCD_Dis_num(Bcd_To_Bin_Bit32(rtc.Hours), 6);
            if (Bcd_To_Bin_Bit32(rtc.Hours) < 10)
            {
                LCD_Dis_num(0, 7);
            }
            
            LCD_Dis_num(Bcd_To_Bin_Bit32(rtc.Minutes), 3);
            if (Bcd_To_Bin_Bit32(rtc.Minutes) < 10)
            {
                LCD_Dis_num(0, 4);
            }
            
            LCD_Dis_num(Bcd_To_Bin_Bit32(rtc.Seconds), 0);
            if (Bcd_To_Bin_Bit32(rtc.Seconds) < 10)
            {
                LCD_Dis_num(0, 1);
            }
            SegmentLCD_Pic(SET_POINT1, true);
        }
        break;
    }
    if( Get_Button() == Long_Button )//长按键之后进入下一个状态
    {
        Set_Button( Null_Button ) ;  
        if (LcdState.Page_0 == 3)//在充值界面长按
        {
            /*
            调用通讯程序
            
            
            */
        }
        else if (LcdState.Page_0 == 4)   //在ID号界面长按
        {
            LcdState.Menu = 1;
            LcdState.Page_1 = 1;
        }
    }
    
    if (LCD_Page0_time++ >= 30) //超时清屏 
    {
        LCD_Page0_time = 0;
        LcdState.Page_0 = 0;
        LCD_ClearDisp();
    }

}

void LCD_Page_1 (void)
{
    static uint8_t LCD_Page1_time = 0;
    
    
    if(Get_Button() == Short_Button)
    {
        LCD_Page1_time = 0;
        Set_Button( Null_Button ) ; 
        if(++LcdState.Page_1 >= 5)
        {
            LcdState.Page_1 = 1; 
        }
    }
    LCD_ClearDisp();
    
    switch (LcdState.Page_1)
    {
      case 1:   //电池电压
        {
            Lcd_Dis_Num(Batt_Vol*10, 2, false);
            SegmentLCD_Pic(SET_BATTERY1, true);
            SegmentLCD_Pic(SET_BATTERY2, true);
        }
        break;
      case 2:   //阀门状态
        {
            Lcd_Dis_Num(Valve_State, 0, false);
            SegmentLCD_Pic(SET_VALVEFLAG, true);
        }
        break;
      case 3:   //水表状态
        {
            
        }
        break;
      case 4:
        {
            Lcd_Displaydigit(EquipmentID[0] & 0x0f, 7);
            Lcd_Displaydigit((EquipmentID[0] & 0xf0) >> 4, 6);
            
            Lcd_Displaydigit(EquipmentID[1] & 0x0f, 5);
            Lcd_Displaydigit((EquipmentID[1] & 0xf0) >> 4, 4);
            
            Lcd_Displaydigit(EquipmentID[2] & 0x0f, 3);
            Lcd_Displaydigit((EquipmentID[2] & 0xf0) >> 4, 2);
            
            Lcd_Displaydigit(EquipmentID[3] & 0x0f, 1);
            Lcd_Displaydigit((EquipmentID[3] & 0xf0) >> 4, 0);
            
            SegmentLCD_Char(SET_METERID, true);
        }
        break;
    }
    if( Get_Button() == Long_Button || LCD_Page1_time++ >= 30)//长按键或者30s之后退出检定状态
    {
        Set_Button( Null_Button ) ;
        LCD_Page1_time = 0;
        LcdState.Menu = 0;
        LcdState.Page_0 = 1;
    }


}














