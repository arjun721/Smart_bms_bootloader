#include "mcal_gpio_interface.h"
  uint8_t datacan;
 
void App_GpioInit(void)
{
     stc_gpio_cfg_t GpioInitStruct;
     DDL_ZERO_STRUCT(GpioInitStruct);
  
     Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
   
     GpioInitStruct.enDrv  = GpioDrvH;
     GpioInitStruct.enDir  = GpioDirOut;
  
     Gpio_Init(BMS_LED_OUTPUT_PORT, BMS_LED_OUTPUT_PIN, &GpioInitStruct);
     Gpio_WriteOutputIO(BMS_LED_OUTPUT_PORT, BMS_LED_OUTPUT_PIN, FALSE);      
  
     Gpio_Init(BMS_WAKUP_OUTPUT_PORT, BMS_WAKUP_OUTPUT_PIN, &GpioInitStruct);
     Gpio_WriteOutputIO(BMS_WAKUP_OUTPUT_PORT, BMS_WAKUP_OUTPUT_PIN, FALSE); 
 
     Gpio_Init(BMS_ALERT_OUTPUT_PORT, BMS_ALERT_OUTPUT_PIN, &GpioInitStruct);
     Gpio_WriteOutputIO(BMS_ALERT_OUTPUT_PORT, BMS_ALERT_OUTPUT_PIN, FALSE);
 
     Gpio_Init(BMS_DISPLAY_OUTPUT_PORT, BMS_DISPLAY_OUTPUT_PIN, &GpioInitStruct);
     Gpio_WriteOutputIO(BMS_DISPLAY_OUTPUT_PORT, BMS_DISPLAY_OUTPUT_PIN, FALSE);
 
     Gpio_Init(BMS_BLE_OUTPUT_PORT, BMS_BLE_OUTPUT_PIN, &GpioInitStruct);
     Gpio_WriteOutputIO(BMS_BLE_OUTPUT_PORT, BMS_BLE_OUTPUT_PIN, FALSE);
 
     Gpio_Init(BMS_RRECHG_OUTPUT_PORT, BMS_RRECHG_OUTPUT_PIN, &GpioInitStruct);
     Gpio_WriteOutputIO(BMS_RRECHG_OUTPUT_PORT, BMS_RRECHG_OUTPUT_PIN, FALSE);
 
     Gpio_Init(BMS_POR_EN_OUTPUT_PORT, BMS_POR_EN_OUTPUT_PIN, &GpioInitStruct);
     Gpio_WriteOutputIO(BMS_POR_EN_OUTPUT_PORT, BMS_POR_EN_OUTPUT_PIN, FALSE);
 
     Gpio_Init(BMS_RS485_EN_OUTPUT_PORT, BMS_RS485_EN_OUTPUT_PIN, &GpioInitStruct);
     Gpio_WriteOutputIO(BMS_RS485_EN_OUTPUT_PORT, BMS_RS485_EN_OUTPUT_PIN, FALSE);
 
     Gpio_Init(BMS_NTC_EN_OUTPUT_PORT, BMS_NTC_EN_OUTPUT_PIN, &GpioInitStruct);
     Gpio_WriteOutputIO(BMS_NTC_EN_OUTPUT_PORT, BMS_NTC_EN_OUTPUT_PIN, FALSE);
     
     Gpio_Init(BMS_POR_SENSE_OUTPUT_PORT, BMS_POR_SENSE_OUTPUT_PIN, &GpioInitStruct);
     Gpio_WriteOutputIO(BMS_POR_SENSE_OUTPUT_PORT, BMS_POR_SENSE_OUTPUT_PIN, FALSE);     
}  


void App_GpioIsrInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
 
    stcGpioCfg.enDir = GpioDirIn;
    stcGpioCfg.enDrv = GpioDrvL;
 
    stcGpioCfg.enPu = GpioPuDisable;
    stcGpioCfg.enPd = GpioPdDisable;
 
    stcGpioCfg.enOD = GpioOdDisable;
    stcGpioCfg.enCtrlMode = GpioAHB;
    Gpio_Init(BMS_AFE_INT_IRQ_PORT, BMS_AFE_INT_IRO_PIN, &stcGpioCfg); 
  
    Gpio_EnableIrq(BMS_AFE_INT_IRQ_PORT, BMS_AFE_INT_IRO_PIN, GpioIrqFalling);
  
    EnableNvic(PORTB_IRQn, IrqLevel3, TRUE);

}
void App_BmsLedOn(void)
{
    Gpio_SetIO(BMS_LED_OUTPUT_PORT, BMS_LED_OUTPUT_PIN);
}

void App_BmsLedOff(void)
{
    Gpio_ClrIO(BMS_LED_OUTPUT_PORT, BMS_LED_OUTPUT_PIN);
}

void App_BmsEnable(void)
{
    Gpio_SetIO(BMS_WAKUP_OUTPUT_PORT, BMS_WAKUP_OUTPUT_PIN);
}

void App_BmsDisable(void)
{
    Gpio_ClrIO(BMS_WAKUP_OUTPUT_PORT, BMS_WAKUP_OUTPUT_PIN);
}

void App_BmsAlertOn(void)
{
    Gpio_SetIO(BMS_ALERT_OUTPUT_PORT, BMS_ALERT_OUTPUT_PIN);
}

void App_BmsAlertOff(void)
{
    Gpio_ClrIO(BMS_ALERT_OUTPUT_PORT, BMS_ALERT_OUTPUT_PIN);
}

void App_BmsDisplayOn(void)
{
    Gpio_SetIO(BMS_DISPLAY_OUTPUT_PORT, BMS_DISPLAY_OUTPUT_PIN);
}

void App_BmsDisplayOff(void)
{
    Gpio_ClrIO(BMS_DISPLAY_OUTPUT_PORT, BMS_DISPLAY_OUTPUT_PIN);
}

void App_BmsBleOn(void)
{
    Gpio_SetIO(BMS_BLE_OUTPUT_PORT, BMS_BLE_OUTPUT_PIN);
}

void App_BmsBleOff(void)
{
    Gpio_ClrIO(BMS_BLE_OUTPUT_PORT, BMS_BLE_OUTPUT_PIN);
}

void App_BmsPrechgOn(void)
{
    Gpio_SetIO(BMS_RRECHG_OUTPUT_PORT, BMS_RRECHG_OUTPUT_PIN);
}

void App_BmsPrechgOff(void)
{
    Gpio_ClrIO(BMS_RRECHG_OUTPUT_PORT, BMS_RRECHG_OUTPUT_PIN);
}

void App_BmsPorEnOn(void)
{
    Gpio_SetIO(BMS_POR_EN_OUTPUT_PORT, BMS_POR_EN_OUTPUT_PIN);
}

void App_BmsPorEnOff(void)
{
    Gpio_ClrIO(BMS_POR_EN_OUTPUT_PORT, BMS_POR_EN_OUTPUT_PIN);
}

void App_BmsRs485EnOn(void)
{
    Gpio_SetIO(BMS_RS485_EN_OUTPUT_PORT, BMS_RS485_EN_OUTPUT_PIN);
}

void App_BmsRs485EnOff(void)
{
    Gpio_ClrIO(BMS_RS485_EN_OUTPUT_PORT, BMS_RS485_EN_OUTPUT_PIN);
}

void App_BmsNtcEnOn(void)
{
   Gpio_ClrIO(BMS_NTC_EN_OUTPUT_PORT, BMS_NTC_EN_OUTPUT_PIN);
}

void App_BmsNtcEnOff(void)
{
    Gpio_SetIO(BMS_NTC_EN_OUTPUT_PORT, BMS_NTC_EN_OUTPUT_PIN);
}

void App_BmsPorOn(void)
{
    Gpio_SetIO(BMS_POR_SENSE_OUTPUT_PORT, BMS_POR_SENSE_OUTPUT_PIN);
}

void App_BmsPorOff(void)
{
  Gpio_ClrIO(BMS_POR_SENSE_OUTPUT_PORT, BMS_POR_SENSE_OUTPUT_PIN);
}
void test(void)
{
   datacan = Gpio_ReadOutputIO(BMS_COMM_MODE_INPUT_PORT,BMS_COMM_MODE_INPUT_PIN);
}
 
 