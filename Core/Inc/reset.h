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
 * @file   reset.h
 *
 * @brief  Header file for RESET functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

#ifndef __RESET_H__
#define __RESET_H__

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"


#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup ResetGroup Reset (RST)
 **
 ******************************************************************************/
//@{

/**
 *******************************************************************************
 ** function prototypes.
 ******************************************************************************/

/*******************************************************************************
 * Global definitions
 ******************************************************************************/


/**
 *******************************************************************************
 ** \brief ??????????
 ** \note
 ******************************************************************************/
typedef enum en_sysctrl_peripheral0
{
    ResetMskUart0      = 1u<<0u,       ///< ??0
    ResetMskUart1      = 1u<<1u,       ///< ??1
    ResetMskLpUart0    = 1u<<2u,       ///< ?????0
    ResetMskLpUart1    = 1u<<3u,       ///< ?????1
    ResetMskI2c0       = 1u<<4u,       ///< I2C0
    ResetMskI2c1       = 1u<<5u,       ///< I2C1
    ResetMskSpi0       = 1u<<6u,       ///< SPI0
    ResetMskSpi1       = 1u<<7u,       ///< SPI1
    ResetMskBaseTim    = 1u<<8u,       ///< ?????TIM0/1/2
    ResetMskLpTim0     = 1u<<9u,       ///< ??????0
    ResetMskAdvTim     = 1u<<10u,      ///< ?????TIM4/5/6
    ResetMskTim3       = 1u<<11u,      ///< ???3
    ResetMskOpa        = 1u<<13u,      ///< OPA
    ResetMskPca        = 1u<<14u,      ///< ???????
    ResetMskAdcBgr     = 1u<<16u,      ///< ADC&BGR
    ResetMskVcLvd      = 1u<<17u,      ///< VC?LVD
    ResetMskRng        = 1u<<18u,      ///< RNG
    ResetMskPcnt       = 1u<<19u,      ///< PCNT
    ResetMskRtc        = 1u<<20u,      ///< RTC
    ResetMskTrim       = 1u<<21u,      ///< ????
    ResetMskLcd        = 1u<<22u,      ///< LCD
    ResetMskTick       = 1u<<24u,      ///< ?????
    ResetMskSwd        = 1u<<25u,      ///< SWD
    ResetMskCrc        = 1u<<26u,      ///< CRC
    ResetMskAes        = 1u<<27u,      ///< AES
    ResetMskGpio       = 1u<<28u,      ///< GPIO
    ResetMskDma        = 1u<<29u,      ///< DMA
	ResetMskHdiv       = 1U<<30U,      ///< HDIV
}en_reset_peripheral0_t;

typedef enum en_sysctrl_peripheral1
{
    ResetMskUsb        = 1u<<0u,      ///< USB
    ResetMskCan        = 1u<<1u,      ///< CAN
	ResetMskCts        = 1u<<2u,      ///< CTS
    ResetMskDac        = 1u<<3u,      ///< DAC
    ResetMskLpTim1     = 1u<<4u,      ///< ??????1
    ResetMskI2s0       = 1u<<5u,      ///< I2S0
    ResetMskI2s1       = 1u<<6u,      ///< I2S1	
    ResetMskUart2      = 1u<<8u,      ///< UART2
    ResetMskUart3      = 1u<<9u,      ///< UART3 
}en_reset_peripheral1_t;

/**
 *******************************************************************************
 ** \brief ???(??)??
 **
 ** \note
 ******************************************************************************/
typedef enum en_reset_flag
{
    ResetFlagMskPor5V     = 1u<<0u,         ///< 5V????
    ResetFlagMskPor1_5V   = 1u<<1u,         ///< 1.5V????
    ResetFlagMskLvd       = 1u<<2u,         ///< ???????
    ResetFlagMskWdt       = 1u<<3u,         ///< ?????
    ResetFlagMskPca       = 1u<<4u,         ///< PCA??
    ResetFlagMskLockup    = 1u<<5u,         ///< ??????
    ResetFlagMskSysreq    = 1u<<6u,         ///< ????
    ResetFlagMskRstb      = 1u<<7u,         ///< RESET? ??
}en_reset_flag_t;

/*******************************************************************************
 * Global variable declarations ('extern', definition in C source)
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
///< ???????
boolean_t Reset_GetFlag(en_reset_flag_t enRstFlg);
///< ???????
void Reset_ClearFlag(en_reset_flag_t enRstFlg);
void Reset_ClearFlagAll(void);

///< ??????
void Reset_RstPeripheralAll(void);
void Reset_RstPeripheral0(en_reset_peripheral0_t enPeri);
void Reset_RstPeripheral1(en_reset_peripheral1_t enPeri);

//@} // ResetGroup

#ifdef __cplusplus
#endif


#endif /* __RESET_H__ */
/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


