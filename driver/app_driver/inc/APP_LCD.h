#ifndef __APP_LCD_H
#define __APP_LCD_H


#ifdef __cplusplus
extern "C" {
#endif 
   
   

typedef struct
{
  uint8_t com[13]; /**< LCD COM line (for multiplexing) */
  uint8_t bit[13]; /**< LCD bit number */
} CHAR_TypeDef;

typedef struct
{
  uint8_t com[7];
  uint8_t bit[7];
} NUMBER_TypeDef;

typedef struct
{
  uint8_t com[7];
  uint8_t bit[7];
} PICTURE_TypeDef;
    
typedef struct
{
    CHAR_TypeDef Char;
    NUMBER_TypeDef  Number[8];
    PICTURE_TypeDef Pic;
} MCU_DISPLAY;

typedef enum
{
    SET_BUYWATER = 0,       //请购水
    SET_PRICE = 1,              //单价
    SET_ERROR = 2,          //故障
    SET_NOW = 3,                //当前
    SET_METERID = 4,        //表号
    SET_UNIT = 5,               //元
    SET_ALARM = 6,          //报警
    SET_CLOSEVALVE = 7,         //关阀
    SET_INTERFERE = 8,      //干扰
    SET_REMAIN = 9,             //剩余
    SET_BUY = 10,           //购买
    SET_SUM = 11,               //累计
    SET_MON = 12,           //当月
}DISPLAY_CHA;

typedef enum
{
    SET_VALVEFLAG       = 0,    //阀门标志
    SET_POINT1          = 1,    //： ：
    SET_BATTERY1        = 2,    //电池标志【    】
    SET_BATTERY2        = 3,    //电池标志  ||||
    SET_UNITM3          = 4,    //立方单位m3
    SET_POINT2          = 5,    //第二位小数点
    SET_POINT3          = 6,    //第三位小数点
}DISPLAY_PIC;

void SegmentLCD_Number(int value);
void SegmentLCD_Pic(DISPLAY_PIC cnum, bool on);
void SegmentLCD_Char(DISPLAY_CHA cnum, bool on);
void Lcd_Displaydigit(uint8_t num,uint8_t location);
void Lcd_Dis_Num(unsigned long int num,unsigned char dp,bool neg);
void LCD_Dis_num(uint32_t num, uint8_t location);   
   
   
   
#ifdef __cplusplus
}
#endif
#endif