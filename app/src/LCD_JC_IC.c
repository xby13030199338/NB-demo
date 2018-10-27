#include "include.h"





SCH_TCB LCDTaskTcb;
static Str_Lcd_State LcdState;


/*****************************************************************************************************************************************
��ʾ��������
         
A0������ʾ����                     P5������ʾ����               A3������ʾ����           A2������ʾ����           A1������ʾ����

1 ��XXXXXXXX  KWH                    0�� P---[0]            
2 ��XXXXX.XXX WH                     1�� P---[1]     --������----------------------------------------------------->>   1�����һ�µ�����
3 ��XXXXXX.XX M3   <<---������--->>  2�� P---[2]    --������---------------------------->>  1: DN--20                  2���ϸ��µ�����
4 ��XXXXX.XXX M3/H                   3��P---[3]   --������------>> 1:xxxx.xxxx kwh         2: Err--x                  3����������������
5 ������ˮ�¶�                                                      2:xxx.xxxxx M3          3: ����ʱ�� xxxxxxh 
6 ���²�                                                            3:xxx.xxxx M3/H         4: �汾��   U xx.xx
7 ��XX-xx-xx������                                                  4:����ˮ�¶�            5������
8 : XX-xx-xxʱ����                                                  5:�²� 
9 ���ۼƹ���ʱ��                                                    6:����  ---������--->>  ���Բ˵�
10������                                                                                    ��ǰʱ��� 

���� :
  ʹ��ģʽ�£�
  �ۼ����� DN15---DN25  0.01M3
          DN32---DN400 0.1M3
  �ۼ����� 1kwh  1mwh  1GJ 
  �¶� 0.1��  
    
  �춨ģʽ�£�
  �ۼ����� DN15---DN25  0.00001M3
          DN32---DN100 0.0001M3
         DN125--DN400 0.001M3
  �ۼ����� 0.001kwh �� 0.001MWH
  �¶� 0.01��  

����A3����ʱBT�ж�ʱ���Ϊ125ms����һ�Ρ�
�ڽ���ÿһ������󣨲�����A0��������ʱ���ܣ���ʱ����10���Ӻ��Ҫ�˳���A3�춨���涨ʱ1Сʱ��������A0����ʱҪ�������A0�µļ�ʱʱ�䣬���¿�ʼ
�������ӽ���ʱ ����ͨ���������Ƴ���A0����
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
    case 0:// ��������ʾ����
      {
          LCD_Page_0();
      }
      break;
      
    case 1://��������ʾ����
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

// ��ֵ���Խ���
void LCD_Page_0(void)
{
    /****************************�̰�����������**********************************/ 
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
      case 1: // �ۼ���ˮ��  x.xxx M3
        {
            WMData.Str_WMData.DaySum = 2111;
            Lcd_Dis_Num(WMData.Str_WMData.Sum,3,false);
            SegmentLCD_Char(SET_SUM, true);
            SegmentLCD_Pic(SET_UNITM3, true);
        }
        break;
      case 2: // ������ˮ��  x.xxx M3 
        {  
            Lcd_Dis_Num(WMData.Str_WMData.DaySum, 3, false);
            SegmentLCD_Char(SET_MON, true);
            SegmentLCD_Pic(SET_UNITM3, true);
        }
        break;  
      case 3: // ���½���ö�  x.xxxx kw
        {  
            Lcd_Dis_Num(WMData.Str_WMData.Cash, 2, false);
            SegmentLCD_Char(SET_NOW, true);
            SegmentLCD_Char(SET_UNIT, true);
            SegmentLCD_Char(SET_REMAIN, true);
        }
        break;
      case 4:  //ID�� 
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
      case 5: // ������
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
      case 6:   //ʱ����
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
    if( Get_Button() == Long_Button )//������֮�������һ��״̬
    {
        Set_Button( Null_Button ) ;  
        if (LcdState.Page_0 == 3)//�ڳ�ֵ���泤��
        {
            /*
            ����ͨѶ����
            
            
            */
        }
        else if (LcdState.Page_0 == 4)   //��ID�Ž��泤��
        {
            LcdState.Menu = 1;
            LcdState.Page_1 = 1;
        }
    }
    
    if (LCD_Page0_time++ >= 30) //��ʱ���� 
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
      case 1:   //��ص�ѹ
        {
            Lcd_Dis_Num(Batt_Vol*10, 2, false);
            SegmentLCD_Pic(SET_BATTERY1, true);
            SegmentLCD_Pic(SET_BATTERY2, true);
        }
        break;
      case 2:   //����״̬
        {
            Lcd_Dis_Num(Valve_State, 0, false);
            SegmentLCD_Pic(SET_VALVEFLAG, true);
        }
        break;
      case 3:   //ˮ��״̬
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
    if( Get_Button() == Long_Button || LCD_Page1_time++ >= 30)//����������30s֮���˳��춨״̬
    {
        Set_Button( Null_Button ) ;
        LCD_Page1_time = 0;
        LcdState.Menu = 0;
        LcdState.Page_0 = 1;
    }


}













