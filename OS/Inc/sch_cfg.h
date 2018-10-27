#ifndef __SCH_CFG_H
#define __SCH_CFG_H


//定义数据类型
typedef unsigned char           SCH_UINT8;
typedef unsigned int            SCH_UINT16;
typedef unsigned long int       SCH_UINT32;

//调度器节拍与硬件系统定时器相关定义
#define SCH_SYS_TICKS_MS        (500)  //定义调度系统时钟节拍时间(ms),51单片机速度比较慢,设置为10ms
#define SCH_HW_TIM_MS 	    	  (500) //硬件定时器中断(溢出)周期(ms),此项根据实际系统调整
#define SCH_TIM_TO_TICKS_CMP	  (SCH_UINT8)(SCH_SYS_TICKS_MS/SCH_HW_TIM_MS)	//硬件定时器到系统节拍计数比较值

//定义可裁剪部分
#define SCH_CFG_Q_EN  1u /* 任务内建消息使能 */



#endif      //__SCH_CFG_H

