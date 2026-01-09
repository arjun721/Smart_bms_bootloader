
/**********Include files*********/
#include "mcal_sysctrl_interface.h"
#include "mcal_gpio_interface.h"

 
/**********Globel variable decliration*********/
volatile uint32_t uwTick;
 
 
void App_ClkInit(void)
{
    stc_sysctrl_pll_cfg_t stcPLLCfg;
    stc_sysctrl_clk_cfg_t stcClkCfg;

    // Step 1: Initialize XTH (8 MHz crystal)
    App_SystemClkInit_XTH(SysctrlXthFreq4_8MHz);

    // Step 2: Flash wait cycle for >24 MHz
    M0P_FLASH->BYPASS = 0x5A5A;\
    M0P_FLASH->BYPASS = 0xA5A5; 
    M0P_FLASH->CR_f.WAIT = 1u;

    // Step 3: Configure PLL to 8 MHz × 6 = 48 MHz
    stcPLLCfg.enInFreq    = SysctrlPllInFreq6_12MHz;
    stcPLLCfg.enOutFreq   = SysctrlPllOutFreq24_36MHz;
    stcPLLCfg.enPllClkSrc = SysctrlPllXthXtal;
    stcPLLCfg.enPllMul    = SysctrlPllMul6;

    Sysctrl_SetPLLFreq(&stcPLLCfg);
    Sysctrl_SetPLLStableTime(SysctrlPllStableCycle16384);
    Sysctrl_ClkSourceEnable(SysctrlClkPLL, TRUE);
  
      // Step 5: Wait until PLL is ready

    // Step 4: Set system and peripheral clocks
    stcClkCfg.enClkSrc  = SysctrlClkPLL;      // PLL = 48 MHz
    stcClkCfg.enHClkDiv = SysctrlHclkDiv2;    // HCLK = 24 MHz
    stcClkCfg.enPClkDiv = SysctrlPclkDiv1;    // PCLK = 24 MHz

    Sysctrl_ClkInit(&stcClkCfg);
}
 

 

void App_SystemClkInit_XTH(en_sysctrl_xth_freq_t enXthFreq)
{
    if(SysctrlXthFreq24_32MHz == enXthFreq)
    {
          
       M0P_FLASH->BYPASS = 0x5A5A;\
       M0P_FLASH->BYPASS = 0xA5A5; 
       M0P_FLASH->CR_f.WAIT = 1u;
    }
    
    Sysctrl_SetXTHFreq(enXthFreq);
    Sysctrl_XTHDriverCfg(SysctrlXtalDriver1);
    Sysctrl_SetXTHStableTime(SysctrlXthStableCycle16384);
    Sysctrl_ClkSourceEnable(SysctrlClkXTH, TRUE);
    Sysctrl_SysClkSwitch(SysctrlClkXTH);
    
    if(SysctrlXthFreq24_32MHz != enXthFreq)
    {
       M0P_FLASH->CR_f.WAIT = 0u;   
    }
}


void 
SysTick_Handler(void)
{
  App_IncTick();
  
 static boolean_t bFlag = TRUE;
    
    if(TRUE == bFlag)
    {
      bFlag = FALSE;
    //  App_BmsLedOn();  
    }
    else
    {
       bFlag = TRUE;
      // App_BmsLedOff();    
    }
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
 
void App_Delay(uint32_t ms)
{
    volatile uint32_t i, j;
    for (i = 0; i < ms; i++)  // outer loop for milliseconds
    {
        for (j = 0; j < 3000; j++)  // tuned for ~1ms at 24 MHz
        {
           // __NOP();  // prevents optimization
        }
    }
}

  
 