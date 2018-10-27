#include "Driverlib.h"






void Resolve_XDKJ_188(uint8_t *point,uint8_t len,Str_Lora *tx)
{
  uint8_t buf[128]={0};
  bool flag = false;
  uint8_t sp;
  
  memcpy(&buf[0],&*point,len);//�������ߵ�����֡ 
  if( len==0 || len>=128 )
  {
    tx->len = 0;
  }

  for(sp=0;sp<len;sp++)
  {
    if((buf[sp] == 0x68) && (buf[buf[sp+10]+12+sp]==0x16))
    {
      flag = true ;
      break;
    }
  } 
  if(flag == true)              //�ж�����֡
  {                             //�жϵ�ַ�Ƿ���ȷ  �ж�У����Ƿ���ȷ
    flag = false;
    if(Count_CheckSum(&buf[sp],buf[sp+10]+7+sp) == buf[buf[sp+10]+11+sp])//У���
    {
      switch(buf[sp+1])//������
      {
      case 0x20://�ȱ��ش�Э��
        {  
          flag = true;
          tx->len = 0;
          memcpy(&tx->buf[tx->len],&buf[sp + 2],4);//��ID
          tx->len++;tx->len++;tx->len++;tx->len++;
          tx->buf[tx->len++] = 0x81;
          tx->buf[tx->len++] = 0x21; 
          tx->buf[tx->len++] = 0x01 + 0x33;
          tx->buf[tx->len++] = 0xA0 + 0x33; 
          
          for(uint8_t i=0;i<4;i++)
          {
            tx->buf[tx->len++] = buf[sp + 19 + i] + 0x33;
          }
          for(uint8_t i=0;i<4;i++)
          {
            tx->buf[tx->len++] = buf[sp + 24 + i] + 0x33;
          }
          for(uint8_t i=0;i<4;i++)
          {
            tx->buf[tx->len++] = buf[sp + 29 + i] + 0x33;
          }
          for(uint8_t i=0;i<4;i++)
          {
            tx->buf[tx->len++] = buf[sp + 34 + i] + 0x33;
          }
          for(uint8_t i=0;i<2;i++)
          {
            tx->buf[tx->len++] = buf[sp + 39 + i] + 0x33;
          }
          for(uint8_t i=0;i<2;i++)
          {
            tx->buf[tx->len++] = buf[sp + 42 + i] + 0x33;
          }
          for(uint8_t i=0;i<9;i++)
          {
            tx->buf[tx->len++] = buf[sp + 45 + i] + 0x33;
          }
          for(uint8_t i=0;i<2;i++)
          {
            tx->buf[tx->len++] = buf[sp + 55 + i] + 0x33;
          }
        }
        break;
      case 0x48://������  ������
        {  
          switch(buf[sp+9])//������
          {
          case 0x81://���Ŷ���������
            {  
              tx->len = 0;
              memcpy(&tx->buf[tx->len],&buf[sp + 2],4);//��ID
              tx->len++;tx->len++;tx->len++;tx->len++;
              tx->buf[tx->len++] = 0x81;
              tx->buf[tx->len++] = 0x15; 
              tx->buf[tx->len++] = 0x01 + 0x33;
              tx->buf[tx->len++] = 0xC0 + 0x33; 
              
              for(uint8_t i=0;i<5;i++)
              {
                tx->buf[tx->len++] = buf[sp + 15 + i] + 0x33;
              }
              for(uint8_t i=0;i<5;i++)
              {
                tx->buf[tx->len++] = buf[sp + 22 + i] + 0x33;
              } 
              for(uint8_t i=0;i<7;i++)//2017�� ֻת��17�� 20��ȥ
              {
                tx->buf[tx->len++] = buf[sp + 28 + i] + 0x33;
              } 
              for(uint8_t i=0;i<2;i++)
              {
                tx->buf[tx->len++] = buf[sp + 36 + i] + 0x33;
              } 
            }
            break;
          case 0xA1:
            {
              flag = true;
              tx->len = 0;
              tx->buf[tx->len++] = 0x68;
              tx->buf[tx->len++] = 0x06;
              tx->buf[tx->len++] = 0x89;
              tx->buf[tx->len++] = LoraNetParams.Str_LoraNetworkParams.FatherID;
              memcpy(&tx->buf[tx->len],&EquipmentID[0],3);
              tx->len++;tx->len++;tx->len++;
              memcpy(&tx->buf[tx->len],&buf[sp + 2],4);
              tx->len++;tx->len++;tx->len++;tx->len++;
              tx->buf[tx->len++] = 0x84;
              tx->buf[tx->len++] = 0x00; 
            }
            break;
          case 0xA2:// �����Żظ�����
            { //0x68,0x06,0x89,0x01,0x78,0x56,0x34,0x04,0x03,0x02,0x01,0x84,0x00,0x88,0x16
             flag = true;
              tx->len = 0;
              memcpy(&tx->buf[tx->len],&buf[sp + 2],4);
              tx->len++;tx->len++;tx->len++;tx->len++;
              tx->buf[tx->len++] = 0x84;
              tx->buf[tx->len++] = 0x00; 
            }
            break;
          case 0xA3:// �رշ��Żظ�����
            { //0x68,0x06,0x89,0x01,0x78,0x56,0x34,0x04,0x03,0x02,0x01,0x84,0x00,0x 88 ,0x16 
              flag = true;
              tx->len = 0;
              memcpy(&tx->buf[tx->len],&buf[sp + 2],4);
              tx->len++;tx->len++;tx->len++;tx->len++;
              tx->buf[tx->len++] = 0x84;
              tx->buf[tx->len++] = 0x00; 
              
            }
            break;
          default:
            {
              tx->len = 0;
            }
            break;
          }
        }
        break;
      default:
        {
          tx->len = 0;
        }
        break;
      }
    }
    else
    {
      tx->len = 0;
    }
  }
} 



//����645Э�� ��֯�´�Ƽ��ȱ������� 188����֡
//
void Combine_XDKJ_188(uint8_t *point,uint8_t len,Str_Lora *tx)
{
  uint8_t       buf[64];
  B16_B08       DI;    //��ȡDI��
  uint8_t       passwordtem[4]={0};
  uint8_t       Ctrl = 0;

  memcpy(&buf[0],&*point,len);//�������ߵ�����֡ 
  
 tx->rssi = 0x00;

    Ctrl = buf[4];
    DI.B8[0] = buf[6]-0x33;
    DI.B8[1] = buf[7]-0x33;
  
  switch(Ctrl)//������
  {
  case 0x01://������  ������
    {
      switch(DI.B8[1])
      { 
      case 0xA0://�ȱ����������� 
        {
          // FE FE FE 68 20 78 56 34 12 00 11 11 01 03 1F 90 01 72 16
          tx->len = 0;
          tx->buf[tx->len++] = 0xFE;
          tx->buf[tx->len++] = 0xFE;
          tx->buf[tx->len++] = 0xFE;
          tx->buf[tx->len++] = 0xFE;
          tx->buf[tx->len++] = 0x68;
          tx->buf[tx->len++] = 0x20;
          memcpy(&tx->buf[tx->len],&buf[0],4);//�������ߵ�����֡ 
          tx->len++;tx->len++;tx->len++;tx->len++;     
          tx->buf[tx->len++] = 0x00;
          tx->buf[tx->len++] = 0x11;tx->buf[tx->len++] = 0x11;
          tx->buf[tx->len++] = 0x01;tx->buf[tx->len++] = 0x03;
          tx->buf[tx->len++] = 0x1F;tx->buf[tx->len++] = 0x90;
          tx->buf[tx->len++] = 0x01; 
          tx->rssi = 0x01;
        }
        break;
      case 0xC0://����ǰ�������4��
        {
          switch(DI.B16 )
          {
          case 0xC001://��ȡ
            {
              //FE FE FE FE 68 48 04 03 02 01 00 00 00 01 03 90 1F 00 6D 16
              tx->len = 0;
              tx->buf[tx->len++] = 0xFE;
              tx->buf[tx->len++] = 0xFE;
              tx->buf[tx->len++] = 0xFE;
              tx->buf[tx->len++] = 0xFE;
              tx->buf[tx->len++] = 0x68;
              tx->buf[tx->len++] = 0x48;
              memcpy(&tx->buf[tx->len],&buf[0],4);//�������ߵ�����֡ 
              tx->len++;tx->len++;tx->len++;tx->len++;     
              tx->buf[tx->len++] = 0x00;tx->buf[tx->len++] = 0x00;tx->buf[tx->len++] = 0x00;
              tx->buf[tx->len++] = 0x01;tx->buf[tx->len++] = 0x03;
              tx->buf[tx->len++] = 0x90;tx->buf[tx->len++] = 0x1f;
              tx->buf[tx->len++] = 0x00; 
              tx->rssi = 0x02;
            }
            break;
            break; 
          default:
            break;
          }
        }
        break;
      }
    }
    break;
  case 0x04://������  д����
    {    
        for(uint8_t num=8;num<12;num++)
        {
          passwordtem[num-8]= buf[num] - 0x33;
        }
      
      if( memcmp(&PassWord[0],&passwordtem[0],4)==0 )
      {
        switch( DI.B16 )
        {
        case 0xC010:// д���ط���
          {//FE FE FE FE 68 48 04 03 02 01 00 00 00 22 0C A0 17 00 00 00 02 07 09 10 15 90 55 BB 16 
            tx->len = 0x00;//��֯�ش�����֡
            if(buf[buf[5]+5] == 0x88)//BUF[5]=������ 
            { 
              tx->len = 0x00;
              tx->len = 0;
              tx->buf[tx->len++] = 0xFE;
              tx->buf[tx->len++] = 0xFE;
              tx->buf[tx->len++] = 0xFE;
              tx->buf[tx->len++] = 0xFE;
              tx->buf[tx->len++] = 0x68;
              tx->buf[tx->len++] = 0x48;
              memcpy(&tx->buf[tx->len],&buf[0],4);//�������ߵ�����֡ 
              tx->len++;tx->len++;tx->len++;tx->len++;     
              tx->buf[tx->len++] = 0x00;tx->buf[tx->len++] = 0x00;tx->buf[tx->len++] = 0x00;
              tx->buf[tx->len++] = 0x22;tx->buf[tx->len++] = 0x0C;
              tx->buf[tx->len++] = 0xA0;tx->buf[tx->len++] = 0x17;
              tx->buf[tx->len++] = 0x00;tx->buf[tx->len++] = 0x00;tx->buf[tx->len++] = 0x00;
              for(uint8_t i=0;i<7;i++)
              {
                tx->buf[tx->len++] = buf[12+i] - 0x33;
              }
              tx->rssi = 0x02;
            }
            else if(buf[buf[5]+5] == 0xCC)
            {// FE FE FE FE 68 48 04 03 02 01 00 00 00 23 04 A0 17 00 99 31 16
              tx->len = 0;
              tx->buf[tx->len++] = 0xFE;
              tx->buf[tx->len++] = 0xFE;
              tx->buf[tx->len++] = 0xFE;
              tx->buf[tx->len++] = 0xFE;
              tx->buf[tx->len++] = 0x68;
              tx->buf[tx->len++] = 0x48;
              memcpy(&tx->buf[tx->len],&buf[0],4);//�������ߵ�����֡ 
              tx->len++;tx->len++;tx->len++;tx->len++;     
              tx->buf[tx->len++] = 0x00;tx->buf[tx->len++] = 0x00;tx->buf[tx->len++] = 0x00;
              tx->buf[tx->len++] = 0x23;tx->buf[tx->len++] = 0x04;
              tx->buf[tx->len++] = 0xA0;tx->buf[tx->len++] = 0x17;
              tx->buf[tx->len++] = 0x00;tx->buf[tx->len++] = 0x99; 
              tx->rssi = 0x02;
            }
          }
          break;
        case 0xC007://д����ʱ�� 
          {// FE FE FE FE 68 48 04 03 02 01 00 00 00 21 0A A0 15 00 00 00 02 07 09 10 15 D1 16
            tx->len = 0x00;//��֯�ش�����֡
            tx->len = 0;
            tx->buf[tx->len++] = 0xFE;
            tx->buf[tx->len++] = 0xFE;
            tx->buf[tx->len++] = 0xFE;
            tx->buf[tx->len++] = 0xFE;
            tx->buf[tx->len++] = 0x68;
            tx->buf[tx->len++] = 0x48;
            memcpy(&tx->buf[tx->len],&buf[0],4);//�������ߵ�����֡ 
            tx->len++;tx->len++;tx->len++;tx->len++;     
            tx->buf[tx->len++] = 0x00;tx->buf[tx->len++] = 0x00;tx->buf[tx->len++] = 0x00;
            tx->buf[tx->len++] = 0x21;tx->buf[tx->len++] = 0x0A;tx->buf[tx->len++] = 0xA0;tx->buf[tx->len++] = 0x15;tx->buf[tx->len++] = 0x00;
            
            for(uint8_t i=0;i<6;i++)
            {
              tx->buf[tx->len++] = buf[12+i] - 0x33;
            }
            tx->buf[tx->len++] = 0x15;
            tx->rssi = 0x02;
          }
          break;
        }
      }
    }
    break;
  }
  
  tx->buf[tx->len]=Count_CheckSum(&(tx->buf[4]),tx->len-4);
  tx->len++;
  tx->buf[tx->len++]=0x16;
  
}