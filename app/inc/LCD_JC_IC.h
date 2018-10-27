#ifndef __LCD_JC_IC_H
#define __LCD_JC_IC_H


#ifdef __cplusplus
extern "C" {
#endif 
   
   
  typedef struct
  {
    uint8_t Menu ;//��ǰ����
    uint8_t Page_0;//�˵�0 ��ת����
    uint8_t Page_1;//�˵�1 ��������ʾ����
    uint8_t Page_2;//�˵�2 ��ʷ����
    uint8_t Page_3;//�˵�3 ��������ʾ����
    uint8_t Page_4;//�˵�4 ��������ʾ����
    uint8_t Page_5;//�˵�5 �춨��ʾ����
    uint8_t Page_6;//�˵�6 ��ʷ������ʾ����
    uint8_t Page_7;//�˵�6 ��ʷ������ʾ����
    uint8_t Page_8;//�˵�6 ��ʷ������ʾ����
  }Str_Lcd_State  ;//�������ݿ�
   
   
extern SCH_TCB LCDTaskTcb;

void LCD_Task(void);
void LCD_Loop(void);
void LCD_Page_0(void);
void LCD_Page_1(void);
void LCD_Alarm_State(void);


   
#ifdef __cplusplus
}
#endif
#endif