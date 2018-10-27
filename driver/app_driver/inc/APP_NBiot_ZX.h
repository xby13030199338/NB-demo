#ifndef __APP_NBiot_ZX_H //�������ģ�� ��������
#define __APP_NBiot_ZX_H

#ifdef __cplusplus
extern "C" {
#endif 
 
#define NB_Iot_BT_ID	0//������ʱ��
#define NB_Iot_LT_ID	1//���ڶ�ʱ��

#define AT_CR		'\r'  //\r ���ո�
#define AT_LF		'\n'  //\n ���س�
  
 
 

/* ͨ�� ATI ָ����Բ�ѯģ���Ӳ����Ϣ 
ATI
Manufacture: HUAWEI
Model: SIM800
Revision: 12.210.10.05.00
IMEI: 351869045435933
+GCAP: +CGSM

OK
*/
  typedef struct
  {
    char Manfacture[12];	/* ���� SIMCOM_Ltd */
    char Model[12];			/* �ͺ� SIM800 */
    char Revision[15 + 1];	/* �̼��汾 R13.08 */
    char IMEI[15 + 1];		/* IMEI �� ��Ҫͨ��AT+GSN��� */
  }SIM800_INFO_T;

 
  
//#define NB_Iot_Reset_En()       (GPIOA->BSRR     |= BIT5)
//#define NB_Iot_Reset_Ds()       (GPIOA->BRR      |= BIT5)
  
#define NB_Iot_Reset_En()       (GPIOB->BSRR     |= BIT2)
#define NB_Iot_Reset_Ds()       (GPIOB->BRR      |= BIT2)
 
 
  
extern uint32_t NB_Iot_State;
extern const char AT_NMGS[];
extern const char AT_SETIMEI[];


uint8_t  NB_iot_WaitResponse(char *reply,uint16_t TimeOut);
uint8_t  NB_Iot_Get_Nband(void);
uint8_t  NB_Iot_Set_Nband(uint8_t nband);
 
void     NB_iot_SendAT(char *_Cmd);
uint8_t  NB_Iot_Reset(void);
uint16_t NB_iot_ReadResponse(char *_pBuf, uint16_t _usBufSize, uint16_t _usTimeOut);
uint8_t  NB_Iot_Signal_Power(void); 
void Hex_To_String(uint8_t *buf , char *str,uint8_t len,uint8_t stroffset);

void rxtest(void);


void NB_Iot_Get_IMEI(uint8_t *imei);
uint8_t NB_Iot_Set_IMEI(uint8_t *imei);
uint8_t NB_Iot_PowerDown(void);
uint8_t NB_Iot_Set_IP(void);


uint8_t NB_Iot_Activate_Network(void);
uint8_t NB_Iot_Tx(uint8_t *dat,uint8_t len);
uint8_t String_To_Hex(uint8_t *dat);




#ifdef __cplusplus
}
#endif
#endif