#ifndef __Protocol_H
#define __Protocol_H
#include "include.h"

#ifdef __cplusplus
extern "C" {
#endif 
typedef union
{
  uint8_t dat[30] ;
  struct
  { 
	uint8_t                   Validity        ;//有效性
	uint8_t                   ControlCode     ;//控制码        
	uint8_t                   Buf[28]         ;
  }Str_Protocol_MSG_Q ;                   //polling 任务消息队列
}Union_Protocol_MSG_Q ;
void Resolve_Protocol_Frame(uint8_t *dat,uint8_t len,Union_Protocol_MSG_Q *msg);
void Combine_Protocol_Frame(Union_Protocol_MSG_Q *msg);
void T5557_Task(void);
extern SCH_TCB T5557TaskTcb;
#ifdef __cplusplus
}
#endif
#endif
