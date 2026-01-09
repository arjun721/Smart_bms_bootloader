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
 * @file   flash.c
 *
 * @brief  Source file for FLASH functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "flash.h"
/**
 *******************************************************************************
 ** \addtogroup FlashGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define FLASH_END_ADDR              (0x0001FFFFu)
#define FLASH_BYPASS()              M0P_FLASH->BYPASS = 0x5A5A;\
                                    M0P_FLASH->BYPASS = 0xA5A5; 
#define FLASH_IE_TRUE               (0x03)
#define FLASH_IE_FALSE              (0x00)

#define FLASH_TIMEOUT_INIT          (0xFFu)
#define FLASH_TIMEOUT_PGM           (0xFFu)
#define FLASH_TIMEOUT_ERASE         (0xFFu)

#define FLASH_LOCK_ALL              (0u)
#define FLASH_UNLOCK_ALL            (0xFFFFFFFFu)
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
/**
 ******************************************************************************
 ** \brief FLASH OP
 ** 
 ** Flash ??????????? 
 ******************************************************************************/
typedef enum en_flash_op
{
    Read        = 0u,           ///<????
    Program     = 1u,           ///<?????
    SectorErase = 2u,           ///<???????
    ChipErase   = 3u,           ///<???????
} en_flash_op_t;

/**
 ******************************************************************************
 ** \brief FLASH ????????
 ** 
 ** FLASH???????????? (4MHz)
 ******************************************************************************/
const uint32_t pu32PcgTimer4M[] = { 
                                    0x20u,          //Tnvs   
                                    0x17u,          //Tpgs   
                                    0x1Bu,          //Tprog  
                                    0x4650u,        //Tserase
                                    0x222E0u,       //Tmerase
                                    0x18u,          //Tprcv  
                                    0xF0u,          //Tsrcv  
                                    0x3E8u          //Tmrcv  
                                  };
/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *****************************************************************************
 ** \brief Flash??????
 **
 **
 ** \param [in]  enFlashIntType          Flash????
 ** 
 ** \retval TRUE or FALSE                                      
 *****************************************************************************/
boolean_t Flash_GetIntFlag(en_flash_int_type_t enFlashIntType)
{
    boolean_t bRetVal = FALSE;
    
    if(M0P_FLASH->IFR & enFlashIntType)
    {
        bRetVal =  TRUE;
    }
    
    return bRetVal;
}

/**
 *****************************************************************************
 ** \brief Flash??????
 **
 **
 ** \param [in]  enFlashIntType          Flash????
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Flash_ClearIntFlag(en_flash_int_type_t enFlashIntType)
{
    en_result_t enResult = Error;

    M0P_FLASH->ICLR &= ~(uint32_t)enFlashIntType;
    enResult = Ok;
    
    return enResult;
}

/**
 *****************************************************************************
 ** \brief Flash????
 **
 **
 ** \param [in]  enFlashIntType          Flash????
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Flash_EnableIrq (en_flash_int_type_t enFlashIntType)
{
    en_result_t enResult = Error;

    FLASH_BYPASS();
    M0P_FLASH->CR_f.IE |= enFlashIntType;
    
    enResult = Ok;

    return enResult;
}

/**
 *****************************************************************************
 ** \brief Flash????
 **
 **
 ** \param [in]  enFlashIntType          Flash????
 ** 
 ** \retval Ok or Error                                      
 *****************************************************************************/
en_result_t Flash_DisableIrq(en_flash_int_type_t enFlashIntType)
{
    en_result_t enResult = Error;

    FLASH_BYPASS();
    M0P_FLASH->CR_f.IE &= ~(uint32_t)enFlashIntType;
    
    enResult = Ok;

    return enResult;
}

/**
 *****************************************************************************
 ** \brief FLASH ?????——???????????????????
 **
 ** ????????????????????????????FLASH?????????.
 **
 ** \param [in]  u8FreqCfg        FLASH????????(??HCLK????????):
 **                               1      - 4MHz;
 **                               2      - 8MHz;
 **                               4      - 16MHz;
 **                               6      - 24MHz;
 **                               8      - 32MHz;
 **                               12     - 48MHz;
 **                               other   -  ???
 ** \param [in] bDpstbEn          TRUE  - ?????DeepSleep??,FLASH???????;
 **                               FALSE - ?????DeepSleep??,FLASH????????;
 ** 
 ** \retval Ok                    ????.
 ** \retval ErrorInvalidParameter ????.
 ** \retval ErrorUninitialized    ??????
 *****************************************************************************/
en_result_t Flash_Init(uint8_t u8FreqCfg, boolean_t bDpstbEn)
{
    uint32_t                u32Index  = 0;
    volatile uint32_t       u32TimeOut = FLASH_TIMEOUT_INIT;
    en_result_t             enResult  = Ok;
    uint32_t                u32PrgTimer[8] = {0}; 
    volatile uint32_t       *pu32PrgTimerReg = (volatile uint32_t*)M0P_FLASH;
    
    if ((1  != u8FreqCfg) && (2  != u8FreqCfg) &&
        (4  != u8FreqCfg) && (6  != u8FreqCfg) &&
        (8  != u8FreqCfg) && (12 != u8FreqCfg))
    {
        enResult = ErrorInvalidParameter;
        return (enResult);
    }
    
    M0P_FLASH->CR_f.DPSTB_EN = bDpstbEn;
    
    //flash?????????
    for(u32Index=0; u32Index<8; u32Index++)
    {
        u32PrgTimer[u32Index] = u8FreqCfg * pu32PcgTimer4M[u32Index];
    } 
    
    if(12 == u8FreqCfg)
    {
        u32PrgTimer[1] = 0xFF;
    }
    
    //flash?????????
    for(u32Index=0; u32Index<8; u32Index++)
    {
        u32TimeOut = FLASH_TIMEOUT_INIT;
        while(pu32PrgTimerReg[u32Index]  != u32PrgTimer[u32Index])
        {
            if(u32TimeOut--)
            {
                FLASH_BYPASS();
                pu32PrgTimerReg[u32Index] = u32PrgTimer[u32Index];
            }
            else
            {
                return ErrorUninitialized;
            }
        }
    }
    
    return (enResult);
}

/**
 *****************************************************************************
 ** \brief FLASH ???
 **
 ** ???FLASH??1????.
 **
 ** \param [in]  u32Addr          Flash??
 ** \param [in]  u8Data           1????
 ** 
 ** \retval Ok                    ????.
 ** \retval ErrorInvalidParameter FLASH????
 ** \retval ErrorTimeout          ????
 *****************************************************************************/
en_result_t Flash_WriteByte(uint32_t u32Addr, uint8_t u8Data)
{
    en_result_t             enResult = Ok;    
    volatile uint32_t       u32TimeOut = FLASH_TIMEOUT_PGM;
    
    if (FLASH_END_ADDR < u32Addr)
    {
        enResult = ErrorInvalidParameter;
        return (enResult);
    }
    
    //busy?
    u32TimeOut = FLASH_TIMEOUT_PGM;
    while (TRUE == M0P_FLASH->CR_f.BUSY)
    {
        if(0 == u32TimeOut--)
        {
            return ErrorTimeout;
        }
    }
    
    //set OP
    u32TimeOut = FLASH_TIMEOUT_PGM;
    while(Program != M0P_FLASH->CR_f.OP)
    {
        if(u32TimeOut--)
        {
            FLASH_BYPASS();
            M0P_FLASH->CR_f.OP = Program;
        }
        else
        {
            return ErrorTimeout;
        }
    }
    
    //Flash ??
    Flash_UnlockAll();
    
    //write data
    *((volatile uint8_t*)u32Addr) = u8Data;
    
    //busy?
    u32TimeOut = FLASH_TIMEOUT_PGM;
    while (TRUE == M0P_FLASH->CR_f.BUSY)
    {
        if(0 == u32TimeOut--)
        {
            return ErrorTimeout;
        }
    }
    
    //Flash ??
    Flash_LockAll();
    
    return (enResult);
}

/**
 *****************************************************************************
 ** \brief FLASH ???
 **
 ** ???FLASH????(2??)??.
 **
 ** \param [in]  u32Addr         Flash??
 ** \param [in]  u16Data        ??(2??)??
 ** 
 ** \retval Ok                    ????.
 ** \retval ErrorInvalidParameter FLASH????
 ** \retval ErrorTimeout          ????
 *****************************************************************************/
en_result_t Flash_WriteHalfWord(uint32_t u32Addr, uint16_t u16Data)
{
    en_result_t             enResult = Ok;    
    volatile uint32_t       u32TimeOut = FLASH_TIMEOUT_PGM;
    
    if (FLASH_END_ADDR < u32Addr)
    {
        enResult = ErrorInvalidParameter;
        return (enResult);
    }
    
    //busy?
    u32TimeOut = FLASH_TIMEOUT_PGM;
    while (TRUE == M0P_FLASH->CR_f.BUSY)
    {
        if(0 == u32TimeOut--)
        {
            return ErrorTimeout;
        }
    }
    
    //set OP
    u32TimeOut = FLASH_TIMEOUT_PGM;
    while(Program != M0P_FLASH->CR_f.OP)
    {
        if(u32TimeOut--)
        {
            FLASH_BYPASS();
            M0P_FLASH->CR_f.OP = Program;
        }
        else
        {
            return ErrorTimeout;
        }
    }
    
    //Flash ??
    Flash_UnlockAll();
    
    //write data
    *((volatile uint16_t*)u32Addr) = u16Data;
    
    //busy?
    u32TimeOut = FLASH_TIMEOUT_PGM;
    while (TRUE == M0P_FLASH->CR_f.BUSY)
    {
        if(0 == u32TimeOut--)
        {
            return ErrorTimeout;
        }
    }
    
    //Flash ??
    Flash_LockAll();
    
    return (enResult);
}

/**
 *****************************************************************************
 ** \brief FLASH ??
 **
 ** ???FLASH??1?????.
 **
 ** \param [in]  u32Addr         Flash??
 ** \param [in]  u32Data         1????
 ** 
 ** \retval Ok                    ????.
 ** \retval ErrorInvalidParameter FLASH????
 ** \retval ErrorTimeout          ????
 *****************************************************************************/
en_result_t Flash_WriteWord(uint32_t u32Addr, uint32_t u32Data)
{
    en_result_t             enResult = Ok;    
    volatile uint32_t       u32TimeOut = FLASH_TIMEOUT_PGM;
    
    if (FLASH_END_ADDR < u32Addr)
    {
        enResult = ErrorInvalidParameter;
        return (enResult);
    }
    
    //busy?
    u32TimeOut = FLASH_TIMEOUT_PGM;
    while (TRUE == M0P_FLASH->CR_f.BUSY)
    {
        if(0 == u32TimeOut--)
        {
            return ErrorTimeout;
        }
    }
    
    //Flash ??
    Flash_UnlockAll();
    
    //set OP
    u32TimeOut = FLASH_TIMEOUT_PGM;
    while(Program != M0P_FLASH->CR_f.OP)
    {
        if(u32TimeOut--)
        {
            FLASH_BYPASS();
            M0P_FLASH->CR_f.OP = Program;
        }
        else
        {
            return ErrorTimeout;
        }
    }
    
    //write data
    *((volatile uint32_t*)u32Addr) = u32Data;
    
    //busy?
    u32TimeOut = FLASH_TIMEOUT_PGM;
    while (TRUE == M0P_FLASH->CR_f.BUSY)
    {
        if(0 == u32TimeOut--)
        {
            return ErrorTimeout;
        }
    }
    
    //Flash ??
    Flash_LockAll();
    
    return (enResult);
}

/**
 *****************************************************************************
 ** \brief FLASH ????
 **
 ** FLASH ????.
 **
 ** \param [in]  u32SectorAddr    ?????????
 ** 
 ** \retval Ok                    ????.
 ** \retval ErrorInvalidParameter FLASH???? 
 ** \retval ErrorTimeout          ????
 *****************************************************************************/
en_result_t Flash_SectorErase(uint32_t u32SectorAddr)
{
    en_result_t             enResult = Ok;    
    volatile uint32_t       u32TimeOut = FLASH_TIMEOUT_ERASE;
    
    if (FLASH_END_ADDR < u32SectorAddr)
    {
        enResult = ErrorInvalidParameter;
        return (enResult);
    }
    
    //busy?
    u32TimeOut = FLASH_TIMEOUT_ERASE;
    while (TRUE == M0P_FLASH->CR_f.BUSY)
    {
        if(0 == u32TimeOut--)
        {
            return ErrorTimeout;
        }
    }
    
    //Flash ??
    Flash_UnlockAll();
    
    //set OP
    u32TimeOut = FLASH_TIMEOUT_ERASE;
    while(SectorErase != M0P_FLASH->CR_f.OP)
    {
        if(u32TimeOut--)
        {
            FLASH_BYPASS();
            M0P_FLASH->CR_f.OP = SectorErase;
        }
        else
        {
            return ErrorTimeout;
        }
    }
    
    //write data
    *((volatile uint8_t*)u32SectorAddr) = 0;
    
    //busy?
    u32TimeOut = FLASH_TIMEOUT_ERASE;
    while (TRUE == M0P_FLASH->CR_f.BUSY)
    {
        if(0 == u32TimeOut--)
        {
            return ErrorTimeout;
        }
    }
    
    //Flash ??
    Flash_LockAll();
    
    return (enResult);
}

/**
 *****************************************************************************
 ** \brief FLASH ????(?????RAM???!!!)
 **
 ** FLASH ????.
 **
 ** 
 ** \retval Ok              ????.
 ** \retval ErrorTimeout    ????
 **
 *****************************************************************************/
en_result_t Flash_ChipErase(void)
{
    en_result_t             enResult = Ok;    
    volatile uint32_t       u32TimeOut = FLASH_TIMEOUT_ERASE; 
    
    //busy?
    u32TimeOut = FLASH_TIMEOUT_ERASE;
    while (TRUE == M0P_FLASH->CR_f.BUSY)
    {
        if(0 == u32TimeOut--)
        {
            return ErrorTimeout;
        }
    }
    
    //set OP
    u32TimeOut = FLASH_TIMEOUT_ERASE;
    while(ChipErase != M0P_FLASH->CR_f.OP)
    {
        if(u32TimeOut--)
        {
            FLASH_BYPASS();
            M0P_FLASH->CR_f.OP = ChipErase;
        }
        else
        {
            return ErrorTimeout;
        }
    }
    
    //Flash ??
    Flash_UnlockAll();
    
    //write data
    *((volatile uint8_t*)0) = 0;
    
    //busy?
    u32TimeOut = FLASH_TIMEOUT_ERASE;
    while (TRUE == M0P_FLASH->CR_f.BUSY)
    {
        if(0 == u32TimeOut--)
        {
            return ErrorTimeout;
        }
    }
    
    //Flash ??
    Flash_LockAll();
    
    return (enResult);
}

/**
 *****************************************************************************
 ** \brief FLASH ??????
 **
 ** 
 ** \retval Null                               
 *****************************************************************************/
void Flash_LockAll(void)
{ 
    FLASH_BYPASS();
    M0P_FLASH->SLOCK0 = FLASH_LOCK_ALL;
    FLASH_BYPASS();
    M0P_FLASH->SLOCK1 = FLASH_LOCK_ALL;    

}

/**
 *****************************************************************************
 ** \brief FLASH ??????
 **
 ** 
 ** \retval Null                             
 *****************************************************************************/
void Flash_UnlockAll(void)
{
   
    FLASH_BYPASS();
    M0P_FLASH->SLOCK0 = FLASH_UNLOCK_ALL;
    FLASH_BYPASS();
    M0P_FLASH->SLOCK1 = FLASH_UNLOCK_ALL;

}

/**
 *****************************************************************************
 ** \brief FLASH ???????
 **
 ** \param [in]  enWaitCycle  ??FLASH??????????
 ** 
 ** \retval Ok                    ????
 ** \retval ErrorInvalidParameter ????                                
 *****************************************************************************/
en_result_t Flash_WaitCycle(en_flash_waitcycle_t enWaitCycle)
{
    en_result_t enResult = Ok;
    
    FLASH_BYPASS();
    M0P_FLASH->CR_f.WAIT = enWaitCycle;
    
    return enResult;
}

/**
 *****************************************************************************
 ** \brief FLASH LOCK ??
 **
 ** \param [in]  enLock  @ref en_flash_lock_t
 ** \param [in]  u32LockValue 32bits,??bit=0:??,??Sector?????;??bit=1:???
 ** \note  ?????Sector:[enLock*128 + i*4, enLock*128 + i*4+3]
 **        (i??u32LockValue?bit??,0~31)
 **        ??:enLock = FlashLock1, u32LockValue = 0x00000002,
 **              ???????:[Sector128,Sector131]
 ** \retval Ok                    ????
 ** \retval ErrorInvalidParameter ????                                
 *****************************************************************************/
en_result_t Flash_LockSet(en_flash_lock_t enLock, uint32_t u32LockValue)
{
    FLASH_BYPASS();
    *((&M0P_FLASH->SLOCK0) + enLock) = u32LockValue;
    
    return Ok;
}
//@} // FlashGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
