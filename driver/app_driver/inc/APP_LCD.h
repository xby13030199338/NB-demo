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
    SET_BUYWATER = 0,       //�빺ˮ
    SET_PRICE = 1,              //����
    SET_ERROR = 2,          //����
    SET_NOW = 3,                //��ǰ
    SET_METERID = 4,        //����
    SET_UNIT = 5,               //Ԫ
    SET_ALARM = 6,          //����
    SET_CLOSEVALVE = 7,         //�ط�
    SET_INTERFERE = 8,      //����
    SET_REMAIN = 9,             //ʣ��
    SET_BUY = 10,           //����
    SET_SUM = 11,               //�ۼ�
    SET_MON = 12,           //����
}DISPLAY_CHA;

typedef enum
{
    SET_VALVEFLAG       = 0,    //���ű�־
    SET_POINT1          = 1,    //�� ��
    SET_BATTERY1        = 2,    //��ر�־��    ��
    SET_BATTERY2        = 3,    //��ر�־  ||||
    SET_UNITM3          = 4,    //������λm3
    SET_POINT2          = 5,    //�ڶ�λС����
    SET_POINT3          = 6,    //����λС����
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