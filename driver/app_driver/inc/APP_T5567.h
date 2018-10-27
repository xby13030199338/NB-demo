#ifndef __APP_T5567_H
#define __APP_T5567_H

#include "include.h"

#ifdef __cplusplus
extern "C" {
#endif 
  
/*������ʱʱ�䶨��*/
////ϵͳMCLK��ʱ��Ϊ1M��һ��ָ������Ϊ1us,�ɸ��ݵ�Ƭ����Ƶ��������ʱ
//��ʱ��ʱ��Ϊ2M��һ��ָ������Ϊ1/2us,�ɸ��ݵ�Ƭ����Ƶ��������ʱ
//��ʱ��ʱ��Ϊ1M��һ��ָ������Ϊ1us,�ɸ��ݵ�Ƭ����Ƶ��������ʱ
#define START_TIME          280*1                    //��ʼ��ʱʱ��         ---280us 
#define SEND_ONE_TIME       432*1                    //���� "  1  "ʱ��     ---432us  //54�� 
#define SEND_ZERO_TIME      192*1                    //���� "  0  "ʱ��     ---192us   // 24��
#define SEND_GAP_TIME       100*1                   //���� " gap "ʱ��     ---100us
#define STOP_TIME           8000*1                   //������ʱʱ��         ---6mS

#define READ_15P_TIME       310*1                   //��ȡ1.5pʱ��         ---310us
#define READ_10P_TIME       278*1                   //��ȡ1.0pʱ��         ---278us
#define READ_05P_TIME       176*1                   //��ȡ0.5pʱ��         ---176us 
#define READ_TIME_OUT       0x2000*1               //��ȡ��ʱʱ��

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




