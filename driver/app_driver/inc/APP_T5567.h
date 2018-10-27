#ifndef __APP_T5567_H
#define __APP_T5567_H

#include "include.h"

#ifdef __cplusplus
extern "C" {
#endif 
  
/*软件延时时间定义*/
////系统MCLK主时钟为1M，一个指令周期为1us,可根据单片机主频调以下延时
//定时器时钟为2M，一个指令周期为1/2us,可根据单片机主频调以下延时
//定时器时钟为1M，一个指令周期为1us,可根据单片机主频调以下延时
#define START_TIME          280*1                    //起始延时时间         ---280us 
#define SEND_ONE_TIME       432*1                    //发送 "  1  "时间     ---432us  //54个 
#define SEND_ZERO_TIME      192*1                    //发送 "  0  "时间     ---192us   // 24个
#define SEND_GAP_TIME       100*1                   //发送 " gap "时间     ---100us
#define STOP_TIME           8000*1                   //结束延时时间         ---6mS

#define READ_15P_TIME       310*1                   //读取1.5p时间         ---310us
#define READ_10P_TIME       278*1                   //读取1.0p时间         ---278us
#define READ_05P_TIME       176*1                   //读取0.5p时间         ---176us 
#define READ_TIME_OUT       0x2000*1               //读取超时时间

#define bool _Bool
#define true 1
#define false 0 
 
  
  
  
  
void Enable_Timer(void);
void TIM3_SetCounter(uint16_t Coun);
uint16_t TIM3_GetCounter(void);
void Rf_Cfe_Clr(void);
void Rf_Cfe_Set(void);
void Rf_Power_En(void);
void Close_125K(void);
void Open_125K(void);
void InitRF(void);
void Read_T5567_Card(unsigned char *buf);
void Write_T5567_Card(unsigned char *buf);
unsigned char RFInitCard(unsigned char *p_buf);
void CloseRF(void);

  #ifdef __cplusplus
}
#endif
#endif





