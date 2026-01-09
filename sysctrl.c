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
 * @file   sysctrl.c
 *
 * @brief  Source file for SYSCTRL functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "sysctrl.h"

/**
 *******************************************************************************
 ** \addtogroup SysctrlGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define CLK_TIMEOUT             (1000000u)

///< XTH
#define SYSTEM_XTH          (8*1000*1000u)     ///< 8MHz

///< XTL
#define SYSTEM_XTL          (32768u)            ///< 32768Hz

#define IS_VALID_SRC(x)         (   ClkRCH == (x)||\
                                    ClkXTH == (x)||\
                                    ClkRCL == (x)||\
                                    ClkXTL == (x) )


#define IS_VALID_FUNC(x)        (   ClkFuncWkupRCH == (x)||\
                                    ClkFuncXTHEn == (x)||\
                                    ClkFuncXTLEn == (x)||\
                                    ClkFuncXTLAWSON == (x)||\
                                    ClkFuncFaultEn == (x)||\
                                    ClkFuncRtcLPWEn == (x)||\
                                    ClkFuncLockUpEn == (x)||\
                                    ClkFuncRstPinIOEn == (x)||\
                                    ClkFuncSwdPinIOEn == (x) )

#define RC_TRIM_BASE_ADDR           ((volatile uint16_t*) (0x00100C00ul))
#define RCH_CR_TRIM_24M_VAL         (*((volatile uint16_t*) (0x00100C00ul)))
#define RCH_CR_TRIM_22_12M_VAL      (*((volatile uint16_t*) (0x00100C02ul)))
#define RCH_CR_TRIM_16M_VAL         (*((volatile uint16_t*) (0x00100C04ul)))
#define RCH_CR_TRIM_8M_VAL          (*((volatile uint16_t*) (0x00100C06ul)))
#define RCH_CR_TRIM_4M_VAL          (*((volatile uint16_t*) (0x00100C08ul)))

#define RCL_CR_TRIM_38400_VAL       (*((volatile uint16_t*) (0x00100C20ul)))
#define RCL_CR_TRIM_32768_VAL       (*((volatile uint16_t*) (0x00100C22ul)))
    
#define RC48_TRIM_BASE_ADDR         ((volatile uint16_t*) (0x00100C0Aul))

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
extern uint32_t SystemCoreClock;
/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

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
 *******************************************************************************
 ** \brief SYSCTRL0\SYSCTRL1???????
 **
 ** \retval None
 ******************************************************************************/
static void _SysctrlUnlock(void)
{
    M0P_SYSCTRL->SYSCTRL2 = 0x5A5A;
    M0P_SYSCTRL->SYSCTRL2 = 0xA5A5;
}

/**
 *******************************************************************************
 ** \brief ???????
 ** \param [in]  enSource   ?????
 ** \param [in]  bFlag      ??1-?/0-?
 ** \retval      Ok         ????
 **             ??        ????
 ******************************************************************************/
en_result_t Sysctrl_ClkSourceEnable(en_sysctrl_clk_source_t enSource, boolean_t bFlag)
{
    en_result_t enRet = Ok;
    uint32_t u32Temp;
    
    _SysctrlUnlock();
    bFlag = !!bFlag;

    u32Temp = M0P_SYSCTRL->PERI_CLKEN0;
    switch (enSource)
    {
        case SysctrlClkRCH:
            M0P_SYSCTRL->SYSCTRL0_f.RCH_EN = bFlag;
            while(bFlag && (1 != M0P_SYSCTRL->RCH_CR_f.STABLE))
            {
                ;
            }
            break;

        case SysctrlClkXTH:
            M0P_SYSCTRL->PERI_CLKEN0_f.GPIO = TRUE;
            M0P_GPIO->PFADS |= 3u;
            M0P_SYSCTRL->SYSCTRL0_f.XTH_EN = bFlag;
            while(bFlag && (1 != M0P_SYSCTRL->XTH_CR_f.STABLE))
            {
                ;
            }
            break;

        case SysctrlClkRCL:
            
            M0P_SYSCTRL->SYSCTRL0_f.RCL_EN = bFlag;
            while(bFlag && (1 != M0P_SYSCTRL->RCL_CR_f.STABLE))
            {
                ;
            }
            break;

        case SysctrlClkXTL:
            M0P_SYSCTRL->PERI_CLKEN0_f.GPIO = TRUE;
            M0P_GPIO->PCADS |= 0xC000;
            M0P_SYSCTRL->SYSCTRL0_f.XTL_EN = bFlag;
            while(bFlag && (1 != M0P_SYSCTRL->XTL_CR_f.STABLE))
            {
                ;
            }
            break;

        case SysctrlClkPLL:
            M0P_SYSCTRL->PERI_CLKEN0_f.ADC = TRUE;
            M0P_BGR->CR_f.BGR_EN = TRUE;
            delay10us(20);
            M0P_SYSCTRL->SYSCTRL0_f.PLL_EN = bFlag;
            while(bFlag && (1 != M0P_SYSCTRL->PLL_CR_f.STABLE))
            {
                ;
            }
            break;
        case SysctrlClkRC48M:
            M0P_SYSCTRL->RC48M_CR_f.EN = bFlag;
            while(bFlag && (1 != M0P_SYSCTRL->RC48M_CR_f.STABLE))
            {
                ;
            }            
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
    }
    M0P_SYSCTRL->PERI_CLKEN0 = u32Temp;
    
    return enRet;
}

/**
 *******************************************************************************
 ** \brief ??????????
 ** \param [in]  enFreq     ????????????
 ** \param [in]  enDriver   ????????????
 ** \retval      Ok         ????
 **             ??        ????
 ******************************************************************************/
en_result_t Sysctrl_XTHDriverCfg(en_sysctrl_xtal_driver_t enDriver)
{
    en_result_t enRet = Ok;

    M0P_SYSCTRL->XTH_CR_f.DRIVER   = enDriver;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief ??????????
 ** \param [in]  enFreq     ????????????
 ** \param [in]  enDriver   ????????????
 ** \retval      Ok         ????
 **             ??        ????
 ******************************************************************************/
en_result_t Sysctrl_XTLDriverCfg(en_sysctrl_xtl_amp_t enAmp, en_sysctrl_xtal_driver_t enDriver)
{
    en_result_t enRet = Ok;

    M0P_SYSCTRL->XTL_CR_f.AMP_SEL = enAmp;
    M0P_SYSCTRL->XTL_CR_f.DRIVER  = enDriver;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief ????????????
 ** \param [in]  enCycle     ????????????
 ** \retval      Ok         ????
 **             ??        ????
 ******************************************************************************/
en_result_t Sysctrl_SetXTHStableTime(en_sysctrl_xth_cycle_t enCycle)
{
    en_result_t enRet = Ok;
    M0P_SYSCTRL->XTH_CR_f.STARTUP = enCycle;
    return enRet;
}

/**
 *******************************************************************************
 ** \brief ????????????
 ** \param [in]  enCycle     ????????????
 ** \retval      Ok         ????
 **             ??        ????
 ******************************************************************************/
en_result_t Sysctrl_SetRCLStableTime(en_sysctrl_rcl_cycle_t enCycle)
{
    en_result_t enRet = Ok;
    M0P_SYSCTRL->RCL_CR_f.STARTUP = enCycle;
    return enRet;
}

/**
 *******************************************************************************
 ** \brief ????????????
 ** \param [in]  enCycle     ????????????
 ** \retval      Ok         ????
 **             ??        ????
 ******************************************************************************/
en_result_t Sysctrl_SetXTLStableTime(en_sysctrl_xtl_cycle_t enCycle)
{
    en_result_t enRet = Ok;
    M0P_SYSCTRL->XTL_CR_f.STARTUP = enCycle;
    return enRet;
}

/**
 *******************************************************************************
 ** \brief PLL??????
 ** \param [in]  enCycle    PLL??????
 ** \retval      Ok         ????
 **             ??        ????
 ******************************************************************************/
en_result_t Sysctrl_SetPLLStableTime(en_sysctrl_pll_cycle_t enCycle)
{
    en_result_t enRet = Ok;
    M0P_SYSCTRL->PLL_CR_f.STARTUP = enCycle;
    return enRet;
}

/**
 *******************************************************************************
 ** \brief ?????,?????????????
 ** \note  ???????,???????????????/????/??????
 ** \param [in]  enSource   ????
 **
 ** \retval      Ok         ????
 **              ??       ????
 ******************************************************************************/
en_result_t Sysctrl_SysClkSwitch(en_sysctrl_clk_source_t enSource)
{
    en_result_t enRet = Ok;

    en_sysctrl_clk_source_t ClkNew = enSource;

    _SysctrlUnlock();
    M0P_SYSCTRL->SYSCTRL0_f.CLKSW = ClkNew;

    //??Core??(HCLK)
    SystemCoreClockUpdate();

    return enRet;
}

/**
 *******************************************************************************
 ** \brief   ??????(HCLK)???
 ** \retval  uint32_t         HCLK???
 **
 ******************************************************************************/
uint32_t Sysctrl_GetHClkFreq(void)
{
    uint32_t u32Val = 0;
    const uint32_t u32hcr_tbl[] = { 4000000, 8000000, 16000000, 22120000, 24000000};
    const uint16_t u32lcr_tbl[] = { 32768, 38400};
    en_sysctrl_clk_source_t enSrc;
    uint16_t u16Trim[5] = {0};
    u16Trim[4] = RCH_CR_TRIM_24M_VAL;
    u16Trim[3] = RCH_CR_TRIM_22_12M_VAL;
    u16Trim[2] = RCH_CR_TRIM_16M_VAL;
    u16Trim[1] = RCH_CR_TRIM_8M_VAL;
    u16Trim[0] = RCL_CR_TRIM_38400_VAL;

    //????????
    enSrc = (en_sysctrl_clk_source_t)(M0P_SYSCTRL->SYSCTRL0_f.CLKSW);

    switch (enSrc)
    {
        case SysctrlClkRCH:
            {

                if((M0P_SYSCTRL->RCH_CR_f.TRIM) == (u16Trim[4]))
                {
                    u32Val = u32hcr_tbl[4];
                }
                else if((M0P_SYSCTRL->RCH_CR_f.TRIM) == (u16Trim[3]))
                {
                    u32Val = u32hcr_tbl[3];
                }
                else if((M0P_SYSCTRL->RCH_CR_f.TRIM) == (u16Trim[2]))
                {
                    u32Val = u32hcr_tbl[2];
                }
                else if((M0P_SYSCTRL->RCH_CR_f.TRIM) == (u16Trim[1]))
                {
                    u32Val = u32hcr_tbl[1];
                }
                else
                {
                    u32Val = u32hcr_tbl[0];
                }
            }
            break;
        case SysctrlClkXTH:
            u32Val = SYSTEM_XTH;
            break;
        case SysctrlClkRCL:
            {
                if(u16Trim[0] == (M0P_SYSCTRL->RCL_CR_f.TRIM))
                {
                    u32Val = u32lcr_tbl[1];
                }
                else
                {
                    u32Val = u32lcr_tbl[0];
                }
            }
            break;
        case SysctrlClkXTL:
            u32Val = SYSTEM_XTL;
            break;
        case SysctrlClkPLL:
            {
                if (SysctrlPllRch == M0P_SYSCTRL->PLL_CR_f.REFSEL)
                {
                    if(u16Trim[4] == M0P_SYSCTRL->RCH_CR_f.TRIM)
                    {
                        u32Val = u32hcr_tbl[4];
                    }
                    else if(u16Trim[3] == M0P_SYSCTRL->RCH_CR_f.TRIM)
                    {
                        u32Val = u32hcr_tbl[3];
                    }
                    else if(u16Trim[2] == M0P_SYSCTRL->RCH_CR_f.TRIM)
                    {
                        u32Val = u32hcr_tbl[2];
                    }
                    else if(u16Trim[1] == M0P_SYSCTRL->RCH_CR_f.TRIM)
                    {
                        u32Val = u32hcr_tbl[1];
                    }
                    else
                    {
                        u32Val = u32hcr_tbl[0];
                    }
                }
                else
                {
                    u32Val = SYSTEM_XTH;
                }

                u32Val = (u32Val * M0P_SYSCTRL->PLL_CR_f.DIVN);
            }
            break;
        default:
            u32Val = 0u;
            break;
    }

    u32Val = (u32Val >> M0P_SYSCTRL->SYSCTRL0_f.HCLK_PRS);

    return u32Val;
}

/**
 *******************************************************************************
 ** \brief ??????(PCLK)???
 ** \retval uint32_t   PCLK???(Hz)
 **
 ******************************************************************************/
uint32_t Sysctrl_GetPClkFreq(void)
{
    uint32_t u32Val = 0;

    u32Val = Sysctrl_GetHClkFreq();
    u32Val = (u32Val >> (M0P_SYSCTRL->SYSCTRL0_f.PCLK_PRS));

    return u32Val;
}


/**
 *******************************************************************************
 ** \brief ???????
 ** \param [in]  pstcCfg    ???????
 ** \retval      Ok         ????
 **             ??        ????
 ******************************************************************************/
en_result_t Sysctrl_ClkInit(stc_sysctrl_clk_cfg_t *pstcCfg)
{
    en_result_t enRet = Ok;

    //????????
    switch(pstcCfg->enClkSrc)
    {
        case SysctrlClkRCH:

            break;
        case SysctrlClkXTH:
            Sysctrl_XTHDriverCfg(SysctrlXtalDriver3);
            Sysctrl_SetXTHStableTime(SysctrlXthStableCycle16384);
            break;
        case SysctrlClkRCL:
            Sysctrl_SetRCLStableTime(SysctrlRclStableCycle256);
            break;
        case SysctrlClkXTL:
            Sysctrl_XTLDriverCfg(SysctrlXtlAmp3, SysctrlXtalDriver3);
            Sysctrl_SetXTLStableTime(SysctrlXtlStableCycle16384);
            break;
        case SysctrlClkPLL:
            Sysctrl_SetPLLStableTime(SysctrlPllStableCycle16384);
            break;
        default:
            enRet = ErrorInvalidParameter;
            break;
    }

    //?????
    Sysctrl_ClkSourceEnable(pstcCfg->enClkSrc, TRUE);

    //?????
    Sysctrl_SysClkSwitch(pstcCfg->enClkSrc);

    //??????
    Sysctrl_SetHCLKDiv(pstcCfg->enHClkDiv);
    Sysctrl_SetPCLKDiv(pstcCfg->enPClkDiv);

    return enRet;
}

/**
 *******************************************************************************
 ** \brief ????????
 ** \param [in]
 ** \retval      Ok         ????
 **             ??        ????
 ******************************************************************************/
en_result_t Sysctrl_ClkDeInit(void)
{
    en_result_t enRet = Ok;

    //??RCH???4Hz
    Sysctrl_SetRCHTrim(SysctrlRchFreq4MHz);

    //?????
    Sysctrl_ClkSourceEnable(SysctrlClkRCH, TRUE);

    //?????
    Sysctrl_SysClkSwitch(SysctrlClkRCH);

    //?????????
    Sysctrl_ClkSourceEnable(SysctrlClkXTH, FALSE);
    Sysctrl_ClkSourceEnable(SysctrlClkRCL, FALSE);
    Sysctrl_ClkSourceEnable(SysctrlClkXTL, FALSE);
    Sysctrl_ClkSourceEnable(SysctrlClkPLL, FALSE);

    //??????
    Sysctrl_SetHCLKDiv(SysctrlHclkDiv1);
    Sysctrl_SetPCLKDiv(SysctrlPclkDiv1);

    return enRet;
}

/**
 *******************************************************************************
 ** \brief ????????TRIM???
 ** \param [in]  enRCHFreq  ???RCH?????
 ** \retval      Ok         ????
 **             ??        ??????????
 ******************************************************************************/
en_result_t Sysctrl_SetRCHTrim(en_sysctrl_rch_freq_t enRCHFreq)
{
    //??RCH Trim?
    if(SysctrlRchFreq48MHz != enRCHFreq)
    {
        M0P_SYSCTRL->RCH_CR_f.TRIM = *(RC_TRIM_BASE_ADDR + enRCHFreq);
    }
    else
    {
        M0P_SYSCTRL->RC48M_CR_f.TRIM = *(RC48_TRIM_BASE_ADDR);
    }


    return Ok;
}

/**
 *******************************************************************************
 ** \brief ????????????
 ** \param [in]  enXTHFreq  ??????
 ** \retval      Ok         ????
 **              ??       ??????????
 ******************************************************************************/
en_result_t Sysctrl_SetXTHFreq(en_sysctrl_xth_freq_t enXTHFreq)
{
    en_result_t enRet = Ok;

    M0P_SYSCTRL->XTH_CR_f.XTH_FSEL = enXTHFreq;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief PLL????
 ** \param [in]  pstcPLLCfg PLL???????
 ** \retval      Ok         ????
 **              ??       ???????????
 ******************************************************************************/
en_result_t Sysctrl_SetPLLFreq(stc_sysctrl_pll_cfg_t *pstcPLLCfg)
{
    en_result_t enRet = Ok;

    uint16_t u16Trim[5] = {0};
    u16Trim[4] = RCH_CR_TRIM_24M_VAL;
    u16Trim[3] = RCH_CR_TRIM_22_12M_VAL;
    u16Trim[2] = RCH_CR_TRIM_16M_VAL;
    u16Trim[1] = RCH_CR_TRIM_8M_VAL;

    ////PLL????????48MHz
    //RCH??PLL??
    if (SysctrlPllRch == pstcPLLCfg->enPllClkSrc)
    {
        if( ((u16Trim[4] == M0P_SYSCTRL->RCH_CR_f.TRIM) && (pstcPLLCfg->enPllMul > 2)) ||
            ((u16Trim[3] == M0P_SYSCTRL->RCH_CR_f.TRIM) && (pstcPLLCfg->enPllMul > 2)) ||
            ((u16Trim[2] == M0P_SYSCTRL->RCH_CR_f.TRIM) && (pstcPLLCfg->enPllMul > 3)) ||
            ((u16Trim[1] == M0P_SYSCTRL->RCH_CR_f.TRIM) && (pstcPLLCfg->enPllMul > 6)))
        {
            return ErrorInvalidMode;
        }
    }
    else    //XTH??PLL??
    {
        if ((SYSTEM_XTH * pstcPLLCfg->enPllMul) > 48*1000*1000)
        {
            return ErrorInvalidMode;
        }
    }

    M0P_SYSCTRL->PLL_CR_f.FRSEL  = pstcPLLCfg->enInFreq;
    M0P_SYSCTRL->PLL_CR_f.FOSC   = pstcPLLCfg->enOutFreq;
    M0P_SYSCTRL->PLL_CR_f.DIVN   = pstcPLLCfg->enPllMul;
    M0P_SYSCTRL->PLL_CR_f.REFSEL = pstcPLLCfg->enPllClkSrc;

    return enRet;
}

/**
 *******************************************************************************
 ** \brief ????????TRIM???
 ** \param [in]  enRCLFreq  ???RCL?????
 ** \retval      Ok         ????
 **             ??        ????
 ******************************************************************************/
en_result_t Sysctrl_SetRCLTrim(en_sysctrl_rcl_freq_t enRCLFreq)
{
    M0P_SYSCTRL->RCL_CR_f.TRIM = *(RC_TRIM_BASE_ADDR + enRCLFreq);

    return Ok;
}

/**
 *******************************************************************************
 ** \brief ????(HCLK)????
 ** \param [in]  enHCLKDiv  ?????
 ** \retval      Ok         ????
 **             ??        ????
 ******************************************************************************/
en_result_t Sysctrl_SetHCLKDiv(en_sysctrl_hclk_div_t enHCLKDiv)
{
    _SysctrlUnlock();
    M0P_SYSCTRL->SYSCTRL0_f.HCLK_PRS = enHCLKDiv;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief ????(PCLK)????
 ** \param [in]  enPCLKDiv  ?????
 ** \retval      Ok         ????
 **             ??        ????
 ******************************************************************************/
en_result_t Sysctrl_SetPCLKDiv(en_sysctrl_pclk_div_t enPCLKDiv)
{
    _SysctrlUnlock();
    M0P_SYSCTRL->SYSCTRL0_f.PCLK_PRS = enPCLKDiv;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief ??????????
 ** \param [in]  enPeripheral   ????
 ** \param [in]  bFlag          ????
 ** \retval      Ok             ????
 **             ??            ????
 ******************************************************************************/
en_result_t Sysctrl_SetPeripheralGate(en_sysctrl_peripheral_gate_t enPeripheral, boolean_t bFlag)
{
    if(enPeripheral&0x20u)
    {
        enPeripheral &= ~0x20u;
        SetBit((uint32_t)(&(M0P_SYSCTRL->PERI_CLKEN1)), enPeripheral, bFlag);
    }
    else
    {
        SetBit((uint32_t)(&(M0P_SYSCTRL->PERI_CLKEN0)), enPeripheral, bFlag);
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief ????????????
 ** \param [in]  enPeripheral   ????
 ** \retval      TRUE           ?
 **              FALSE          ?
 ******************************************************************************/
boolean_t Sysctrl_GetPeripheralGate(en_sysctrl_peripheral_gate_t enPeripheral)
{
    if(enPeripheral&0x20u)
    {
        enPeripheral &= ~0x20u;
        return GetBit((uint32_t)(&(M0P_SYSCTRL->PERI_CLKEN1)), enPeripheral);
    }
    else
    {
        return GetBit((uint32_t)(&(M0P_SYSCTRL->PERI_CLKEN0)), enPeripheral);
    }

}

/**
 *******************************************************************************
 ** \brief ??????
 ** \param [in]  enFunc     ????????
 ** \param [in]  bFlag      1-?/0-?
 ** \retval      Ok         ????
 **             ??        ????
 ******************************************************************************/
en_result_t Sysctrl_SetFunc(en_sysctrl_func_t enFunc, boolean_t bFlag)
{
    _SysctrlUnlock();
    SetBit((uint32_t)(&(M0P_SYSCTRL->SYSCTRL1)), enFunc, bFlag);

    return Ok;
}

/**
 *******************************************************************************
 ** \brief ??RTC??????
 ** \param [in]  enRtcAdj   ?????
 ** \retval      Ok         ????
 **             ??        ????
 ******************************************************************************/
en_result_t Sysctrl_SetRTCAdjustClkFreq(en_sysctrl_rtc_adjust_t enRtcAdj)
{
    _SysctrlUnlock();
    M0P_SYSCTRL->SYSCTRL1_f.RTC_FREQ_ADJUST = enRtcAdj;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief USB ????
 ** \param [in]  enUsbClk   @ref en_sysctrl_usbclk_sel_t
 ** \retval      Null
 ******************************************************************************/
void Sysctrl_UsbClkSel(en_sysctrl_usbclk_sel_t enUsbClk)
{
    _SysctrlUnlock();
    M0P_SYSCTRL->SYSCTRL1_f.USB48MSEL = enUsbClk;

}

/**
 *******************************************************************************
 ** \brief ?????????
 ** \param [in]  enTimClk   @ref en_sysctrl_timer_pllclk_sel_t
 ** \retval      Null
 ******************************************************************************/
void Sysctrl_TimerPllClkSel(en_sysctrl_timer_pllclk_sel_t enTimClk)
{
    _SysctrlUnlock();
    M0P_SYSCTRL->OVCK_CR = enTimClk;

}
//@} // SysctrlGroup

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
