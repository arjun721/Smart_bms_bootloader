#include "mcal_gui_interface.h"

uint8_t uart_count;
extern uint16_t cell_volt[17];
extern uint16_t status1x;
extern uint16_t fetCtrlx;
extern float  system_currentx;



//void UART0_2_IRQHandler(void)
//{
//   if (Uart_GetStatus(M0P_UART0, UartTC))
//   {
//        Uart_DisableIrq(M0P_UART0, UartTxIrq);
//        //Uart_EnableIrq(M0P_UART0, UartRxIrq);  // Ready to receive next
//        // Optional: set flag for main loop that TX is done
//    }
//    else
//    {
//          
//    }
//    Uart_ClrStatus(M0P_UART0, UartTC);
//}



void App_UartPortInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    DDL_ZERO_STRUCT(stcGpioCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE); 
    
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(GpioPortA,GpioPin9,&stcGpioCfg);
    Gpio_SetAfMode(GpioPortA,GpioPin9,GpioAf1); //PA09-UART0 TX
  
    stcGpioCfg.enDir = GpioDirIn;
    Gpio_Init(GpioPortA,GpioPin10,&stcGpioCfg);
    Gpio_SetAfMode(GpioPortA,GpioPin10,GpioAf1); //PA10-UART0 RX
}

 
void App_UartInit(void)
{
    App_UartPortInit();
  
    stc_uart_cfg_t  stcCfg;
    stc_uart_multimode_t stcMulti;
    stc_uart_baud_t stcBaud;

    DDL_ZERO_STRUCT(stcCfg);
    DDL_ZERO_STRUCT(stcMulti);
    DDL_ZERO_STRUCT(stcBaud);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralUart0,TRUE);//UART0 
    
    stcCfg.enRunMode = UartMskMode3;     //??3
    stcCfg.enStopBit = UartMsk1bit;      //1????
    stcCfg.enMmdorCk = UartMskEven;      //???
    stcCfg.stcBaud.u32Baud = 9600;       //???9600
    stcCfg.stcBaud.enClkDiv = UartMsk8Or16Div;         //????????
    stcCfg.stcBaud.u32Pclk = Sysctrl_GetPClkFreq();    //??????(PCLK)???
    Uart_Init(M0P_UART0, &stcCfg);       //?????

    Uart_ClrStatus(M0P_UART0,UartRC);    //?????
    Uart_ClrStatus(M0P_UART0,UartTC);    //?????
    Uart_EnableIrq(M0P_UART0,UartRxIrq); //????????
    Uart_EnableIrq(M0P_UART0,UartTxIrq); //????????
     
   // EnableNvic(UART0_2_IRQn, IrqLevel3, TRUE);       ///<??????
}
 
void App_UartTransmit(M0P_UART_TypeDef* UARTx, uint8_t* u8data)
{
    Uart_SendDataIt(UARTx, (uint8_t)*u8data);  // Send first byte
    Uart_EnableIrq(UARTx, UartTxIrq);          // Enable TX interrupt
}


void App_TransmitCellVolt(M0P_UART_TypeDef* UARTx)
{
  Uart_SendDataPoll(UARTx, 0xAA);  // start byte
        
  for (int i = 0; i < 17; i++) {
     Uart_SendDataPoll(UARTx, cell_volt[i] & 0xFF);       // LSB
     Uart_SendDataPoll(UARTx, (cell_volt[i] >> 8) & 0xFF); // MSB
  }
}


void App_TransmitFetctrlStatus(M0P_UART_TypeDef* UARTx)
{
  Uart_SendDataPoll(UARTx, 0xAB);  // start byte
   
  Uart_SendDataPoll(UARTx, status1x & 0xFF);       // LSB
  Uart_SendDataPoll(UARTx, (status1x >> 8) & 0xFF); // MSB
  
  Uart_SendDataPoll(UARTx, fetCtrlx & 0xFF);       // LSB
  Uart_SendDataPoll(UARTx, (fetCtrlx >> 8) & 0xFF); // MSB 
  
 
} 
 

void App_TransmitSysCurrent(void) 
{
  uint8_t* float_ptr = (uint8_t*)&system_currentx;

  Uart_SendDataPoll(M0P_UART0, 0xAC);  // start byte
 // Send 4 bytes of float (LSB first if using little-endian)
  for (int i = 0; i < 4; i++) 
  {
    Uart_SendDataPoll(M0P_UART0, float_ptr[i]);
  }
  
}
