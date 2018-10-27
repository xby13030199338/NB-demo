#ifndef __Varialbe_H
#define __Varialbe_H

#ifdef __cplusplus
extern "C" {
#endif 
  


/*************************************************************************
  保存冻结数据
  把第一页分为32段  0---31   每段8个字节  一共256字节
  0段  表ID
  1段  modbusID
  2段  Sys_Version_Num
  3段  System_Uptime
  4段  HeatMeter_L
  5段  HeatMeter_M3
  6段  HeatMeter_Energy_GJ
  7段  HeatMeter_Energy_J
0---1    全局数据存储
1---9    36个月冻结
10--18   36个故障代码存储
19--292  274*4=1096天冻结
  
  
  
  
  
  
  
  
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
  
  //Error identification 错误识别
  /*********************错误识别***************************/
  typedef union
  {
    uint8_t Error ;
    struct
    {
      bool PasswordErr    : 1; //密码错误
      bool OrderErr       : 1; //购水次序错误
      bool DataErr        : 1; //数据非法错误
      bool NumErr         : 1; //用号编号错误
      bool ValveErr       : 1; //阀门故障
      uint8_t Reserve     : 3; //保留
    }Str_Error ;               //运行数据块
  }Union_Error ;


  /*********************运行状态***************************/
  typedef enum 
  {
    SleepDisable        = 0,//停止休眠      1：停止        0：开启 
    FreezeFradeData     = 1,//冻结数据      1：有          0：没有 
    WriteIDState        = 2,
    Valveing            = 3,//阀门正在动作
    Buttoning           = 4,//按键检查正在进行
    Lpuart0RcvOK        = 5,//串口0接收完成
    Lpuart0SendOK       = 6,//串口0发送完成
    Lpuart1RcvOK        = 7,//串口0接收完成
    Lpuart1SendOK       = 8,//串口0发送完成
  } Enum_State;
  
   typedef union
  {
    uint8_t Buf[16]     ;
    struct
    {                   
      uint32_t Sum      ;// 当前总用水量
      uint32_t YearSum  ;// 年用水量
      uint32_t DaySum   ;// 当月用水量
      int32_t  Cash     ;// 当月用水金额  
    }Str_WMData         ;// 运行数据块
  }Union_WMData         ;// 
  
  typedef union
  {
    uint8_t Buf[6]      ;
    struct         
    {
      uint32_t Sum      ;//充值金额
      uint16_t Num      ;//充值次数
    }Str_RechargeRecord ;//充值
  }Union_RechargeRecord ;
  
  typedef union
  {
    uint8_t     Buf[15]         ;
    struct                       //复费率
    {
      uint16_t  One             ;//1代表0.1吨水 阶梯1 
      uint16_t  RateOne         ;//1代表0.1元钱  
      uint16_t  Two             ;//1代表0.1吨水 阶梯2 
      uint16_t  RateTwo         ;//1代表0.1元钱 
      uint16_t  Three           ;//1代表0.1吨水 阶梯3
      uint16_t  RateThree       ;//1代表0.1元钱
      uint8_t   EnableTime[3]	;//ENABLETIME[0]=年ENABLETIME[1]=月ENABLETIME[2]=日
    }Str_MultiRate           	;//复费率
  }Union_MultiRate;
  /*********************阀门状态***************************/
    typedef enum 
  {
    EnumValveOpen = ((uint8_t)0x11),//开启
    EnumValveClose= ((uint8_t)0x22),//关闭
    EnumValveErr  = ((uint8_t)0x33),//故障
    EnumValveReserve=((uint8_t)0x00),//状态不确定
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
extern uint32_t          SysState                ; //运行状态字  不进行存储 
extern uint8_t           Batt_Vol                ; 
extern uint8_t           EquipmentID[6]         ;//水表表号 6字节 进行存储 
extern uint8_t          Valve_State;



extern Union_WMData            WMData           ;
extern Union_RechargeRecord    RechargeRecord   ;
extern Union_MultiRate         MultiRate        ;
extern Union_MultiRate         NewMultiRate     ;
extern uint8_t                 ExternalDamageSum ; //开盖总次数
extern uint8_t                 MagnetAttackSum   ; //磁攻击总次数 
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