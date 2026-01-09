
#ifndef _MCAL_GPIO_INTERFACE_H_
#define _MCAL_GPIO_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gpio.h"

// BMS Output Pin Configuration
#define BMS_LED_OUTPUT_PORT        GpioPortB
#define BMS_LED_OUTPUT_PIN         GpioPin1
#define BMS_WAKUP_OUTPUT_PORT      GpioPortB
#define BMS_WAKUP_OUTPUT_PIN       GpioPin0
#define BMS_ALERT_OUTPUT_PORT      GpioPortC
#define BMS_ALERT_OUTPUT_PIN       GpioPin14
#define BMS_DISPLAY_OUTPUT_PORT    GpioPortC
#define BMS_DISPLAY_OUTPUT_PIN     GpioPin13
#define BMS_BLE_OUTPUT_PORT        GpioPortC
#define BMS_BLE_OUTPUT_PIN         GpioPin15
#define BMS_RRECHG_OUTPUT_PORT     GpioPortC
#define BMS_RRECHG_OUTPUT_PIN      GpioPin1
#define BMS_POR_EN_OUTPUT_PORT     GpioPortA
#define BMS_POR_EN_OUTPUT_PIN      GpioPin0
#define BMS_RS485_EN_OUTPUT_PORT   GpioPortC 
#define BMS_RS485_EN_OUTPUT_PIN    GpioPin8
#define BMS_NTC_EN_OUTPUT_PORT     GpioPortB
#define BMS_NTC_EN_OUTPUT_PIN      GpioPin5

#define BMS_POR_SENSE_OUTPUT_PORT   GpioPortB   
#define BMS_POR_SENSE_OUTPUT_PIN    GpioPin2

// BMS Input Pin Configuration
#define BMS_POR_SENSE_INPUT_PORT   GpioPortB   
#define BMS_POR_SENSE_INPUT_PIN    GpioPin2
#define BMS_COMM_MODE_INPUT_PORT   GpioPortC
#define BMS_COMM_MODE_INPUT_PIN    GpioPin9
#define BMS_CURR_SWT_INPUT_PORT    GpioPortA
#define BMS_CURR_SWT_INPUT_PIN     GpioPin15
#define BMS_CURR_SWT1_INPUT_PORT   GpioPortC
#define BMS_CURR_SWT1_INPUT_PIN    GpioPin10
#define BMS_CHG_DET_INPUT_PORT     GpioPortD
#define BMS_CHG_DET_INPUT_PIN      GpioPin2

// BMS IRQ Pin Configuration
#define BMS_AFE_INT_IRQ_PORT      GpioPortB
#define BMS_AFE_INT_IRO_PIN       GpioPin7

void test(void);
void 
App_GpioInit(void);

void 
App_GpioIsrInit(void);

void 
App_BmsLedOn(void);
 
void 
App_BmsLedOff(void);
 
void 
App_BmsEnable(void);
 
void 
App_BmsDisable(void);

void 
App_BmsAlertOn(void);

void
App_BmsAlertOff(void);

void 
App_BmsDisplayOn(void);

void 
App_BmsDisplayOff(void);
 
void 
App_BmsBleOn(void);

void 
App_BmsBleOff(void);
 
void 
App_BmsPrechgOn(void);

void 
App_BmsPrechgOff(void);
 
void 
App_BmsPorEnOn(void);

void 
App_BmsPorEnOff(void);
 
void 
App_BmsRs485EnOn(void);

void 
App_BmsRs485EnOff(void);
 
void 
App_BmsNtcEnOn(void);

void 
App_BmsNtcEnOff(void);

void 
App_BmsPorOn(void);
 
void 
App_BmsPorOff(void);


#ifdef __cplusplus
}
#endif
 
#endif