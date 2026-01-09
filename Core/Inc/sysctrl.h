/******************************************************************************
 * Copyright (C) 2021, Xiaohua Semiconductor Co., Ltd. All rights reserved.
 *
 * This software component is licensed by XHSC under BSD 3-Clause license
 * (the "License"); You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                    opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************/

/******************************************************************************
 * @file   sysctrl.h
 *
 * @brief  Header file for SYSCTRL functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

#ifndef __SYSCTRL_H__
#define __SYSCTRL_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"


#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup SysCtrlGroup (SYSCTRL)
  **
 ******************************************************************************/
//@{

/**
 *******************************************************************************
 ** function prototypes.
 ******************************************************************************/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/
     
/**
 *******************************************************************************
 ** \brief ???????????
 ** \note
 ******************************************************************************/
typedef enum en_sysctrl_clk_source
{
    SysctrlClkRCH  = 0u,               ///< ??????
    SysctrlClkXTH  = 1u,               ///< ??????
    SysctrlClkRCL  = 2u,               ///< ??????
    SysctrlClkXTL  = 3u,               ///< ??????
    SysctrlClkPLL  = 4u,               ///< PLL??
    SysctrlClkRC48M = 0x48,             ///< RC48M?? 
}en_sysctrl_clk_source_t;

/**
 *******************************************************************************
 ** \brief RCH?????????
 ******************************************************************************/
typedef enum en_sysctrl_rch_freq
{
    SysctrlRchFreq4MHz     = 4u,               ///< 4MHz
    SysctrlRchFreq8MHz     = 3u,               ///< 8MHz
    SysctrlRchFreq16MHz    = 2u,               ///< 16MHz
    SysctrlRchFreq22_12MHz = 1u,               ///< 22.12MHz
    SysctrlRchFreq24MHz    = 0u,               ///< 24MHz
	SysctrlRchFreq48MHz    = 0x48u,			   ///< 48MHz
}en_sysctrl_rch_freq_t;

/**
 *******************************************************************************
 ** \brief XTAL????????
 ******************************************************************************/
typedef enum en_sysctrl_xtal_driver
{
    SysctrlXtalDriver0 = 0u,                ///< ??????
    SysctrlXtalDriver1 = 1u,                ///< ?????
    SysctrlXtalDriver2 = 2u,                ///< ??????
    SysctrlXtalDriver3 = 3u,                ///< ??????
}en_sysctrl_xtal_driver_t;

/**
 *******************************************************************************
 ** \brief XTH???????????
 ******************************************************************************/
typedef enum en_sysctrl_xth_freq
{
    SysctrlXthFreq4_8MHz   = 0u,                ///< 4~8MHz
    SysctrlXthFreq8_16MHz  = 1u,                ///< 8~16MHz
    SysctrlXthFreq16_24MHz = 2u,                ///< 16~24MHz
    SysctrlXthFreq24_32MHz = 3u,                ///< 24~32MHz
}en_sysctrl_xth_freq_t;

/**
 *******************************************************************************
 ** \brief XTH???????????
 ******************************************************************************/
typedef enum en_sysctrl_xth_cycle
{
    SysctrlXthStableCycle256   = 0u,              ///< 256 ????
    SysctrlXthStableCycle1024  = 1u,              ///< 1024 ????
    SysctrlXthStableCycle4096  = 2u,              ///< 4096 ????
    SysctrlXthStableCycle16384 = 3u,              ///< 16384 ????
}en_sysctrl_xth_cycle_t;

/**
 *******************************************************************************
 ** \brief RCL?????????
 ******************************************************************************/
typedef enum en_sysctrl_rcl_freq
{
    SysctrlRclFreq32768    = 0x11u,               ///< 32.768KHz
    SysctrlRclFreq38400    = 0x10u,               ///< 38.4KHz
}en_sysctrl_rcl_freq_t;

/**
 *******************************************************************************
 ** \brief RCL???????????
 ******************************************************************************/
typedef enum en_sysctrl_rcl_cycle
{
    SysctrlRclStableCycle4   = 0u,                  ///< 4 ????
    SysctrlRclStableCycle16  = 1u,                  ///< 16 ????
    SysctrlRclStableCycle64  = 2u,                  ///< 64 ????
    SysctrlRclStableCycle256 = 3u,                  ///< 256 ????
}en_sysctrl_rcl_cycle_t;

/**
 *******************************************************************************
 ** \brief XTL???????????
 ******************************************************************************/
typedef enum en_sysctrl_xtl_cycle
{
    SysctrlXtlStableCycle256   = 0u,                  ///< 256 ????
    SysctrlXtlStableCycle1024  = 1u,                  ///< 1024 ????
    SysctrlXtlStableCycle4096  = 2u,                  ///< 4096 ????
    SysctrlXtlStableCycle16384 = 3u,                  ///< 16384 ????
}en_sysctrl_xtl_cycle_t;

/**
 *******************************************************************************
 ** \brief XTL??????????
 ******************************************************************************/
typedef enum en_sysctrl_xtl_amp
{
    SysctrlXtlAmp0 = 0u,                ///< ????
    SysctrlXtlAmp1 = 1u,                ///< ???
    SysctrlXtlAmp2 = 2u,                ///< ????
    SysctrlXtlAmp3 = 3u,                ///< ????
}en_sysctrl_xtl_amp_t;

/**
 *******************************************************************************
 ** \brief PLL???????????
 ******************************************************************************/
typedef enum en_sysctrl_pll_cycle
{
    SysctrlPllStableCycle128   = 0u,                  ///< 128????
    SysctrlPllStableCycle256   = 1u,                  ///< 256????
    SysctrlPllStableCycle512   = 2u,                  ///< 512????
    SysctrlPllStableCycle1024  = 3u,                  ///< 1024????
    SysctrlPllStableCycle2048  = 4u,                  ///< 2048????
    SysctrlPllStableCycle4096  = 5u,                  ///< 4096????
    SysctrlPllStableCycle8192  = 6u,                  ///< 8192????
    SysctrlPllStableCycle16384 = 7u,                  ///< 16384????    
}en_sysctrl_pll_cycle_t;

/**
 *******************************************************************************
 ** \brief PLL??????????
 ******************************************************************************/
typedef enum en_sysctrl_pll_infreq
{
    SysctrlPllInFreq4_6MHz   = 0u,                  ///< 4~16MHz
    SysctrlPllInFreq6_12MHz  = 1u,                  ///< 6~12MHz
    SysctrlPllInFreq12_20MHz = 2u,                  ///< 12~20MHz
    SysctrlPllInFreq20_24MHz = 3u,                  ///< 20~24MHz  
}en_sysctrl_pll_infreq_t;

/**
 *******************************************************************************
 ** \brief PLL??????????
 ******************************************************************************/
typedef enum en_sysctrl_pll_outfreq
{
    SysctrlPllOutFreq8_12MHz  = 0u,                 ///< 8~12MHz
    SysctrlPllOutFreq12_18MHz = 1u,                 ///< 12~18MHz
    SysctrlPllOutFreq18_24MHz = 2u,                 ///< 18~24MHz
    SysctrlPllOutFreq24_36MHz = 3u,                 ///< 24~36MHz
    SysctrlPllOutFreq36_48MHz = 4u,                 ///< 36~48MHz
}en_sysctrl_pll_outfreq_t;

/**
 *******************************************************************************
 ** \brief PLL?????????
 ******************************************************************************/
typedef enum en_sysctrl_pll_clksource
{
    SysctrlPllXthXtal   = 0u,                  ///< XTH???????
    SysctrlPllXthIn     = 1u,                  ///< XTH????????
    SysctrlPllRch       = 3u,                  ///< RCH??
}en_sysctrl_pll_clksource_t;

/**
 *******************************************************************************
 ** \brief PLL?????????
 ******************************************************************************/
typedef enum en_sysctrl_pll_mul
{
    SysctrlPllMul2  = 2u,                   ///< 2??
    SysctrlPllMul3  = 3u,                   ///< 3??
    SysctrlPllMul4  = 4u,                   ///< 4??
    SysctrlPllMul5  = 5u,                   ///< 5??
    SysctrlPllMul6  = 6u,                   ///< 6??
    SysctrlPllMul7  = 7u,                   ///< 7??
    SysctrlPllMul8  = 8u,                   ///< 8??
    SysctrlPllMul9  = 9u,                   ///< 9??
    SysctrlPllMul10 = 10u,                  ///< 10??
    SysctrlPllMul11 = 11u,                  ///< 11??
    SysctrlPllMul12 = 12u,                  ///< 12??
}en_sysctrl_pll_mul_t;

/**
 *******************************************************************************
 ** \brief HCLK??????????
 ******************************************************************************/
typedef enum en_sysctrl_hclk_div
{
    SysctrlHclkDiv1   = 0u,              ///< SystemClk
    SysctrlHclkDiv2   = 1u,              ///< SystemClk/2
    SysctrlHclkDiv4   = 2u,              ///< SystemClk/4
    SysctrlHclkDiv8   = 3u,              ///< SystemClk/8
    SysctrlHclkDiv16  = 4u,              ///< SystemClk/16
    SysctrlHclkDiv32  = 5u,              ///< SystemClk/32
    SysctrlHclkDiv64  = 6u,              ///< SystemClk/64
    SysctrlHclkDiv128 = 7u,              ///< SystemClk/128
}en_sysctrl_hclk_div_t;

/**
 *******************************************************************************
 ** \brief PCLK????
 ******************************************************************************/
typedef enum en_sysctrl_pclk_div
{
    SysctrlPclkDiv1 = 0u,                ///< HCLK
    SysctrlPclkDiv2 = 1u,                ///< HCLK/2
    SysctrlPclkDiv4 = 2u,                ///< HCLK/4
    SysctrlPclkDiv8 = 3u,                ///< HCLK/8
}en_sysctrl_pclk_div_t;

/**
 *******************************************************************************
 ** \brief RTC????????????????
 ******************************************************************************/
typedef enum en_sysctrl_rtc_adjust
{
    SysctrlRTC4MHz  = 0u,                ///< 4MHz
    SysctrlRTC6MHz  = 1u,                ///< 6MHz
    SysctrlRTC8MHz  = 2u,                ///< 8MHz
    SysctrlRTC12MHz = 3u,                ///< 12MHz
    SysctrlRTC16MHz = 4u,                ///< 16MHz
    SysctrlRTC20MHz = 5u,                ///< 20MHz
    SysctrlRTC24MHz = 6u,                ///< 24MHz
    SysctrlRTC32MHz = 7u,                ///< 32MHz
}en_sysctrl_rtc_adjust_t;

/**
 *******************************************************************************
 ** \brief ????????????????
 ******************************************************************************/
typedef enum en_sysctrl_func
{
    SysctrlEXTHEn           = 1u,                    ///< ???????????????
    SysctrlEXTLEn           = 2u,                    ///< ????????????????
    SysctrlXTLAlwaysOnEn    = 3u,                    ///< ???XTL_EN????
    SysctrlClkFuncRTCLpmEn  = 5u,                    ///< ??RTC?????
    SysctrlCMLockUpEn       = 6u,                    ///< ???CPU?????????MCU
    SysctrlSWDUseIOEn       = 8u,                    ///< SWD????IO??
}en_sysctrl_func_t;

/**
 *******************************************************************************
 ** \brief USB??????????
 ******************************************************************************/
typedef enum en_sysctrl_usbclk_sel
{
    SysctrlUsbClkRch48M      = 0u,                    ///<USB????RCH48M 
    SysctrlUsbClkPll         = 1u,                    ///<USB????PLL 
}en_sysctrl_usbclk_sel_t;


/**
 *******************************************************************************
 ** \brief ?????????????
 ******************************************************************************/
typedef enum en_sysctrl_timer_pllclk_sel
{
    SysctrlTimerClkPll      = 0u,                    ///<?????PLL?,???????? 
    SysctrlTimerClkPll2     = 1u,                    ///<?????PLL?,??????2?? 
}en_sysctrl_timer_pllclk_sel_t;

/**
 *******************************************************************************
 ** \brief ????????????
 ******************************************************************************/
typedef enum en_sysctrl_peripheral_gate
{
    SysctrlPeripheralUart0      = 0u,       ///< ??0
    SysctrlPeripheralUart1      = 1u,       ///< ??1
    SysctrlPeripheralLpUart0    = 2u,       ///< ?????0
    SysctrlPeripheralLpUart1    = 3u,       ///< ?????1
    SysctrlPeripheralI2c0       = 4u,       ///< I2C0
    SysctrlPeripheralI2c1       = 5u,       ///< I2C1
    SysctrlPeripheralSpi0       = 6u,       ///< SPI0
    SysctrlPeripheralSpi1       = 7u,       ///< SPI1
    SysctrlPeripheralBaseTim    = 8u,       ///< ?????TIM0/1/2
    SysctrlPeripheralLpTim0     = 9u,       ///< ??????0
    SysctrlPeripheralAdvTim     = 10u,      ///< ?????TIM4/5/6
    SysctrlPeripheralTim3       = 11u,      ///< ???3
    SysctrlPeripheralOpa        = 13u,      ///< OPA
    SysctrlPeripheralPca        = 14u,      ///< ???????
    SysctrlPeripheralWdt        = 15u,      ///< ???
    SysctrlPeripheralAdcBgr     = 16u,      ///< ADC&BGR
    SysctrlPeripheralVcLvd      = 17u,      ///< VC?LVD
    SysctrlPeripheralRng        = 18u,      ///< RNG
    SysctrlPeripheralPcnt       = 19u,      ///< PCNT
    SysctrlPeripheralRtc        = 20u,      ///< RTC
    SysctrlPeripheralTrim       = 21u,      ///< ????
    SysctrlPeripheralLcd        = 22u,      ///< LCD
    SysctrlPeripheralTick       = 24u,      ///< ?????
    SysctrlPeripheralSwd        = 25u,      ///< SWD
    SysctrlPeripheralCrc        = 26u,      ///< CRC
    SysctrlPeripheralAes        = 27u,      ///< AES
    SysctrlPeripheralGpio       = 28u,      ///< GPIO
    SysctrlPeripheralDma        = 29u,      ///< DMA
    SysctrlPeripheralHdiv       = 30u,      ///< ???
    SysctrlPeripheralFlash      = 31u,      ///< Flash
    SysctrlPeripheralUsb        = 32u,      ///< USB
    SysctrlPeripheralCan        = 33u,      ///< CAN
    SysctrlPeripheralCts        = 34u,      ///< CTS
    SysctrlPeripheralDac        = 35u,      ///< DAC
    SysctrlPeripheralLpTim1     = 36u,      ///< ??????1
    SysctrlPeripheralI2s0       = 37u,      ///< I2S0
    SysctrlPeripheralI2s1       = 38u,      ///< I2S1
    SysctrlPeripheralUart2      = 40u,      ///< UART2
    SysctrlPeripheralUart3      = 41u,      ///< UART3    
}en_sysctrl_peripheral_gate_t;

/**
 *******************************************************************************
 ** \brief ????????????
 ******************************************************************************/
typedef struct
{
    en_sysctrl_clk_source_t  enClkSrc;       ///< ?????
    en_sysctrl_hclk_div_t    enHClkDiv;      ///< HCLK????
    en_sysctrl_pclk_div_t    enPClkDiv;      ///< PCLK????
}stc_sysctrl_clk_cfg_t;

/**
 *******************************************************************************
 ** \brief ????????????
 ******************************************************************************/
typedef struct
{
    en_sysctrl_pll_infreq_t    enInFreq;        ///< PLL??????????
    en_sysctrl_pll_outfreq_t   enOutFreq;       ///< PLL??????????
    en_sysctrl_pll_clksource_t enPllClkSrc;     ///< PLL???????
    en_sysctrl_pll_mul_t       enPllMul;        ///< PLL??????
}stc_sysctrl_pll_cfg_t;

/******************************************************************************
 * Global variable declarations ('extern', definition in C source)
 ******************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
///< ???????API:?????,???????????????????; 
///< ??1:???????????????,??????????????TRIM????????????,
///< ??2:XTH?XTL???????,??????????,
///< ??3:????????:SYSTEM_XTH=8MHz,SYSTEM_XTL=32768Hz,?????????,?????????????
en_result_t Sysctrl_ClkInit(stc_sysctrl_clk_cfg_t *pstcCfg);

///< ????????API:?????????->PCLK=HCLK=SystemClk=RCH4MHz
en_result_t Sysctrl_ClkDeInit(void);

///< ?????????????
///< ??:????????????,??????????????TRIM????????????
en_result_t Sysctrl_ClkSourceEnable(en_sysctrl_clk_source_t enSource, boolean_t bFlag);

///<????????:?????Sysctrl_ClkInit()??,????????????????,?????Sysctrl_ClkInit()??????;
en_result_t Sysctrl_XTHDriverCfg(en_sysctrl_xtal_driver_t enDriver);
en_result_t Sysctrl_XTLDriverCfg(en_sysctrl_xtl_amp_t enAmp, en_sysctrl_xtal_driver_t enDriver);

///<????????:?????Sysctrl_ClkInit()??,?????????????????,??????;
en_result_t Sysctrl_SetXTHStableTime(en_sysctrl_xth_cycle_t enCycle);
en_result_t Sysctrl_SetRCLStableTime(en_sysctrl_rcl_cycle_t enCycle);
en_result_t Sysctrl_SetXTLStableTime(en_sysctrl_xtl_cycle_t enCycle);
en_result_t Sysctrl_SetPLLStableTime(en_sysctrl_pll_cycle_t enCycle);

///<??????????????:????????????Sysctrl_ClkInit()?????????,??????;
///< ??????,???????????Flash?????,????????0?1?2,
///< ??!!!:?HCLK??24MHz?,FLASH???????????1,??????????????
en_result_t Sysctrl_SysClkSwitch(en_sysctrl_clk_source_t enSource);

///< ???????:??????,?????????????;
///< ???????,???????????Flash?????,????????0?1?2,
///< ??XTL??????????,?????
en_result_t Sysctrl_SetRCHTrim(en_sysctrl_rch_freq_t enRCHFreq);
en_result_t Sysctrl_SetRCLTrim(en_sysctrl_rcl_freq_t enRCLFreq);
en_result_t Sysctrl_SetXTHFreq(en_sysctrl_xth_freq_t enXTHFreq);
en_result_t Sysctrl_SetPLLFreq(stc_sysctrl_pll_cfg_t *pstcPLLCfg);

///< ??????:??????,????HCLK?PCLK????;
en_result_t Sysctrl_SetHCLKDiv(en_sysctrl_hclk_div_t enHCLKDiv);
en_result_t Sysctrl_SetPCLKDiv(en_sysctrl_pclk_div_t enPCLKDiv);

///< ??????:??????,????HCLK?PCLK????
uint32_t Sysctrl_GetHClkFreq(void);
uint32_t Sysctrl_GetPClkFreq(void);

///< ??????/????:???????????,??????????,????????????;
en_result_t Sysctrl_SetPeripheralGate(en_sysctrl_peripheral_gate_t enPeripheral, boolean_t bFlag);
boolean_t   Sysctrl_GetPeripheralGate(en_sysctrl_peripheral_gate_t enPeripheral);

///< ??????:??????????????;
en_result_t Sysctrl_SetFunc(en_sysctrl_func_t enFunc, boolean_t bFlag);

///< RTC??????:????RTC??????????
en_result_t Sysctrl_SetRTCAdjustClkFreq(en_sysctrl_rtc_adjust_t enRtcAdj);

///< USB????
void Sysctrl_UsbClkSel(en_sysctrl_usbclk_sel_t enUsbClk);

///< ?????PLL?,????
void Sysctrl_TimerPllClkSel(en_sysctrl_timer_pllclk_sel_t enTimClk);

//@} // Sysctrl Group

#ifdef __cplusplus
#endif

#endif /* __SYSCTRL_H__ */
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


