#include "main.h"
#include "uart.h"

int32_t main(void)
{
   App_BmsInit();
 
   delay1ms(1);
  
   while(1)
   {
 
       App_BmsLedOn();  
   
       delay1ms(100);
 
       App_BmsLedOff();     
       delay1ms(100);
    }
   }

      
 


 

 
   
   
 