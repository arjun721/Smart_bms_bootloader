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

#define VREF        3.3f
#define ADC_MAX     4095.0f
#define R_FIXED     10000.0f  // 10kO
#define R0_temp          10000.0f  // Thermistor resistance at T0
#define T0_K        298.15f   // 25°C in Kelvin
#define BETA        3950.0f   // From thermistor datasheet

void 
App_AdcPortInit(void);

void 
App_AdcInit(void);

void 
App_AdcSQRCfg(void);

void App_GetTempPoll(void);
void App_GetTemp(int16_t* temp);
void App_AdcTemp(uint32_t u32adcresult[5], int16_t* tempC);
 

 
#endif