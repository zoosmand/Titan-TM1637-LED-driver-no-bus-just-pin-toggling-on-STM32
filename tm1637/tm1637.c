#include "tm1637.h"

const uint8_t symbols[] = { 0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x00, 0x40, 0x80, 0x79, 0x50, 0x78, 0x63, 0x4e, 0x58 };
/*
0 - 0x3f
1 - 0x06
2 - 0x5b
3 - 0x4f
4 - 0x66
5 - 0x6d
6 - 0x7d
7 - 0x07
8 - 0x7f
9 - 0x6f
  - 0x00
- - 0x40
. - 0x80
E - 0x79
r - 0x50
t - 0x78
o - 0x63
o - 0x4e
c - 0x58
*/
uint8_t tick = 0;
uint32_t displayData = 0;

tm1637_t tm1637;


  
// -------------------------------------------------------------
tm1637_t Init_TM1637(GPIO_TypeDef* __gpio, uint16_t __clk, uint16_t __dio, tm1637TypeDef __type)
{
  tm1637_t tm1637;
  
  tm1637.Type = __type;
  tm1637.GPIOx = __gpio;
  tm1637.SLKPin = __clk;
  tm1637.DIOPin = __dio;
  tm1637.ChipQuantity = 1;
  tm1637.Lock = 0;
  
  // TM1637 DIO
  GPIO_InitTypeDef tm1637_pin;
  tm1637_pin.GPIO_Pin = tm1637.DIOPin;
  tm1637_pin.GPIO_Mode = GPIO_Mode_Out_PP;
  tm1637_pin.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(tm1637.GPIOx, &tm1637_pin);  
  GPIO_WriteBit(tm1637.GPIOx, tm1637.DIOPin, Bit_SET);

  // TM1637 SCK
  tm1637_pin.GPIO_Pin = tm1637.SLKPin;
  GPIO_Init(tm1637.GPIOx, &tm1637_pin);  
  GPIO_WriteBit(tm1637.GPIOx, tm1637.SLKPin, Bit_SET);
  
  GPIO_PinLockConfig(tm1637.GPIOx, tm1637.DIOPin);
  GPIO_PinLockConfig(tm1637.GPIOx, tm1637.SLKPin);
  
  return tm1637;
}


// -------------------------------------------------------------
void TM1637_ClkLow(void)
{
  GPIO_WriteBit(tm1637.GPIOx, tm1637.SLKPin, Bit_RESET);
}

// -------------------------------------------------------------
void TM1637_ClkHigh(void)
{
  GPIO_WriteBit(tm1637.GPIOx, tm1637.SLKPin, Bit_SET);
}

// -------------------------------------------------------------
void TM1637_DioLow(void)
{
  GPIO_WriteBit(tm1637.GPIOx, tm1637.DIOPin, Bit_RESET);
}

// -------------------------------------------------------------
void TM1637_DioHigh(void)
{
  GPIO_WriteBit(tm1637.GPIOx, tm1637.DIOPin, Bit_SET);
}

// -------------------------------------------------------------
void TM1637_WaitAck(void)
{
  TM1637_ClkLow();
  TM1637_DioLow();
  Delay_us(5);
  if (GPIO_ReadOutputDataBit(tm1637.GPIOx, tm1637.DIOPin))
  {
    Error_Handler("TM1637 ACK receive Error!");
  }
  TM1637_ClkHigh();
  Delay_us(3);
//  TM1637_ClkLow();
}


// -------------------------------------------------------------
void TM1637_Start(void)
{
  TM1637_ClkHigh();
  TM1637_DioHigh();
  Delay_us(2);
  TM1637_DioLow();
}


// -------------------------------------------------------------
void TM1637_Stop()
{
  TM1637_ClkLow();
  Delay_us(2);
  TM1637_DioLow();
  Delay_us(2);
  TM1637_ClkHigh();
  Delay_us(2);
  TM1637_DioHigh();
}


// -------------------------------------------------------------
void TM1637_SendByte(uint32_t* __pack)
{
  int counter;
  __ASM
  {
    MOVS counter, 8
    TM1637_SendByte_Bit:
      BL TM1637_ClkLow
      LSRS *__pack, 1
      BCS TM1637_SendByte_Bit_ONE
    
    //TM1637_SendByte_Bit_ZERO:
      BL TM1637_DioLow
      B TM1637_SendByte_Next_Bit
    
    TM1637_SendByte_Bit_ONE:
      BL TM1637_DioHigh
    
    TM1637_SendByte_Next_Bit:
      BL Delay_us, {R0 = 3}
      BL TM1637_ClkHigh
      BL Delay_us, {R0 = 3}
      SUBS counter, 1
      BNE TM1637_SendByte_Bit
    
      BL TM1637_WaitAck
  }
}


// -------------------------------------------------------------
void TM1637_Init()
{
  uint32_t displayPack = 0;
  TM1637_Start();
  displayPack = 0x40;
  TM1637_SendByte(&displayPack);
  TM1637_Stop();
  
  TM1637_Start();
  displayPack = 0xc0;
  TM1637_SendByte(&displayPack);
  displayPack = 0;
  for(int i = 0; i < 4; i++)
  {
    TM1637_SendByte(&displayPack);
  }
  TM1637_Stop();
  
  TM1637_Start();
  displayPack = 0x8a;
  TM1637_SendByte(&displayPack);
  TM1637_Stop();    
}


// -------------------------------------------------------------
void TM1637_Run(uint16_t __com_addr)
{
  uint8_t __command = __com_addr & 0xff;
  uint8_t __address = (__com_addr >> 8) & 0xff;
  
  switch (__command)
  {
  	case 0x40:
  	case 0x44:
      goto TM1637_Run_Run;
  		break;
  	default:
      Error_Handler("Error!");
  		break;
  }
  
  TM1637_Run_Run:
    TM1637_Start();
    TM1637_SendByte((uint32_t*)&__command);
    TM1637_Stop(); 

    TM1637_Start();
    TM1637_SendByte((uint32_t*)&__address);    

    if ((__com_addr & 0xff) == 0x40)
    {
      for(int i = 0; i < 4; i++)
      {
        TM1637_SendByte(&displayData);
      }
    }
    else
    {
      TM1637_SendByte(&displayData);
    }
    TM1637_Stop();
}


// -------------------------------------------------------------
uint8_t Display(int32_t __data, DisplayInfoTypeDef __info_type, ErrorState __er_state)
{
  if (__er_state == NO_ERR)
  {
    displayData = 0;
    switch (__info_type)
    {
      case CLOCK:
        displayData |= 
            symbols[(__data/1000) ? __data/1000 : 0x0a]
          | ((symbols[__data%1000/100] | tick) << 8)
          | (symbols[__data%100/10] << 16)
          | (symbols[__data%10] << 24);
        break;
      case DIG_W_DOT_1:
      case DIG_W_DOT_2:
      case DIG_W_DOT_3:
        displayData |= (0x80 << (8*__info_type));
        if (__data >= 0)
        {
          if (__data >= 10000)
          {
            Display(0, DIGITS, ERR_1);
            break;
          }
          if (__data < 1000)
          {
            if (__info_type == DIG_W_DOT_3)
            {
              displayData |= symbols[0];
            }
          }
          if (__data < 100)
          {
            if ((__info_type == DIG_W_DOT_2) || (__info_type == DIG_W_DOT_3))
            {
              displayData |= symbols[0] << 8; 
            }
          }
          if (__data < 10)
          {
            displayData |= symbols[0] << 16; 
          }

          displayData |= 
              symbols[(__data/1000) ? __data/1000 : 0x0a]
            | ((symbols[(__data%1000/100) ? __data%1000/100 : 0x0a]) << 8)
            | ((symbols[(__data%100/10) ? __data%100/10 : 0x0a]) << 16)
            | (symbols[__data%10] << 24);
        }
        else
        {
          __data *= -1;
          if ((__data >= 1000) || (__info_type == DIG_W_DOT_3))
          {
            Display(0, DIGITS, ERR_2);
            break;
          }
          if (__info_type == DIG_W_DOT_2)
          {
            displayData |= 
                symbols[(__data < 1000) ? 0x0b : (__data < 100) ? 0x0b : 0x0a]
              | ((symbols[(__data < 100) ? 0 : (__data < 10) ? 0x0b : (__data%1000/100)]) << 8);
          }
          if (__info_type == DIG_W_DOT_1)
          {
            displayData |= 
                symbols[(__data > 99) ? 0x0b : 0x0a]
            | ((symbols[(__data < 100) ? 0x0b : (__data < 10) ? 0x0b : (__data%1000/100)]) << 8);
          }

          displayData |= 
              ((symbols[(__data < 10) ? 0 : (__data%100/10)]) << 16)
            | (symbols[__data%10] << 24);
        }
        break;
        
      default:
        if (__data >= 0)
        {
          if (__data >= 10000)
          {
            Display(0, DIGITS, ERR_1);
            break;
          }
          displayData |= 
              symbols[(__data/1000) ? __data/1000 : 0x0a]
            | ((symbols[(__data%1000/100) ? __data%1000/100 : 0x0a]) << 8)
            | ((symbols[(__data%100/10) ? __data%100/10 : 0x0a]) << 16)
            | (symbols[__data%10] << 24);
        }
        else
        {
          __data *= -1;
          if (__data >= 1000)
          {
            Display(0, DIGITS, ERR_2);
            break;
          }
          displayData |= 
              symbols[(__data > 99) ? 0x0b : 0x0a]
            | ((symbols[(__data < 100) ? (__data < 10) ? 0x0a : 0x0b : (__data%1000/100)]) << 8)
            | ((symbols[(__data < 10) ? (__data < 0) ? 0x0a : 0x0b : (__data%100/10)]) << 16)
            | (symbols[__data%10] << 24);
        }
        break;
    }
  }
  else
  {
    displayData = 
        symbols[0x0d]
      | (symbols[0x0e]<< 8)
      | (symbols[0x0e] << 16)
      | (symbols[__er_state] << 24);
  }
  SetFlag(&_EREG_, _RDF_, FLAG_SET);
  return (tick) ? 0 : 0x80;
}
