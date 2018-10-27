#ifndef __APP_Lpuart0_H
#define __APP_Lpuart0_H

#ifdef __cplusplus
extern "C" {
#endif 

  
//typedef struct 
//{
//  volatile uint8_t lock;
//  uint8_t len;
//  uint8_t buf[128];
//}Str_Com;
void Lpuart0_Q_Init(void);
uint8_t Lpuart0_Rx_Cpy(char *dest);
void Lpuart0_Rx_Init(void);
void Lpuart0_Tx_Init(void);
void Padding_Lpuart0_Tx_Buf(uint8_t *buf,uint8_t len);
void Enable_Lpuart0_Tx(void);

uint8_t Get_Lpuart0_RxBuf_Num(void);
uint8_t Get_Lpuart0_Rx_Data(void);
void Lpuart0_Tx(void);
void Lpuart0_Rx(uint8_t val);




#ifdef __cplusplus
}
#endif
#endif