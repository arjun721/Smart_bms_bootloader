#ifndef _MCAL_ADC_INTERFACE_H_
#define _MCAL_ADC_INTERFACE_H_

#include <math.h>
#include <stdio.h>
#include "adc.h"
#include "gpio.h"
#include "reset.h"

#define BMS_EXT_NTC1_PORT      GpioPortB
#define BMS_EXT_NTC1_PIN       GpioPin10
#define BMS_EXT_NTC2_PORT      GpioPortB
#define BMS_EXT_NTC2_PIN       GpioPin11
#define BMS_EXT_NTC3_PORT      GpioPortC
#define BMS_EXT_NTC3_PIN       GpioPin4
#define BMS_EXT_NTC4_PORT      GpioPortC
#define BMS_EXT_NTC4_PIN       GpioPin5
#define BMS_INT_NTC_PORT       GpioPortA
#define BMS_INT_NTC_PIN        GpioPin1
#define BMS_CM_EN_PORT         GpioPortC
#define BMS_CM_EN_PIN          GpioPin0




void 
App_AdcPortInit(void);

void 
App_AdcInit(void);

void 
App_AdcSQRCfg(void);

void 
get_temp(uint32_t u32adcresult[5], float tempC[5]);
 





#endif