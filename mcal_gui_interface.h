#ifndef _MCAL_GUI_INTERFACE_H_
#define _MCAL_GUI_INTERFACE_H_

#include <math.h>
#include <stdio.h>
#include "uart.h"
#include "gpio.h"
#include "reset.h"

#define BMS_UART_TX_PORT   GpioPortA
#define BMS_UART_TX_PIN    GpioPin9
#define BMS_UART_RX_PORT   GpioPortA
#define BMS_UART_RX_PIN    GpioPin10

void 
App_UartInit(void);
  
void 
App_UartPortInit(void);

void
App_UartTransmit(M0P_UART_TypeDef* UARTx, uint8_t* u8data);

void 
App_TransmitCellVolt(M0P_UART_TypeDef* UARTx);

void 
App_TransmitFetctrlStatus(M0P_UART_TypeDef* UARTx);

void 
App_TransmitSysCurrent(void);


#endif