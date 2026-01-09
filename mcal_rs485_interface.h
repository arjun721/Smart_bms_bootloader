#ifndef _MCAL_RS485_INTERFACE_H_
#define _MCAL_RS485_INTERFACE_H_

#include <math.h>
#include <stdio.h>
#include "uart.h"
#include "gpio.h"
#include "reset.h"

#define BMS_RS485_TX_PORT   GpioPortC
#define BMS_RS485_TX_PIN    GpioPin7
#define BMS_RS485_RX_PORT   GpioPortC
#define BMS_RS485_RX_PIN    GpioPin6


void
App_Rs485PortInit(void);

void 
App_Rs485UartCfg(void);

void 
App_TransmitDataPoll(uint8_t u8data);

void 
App_BmsRs485TxMode(void);
 
void 
App_BmsRs485RxMode(void);

#endif