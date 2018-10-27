#ifndef __FlowMeasurement_H
#define __FlowMeasurement_H

#ifdef __cplusplus
extern "C" {
#endif 

   
#define LLIO_One_Port GpioPortD
#define LLIO_One_Pin  GpioPin4
  
#define LLIO_Two_Port GpioPortD
#define LLIO_Two_Pin  GpioPin5 
  
extern SCH_TCB FluxMeasureTcb;
  
  
void Flux_Measure_Task(void);
void Flux_Measure(void);
void Accounting_Management(void);
uint32_t  Multistep_Fee( uint32_t dat);
  
  
  
  
  
  
  
#ifdef __cplusplus
}
#endif
#endif