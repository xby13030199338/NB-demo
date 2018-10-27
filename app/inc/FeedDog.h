#ifndef __FEEDDOG_H
#define __FEEDDOG_H

#include "schedule.h"

#ifdef __cplusplus
extern "C" {
#endif 
  
#define WatchDogCyc 3 

extern SCH_TCB WatchDogTaskTcb;
extern uint8_t WatchDogCount;

void IWDG_Init(void);
void IWDG_Config(void);
void Feed_Watch_Dog(void);
void Watch_Dog_Task(void);






  #ifdef __cplusplus
}
#endif
#endif
