#ifndef __SYSINIT_H
#define __SYSINIT_H

void SysClk_Init(void);
/*rtc*/
void RTC_Init(void);

void GPIO_Init(void);
/*pwc*/
void Pwc_Run(void);
void Pwc1_Stop(void);
void Pwc2_Stop(void);
void Pwc_Init(void);
/*Timer3*/
void Tim3Int(void);
void Timer3Init(void);
void Timer3_Closed(void);
/*vcc*/
void Vc_Init(void);
void Vc_Run(void);
void VC_Stop(void);
/*低功耗配置*/
void LpConfig(void);

/*低功耗串口0*/
void Lpuart0_Init(void);
/*低功耗串口1*/
void Lpuart1_Init(void);
/*低功耗定时器*/
void Sys_Lptim_Config(void);
void Sys_Lptim_Disable(void);
/*看门狗*/
void WatchDog_Config(void);
/*液晶屏*/
void LCD_Config(void);
//串口
void Timer0_Init(void);
extern void Timer0Init(void);
#endif





