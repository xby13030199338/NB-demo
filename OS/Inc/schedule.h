/*********************************Copyright(c)*********************************
**                     Electrical Technology Co.,Ltd..
**
**                            http://www.***.com
**
**----------------------------File Info----------------------------------------
** File Name:            schedule.h
** Latest modified Date: 2013-06-15
** Description:          ���Ƶ���������ȡ�
**							 
**-----------------------------------------------------------------------------
** Created By:           QQ 237765062
** Created date:         2013-04-13
** Descriptions:         ����ͳһͨ�ó�����
**
**-----------------------------------------------------------------------------
** Modified by:          QQ:237765062
** Modified date:        2013-06-15
** Description:          ִ��"SubTaskName"ǰ����"pCurTCB->TimeCounter = SCH_TASK_RUN;"
**                       �ȴ���Ϣʱ����"pCurTCB->Size"
** 
**-----------------------------------------------------------------------------
** Modified by:          QQ:237765062
** Modified date:        2013-07-22
** Description:          "g_u8SchedTicksCnt++"��Ϊ�궨�塰SchedTicksInc()�����汾����Ϊ V1.01
** 
**-----------------------------------------------------------------------------
** Modified by:          QQ:237765062
** Modified date:        2014-06-21
** Description:          ��Ϣ����,�ú���ʵ��,�ܿ�Keil��֧�ֺ�����������,�汾����Ϊ V1.03
** 
******************************************************************************/
#ifndef __SCHEDULE_H
#define __SCHEDULE_H

#include "sch_cfg.h"


//U16��ʱ���Ĳ��ܴ���65534,�����������ʱʱ��65534*SCH_SYS_TICKS_MS(1ms)=65.534S
#define SCH_DLY_TYPE            SCH_UINT32

#define SCH_MAX_TASKS      255	 	              //�������������Ϊ255����

#if	SCH_MAX_TASKS   <= 255
#define SCH_MAX_TASK_TYPE       SCH_UINT8             //���������<=255ʱ����Ϊu8
#else
#define SCH_MAX_TASK_TYPE       SCH_UINT16	  //�������Ϊ>255����Ϊu16
#endif

#define SCH_TASK_RUN    0
#define SCH_TASK_PEND   (SCH_DLY_TYPE)0xffffffff

#define SCH_CURR_LINE   (SCH_UINT32)(__LINE__+(!__LINE__))			          //���嵱ǰ�кŵľ���ʵ��
#define SCHTaskBegin()  static SCH_UINT32 SchLc=0; switch(SchLc){ case 0://��ת��ʼ,�м�ɲ�����ʱ,�����Ӻ���;(������/�Ӻ���)
#define SCHTaskEnd()    ;}; SchLc=0; 									                  //��ת����

#define SchedTicksInc() g_u8SchedTicksCnt++

struct SchTcb
{
#if SCH_CFG_Q_EN > 0u
  void          *pData;       //��Ϣָ��
  SCH_UINT32     Size;         //��Ϣ�Ĵ�С
#endif
  SCH_DLY_TYPE	TimeCounter;  //��ʱ������,ʱ��Ϊ "SCH_SYS_TICKS_MS"
  void          (*pTask)();   //����ָ��
  struct SchTcb *pNextTCB;    //��һ��������ƿ�ָ��
};

typedef struct SchTcb SCH_TCB;



//-----------------------------------��������ǰ���񼰵���������------------------------------------------

//������ͣ)��ǰ����,���������� 
#define SCHCurTaskPend() {SchLc=SCH_CURR_LINE;pCurTCB->TimeCounter=SCH_TASK_PEND;}return;case SCH_CURR_LINE:                                                                     

//��ǰ������ʱX��ʱ����ĺ�ָ�
#define SCHCurTaskDly(Ticks) {SchLc=SCH_CURR_LINE;pCurTCB->TimeCounter=Ticks;}return ;case SCH_CURR_LINE:

//�����ڵ���������
#define SCHTaskCallSub(SubTaskName)                                                     \
{                                                                                       \
  {SchLc=SCH_CURR_LINE;pCurTCB->TimeCounter=SCH_TASK_RUN;}return;case SCH_CURR_LINE:    \
    pCurTCB->TimeCounter = SCH_TASK_RUN;                                                  \
      SubTaskName();                                                                        \
        if (pCurTCB->TimeCounter != SCH_TASK_RUN)                                             \
          return ;                                                                            \
}

//----------------------------------����ָ������,������---------------------------------------------------
//������ͣ��ָ������
#define SCHTaskPend(TaskPendTCB)      TaskPendTCB.TimeCounter = SCH_TASK_PEND

//�ָ�ָ���������У�
#define SCHTaskResume(TaskResumeTCB)  TaskResumeTCB.TimeCounter = SCH_TASK_RUN

//ָ��������ʱX��ʱ����ĺ�ָ�
#define SCHTaskDly(TaskDlyTCB, Ticks) TaskDlyTCB.TimeCounter = Ticks

//----------------------------------------��Ϣ-------------------------------------------
#if SCH_CFG_Q_EN > 0u

#define SCH_Q_FREE  1
#define SCH_Q_BUSY  0

//�ȴ���Ϣ
#define SCHTaskQpend() {SchLc=SCH_CURR_LINE;pCurTCB->TimeCounter=SCH_TASK_PEND;pCurTCB->pData=(void *)0;pCurTCB->Size=0;}return;case SCH_CURR_LINE:

//�ͷ���Ϣ
#define SCHTaskQpost(PostTCB, pDat, Len)  PostTCB.pData = pDat; PostTCB.Size = Len; PostTCB.TimeCounter = SCH_TASK_RUN

//��ѯ��Ϣ�ж�״̬,�Ƿ�������(����)��æ(������),����SCHTaskQpend()ʱ�Ὣ������Ϊ����״̬
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