
/**********Include files*********/
#include "mcal_sysctrl_interface.h"
#include "mcal_gpio_interface.h"
 
 
/**********Globel variable decliration*********/
volatile uint32_t uwTick;
 volatile boolean_t current_tick;


void
App_ClkInit(void)
{
   stc_sysctrl_clk_cfg_t stcClkCfg;
//  // Set the system clock source (e.g., external high osc)
   stcClkCfg.enClkSrc = SysctrlClkXTH;  // 8Mhz
//  // Set HCLK divider (e.g., divide by 1)
   stcClkCfg.enHClkDiv = SysctrlHclkDiv1;  // main clock --> 8Mhz
//  // Set PCLK divider (e.g., divide by 1)
   stcClkCfg.enPClkDiv = SysctrlPclkDiv1;  // pherh clock --> 8Mhz
//  // Call the init function with the configured structure
   Sysctrl_ClkInit(&stcClkCfg);
}


void 
App_InitClkout(void)
{
  stc_gpio_cfg_t pstcGpioCfg;
  // Enable GPIO peripheral clock
  Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE);
  // Configure GPIO as output with high drive, no pull-up/down, push-pull mode
  pstcGpioCfg.enDir = GpioDirOut;         // Output direction
  pstcGpioCfg.enDrv = GpioDrvH;           // High drive capability
  pstcGpioCfg.enPu  = GpioPuDisable;      // Disable pull-up resistor
  pstcGpioCfg.enPd  = GpioPdDisable;      // Disable pull-down resistor
  pstcGpioCfg.enOD  = GpioOdDisable;      // Disable open-drain

  // Initialize PA01 and PA02 with the above configuration
  Gpio_Init(GpioPortA, GpioPin1, &pstcGpioCfg);   // Configure PA01
  Gpio_Init(GpioPortA, GpioPin2, &pstcGpioCfg);   // Configure PA02

  // Enable HCLK output on PA01 (for clock monitoring/debugging)
  Gpio_SfHClkOutputCfg(GpioSfHclkOutEnable, GpioSfHclkOutDiv1);  // Output HCLK without division
  Gpio_SetAfMode(GpioPortA, GpioPin1, GpioAf6);                  // Set PA01 to alternate function 6 (HCLK out)

  // Enable PCLK output on PA02 (for peripheral clock monitoring)
  Gpio_SfPClkOutputCfg(GpioSfPclkOutEnable, GpioSfPclkOutDiv1);  // Output PCLK without division
  Gpio_SetAfMode(GpioPortA, GpioPin2, GpioAf6);                  // Set PA02 to alternate function 6 (PCLK out)
}


void 
SysTick_Handler(void)
{
  App_IncTick();
}

void 
App_SystickInit(void)
{
  SystemInit();
  SysTick_Config(SYSTICK_TICKS(SYSTICK_TICK_FREQ_HZ));
}

void 
App_IncTick(void)
{
  uwTick += SYSTICK_INC_TICKS;
}

uint32_t
App_GetTick(void)
{
  return uwTick;
}
  
 