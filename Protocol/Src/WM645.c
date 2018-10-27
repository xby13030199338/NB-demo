#include "include.h"

//解析节点645程序
bool Resolve_Node_645(uint8_t *spoint,uint8_t len,Str_Com *tx)
{
  bool flag = false;
  uint8_t buf[128];
  uint8_t offset;
  
  memcpy(&buf[0],&*spoint,len);//接收无线的数据帧 
  
  if(len==0 || len>=128)
    return(false);
  
  for(offset=0;offset<len;offset++)
  {
    if((buf[offset] == 0x68) && (buf[buf[offset + 9] + 11 + offset] == 0x16))
    {
      if(memcmp(&EquipmentID[0],&buf[offset+1],6)==0 || memcmp(&BroadcastId[0],&buf[offset+1],6)==0 )
      {
        if(Count_CheckSum(&buf[offset],(buf[offset+9]+10+offset)) == buf[buf[offset+9]+10+offset])
        { 
          __no_operation();
            switch(buf[8 + offset])  //控制码
            {
            case 0x01://控制码  读操作
              {
                switch(buf[11 + offset])
                {
//                case 0xF3://阀门部分
//                  {
//                    switch(buf[6] )
//                    {
//                    case 0x34://读取运行数据
//                      {
//                        Str_Calendar rtc;
//                        
//                        RTC_Get_Calendar(&rtc);
//                        tx->len = 0;
//                        memcpy(&tx->buf[tx->len],&buf[0],4);//表ID
//                        tx->len++;tx->len++;tx->len++;tx->len++;
//                        tx->buf[tx->len++] = 0x81;//组合645数据帧
//                        tx->buf[tx->len++] = 0x16; 
//                        tx->buf[tx->len++] = 0x34;
//                        tx->buf[tx->len++] = 0xF3; 
//                        
//                        for(uint8_t i=0;i<11;i++)
//                        {
//                          tx->buf[tx->len++] = Valve_Dat.Buf[i] + 0x33;
//                        }
//                        tx->buf[tx->len++] = Batt_Vol + 0x33;
//                        
//                        tx->buf[tx->len++] = rtc.Seconds + 0x33;
//                        tx->buf[tx->len++] = rtc.Minutes + 0x33;
//                        tx->buf[tx->len++] = rtc.Hours + 0x33;
//                        tx->buf[tx->len++] = rtc.DayOfMonth + 0x33;
//                        tx->buf[tx->len++] = rtc.Month + 0x33;
//                        tx->buf[tx->len++] = rtc.Year + 0x33;
//                        tx->buf[tx->len++] = SysState & 0x0F + 0x33;
//                        tx->buf[tx->len++] = SysState & 0xF0 + 0x33;
//                      }
//                      break;
//                      
//                    default:
//                      {
//                      }
//                      break;
//                    }
//                  }
//                  break; 
                case 0x23://水表部分
                  {
                    switch(buf[10 + offset] )
                    {
                    case 0x34://读取运行数据
                      {
                        Str_Calendar rtc;
                        
                        RTC_Get_Calendar(&rtc);
                        tx->len = 0;
                        tx->buf[tx->len++] = 0x68;//组合645数据帧
                        memcpy(&tx->buf[tx->len],&EquipmentID[0],6);//表ID
                        tx->len++;tx->len++;tx->len++;
                        tx->len++;tx->len++;tx->len++;
                        tx->buf[tx->len++] = 0x68;//组合645数据帧
                        tx->buf[tx->len++] = 0x81;//组合645数据帧
                        tx->buf[tx->len++] = 0x17; 
                        tx->buf[tx->len++] = 0x34;
                        tx->buf[tx->len++] = 0x23; 
                        
                        for(uint8_t i=0;i<12;i++)
                        {
                          tx->buf[tx->len++] = WMData.Buf[i] + 0x33;
                        }
                        tx->buf[tx->len++] = Batt_Vol + 0x33;
                        
                        tx->buf[tx->len++] = rtc.Seconds + 0x33;
                        tx->buf[tx->len++] = rtc.Minutes + 0x33;
                        tx->buf[tx->len++] = rtc.Hours   + 0x33;
                        tx->buf[tx->len++] = rtc.DayOfMonth + 0x33;
                        tx->buf[tx->len++] = rtc.Month   + 0x33;
                        tx->buf[tx->len++] = rtc.Year    + 0x33;
                        tx->buf[tx->len++] = SysState    & 0x0F + 0x33;
                        tx->buf[tx->len++] = SysState    & 0xF0 + 0x33;
  
                        tx->buf[tx->len  ] = Count_CheckSum(&(tx->buf[0]),tx->len);
                        tx->len++;
                        tx->buf[tx->len++] = 0x16;

                      }
                      break;
                    case 0x36://读取充值记录
                      {
                        tx->len = 0;
                        memcpy(&tx->buf[tx->len],&buf[0],4);//表ID
                        tx->len++;tx->len++;tx->len++;tx->len++;
                        tx->buf[tx->len++] = 0x81;//组合645数据帧
                        tx->buf[tx->len++] = 0x08; 
                        tx->buf[tx->len++] = 0x36;
                        tx->buf[tx->len++] = 0x23; 
                        
                        for(uint8_t i=0;i<6;i++)
                        {
                          tx->buf[tx->len++] = RechargeRecord.Buf[i] + 0x33;
                        }
          
                        tx->buf[tx->len  ] = Count_CheckSum(&(tx->buf[0]),tx->len);
                        tx->len++;
                        tx->buf[tx->len++] = 0x16;
                      }
                      break;
                    case 0x58://读取当前水价
                      {
                        tx->len = 0;
                        memcpy(&tx->buf[tx->len],&buf[0],4);//表ID
                        tx->len++;tx->len++;tx->len++;tx->len++;
                        tx->buf[tx->len++] = 0x81;//组合645数据帧
                        tx->buf[tx->len++] = 0x11; 
                        tx->buf[tx->len++] = 0x58;
                        tx->buf[tx->len++] = 0x23; 
                        
                        for(uint8_t i=0;i<15;i++)
                        {
                          tx->buf[tx->len++] = MultiRate.Buf[i] + 0x33;
                        }
                        tx->buf[tx->len  ] = Count_CheckSum(&(tx->buf[0]),tx->len);
                        tx->len++;
                        tx->buf[tx->len++] = 0x16;
                      }
                      break;
                    case 0x59://读取新水价
                      {
                        tx->len = 0;
                        memcpy(&tx->buf[tx->len],&buf[0],4);//表ID
                        tx->len++;tx->len++;tx->len++;tx->len++;
                        tx->buf[tx->len++] = 0x81;//组合645数据帧
                        tx->buf[tx->len++] = 0x11; 
                        tx->buf[tx->len++] = 0x59;
                        tx->buf[tx->len++] = 0x23; 
                        
                        for(uint8_t i=0;i<15;i++)
                        {
                          tx->buf[tx->len++] = NewMultiRate.Buf[i] + 0x33;
                        }
                        tx->buf[tx->len  ] = Count_CheckSum(&(tx->buf[0]),tx->len);
                        tx->len++;
                        tx->buf[tx->len++] = 0x16;
                      }
                      break;
                    default:
                      {
                      }
                      break;
                    }
                  }
                  break;
                }
              }
              break;
            case 0x04://控制码  写操作
              {    
                if( memcmp(&PassWord[0],&buf[8],4)==0 )
                {
                  switch(buf[7])
                  {
//                  case 0xF3://阀门部分
//                    {
//                      switch(buf[6] )
//                      {
//                      case 0x43://读取运行数据
//                        {
//                          if(buf[18] == 0x88)//开阀门
//                          { 
//                            tx->len = 0;
//                            memcpy(&tx->buf[tx->len],&buf[0],4);//表ID
//                            tx->len++;tx->len++;tx->len++;tx->len++;
//                            tx->buf[tx->len++] = 0x84;//组合645数据帧
//                            tx->buf[tx->len++] = 0x04; 
//                            tx->buf[tx->len++] = 0x43;
//                            tx->buf[tx->len++] = 0xF3; 
//                            tx->buf[tx->len++] = 0x88; 
//                            tx->buf[tx->len++] = 0x00;
//                            
//                            Valve_Dat.Str_Valve_Dat.Angle = buf[17] - 0x33;
//                            Valve_Dat.Str_Valve_Dat.Vclose.Year = buf[16]- 0x33;
//                            Valve_Dat.Str_Valve_Dat.Vclose.Month = buf[15]- 0x33;
//                            Valve_Dat.Str_Valve_Dat.Vclose.Day = buf[14]- 0x33;
//                            Valve_Dat.Str_Valve_Dat.Vclose.Hours = buf[13]- 0x33;
//                            Valve_Dat.Str_Valve_Dat.Vclose.Minutes = buf[12]- 0x33;
//                            //启动阀门任务
//                          }
//                          else if(buf[12] == 0xCC)
//                          {
//                            tx->len = 0;
//                            memcpy(&tx->buf[tx->len],&buf[0],4);//表ID
//                            tx->len++;tx->len++;tx->len++;tx->len++;
//                            tx->buf[tx->len++] = 0x84;//组合645数据帧
//                            tx->buf[tx->len++] = 0x04; 
//                            tx->buf[tx->len++] = 0x43;
//                            tx->buf[tx->len++] = 0xF3; 
//                            tx->buf[tx->len++] = 0xCC; 
//                            tx->buf[tx->len++] = 0x00;
//                            //启动阀门任务
//                          }
//                        }
//                        break;
//                      default:
//                        {
//                        }
//                        break;
//                      }
//                    }
//                    break; 
                  case 0x23:
                    {
                      switch(buf[6] )
                      {
                      case 0x43://开关阀门
                        {
                          if(buf[13] == 0x88)//开阀门
                          { 
                            tx->len = 0;
                            memcpy(&tx->buf[tx->len],&buf[0],4);//表ID
                            tx->len++;tx->len++;tx->len++;tx->len++;
                            tx->buf[tx->len++] = 0x84;//组合645数据帧
                            tx->buf[tx->len++] = 0x04; 
                            tx->buf[tx->len++] = 0x43;
                            tx->buf[tx->len++] = 0x23; 
                            tx->buf[tx->len++] = 0x88; 
                            tx->buf[tx->len++] = 0x00;//正确应答
                            
                            tx->buf[tx->len  ] = Count_CheckSum(&(tx->buf[0]),tx->len);
                            tx->len++;
                            tx->buf[tx->len++] = 0x16;
                            //启动阀门任务
                          }
                          else if(buf[12] == 0xCC)
                          {
                            tx->len = 0;
                            memcpy(&tx->buf[tx->len],&buf[0],4);//表ID
                            tx->len++;tx->len++;tx->len++;tx->len++;
                            tx->buf[tx->len++] = 0x84;//组合645数据帧
                            tx->buf[tx->len++] = 0x04; 
                            tx->buf[tx->len++] = 0x43;
                            tx->buf[tx->len++] = 0x23; 
                            tx->buf[tx->len++] = 0xCC; 
                            tx->buf[tx->len++] = 0x00;

                            tx->buf[tx->len  ] = Count_CheckSum(&(tx->buf[0]),tx->len);
                            tx->len++;
                            tx->buf[tx->len++] = 0x16;
                            //启动阀门任务
                          }
                        }
                        break;
                      case 0x59://写新水价
                        {
                          tx->len = 0;
                          memcpy(&tx->buf[tx->len],&buf[0],4);//表ID
                          tx->len++;tx->len++;tx->len++;tx->len++;
                          tx->buf[tx->len++] = 0x84;//组合645数据帧
                          tx->buf[tx->len++] = 0x03; 
                          tx->buf[tx->len++] = 0x59;
                          tx->buf[tx->len++] = 0x23; 
                          tx->buf[tx->len++] = 0x00;//正确应答
                          
                          for(uint8_t i=0;i<15;i++)
                          {
                            NewMultiRate.Buf[i] = buf[i + 12] - 0x33;
                          }
          
                          tx->buf[tx->len  ] = Count_CheckSum(&(tx->buf[0]),tx->len);
                          tx->len++;
                          tx->buf[tx->len++] = 0x16;
                        }
                        break;
                      case 0x37://充值
                        {
                          B16_B08 B16;
                          B32_B08 B32;
                          uint8_t crc[10]={0};
                          
                          tx->len = 0;
                          memcpy(&tx->buf[tx->len],&buf[0],4);//表ID
                          tx->len++;tx->len++;tx->len++;tx->len++;
          
                          B16.B8[0] = buf[16] - 0x33;
                          B16.B8[1] = buf[17] - 0x33;
                          ////判断序号是否大于已有序号  大于才可充值
                          if( (B16.B16 == RechargeRecord.Str_RechargeRecord.Num + 1)&&(B16.B16 >0 ) )
                          {
                            for(uint8_t i=0;i<4;i++)
                            {
                              crc[i]  =EquipmentID[i];//水表
                            }
                            for(uint8_t i=0;i<6;i++)
                            {
                              crc[i+4]  =buf[i+12] - 0x33;//水表
                            }
                            if(Count_CheckSum(&crc[0] ,10) ==  buf[18]-0x33)
                            {
                              B32.B8[0] = buf[12]-0x33;
                              B32.B8[1] = buf[13]-0x33;
                              B32.B8[2] = buf[14]-0x33;
                              B32.B8[3] = buf[15]-0x33;
                              
                              RechargeRecord.Str_RechargeRecord.Num = B16.B16;
                              RechargeRecord.Str_RechargeRecord.Sum = B32.B32;
                              WMData.Str_WMData.Cash += B32.B32;
                              if(WMData.Str_WMData.Cash > 0  )// 当剩余金额 大于0 开启阀门
                              {
                                //开启阀门
                              }
                              //数据保存到EEPROM
                            }
                            else//mac错误
                            {
                              tx->buf[tx->len++] = 0xC4;//组合645数据帧
                              tx->buf[tx->len++] = 0x03; 
                              tx->buf[tx->len++] = 0x37;
                              tx->buf[tx->len++] = 0x23; 
                              tx->buf[tx->len++] = (uint8_t)BIT2;//正确应答 
                            }
                          }
                          else//序号错误
                          {
                            tx->buf[tx->len++] = 0xC4;//组合645数据帧
                            tx->buf[tx->len++] = 0x03; 
                            tx->buf[tx->len++] = 0x37;
                            tx->buf[tx->len++] = 0x23; 
                            tx->buf[tx->len++] = (uint8_t)BIT1;//正确应答 
                          }
                  
                          tx->buf[tx->len  ] = Count_CheckSum(&(tx->buf[0]),tx->len);
                          tx->len++;
                          tx->buf[tx->len++] = 0x16;
                        } 
                        break;
                      default:
                        {
                        }
                        break;
                      } 
                    }
                    break;
                  }
                }
                else//下发写数据  密码错误回传数据帧
                {
                  
                }
              }
              break;
            }
        }
        else
        {
          flag = false;
        }
        break;
      }
      else
      {
        flag = false;
      }
      break;
    }
  }
  

 return(flag);
}









