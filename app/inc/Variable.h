#ifndef __Varialbe_H
#define __Varialbe_H

#ifdef __cplusplus
extern "C" {
#endif 
  


/*************************************************************************
  ���涳������
  �ѵ�һҳ��Ϊ32��  0---31   ÿ��8���ֽ�  һ��256�ֽ�
  0��  ��ID
  1��  modbusID
  2��  Sys_Version_Num
  3��  System_Uptime
  4��  HeatMeter_L
  5��  HeatMeter_M3
  6��  HeatMeter_Energy_GJ
  7��  HeatMeter_Energy_J
0---1    ȫ�����ݴ洢
1---9    36���¶���
10--18   36�����ϴ���洢
19--292  274*4=1096�춳��
  
  
  
  
  
  
  
  
  ********************************************************************/

#define PageAddr         16
  
#define EquipmentID_Page        1 
#define EquipmentID_Page_Offset 0
#define EquipmentID_Addr        (EquipmentID_Page*PageAddr + EquipmentID_Page_Offset)
  
#define Valve_State_Page        1 
#define Valve_State_Page_Offset 6
#define Valve_State_Addr        (Valve_State_Page*PageAddr + Valve_State_Page_Offset)   
  
  
#define RechargeRecord_Page     1 
#define RechargeRecord_Page_Offset 7
#define RechargeRecord_Addr     (RechargeRecord_Page*PageAddr + RechargeRecord_Page_Offset)   
   
 
#define WMData_Page             2 
#define WMData_Page_Offset      0
#define WMData_Addr        (WMData_Page*PageAddr + WMData_Page_Offset)   
    
  
#define MultiRate_Page             3 
#define MultiRate_Page_Offset      0
#define MultiRate_Addr        (MultiRate_Page*PageAddr + MultiRate_Page_Offset)   
    
#define NewMultiRate_Page             4
#define NewMultiRate_Page_Offset      0
#define NewMultiRate_Addr        (NewMultiRate_Page*PageAddr + NewMultiRate_Page_Offset)   
    
 
 
#define         YY              0x5
#define         MM              0x4
#define         DD              0x3
#define         hh              0x2
#define         mm              0x1
#define         ss              0x0 
  
  
  
#pragma pack(1)
  
  typedef struct 
  {
    uint8_t Month;
    uint8_t Day  ;
    uint8_t Hours;
    uint8_t Minutes;
    uint8_t Year;
  }Str_Valve_Rtc;
  
  typedef union 
  {
    uint8_t Buf[11];
    struct 
    {
      Str_Valve_Rtc Vclose;
      uint8_t Angle;
    }Str_Valve_Dat;
  }Union_Valve_Dat;
  
  typedef struct 
{
  volatile uint8_t lock;
  uint8_t len;
  uint8_t buf[128];
}Str_Com;
  
  typedef union 
  {
    uint32_t B32;
    uint8_t B8[4];
  }B32_B08;
  
  typedef union 
  {
    uint16_t B16;
    uint8_t  B8[2];
  }B16_B08 ;
  
  //Error identification ����ʶ��
  /*********************����ʶ��***************************/
  typedef union
  {
    uint8_t Error ;
    struct
    {
      bool PasswordErr    : 1; //�������
      bool OrderErr       : 1; //��ˮ�������
      bool DataErr        : 1; //���ݷǷ�����
      bool NumErr         : 1; //�úű�Ŵ���
      bool ValveErr       : 1; //���Ź���
      uint8_t Reserve     : 3; //����
    }Str_Error ;               //�������ݿ�
  }Union_Error ;


  /*********************����״̬***************************/
  typedef enum 
  {
    SleepDisable        = 0,//ֹͣ����      1��ֹͣ        0������ 
    FreezeFradeData     = 1,//��������      1����          0��û�� 
    WriteIDState        = 2,
    Valveing            = 3,//�������ڶ���
    Buttoning           = 4,//����������ڽ���
    Lpuart0RcvOK        = 5,//����0�������
    Lpuart0SendOK       = 6,//����0�������
    Lpuart1RcvOK        = 7,//����0�������
    Lpuart1SendOK       = 8,//����0�������
  } Enum_State;
  
   typedef union
  {
    uint8_t Buf[16]     ;
    struct
    {                   
      uint32_t Sum      ;// ��ǰ����ˮ��
      uint32_t YearSum  ;// ����ˮ��
      uint32_t DaySum   ;// ������ˮ��
      int32_t  Cash     ;// ������ˮ���  
    }Str_WMData         ;// �������ݿ�
  }Union_WMData         ;// 
  
  typedef union
  {
    uint8_t Buf[6]      ;
    struct         
    {
      uint32_t Sum      ;//��ֵ���
      uint16_t Num      ;//��ֵ����
    }Str_RechargeRecord ;//��ֵ
  }Union_RechargeRecord ;
  
  typedef union
  {
    uint8_t     Buf[15]         ;
    struct                       //������
    {
      uint16_t  One             ;//1����0.1��ˮ ����1 
      uint16_t  RateOne         ;//1����0.1ԪǮ  
      uint16_t  Two             ;//1����0.1��ˮ ����2 
      uint16_t  RateTwo         ;//1����0.1ԪǮ 
      uint16_t  Three           ;//1����0.1��ˮ ����3
      uint16_t  RateThree       ;//1����0.1ԪǮ
      uint8_t   EnableTime[3]	;//ENABLETIME[0]=��ENABLETIME[1]=��ENABLETIME[2]=��
    }Str_MultiRate           	;//������
  }Union_MultiRate;
  /*********************����״̬***************************/
    typedef enum 
  {
    EnumValveOpen = ((uint8_t)0x11),//����
    EnumValveClose= ((uint8_t)0x22),//�ر�
    EnumValveErr  = ((uint8_t)0x33),//����
    EnumValveReserve=((uint8_t)0x00),//״̬��ȷ��
  } Enum_ValveState;
  
  typedef union 
  {
    uint8_t Buf[2];
    struct 
    {
      Enum_ValveState State;
      uint8_t Angle;
    }Str_ValveState;
  }Union_ValveState;
  
extern const uint8_t    BroadcastId[6]          ;
extern const uint8_t    BroadcastLoraId[6]       ;
extern const uint8_t    PassWord[4]             ;      
extern uint32_t          SysState                ; //����״̬��  �����д洢 
extern uint8_t           Batt_Vol                ; 
extern uint8_t           EquipmentID[6]         ;//ˮ������ 6�ֽ� ���д洢 
extern uint8_t          Valve_State;



extern Union_WMData            WMData           ;
extern Union_RechargeRecord    RechargeRecord   ;
extern Union_MultiRate         MultiRate        ;
extern Union_MultiRate         NewMultiRate     ;
extern uint8_t                 ExternalDamageSum ; //�����ܴ���
extern uint8_t                 MagnetAttackSum   ; //�Ź����ܴ��� 
extern Union_ValveState        ValvePara ;
extern uint8_t                 FluxStateMachine  ; 





void Sys_Software_Init(void);
void Write_Global_Variable_Eeprom(void);

void W_EquipmentID(void);
void R_EquipmentID(void);
void W_Valve_State(void);
void R_Valve_State(void);
void W_WMData(void);
void R_WMData(void);
void W_RechargeRecord(void);
void R_RechargeRecord(void);
void W_MultiRate(void);
void R_MultiRate(void);
void W_NewMultiRate(void);
void R_NewMultiRate(void);

#ifdef __cplusplus
}
#endif
#endif