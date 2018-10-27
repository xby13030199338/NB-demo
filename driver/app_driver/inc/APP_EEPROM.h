#ifndef __APP_EEPROM_H
#define __APP_EEPROM_H


#ifdef __cplusplus
extern "C" {
#endif 

 
#define DeviceAddrLow   0xA0  //小于等于 0xFFFF 时的器件地址        
#define DeviceAddrHigh  0xA2  //大于     0xFFFF 时的器件地址 

bool M24C16_Array_Write(uint16_t addr,uint8_t *buff,uint8_t lengh);
bool M24C16_Array_Read(uint16_t addr,uint8_t *buff,uint8_t lengh);
  
  





  
    #ifdef __cplusplus
}
#endif
#endif



