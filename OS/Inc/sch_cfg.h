#ifndef __SCH_CFG_H
#define __SCH_CFG_H


//������������
typedef unsigned char           SCH_UINT8;
typedef unsigned int            SCH_UINT16;
typedef unsigned long int       SCH_UINT32;

//������������Ӳ��ϵͳ��ʱ����ض���
#define SCH_SYS_TICKS_MS        (500)  //�������ϵͳʱ�ӽ���ʱ��(ms),51��Ƭ���ٶȱȽ���,����Ϊ10ms
#define SCH_HW_TIM_MS 	    	  (500) //Ӳ����ʱ���ж�(���)����(ms),�������ʵ��ϵͳ����
#define SCH_TIM_TO_TICKS_CMP	  (SCH_UINT8)(SCH_SYS_TICKS_MS/SCH_HW_TIM_MS)	//Ӳ����ʱ����ϵͳ���ļ����Ƚ�ֵ

//����ɲü�����
#define SCH_CFG_Q_EN  1u /* �����ڽ���Ϣʹ�� */



#endif      //__SCH_CFG_H
