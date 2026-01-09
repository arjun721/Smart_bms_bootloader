#include "main.h"
#include <stddef.h> // for NULL

extern float temperatureC[10];
struct oz3717_chip oz_data;
extern float  system_currentx;
float temp;
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
        App_BmsPorEnOn,
        App_BmsEnable,
        App_BmsNtcEnOn,
    #endif

    #if SPI_ENABLE == 1
        App_SpiAfeInit,    
        App_BmsEnable,
    #endif

    #if TIMER_ENABLE == 1
        App_TimerInit,
        App_TimerStart,
    #endif

    #if FDCAN_ENABLE == 1
        App_CanInit,
    #endif

    #if ADC_ENABLE == 1
        App_AdcInit,
        App_AdcSQRCfg,
    #endif
    
    #if RS485_ENABLE == 1
        App_Rs485UartCfg,
        App_UartInit,
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

void App_BmsTempControl(void)
{
    temp = system_currentx;
    temp = (temp *(-1));
  // this will monitor the external ntc;
  for (int i = 0; i < 5; i++) 
  {
    if(temperatureC[i] <= 0.0f || temperatureC[i] > 4000.0f || temperatureC[5] > 4000 || temperatureC[6] > 4000
      || temperatureC[7] >4000 || temperatureC[8] >4000) 
       {
        //o2m_register_write(OZ3717_FETCTRL,OZ3717_FETCTRL_CHG_DISABLE|OZ3717_FETCTRL_DSG_DISABLE);
        break;  // No need to check further
       }
    else if( temp >(float)(1600))
    {
        //o2m_register_write(OZ3717_FETCTRL,OZ3717_FETCTRL_CHG_DISABLE|OZ3717_FETCTRL_DSG_DISABLE);
    }
     else  
       {
       o2m_register_write(OZ3717_FETCTRL,OZ3717_FETCTRL_CHG_DISABLE|OZ3717_FETCTRL_DSG_ENABLE);
       }
  }
}


 
 
