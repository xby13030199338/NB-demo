#ifndef __APP_EEPROM_H
#define __APP_EEPROM_H


#ifdef __cplusplus
extern "C" {
#endif 

 
#define DeviceAddrLow   0xA0  //С�ڵ��� 0xFFFF ʱ��������ַ        
#define DeviceAddrHigh  0xA2  //����     0xFFFF ʱ��������ַ 

bool M24C16_Array_Write(uint16_t addr,uint8_t *buff,uint8_t lengh);
bool M24C16_Array_Read(uint16_t addr,uint8_t *buff,uint8_t lengh);
  
  





  
    #ifdef __cplusplus
}
#endif
#endif


