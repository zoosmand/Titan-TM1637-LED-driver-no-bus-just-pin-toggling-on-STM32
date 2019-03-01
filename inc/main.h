#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
  extern "C" {
#endif 
    
#ifndef STM32F10X_HD
#define STM32F10X_HD
#endif 

#ifdef __cplusplus
#define restricted
#endif

    
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "utils.h"
#include "tm1637.h"
    
void Cron_Handler(void);


    // _EREG_ Flags    
#define _BT7F_  0 // Basic Timer7 Flag
#define _RTCAF_ 1 // RTC Alarm Flag
#define _RTCSF_ 2 // RTC Second Flag
#define _RDF_   3 // Run Display Flag
#define _DDF_   4 // Display Delay Flag



#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
