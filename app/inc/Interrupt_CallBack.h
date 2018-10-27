#ifndef __Interrupt_CallBack_H
#define __Interrupt_CallBack_H

 
#ifdef __cplusplus
extern "C"
{
#endif


void RtcCycCb(void);
void RtcAlarmCb(void);


void Tim0Int(void);

void TxIntCallback(void);
void RxIntCallback(void);
void ErrIntCallback(void);
void PErrIntCallBack(void);
void CtsIntCallBack(void);
void AdcContIrqCallback(void);
void LpTimIntCallback(void);
void WdtCallback(void);
void Tim1Int(void);
void Tim2Int(void);
void Tim3Int(void);








#endif





