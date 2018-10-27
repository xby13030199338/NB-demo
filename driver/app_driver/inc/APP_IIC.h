#ifndef __APP_IIC_H
#define __APP_IIC_H
#include "include.h"  

#ifdef __cplusplus
extern "C" {
#endif 
  
 
#define SDA_PORT        GpioPortB
#define SDA_PIN         GpioPin4                                       
#define SCL_PORT        GpioPortB
#define SCL_PIN         GpioPin5 
#define WP_PORT         GpioPortB
#define WP_PIN          GpioPin6 
#define VCC_PORT        GpioPortB
#define VCC_PIN         GpioPin7 



  
  
 
  
  uint8_t EEPROM_ReadSDA(void);
  void Eeprom_Wp_En(void);
  void Eeprom_Wp_Ds(void);
  void Eeprom_Vcc_En(void);
  void Eeprom_Vcc_Ds(void);
  void EEPROM_SCL_HIGH(void);
  void EEPROM_SCL_LOW(void);
  void EEPROM_SDA_HIGH(void);
  void EEPROM_SDA_LOW(void);
  void EEPROM_Start_IIc(void);
  void EEPROM_Stop_IIc(void);
  bool EEPROM_SendByte(uint8_t WriteData);
  uint8_t EEPROM_RcvByte(void);
  void EEPROM_Ack_IIc(const bool ack);
  bool EEPROM_IIC_Send_Byte(uint8_t sla,uint8_t WriteData);
  bool EEPROM_IIC_Rcv_Byte(uint8_t sla,uint8_t *RcvData);
  void EEPROM_IIC_Send_Str(uint8_t sla,uint8_t suba,uint8_t *buf,uint8_t len);
  void EEPROM_IIC_Rcv_Str(uint8_t sla,uint8_t suba,uint8_t *buf,uint8_t len);
  
  
  
  
  
 
#endif
















