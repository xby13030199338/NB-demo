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

/* ������Ĵ��� */
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


/* ���񴴽�,����ʱ�������񴴽�ʧ�ܴ����öϵ� */
void SCHTaskCreate(SCH_TCB       *pNewTCB          ,
                   void         (*pNewTask)(void)  ,
		   SCH_DLY_TYPE   TimeCounterInit   )
{
  if (TaskNumberSum == 0)
  {
    pFirstTCB = pNewTCB;                    //���ݵ�һ��������ƿ��ַ
    pNewTCB->pTask = pNewTask;              //�´�����������ƿ�ĺ���ָ��ָ���´���������
    pCurTCB   = pNewTCB;                    //����������ƿ��ַ����ǰ������ƿ�ָ��
    pCurTCB->pNextTCB = pCurTCB;            //��Ϊֻ��һ������,����ָ�����һ��������ƿ��ַ�����Լ�
    pCurTCB->TimeCounter = TimeCounterInit;
  }
  else if (TaskNumberSum < SCH_MAX_TASKS)
  {
    pNewTCB->pTask = pNewTask;              //�´�����������ƿ�ĺ���ָ��ָ���´���������
    pNewTCB->pNextTCB = pCurTCB->pNextTCB;  //��ǰ������ƿ�ָ�����һ��������ƿ����½���������ƿ���ָ��
    pCurTCB->pNextTCB = pNewTCB;            //��ǰ������ƿ�ָ�����һ��������ƿ����Ϊ�½�������
    pCurTCB = pNewTCB;                      //�½���������ƿ����Ϊ��ǰ������ƿ�
    pCurTCB->TimeCounter = TimeCounterInit;
  }
  else
  {
    TaskNumberSum--;                        //���񴴽�ʧ��,����ʱ���ڴ˷��öϵ�
  }
  
  TaskNumberSum++;
#if SCH_CFG_Q_EN > 0u
  pNewTCB->pData    = (void *)0;
  pNewTCB->Size = 0;
#endif
}


void SCHTaskSchedStart(void)
{
  pCurTCB = pFirstTCB;                        //ָ���һ������������,֮�󰴴���ʱ��˳��ִ����ȥ
  
  while (1)                                   //��������,����һֱѭ����ȥ
  {
    SCHTimeTick();                            //�������Tick��������,�������ڿ�ִ��״̬
    
    if (SCH_TASK_RUN == pCurTCB->TimeCounter) //�����ڿ�ִ��״̬
    {
      pCurTCB->TimeCounter = SCH_TASK_PEND;   //����Ϊ����״̬,��֤����ִֻ��һ��
      pCurTCB->pTask();                       //ִ�е�ǰ������ƿ�ָ�������
      pCurTCB = pFirstTCB;                    //ÿִ����һ������,���ص�������²���һ�ο�ִ��������ȼ�����
    }
    else
    {
      pCurTCB = pCurTCB->pNextTCB;            //ָ����һ��������ƿ�,�����¸������Ƿ��ִ��
    }
  }
}

