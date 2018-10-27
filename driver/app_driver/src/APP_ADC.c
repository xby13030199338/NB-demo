#include "include.h"



void Disable_ADC(void)
{
  Adc_DeInit();
  Adc_Disable();
  Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr, FALSE); 
}


uint8_t BatterVolCheck(void)
{
  uint16_t AdcTem = 0;
  
  stc_adc_cfg_t              stcAdcCfg;
  stc_adc_irq_t              stcAdcIrq;
  stc_adc_irq_calbakfn_pt_t  stcAdcIrqCalbaks;
  
  DDL_ZERO_STRUCT(stcAdcCfg);
  DDL_ZERO_STRUCT(stcAdcIrq);
  DDL_ZERO_STRUCT(stcAdcIrqCalbaks);
  
  Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr, TRUE); 
  
  Adc_DeInit();
  Adc_Enable();
  M0P_BGR->CR_f.BGR_EN = 0x1u;                
  M0P_BGR->CR_f.TS_EN = 0x0u;
  delay100us(1);
  
  stcAdcCfg.enAdcOpMode 	= AdcSglMode; 			
  stcAdcCfg.enAdcClkDiv 	= AdcClkSysTDiv8;		
  stcAdcCfg.enAdcSampTimeSel 	= AdcSampTime12Clk; 
  stcAdcCfg.enAdcRefVolSel   	= RefVolSelInBgr1p5; 	  
  stcAdcCfg.bAdcInBufEn	        = TRUE;  				
  
  Adc_Init(&stcAdcCfg);
  Adc_ConfigSglMode(&stcAdcCfg);  
  Adc_ConfigSglChannel(AdcAVccDiV3Input);
  Adc_SGL_Start();
  
  delay100us(1);

  Adc_GetSglResult(&AdcTem);
  Adc_SGL_Stop();
  Disable_ADC();
  
  AdcTem = AdcTem * 11;
  AdcTem = AdcTem / 1000;
  
  return( AdcTem );
}


uint16_t Valve_Current_Measure(void)
{
  uint32_t AdcTem = 0;
  uint16_t AdcVal = 0;
  stc_adc_cfg_t              stcAdcCfg;
  stc_adc_irq_t              stcAdcIrq;
  stc_adc_irq_calbakfn_pt_t  stcAdcIrqCalbaks;
  
  DDL_ZERO_STRUCT(stcAdcCfg);
  DDL_ZERO_STRUCT(stcAdcIrq);
  DDL_ZERO_STRUCT(stcAdcIrqCalbaks);
  
  Sysctrl_SetPeripheralGate(SysctrlPeripheralAdcBgr, TRUE); 

  Adc_Enable();
  M0P_BGR->CR_f.BGR_EN = 0x1u;                
  M0P_BGR->CR_f.TS_EN = 0x0u;
  delay100us(1);
  
  stcAdcCfg.enAdcOpMode 		= AdcSglMode; 			
  stcAdcCfg.enAdcClkDiv 		= AdcClkSysTDiv8;		
  stcAdcCfg.enAdcSampTimeSel 	        = AdcSampTime12Clk; 
  stcAdcCfg.enAdcRefVolSel   	        = RefVolSelInBgr1p5; 	  
  stcAdcCfg.bAdcInBufEn			= TRUE;  				
  
  Adc_Init(&stcAdcCfg);
  Adc_ConfigSglMode(&stcAdcCfg);  
  Adc_ConfigSglChannel(AdcExInputCH10);
  Adc_SGL_Start();
  
  delay100us(1);

  Adc_GetSglResult(&AdcVal);
  Adc_SGL_Stop();
  Disable_ADC();
  
  AdcTem = AdcVal;
  
  AdcTem = AdcTem * 779;
  AdcTem = AdcTem / 1000;

  return(AdcTem);
}





