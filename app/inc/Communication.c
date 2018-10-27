#include "include.h"



SCH_TCB CommTaskTcb;




//
//
//void Comm_Task(void)
//{
//  static uint16_t CommTaskDelay = 0xffff;
//  
//  SCHTaskBegin();
//  
//  while (1)
//  {
//    CommTaskDelay = Comm();
//    if(ButtonTaskDelay == 0xffff)
//    {
//      SCHCurTaskPend();
//    }
//    else
//    {
//      SCHCurTaskDly( 500 / SCH_HW_TIM_MS );  //delay 250ms 
//    }
//  }
//  
//  SCHTaskEnd();
//}
//
//uint16_t Comm(void)
//{
//  uint16_t delay = 0xffff;
//  
//  switch()
//  {
//  case :
//    {}
//    break;
//  case :
//    {}
//    break;
//  case :
//    {}
//    break;
//  default:
//    break;
//  }
//
//}











