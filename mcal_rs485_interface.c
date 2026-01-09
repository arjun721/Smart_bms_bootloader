#include "mcal_rs485_interface.h"
#include "mcal_gpio_interface.h"

void App_Rs485PortInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    DDL_ZERO_STRUCT(stcGpioCfg); 
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);   
    
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(BMS_RS485_TX_PORT,BMS_RS485_TX_PIN,&stcGpioCfg);
    Gpio_SetAfMode(BMS_RS485_TX_PORT,BMS_RS485_TX_PIN,GpioAf6);              
    stcGpioCfg.enDir = GpioDirIn;
    Gpio_Init(BMS_RS485_RX_PORT,BMS_RS485_RX_PIN,&stcGpioCfg);
    Gpio_SetAfMode(BMS_RS485_RX_PORT,BMS_RS485_RX_PIN,GpioAf6);             
}

void App_Rs485UartCfg(void)
{
    App_Rs485PortInit();
  
    stc_uart_cfg_t  stcCfg;
    stc_uart_multimode_t stcMulti;
    stc_uart_baud_t stcBaud;

    DDL_ZERO_STRUCT(stcCfg);
    DDL_ZERO_STRUCT(stcMulti);
    DDL_ZERO_STRUCT(stcBaud);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralUart3,TRUE); 
    
    stcCfg.enRunMode = UartMskMode1;      
    stcCfg.enStopBit = UartMsk1bit;       
    stcCfg.enMmdorCk = UartMskEven;       
    stcCfg.stcBaud.u32Baud = 9600;        
    stcCfg.stcBaud.enClkDiv = UartMsk8Or16Div;          
    stcCfg.stcBaud.u32Pclk = Sysctrl_GetPClkFreq();     
    Uart_Init(M0P_UART3, &stcCfg);        

    Uart_ClrStatus(M0P_UART3,UartRC);     
    Uart_ClrStatus(M0P_UART3,UartTC);     
    Uart_EnableIrq(M0P_UART3,UartRxIrq);  
    Uart_EnableIrq(M0P_UART3,UartTxIrq);  

}


void App_TransmitDataPoll(uint8_t u8data)
{
   App_BmsRs485TxMode();
   Uart_SendDataPoll(M0P_UART3, u8data);
   App_BmsRs485RxMode();  
}
 

// Enable RS485 Transmit mode (RE/DE = HIGH)
void App_BmsRs485TxMode(void)
{
    App_BmsRs485EnOn();  
}

// Enable RS485 Receive mode (RE/DE = LOW)
void App_BmsRs485RxMode(void)
{
    App_BmsRs485EnOff();  
}

