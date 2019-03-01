#include "utils.h"

uint32_t delay_tmp = 0;


// -------------------------------------------------------------
FlagStateTypeDef GetFlag(uint32_t* __registry, uint8_t __flag)
{
  return (*__registry & (1 << __flag)) ? FLAG_SET : FLAG_CLEAR;
}


// -------------------------------------------------------------
void SetFlag(uint32_t* __registry, uint8_t __flag, FlagStateTypeDef __state)
{
  if (__state == FLAG_SET)
  {
    *__registry |= (1 << __flag);
  }
  else if (__state == FLAG_CLEAR)
  {
    *__registry &= ~(1 << __flag);
  }
  else
  {
    Error_Handler("Flag Error!");
  }
}



// -------------------------------------------------------------
void Error_Handler(char* __error_message)
{
  //printf(__error_message);
  while(1)
  {
  }
}


// -------------------------------------------------------------
void Display_Handler(DisplayTypeTypeDef __dt)
{
  switch (__dt)
  {
  	case TM1637:
      TM1637_Run(0xc040);
  		break;
  	case TM1803:
      Error_Handler("TM1803 Error!");
  		break;
  	default:
      Error_Handler("Error!");
  		break;
  }
}


// -------------------------------------------------------------
void Delay(uint32_t __ms_delay)
{
  delay_tmp = sysQuantum + __ms_delay * 100;
  Cron_Handler();
}

// -------------------------------------------------------------
void Delay_us(uint32_t __us_delay)
{
  uint32_t i = 0;
  while (i++ < __us_delay*72);
}


