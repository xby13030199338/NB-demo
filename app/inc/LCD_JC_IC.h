#ifndef __LCD_JC_IC_H
#define __LCD_JC_IC_H


#ifdef __cplusplus
extern "C" {
#endif 
   
   
  typedef struct
  {
    uint8_t Menu ;//当前流速
    uint8_t Page_0;//菜单0 跳转界面
    uint8_t Page_1;//菜单1 表数据显示界面
    uint8_t Page_2;//菜单2 历史数据
    uint8_t Page_3;//菜单3 表参数显示界面
    uint8_t Page_4;//菜单4 表故障显示界面
    uint8_t Page_5;//菜单5 检定显示界面
    uint8_t Page_6;//菜单6 历史数据显示界面
    uint8_t Page_7;//菜单6 历史数据显示界面
    uint8_t Page_8;//菜单6 历史数据显示界面
  }Str_Lcd_State  ;//运行数据块
   
   
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