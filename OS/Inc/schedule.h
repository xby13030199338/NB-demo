/*********************************Copyright(c)*********************************
**                     Electrical Technology Co.,Ltd..
**
**                            http://www.***.com
**
**----------------------------File Info----------------------------------------
** File Name:            schedule.h
** Latest modified Date: 2013-06-15
** Description:          完善调用子任务等。
**							 
**-----------------------------------------------------------------------------
** Created By:           QQ 237765062
** Created date:         2013-04-13
** Descriptions:         建立统一通用程序框架
**
**-----------------------------------------------------------------------------
** Modified by:          QQ:237765062
** Modified date:        2013-06-15
** Description:          执行"SubTaskName"前加入"pCurTCB->TimeCounter = SCH_TASK_RUN;"
**                       等待消息时清零"pCurTCB->Size"
** 
**-----------------------------------------------------------------------------
** Modified by:          QQ:237765062
** Modified date:        2013-07-22
** Description:          "g_u8SchedTicksCnt++"改为宏定义“SchedTicksInc()”，版本升级为 V1.01
** 
**-----------------------------------------------------------------------------
** Modified by:          QQ:237765062
** Modified date:        2014-06-21
** Description:          消息部分,用宏来实现,避开Keil不支持函数重入问题,版本升级为 V1.03
** 
******************************************************************************/
#ifndef __SCHEDULE_H
#define __SCHEDULE_H

#include "sch_cfg.h"


//U16延时节拍不能大于65534,即任务最大延时时间65534*SCH_SYS_TICKS_MS(1ms)=65.534S
#define SCH_DLY_TYPE            SCH_UINT32

#define SCH_MAX_TASKS      255	 	              //任务数量，最大为255个。

#if	SCH_MAX_TASKS   <= 255
#define SCH_MAX_TASK_TYPE       SCH_UINT8             //最大任务数<=255时定义为u8
#else
#define SCH_MAX_TASK_TYPE       SCH_UINT16	  //最大任务为>255则定义为u16
#endif

#define SCH_TASK_RUN    0
#define SCH_TASK_PEND   (SCH_DLY_TYPE)0xffffffff

#define SCH_CURR_LINE   (SCH_UINT32)(__LINE__+(!__LINE__))			          //定义当前行号的具体实现
#define SCHTaskBegin()  static SCH_UINT32 SchLc=0; switch(SchLc){ case 0://跳转开始,中间可插入延时,调用子函数;(适用主/子函数)
#define SCHTaskEnd()    ;}; SchLc=0; 									                  //跳转结束

#define SchedTicksInc() g_u8SchedTicksCnt++

struct SchTcb
{
#if SCH_CFG_Q_EN > 0u
  void          *pData;       //消息指针
  SCH_UINT32     Size;         //消息的大小
#endif
  SCH_DLY_TYPE	TimeCounter;  //定时计数器,时基为 "SCH_SYS_TICKS_MS"
  void          (*pTask)();   //任务指针
  struct SchTcb *pNextTCB;    //下一个任务控制块指针
};

typedef struct SchTcb SCH_TCB;



//-----------------------------------操作作当前任务及调用子任务------------------------------------------

//挂起（暂停)当前任务,即任务自身 
#define SCHCurTaskPend() {SchLc=SCH_CURR_LINE;pCurTCB->TimeCounter=SCH_TASK_PEND;}return;case SCH_CURR_LINE:                                                                     

//当前任务延时X个时间节拍后恢复
#define SCHCurTaskDly(Ticks) {SchLc=SCH_CURR_LINE;pCurTCB->TimeCounter=Ticks;}return ;case SCH_CURR_LINE:

//任务内调用子任务
#define SCHTaskCallSub(SubTaskName)                                                     \
{                                                                                       \
  {SchLc=SCH_CURR_LINE;pCurTCB->TimeCounter=SCH_TASK_RUN;}return;case SCH_CURR_LINE:    \
    pCurTCB->TimeCounter = SCH_TASK_RUN;                                                  \
      SubTaskName();                                                                        \
        if (pCurTCB->TimeCounter != SCH_TASK_RUN)                                             \
          return ;                                                                            \
}

//----------------------------------操作指定任务,不常用---------------------------------------------------
//挂起（暂停）指定任务
#define SCHTaskPend(TaskPendTCB)      TaskPendTCB.TimeCounter = SCH_TASK_PEND

//恢复指定任务（运行）
#define SCHTaskResume(TaskResumeTCB)  TaskResumeTCB.TimeCounter = SCH_TASK_RUN

//指定任务延时X个时间节拍后恢复
#define SCHTaskDly(TaskDlyTCB, Ticks) TaskDlyTCB.TimeCounter = Ticks

//----------------------------------------消息-------------------------------------------
#if SCH_CFG_Q_EN > 0u

#define SCH_Q_FREE  1
#define SCH_Q_BUSY  0

//等待消息
#define SCHTaskQpend() {SchLc=SCH_CURR_LINE;pCurTCB->TimeCounter=SCH_TASK_PEND;pCurTCB->pData=(void *)0;pCurTCB->Size=0;}return;case SCH_CURR_LINE:

//释放消息
#define SCHTaskQpost(PostTCB, pDat, Len)  PostTCB.pData = pDat; PostTCB.Size = Len; PostTCB.TimeCounter = SCH_TASK_RUN

//查询消息列队状态,是否是自由(可用)或忙(不可用),调用SCHTaskQpend()时会将其设置为自由状态
#define SCHTaskGetQFree(TaskTCB, RetStatus)  RetStatus = SCH_Q_BUSY; if (TaskTCB.TimeCounter == SCH_TASK_PEND){RetStatus = SCH_Q_FREE;}
#endif


extern SCH_UINT32 g_u8SchedTicksCnt;
extern SCH_TCB   *pCurTCB;


extern void SCHTimeTick(void);

extern void SCHTaskSchedStart(void);

extern void SCHTaskCreate(SCH_TCB       *pNewTCB,
                          void          (*pNewTask)(void),
                          SCH_DLY_TYPE   TimeCounterInit);


#endif	//__SCHEDULE_H
