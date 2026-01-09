#include "main.h"
#include <stddef.h> // for NULL

 
 
typedef void (*InitFunc)(void);

// Initialization function table
InitFunc init_table[] = {
  
    #if SYSCTRL_ENABLE == 1
        App_ClkInit,
        App_SystickInit,
    #endif

    #if GPIO_ENABLE == 1
        App_GpioInit,
        App_GpioIsrInit,
    #endif

    #if TIMER_ENABLE == 1
        App_TimerInit,
        App_TimerStart,
    #endif

    #if FDCAN_ENABLE == 1
        App_CanInit,
    #endif
   
    NULL  //Marks end of function list
};


//System init function
void App_BmsInit(void)
{
    for (int i = 0; init_table[i] != NULL; i++) {
        init_table[i]();
    }
}

 

 
 
