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
 * @file   flash.h
 *
 * @brief  Header file for FLASH functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

#ifndef __FLASH_H__
#define __FLASH_H__

/******************************************************************************/
/* Include files                                                              */
/******************************************************************************/
#include "ddl.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup FlashGroup Flash Controller (Flash)
 **
 **
 ******************************************************************************/
//@{

/******************************************************************************
 * Global type definitions
 ******************************************************************************/
/**
 ******************************************************************************
  ** \brief Flash???????
 *****************************************************************************/
typedef enum en_flash_int_type
{
    FlashPCInt    = 0x01u,          ///<??PC??????
    FlashSlockInt = 0x02u,          ///<????????
} en_flash_int_type_t;


/**
 ******************************************************************************
  ** \brief Flash??????????
 *****************************************************************************/
typedef enum en_flash_waitcycle
{
    FlashWaitCycle0 = 0u,       ///< ????????0-?????1(?HCLK????24MHz?)
    FlashWaitCycle1 = 1u,       ///< ????????1-?????2(?HCLK??24MHz??????1)
    FlashWaitCycle2 = 2u,       ///< ????????2-?????3(?HCK??48MHz??????2)
} en_flash_waitcycle_t;

/**
 ******************************************************************************
  ** \brief Flash?????????
 *****************************************************************************/
typedef enum en_flash_lock
{
    FlashLock0 = 0u,           ///<LOCK0    
    FlashLock1 = 1u,           ///<LOCK1      
} en_flash_lock_t;

/**
 ******************************************************************************
 ** \brief Redefinition of FLASH register structure 
 ******************************************************************************/

/******************************************************************************
 * Global definitions
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions ('extern')                                     
 ******************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)                        
 ******************************************************************************/
///<Flash ?????(??????????????????)
en_result_t Flash_Init(uint8_t u8FreqCfg, boolean_t bDpstbEn);

///<Flash ?/????
en_result_t Flash_SectorErase(uint32_t u32SectorAddr);
en_result_t Flash_ChipErase(void);

///<Flash ??/??/??
en_result_t Flash_WriteByte(uint32_t u32Addr, uint8_t u8Data);
en_result_t Flash_WriteHalfWord(uint32_t u32Addr, uint16_t u16Data);
en_result_t Flash_WriteWord(uint32_t u32Addr, uint32_t u32Data);

///<Flash ??????/??
void Flash_LockAll(void);
void Flash_UnlockAll(void);
en_result_t Flash_LockSet(en_flash_lock_t enLock, uint32_t u32LockValue);


///<Flash ???????
en_result_t Flash_WaitCycle(en_flash_waitcycle_t enWaitCycle);

///<??????
///<????/??
en_result_t Flash_EnableIrq(en_flash_int_type_t enFlashIntType);
en_result_t Flash_DisableIrq(en_flash_int_type_t enFlashIntType);
///<??????
boolean_t Flash_GetIntFlag(en_flash_int_type_t enFlashIntType);
///<??????
en_result_t Flash_ClearIntFlag(en_flash_int_type_t enFlashIntType);

//@} // FlashGroup

#ifdef __cplusplus
}
#endif

#endif /* __FLASH_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
