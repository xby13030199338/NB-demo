#include "include.h"

/**

if(num++ % 2)
{
Gpio_SetIO(GpioPortA, GpioPin5);//  Motor_Close_H();
    }
    else
{
Gpio_ClrIO(GpioPortA, GpioPin5);//  Motor_Open_L();
    }

**/

void Sys_Task_Create(void)
{
  /**********************按键任务*****************************/
//  SCHTaskCreate(&CommTaskTcb,    Comm_Task,               SCH_TASK_PEND ) ;
  /**********************按键任务*****************************/
  SCHTaskCreate(&ButtonTaskTcb,  Button_Task,           SCH_TASK_PEND) ;
  /**********************蜂鸣器播放任务********************* **/
  SCHTaskCreate(&BuzzerTaskTcb,  Buzzer_Task ,          SCH_TASK_PEND) ;//SCH_TASK_PEND
    /**********************LCD任务*****************************/
  SCHTaskCreate(&LCDTaskTcb,     LCD_Task,              SCH_TASK_PEND) ;
  /**********************流量任务*****************************/
  SCHTaskCreate(&FluxMeasureTcb, Flux_Measure_Task ,    0 * SCH_SYS_TICKS_MS);
  /**********************蜂鸣器播放任务***********************/
  SCHTaskCreate(&ValveTaskTcb,   Valve_Task ,           0 * SCH_SYS_TICKS_MS);//SCH_TASK_PEND
  /**********************看门狗任务**************************/
  SCHTaskCreate(&WatchDogTaskTcb,Watch_Dog_Task,        0 * SCH_SYS_TICKS_MS);
  /**********************休眠任务****************************/
//  SCHTaskCreate(&DeepSleepTaskTcb,Deep_Sleep_Task,0 * SCH_SYS_TICKS_MS);
}

Str_Com lpuarttx;                                                                    
uint8_t tem[]={0x68,0x78,0x56,0x34,0x12,0x00,0x00,0x68,0x01,0x02,0x34,0x23,0x3E,0x16};

int main(void)
{
    //  delay1ms(1000);
    //  delay1ms(1000);
    //  delay1ms(1000); 
    SysClk_Init();
    GPIO_Init();
    RTC_Init();
    Lpuart0_Init();
    LCD_Config();
    Sys_Software_Init();
    //  WatchDog_Config();
    /*********任务创建**********/
    
    
    EquipmentID[0]=0x78;EquipmentID[1]=0x56;EquipmentID[2]=0x34;
    EquipmentID[3]=0x12;EquipmentID[4]=0x00;EquipmentID[5]=0x00;
    Resolve_Node_645(&tem[0],sizeof(tem),&lpuarttx);
    Str_Calendar rtc;
    
    rtc.DayOfMonth = 0x20; rtc.Hours = 0x23 ;
    rtc.Minutes = 0x50;rtc.Month = 0x10;rtc.Seconds = 0x50;rtc.Year=0x18;
    
    RTC_Init_Calendar(rtc);
    
    Sys_Task_Create();
    /*********任务调度**********/
    SCHTaskSchedStart();
}





