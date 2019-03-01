#ifndef __UTILS_H
#define __UTILS_H

#ifdef __cplusplus
  extern "C" {
#endif 

#include "main.h"

typedef enum
{
  TM1637 = 0,
  TM1803 = 1
} DisplayTypeTypeDef;

typedef enum
{
  FLAG_SET = 1,
  FLAG_CLEAR = 0
} FlagStateTypeDef;

typedef enum
{
  DIGITS = 4,
  CLOCK = 3,
  DIG_W_DOT_1 = 2,
  DIG_W_DOT_2 = 1,
  DIG_W_DOT_3 = 0
} DisplayInfoTypeDef;

typedef enum
{
  ERR_0 = 0, 
  ERR_1 = 1, 
  ERR_2 = 2, 
  ERR_3 = 3, 
  ERR_4 = 4,
  ERR_5 = 5,
  ERR_6 = 6,
  ERR_7 = 7,
  ERR_8 = 8,
  ERR_9 = 9,
  NO_ERR = 0xff
} ErrorState;

extern const uint8_t symbols[];

extern uint32_t delay_tmp;
extern uint32_t sysQuantum;

extern uint8_t tick;


void Error_Handler(char*);
FlagStateTypeDef GetFlag(uint32_t*, uint8_t);
void SetFlag(uint32_t*, uint8_t, FlagStateTypeDef);
void Display_Handler(DisplayTypeTypeDef);
extern void Delay_Handler(void);

void Delay_us(uint32_t);

uint8_t Display(int32_t, DisplayInfoTypeDef, ErrorState);


#ifdef __cplusplus
}
#endif

#endif /* __UTILS_H */
