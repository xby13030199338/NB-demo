#include "include.h"

     
//所有全局变量


const uint8_t BroadcastId[6]    ={0x01,0x00,0x00,0x00,0x00,0x00};
const uint8_t PassWord[4]       ={0x33,0x44,0x44,0x44};     
     
uint32_t                SysState          ; //运行状态字  不进行存储 
uint8_t                 Batt_Vol          ; //电池电压

/**********************以下部分需要存储****************************/
uint8_t                 EquipmentID[6]    ; //水表表号 6字节 进行存储  
uint8_t                 Valve_State       ; //阀门状态  0% 全关  99%全开
Union_WMData            WMData            ; //水表运行数据
Union_RechargeRecord    RechargeRecord    ; //充值记录
Union_MultiRate         MultiRate         ; //阶梯费率
Union_MultiRate         NewMultiRate      ; //新阶梯费率
uint8_t                 ExternalDamageSum ; //开盖总次数
uint8_t                 MagnetAttackSum   ; //磁攻击总次数
Union_ValveState        ValvePara         ;
uint8_t                 FluxStateMachine  ;      




void Sys_Software_Init(void)
{
    uint32_t pu32Data[2] = {0};
//  R_EquipmentID();
//  R_WMData();
  
  Lpuart0_Q_Init();
  Buzzer_Q_Init();
  Valve_Q_Init();
  Batt_Vol = BatterVolCheck();
  
  Rng_Init();///< 上电第一次生成随机数必须使用初始化函数生成并获取随机数
  pu32Data[0] = Rng_GetData()[0];
  pu32Data[1] = Rng_GetData()[1];
  
  
}

//生成随机数 
uint32_t Random_Num(void)
{
  uint32_t pu32Data[2] = {0};
  
  Rng_Generate();
  pu32Data[0] = Rng_GetData()[0];
  pu32Data[1] = Rng_GetData()[1];
  
  return((pu32Data[0]>>1)+(pu32Data[1]>>2));
}




void W_EquipmentID(void)
{
  M24C16_Array_Write(EquipmentID_Addr,&EquipmentID[0],sizeof(EquipmentID));
}
void R_EquipmentID(void)
{
  M24C16_Array_Read(EquipmentID_Addr,&EquipmentID[0],sizeof(EquipmentID));
}


void W_Valve_State(void)
{
  M24C16_Array_Write(Valve_State_Addr,&Valve_State,sizeof(Valve_State));
}
void R_Valve_State(void)
{
  M24C16_Array_Read(Valve_State_Addr,&Valve_State,sizeof(Valve_State));
}


void W_WMData(void)
{
  M24C16_Array_Write(WMData_Addr,&WMData.Buf[0],sizeof(WMData));
}
void R_WMData(void)
{
  M24C16_Array_Read(WMData_Addr,&WMData.Buf[0],sizeof(WMData));
}


void W_RechargeRecord(void)
{
  M24C16_Array_Write(RechargeRecord_Addr,&RechargeRecord.Buf[0],sizeof(RechargeRecord));
}
void R_RechargeRecord(void)
{
  M24C16_Array_Read(RechargeRecord_Addr,&RechargeRecord.Buf[0],sizeof(RechargeRecord));
}


void W_MultiRate(void)
{
  M24C16_Array_Write(MultiRate_Addr,&MultiRate.Buf[0],sizeof(MultiRate));
}
void R_MultiRate(void)
{
  M24C16_Array_Read(MultiRate_Addr,&MultiRate.Buf[0],sizeof(MultiRate));
}

  
void W_NewMultiRate(void)
{
  M24C16_Array_Write(NewMultiRate_Addr,&NewMultiRate.Buf[0],sizeof(NewMultiRate));
}
void R_NewMultiRate(void)
{
  M24C16_Array_Read(NewMultiRate_Addr,&NewMultiRate.Buf[0],sizeof(NewMultiRate));
}
  
  
/*void W_ExternalDamageSum(void)
{
  M24C16_Array_Write(ExternalDamageSum_Addr,&ExternalDamageSum,sizeof(ExternalDamageSum));
}
void R_ExternalDamageSum(void)
{
  M24C16_Array_Read(ExternalDamageSum_Addr,&ExternalDamageSum,sizeof(ExternalDamageSum));
}
  

void W_MagnetAttackSum(void)
{
  M24C16_Array_Write(MagnetAttackSum_Addr,&MagnetAttackSum,sizeof(MagnetAttackSum));
}
void R_MagnetAttackSum(void)
{
  M24C16_Array_Read(MagnetAttackSum_Addr,&MagnetAttackSum,sizeof(MagnetAttackSum));
}8*/





















