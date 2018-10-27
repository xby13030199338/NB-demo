#include "include.h"
 


/**********************************************
  设置RTC数值
  
  Str_Calendar rtc;

  rtc.DayOfMonth = 0x20; rtc.Hours = 0x23 ;
  rtc.Minutes = 0x59;rtc.Month = 0x10;rtc.Seconds = 0x58;rtc.Year=0x18;
  
  RTC_Init_Calendar(rtc);
  ******************************************/
void RTC_Init_Calendar(Str_Calendar dat)
{
  stc_rtc_time_t  stcTime;

  DDL_ZERO_STRUCT(stcTime);

  stcTime.u8Year        = (uint8_t)dat.Year;
  stcTime.u8Month       = (uint8_t)dat.Month;
  stcTime.u8Day         = (uint8_t)dat.DayOfMonth;
  stcTime.u8Hour        = (uint8_t)dat.Hours;
  stcTime.u8Minute      = (uint8_t)dat.Minutes;
  stcTime.u8Second      = (uint8_t)dat.Seconds;
  stcTime.u8DayOfWeek   = Rtc_CalWeek(&stcTime.u8Day);
  
  Rtc_WriteDateTime(&stcTime,TRUE,TRUE);

}
/**********************************************
  读取RTC数值
  static Str_Calendar dat;
  RTC_Get_Calendar(&dat);
  ******************************************/
void RTC_Get_Calendar(Str_Calendar *rtc)
{
  stc_rtc_time_t tem;
  
  Rtc_ReadDateTime(&tem);
 
  rtc->Hours    = tem.u8Hour;
  rtc->Minutes  = tem.u8Minute;
  rtc->Seconds  = tem.u8Second;
  rtc->Year     = tem.u8Year;
  rtc->Month    = tem.u8Month;
  rtc->DayOfMonth = tem.u8Day;
  rtc->DayOfWeek  = tem.u8DayOfWeek;
}