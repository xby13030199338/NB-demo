#ifndef __APP_RTC_H
#define __APP_RTC_H

#ifdef __cplusplus
extern "C" {
#endif 

   
  typedef struct Calendar
  {
    uint16_t Seconds;//! Seconds of minute between 0-59
    uint16_t Minutes;//! Minutes of hour between 0-59
    uint16_t Hours;//! Hour of day between 0-23
    uint16_t DayOfWeek;//! Day of week between 0-6
    uint16_t DayOfMonth;//! Day of month between 1-31
    uint16_t Month;//! Month between 0-11
    uint16_t Year;//! Year between 0-4095
  } Str_Calendar;
void RTC_Init_Calendar(Str_Calendar dat);
void RTC_Get_Calendar(Str_Calendar *rtc);
#ifdef __cplusplus
}
#endif
#endif