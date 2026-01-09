#include "main.h"
#include "uart.h"

extern volatile uint8_t u8OneSecToggle;
 

int32_t main(void)
{
   App_BmsInit();
 
   o2_es_init();
   delay1ms(1);
  
   while(1)
   {
      if(u8OneSecToggle)
      {
        o2_es_entry();
      }
      
       App_BmsTempControl();
 
       App_TransmitSysCurrent();
      
       App_TransmitCellVolt(M0P_UART0);
      
       App_TransmitFetctrlStatus(M0P_UART0);
       
      delay1ms(1);
    }
   }

      
 


 

 
   
   
 