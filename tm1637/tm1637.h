#ifndef __TM1637_H
#define __TM1637_H

#ifdef __cplusplus
  extern "C" {
#endif 
    

#include "main.h"

typedef struct
{
  uint8_t         Type;
  uint32_t        SLKPin; 
  uint32_t        DIOPin;
  GPIO_TypeDef*   GPIOx;
  uint8_t         ChipQuantity;
  uint8_t         Lock;
} tm1637_t;

typedef enum
{
  DisplaySet = 0,
  ButtonPad
} tm1637TypeDef;


tm1637_t Init_TM1637(GPIO_TypeDef*, uint16_t, uint16_t, tm1637TypeDef);

void TM1637_ClkLow(void);
void TM1637_ClkHigh(void);
void TM1637_DioLow(void);
void TM1637_DioHigh(void);
void TM1637_WaitAck(void);
    
void TM1637_Start(void);
void TM1637_Stop(void);

extern tm1637_t tm1637;
    
void TM1637_SendByte(uint32_t*);
void TM1637_Init(void);
void TM1637_Run(uint16_t);
void Delay(uint32_t);

#ifdef __cplusplus
}
#endif

#endif /* __TM1637_H */

