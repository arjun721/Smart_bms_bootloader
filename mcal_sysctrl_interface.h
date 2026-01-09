
#ifndef _MCAL_SYSCTRL_INTERFACE_H_
#define _MCAL_SYSCTRL_INTERFACE_H_

#include "gpio.h"
#include "sysctrl.h"

#define SYSTICK_INC_TICKS          (1U)

// Tick frequency in Hz (e.g., 1000 for 1ms tick)
#define SYSTICK_TICK_FREQ_HZ      (1000U)

// Macro to compute SysTick reload value based on desired tick frequency
#define SYSTICK_TICKS(FREQ_HZ)    ((SystemCoreClock) / (FREQ_HZ))


void 
App_ClkInit(void);
 
void 
App_InitClkout(void);

void
App_IncTick(void);

uint32_t
App_GetTick(void);

void 
App_SystickInit(void);



#endif