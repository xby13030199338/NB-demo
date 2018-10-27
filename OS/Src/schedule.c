/*********************************Copyright(c)*********************************
**                     Electrical Technology Co.,Ltd..
**
**                       http://www.***.com
**
******************************************************************************/

#include "schedule.h"

SCH_TCB *pFirstTCB, *pCurTCB;

static SCH_MAX_TASK_TYPE TaskNumberSum      = 0;
SCH_UINT32        g_u8SchedTicksCnt  = 0;

/* 任务节拍处理 */
void SCHTimeTick(void)
{
  SCH_MAX_TASK_TYPE i;
  
  SCH_TCB *pTCB;
  
  if (g_u8SchedTicksCnt >= SCH_TIM_TO_TICKS_CMP)
  {
    g_u8SchedTicksCnt -= SCH_TIM_TO_TICKS_CMP;
    
    pTCB = pFirstTCB;
    for (i = 0; i < TaskNumberSum; i++)
    {
      if ((pTCB->TimeCounter != SCH_TASK_PEND) && (pTCB->TimeCounter > 0))
      {
        pTCB->TimeCounter--;
      }
      
      pTCB = pTCB->pNextTCB;
    }
  }
}


/* 任务创建,调试时可在任务创建失败处放置断点 */
void SCHTaskCreate(SCH_TCB       *pNewTCB          ,
                   void         (*pNewTask)(void)  ,
		   SCH_DLY_TYPE   TimeCounterInit   )
{
  if (TaskNumberSum == 0)
  {
    pFirstTCB = pNewTCB;                    //备份第一个任务控制块地址
    pNewTCB->pTask = pNewTask;              //新创建的任务控制块的函数指针指向新创建的任务
    pCurTCB   = pNewTCB;                    //最新任务控制块地址给当前任务控制块指针
    pCurTCB->pNextTCB = pCurTCB;            //因为只有一个任务,所以指令的下一个任务控制块地址就是自己
    pCurTCB->TimeCounter = TimeCounterInit;
  }
  else if (TaskNumberSum < SCH_MAX_TASKS)
  {
    pNewTCB->pTask = pNewTask;              //新创建的任务控制块的函数指针指向新创建的任务
    pNewTCB->pNextTCB = pCurTCB->pNextTCB;  //当前任务控制块指向的下一个任务控制块由新建的任务控制块来指向
    pCurTCB->pNextTCB = pNewTCB;            //当前任务控制块指向的下一个任务控制块更新为新建的任务
    pCurTCB = pNewTCB;                      //新建的任务控制块更新为当前任务控制块
    pCurTCB->TimeCounter = TimeCounterInit;
  }
  else
  {
    TaskNumberSum--;                        //任务创建失败,调试时可在此放置断点
  }
  
  TaskNumberSum++;
#if SCH_CFG_Q_EN > 0u
  pNewTCB->pData    = (void *)0;
  pNewTCB->Size = 0;
#endif
}


void SCHTaskSchedStart(void)
{
  pCurTCB = pFirstTCB;                        //指向第一个创建的任务,之后按创建时的顺序执行下去
  
  while (1)                                   //环形链表,可以一直循环下去
  {
    SCHTimeTick();                            //如果任务Tick满足条件,则将其置于可执行状态
    
    if (SCH_TASK_RUN == pCurTCB->TimeCounter) //任务处于可执行状态
    {
      pCurTCB->TimeCounter = SCH_TASK_PEND;   //设置为挂起状态,保证任务只执行一次
      pCurTCB->pTask();                       //执行当前任务控制块指向的任务
      pCurTCB = pFirstTCB;                    //每执行完一个任务,都回到起点重新查找一次可执行最高优先级任务
    }
    else
    {
      pCurTCB = pCurTCB->pNextTCB;            //指向下一个任务控制块,查找下个任务是否可执行
    }
  }
}


