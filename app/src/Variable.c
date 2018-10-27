#include "include.h"

     
//����ȫ�ֱ���


const uint8_t BroadcastId[6]    ={0x01,0x00,0x00,0x00,0x00,0x00};
const uint8_t PassWord[4]       ={0x33,0x44,0x44,0x44};     
     
uint32_t                SysState          ; //����״̬��  �����д洢 
uint8_t                 Batt_Vol          ; //��ص�ѹ

/**********************���²�����Ҫ�洢****************************/
uint8_t                 EquipmentID[6]    ; //ˮ������ 6�ֽ� ���д洢  
uint8_t                 Valve_State       ; //����״̬  0% ȫ��  99%ȫ��
Union_WMData            WMData            ; //ˮ����������
Union_RechargeRecord    RechargeRecord    ; //��ֵ��¼
Union_MultiRate         MultiRate         ; //���ݷ���
Union_MultiRate         NewMultiRate      ; //�½��ݷ���
uint8_t                 ExternalDamageSum ; //�����ܴ���
uint8_t                 MagnetAttackSum   ; //�Ź����ܴ���
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
  
  Rng_Init();///< �ϵ��һ���������������ʹ�ó�ʼ���������ɲ���ȡ�����
  pu32Data[0] = Rng_GetData()[0];
  pu32Data[1] = Rng_GetData()[1];
  
  
}

//��������� 
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




















