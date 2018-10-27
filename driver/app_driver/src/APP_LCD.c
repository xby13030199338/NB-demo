#include "include.h"



const MCU_DISPLAY EFM_Display = {
    .Char = {
        .com[0] = 0, .bit[0] = 0,   /*请购水*/
        .com[1] = 0, .bit[1] = 1,   /*单价*/
        .com[2] = 1, .bit[2] = 0,   /*故障*/
        .com[3] = 1, .bit[3] = 1,   /*当前*/
        .com[4] = 2, .bit[4] = 1,   /*表号*/
        .com[5] = 3, .bit[5] = 0,   /*元*/
        .com[6] = 3, .bit[6] = 1,   /*报警*/
        .com[7] = 3, .bit[7] = 4,   /*关阀*/
        .com[8] = 3, .bit[8] = 10,   /*干扰*/
        .com[9] = 3, .bit[9] = 12,   /*剩余*/
        .com[10] = 3, .bit[10] = 14,   /*购买*/
        .com[11] = 3, .bit[11] = 16,   /*累计*/
        .com[12] = 3, .bit[12] = 18,   /*当月*/
    },
    .Number = {
        {   /*第一位*/
            .com[0] = 0, .com[1] = 1, .com[2] = 2, .com[3] = 3,
            .bit[0] = 3, .bit[1] = 3, .bit[2] = 3, .bit[3] = 3,
            
            .com[4] = 2, .com[5] = 0, .com[6] = 1,
            .bit[4] = 4, .bit[5] = 4, .bit[6] = 4,
        },
        
        {   /*第二位*/
            .com[0] = 0, .com[1] = 1, .com[2] = 2, .com[3] = 3,
            .bit[0] = 5, .bit[1] = 5, .bit[2] = 5, .bit[3] = 5,
            
            .com[4] = 2, .com[5] = 0, .com[6] = 1,
            .bit[4] = 6, .bit[5] = 6, .bit[6] = 6,
        },
        
        {   /*第三位*/
            .com[0] = 0, .com[1] = 1, .com[2] = 2, .com[3] = 3,
            .bit[0] = 7, .bit[1] = 7, .bit[2] = 7, .bit[3] = 7,
            
            .com[4] = 2, .com[5] = 0, .com[6] = 1,
            .bit[4] = 8, .bit[5] = 8, .bit[6] = 8,
        },
        
        {   /*第四位*/
            .com[0] = 0, .com[1] = 1, .com[2] = 2, .com[3] = 3,
            .bit[0] = 9, .bit[1] = 9, .bit[2] = 9, .bit[3] = 9,
            
            .com[4] = 2, .com[5] = 0, .com[6] = 1,
            .bit[4] = 10, .bit[5] = 10, .bit[6] = 10,
        },
        
        {   /*第五位*/
            .com[0] = 0, .com[1] = 1, .com[2] = 2, .com[3] = 3,
            .bit[0] = 11, .bit[1] = 11, .bit[2] = 11, .bit[3] = 11,
            
            .com[4] = 2, .com[5] = 0, .com[6] = 1,
            .bit[4] = 12, .bit[5] = 12, .bit[6] = 12,
        },
        
        {   /*第六位*/
            .com[0] = 0, .com[1] = 1, .com[2] = 2, .com[3] = 3,
            .bit[0] = 13, .bit[1] = 13, .bit[2] = 13, .bit[3] = 13,
            
            .com[4] = 2, .com[5] = 0, .com[6] = 1,
            .bit[4] = 14, .bit[5] = 14, .bit[6] = 14,
        },
        
        {   /*第七位*/
            .com[0] = 0, .com[1] = 1, .com[2] = 2, .com[3] = 3,
            .bit[0] = 15, .bit[1] = 15, .bit[2] = 15, .bit[3] = 15,
            
            .com[4] = 2, .com[5] = 0, .com[6] = 1,
            .bit[4] = 16, .bit[5] = 16, .bit[6] = 16,
        },        
        
        {   /*第八位*/
            .com[0] = 0, .com[1] = 1, .com[2] = 2, .com[3] = 3,
            .bit[0] = 17, .bit[1] = 17, .bit[2] = 17, .bit[3] = 17,
            
            .com[4] = 2, .com[5] = 0, .com[6] = 1,
            .bit[4] = 18, .bit[5] = 18, .bit[6] = 18,
        },
    },
    
    .Pic = {
        .com[0] = 2, .bit[0] = 0,   /*P1 阀门标志*/
        .com[1] = 0, .bit[1] = 2,   /*P2 :  :*/
        .com[2] = 1, .bit[2] = 2,   /*P3 电池标志【   】*/
        .com[3] = 2, .bit[3] = 2,   /*P4 电池标志 ||||*/
        .com[4] = 3, .bit[4] = 2,   /*P5 m3*/
        .com[5] = 3, .bit[5] = 6,   /*P6 第二位小数点.*/
        .com[6] = 3, .bit[6] = 8,   /*P7 第三位小数点.*/
    },
};

const uint16_t EFM_Numbers[] =
{
  0x003f, /* 0 */
  0x0006, /* 1 */
  0x005b, /* 2 */
  0x004f, /* 3 */
  0x0066, /* 4 */
  0x006d, /* 5 */
  0x007d, /* 6 */
  0x0007, /* 7 */
  0x007f, /* 8 */
  0x006f, /* 9 */
  0x0040, /* - */
};

void SegmentLCD_Number(int value)
{
  int num, i, com, bit, digit, div, neg;
  uint16_t bitpattern;
  
  /* Parameter consistancy check */
  if (value >= 99999999)
  {
    value = 99999999;
  }
  if (value <= -10000000)
  {
    value = -9999999;
  }
  if (value < 0)
  {
    value = abs(value);
    neg   = 1;
  }
  else
  {
    neg = 0;
  }
  
  div = 1;
  LCD_ClearDisp();
  for (digit = 0; digit < 8; digit++)
  {
    num = (value / div) % 10;
    if ((neg == 1) && (digit == 7)) num = 10;
    /* Get number layout of display */
    bitpattern = EFM_Numbers[num];
    for (i = 0; i < 7; i++)
    {
      bit = EFM_Display.Number[digit].bit[i];
      com = EFM_Display.Number[digit].com[i];
      if (bitpattern & (1 << i))
      {
        LCD_WriteRam(com, bit, true);
      }
    }
    div = div * 10;
  }
}



void SegmentLCD_Char(DISPLAY_CHA cnum, bool on)
{
    uint16_t com, bit;
    com = EFM_Display.Char.com[cnum];
    bit = EFM_Display.Char.bit[cnum];
    
    if (on)
    {
        LCD_WriteRam(com, bit, true);
    }
    
    else 
    {
        LCD_WriteRam(com, bit, false);
    }
}

void SegmentLCD_Pic(DISPLAY_PIC cnum, bool on)
{
  uint16_t com, bit;
  com = EFM_Display.Pic.com[cnum];
  bit = EFM_Display.Pic.bit[cnum];
  
  if (on)
  {
    LCD_WriteRam(com, bit, true);
  }
  
  else 
  {
    LCD_WriteRam(com, bit, false);
  } 
}


/**********************************************************************

功能：在LCD上任意位置显示一个数字
入口参数：digit 要显示的数字 
          location 要显示的位置 从右向左 7 <------ 0
    Lcd_Displaydigit(0,0);
    Lcd_Displaydigit(1,1);
    Lcd_Displaydigit(2,2);
    Lcd_Displaydigit(3,3);
    Lcd_Displaydigit(4,4);
    Lcd_Displaydigit(5,5);
    Lcd_Displaydigit(6,6);
    Lcd_Displaydigit(7,7);
    
*********************************************************************/
void Lcd_Displaydigit(uint8_t num,uint8_t location)
{
  uint16_t bitpattern;
  uint16_t com, bit;

  bitpattern = EFM_Numbers[num];
  for (uint8_t i = 0; i < 7; i++)
  {
    bit = EFM_Display.Number[location].bit[i];
    com = EFM_Display.Number[location].com[i];
    if (bitpattern & (1 << i))
    {
      LCD_WriteRam(com, bit, true);
    }
  }
}

/**********************************************************************

功能：在LCD上任意位置显示数字
入口参数：num 要显示的数字 
          location 要显示的位置 从右向左 7 <------ 0
    
*********************************************************************/


void LCD_Dis_num(uint32_t num, uint8_t location)
{
    uint8_t dispbuff[8];
    int i;
    

    do 
    {
        dispbuff[i] = num % 10;
        i ++;
        num /= 10;
        
    }while (num > 0);
    
    for (i = i-1; i >= 0; i--)
    {
        Lcd_Displaydigit(dispbuff[i],location+i);
    }
    
    i = 0;
}

/*******************************************************************************
** Function name:           Lcd_Dis_Num
** Descriptions:            液晶显示程序，可以完成显示数字，显示小数，显示负数
** input parameters:        num------->显示的数字，输入整数型数字
                            dp-------->小数点位置，从左向右 1  2  3  4  5
                            neg------->是否显示负数，大于0是显示，等于零时不显示
** output parameters:       void
** Returned value:          void 
** Created by:              CHENG WEI XI
** Created Date:            2013-7-1
**------------------------------------------------------------------------------
** Modified by:             数字从左到右  对应寄存器 LCD4(LCD[3]) LCD5(LCD[4]) LCD6(LCD[5]) ---- LCD11(LCD[10])         
** Modified date:           小数点从左到右 对应      1 2 3 4 5 
*******************************************************************************/
void Lcd_Dis_Num(unsigned long int num,unsigned char dp,bool neg)
{
  unsigned char dispbuff[8];
  unsigned char i;
  
  LCD_ClearDisp();
   
  //分解整数
  for(unsigned char i=0;i<8;i++)
  {
    dispbuff[i] = num % 10;
    num /= 10;
  }
  //清除不需要显示位
 for( i=8;i>0;i--)
  {
    if(dispbuff[i-1]==0) 
    {
      dispbuff[i-1]=255;
      if(i==1)
      {
        dispbuff[i-1]=0;
      }
    }
    else  
      break;
  } 
  //显示小数
  if(dp>0)
  {
    switch (dp)
    {
    case 2:
        SegmentLCD_Pic(SET_POINT2,true);
      break;
    case 3:
        SegmentLCD_Pic(SET_POINT3,true);
      break;
    default:
      break;
    }
    for( i=dp;i>0;i--)
    {
      if(dispbuff[i]==255) 
        dispbuff[i]=0;
      else  
        break;
    }
  }
  for( i=0;i<8;i++)
  {
    if(dispbuff[i]!= 255)  
       Lcd_Displaydigit(dispbuff[i],i);
     else 
       break;
  }
    //显示负数
  if(neg>0)
  {
    for(i=0;i<8;i++)
    {   
      if(dispbuff[i]==255) 
      { 
        Lcd_Displaydigit(10,i);
        break;
      }
    }
  }
}









