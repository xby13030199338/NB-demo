
#ifndef __Sleep_H
#define __Sleep_H

#ifdef __cplusplus
extern "C" {
#endif 
  
#include "schedule.h"
  

extern SCH_TCB DeepSleepTaskTcb;


/*******************************************************************************
** Function name:           Sleep_Task
** Descriptions:            系统休眠任务 每个调度周期执行一次 
** input parameters:        void 
** output parameters:       void
** Returned value:          void 
** Created by:              程卫玺
** Created Date:            2016-5-3   
*******************************************************************************/
void Sleep_Task(void);
void Deep_Sleep_Task(void);






#ifdef __cplusplus
}
#endif
#endif


