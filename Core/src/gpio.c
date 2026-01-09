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
 * @file   gpio.c
 *
 * @brief  Source file for GPIO functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "gpio.h"

/**
 *******************************************************************************
 ** \addtogroup GpioGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define IS_VALID_PIN(port,pin)      (  )
/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')         *
 ******************************************************************************/

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief GPIO ???
 **
 ** \param [in]  enPort          IO Port?
 ** \param [in]  enPin           IO Pin?
 ** \param [in]  pstcGpioCfg     IO ???????
 **
 ** \retval Ok         ????
 **         ???     ????
 ******************************************************************************/
en_result_t Gpio_Init(en_gpio_port_t enPort, en_gpio_pin_t enPin, stc_gpio_cfg_t  *pstcGpioCfg)
{
    //??????,GPIO??
    SetBit((uint32_t)&M0P_GPIO->PAADS + enPort, enPin, FALSE);
    *((uint32_t*)(((uint32_t)(&(M0P_GPIO->PA00_SEL)) + enPort) + (((uint32_t)enPin)<<2))) = GpioAf0;

    //???????
    SetBit(((uint32_t)&M0P_GPIO->PAOUT + enPort), enPin, pstcGpioCfg->bOutputVal);
    //????
    SetBit(((uint32_t)&M0P_GPIO->PADIR + enPort), enPin, (boolean_t)(pstcGpioCfg->enDir));
    //??????
    SetBit(((uint32_t)&M0P_GPIO->PADR + enPort), enPin, (boolean_t)(pstcGpioCfg->enDrv));
    //??/????
    SetBit(((uint32_t)&M0P_GPIO->PAPU + enPort), enPin, (boolean_t)(pstcGpioCfg->enPu));
    SetBit(((uint32_t)&M0P_GPIO->PAPD + enPort), enPin, (boolean_t)(pstcGpioCfg->enPd));
    //??????
    SetBit(((uint32_t)&M0P_GPIO->PAOD + enPort), enPin, (boolean_t)(pstcGpioCfg->enOD));

    M0P_GPIO->CTRL2_f.AHB_SEL = pstcGpioCfg->enCtrlMode;

    return Ok;
}



/**
 *******************************************************************************
 ** \brief GPIO IO?????
 **
 ** \param [in]  enPort          IO Port?
 ** \param [in]  enPin           IO Pin?
 **
 ** \retval boolean_t            IO????
 ******************************************************************************/
boolean_t Gpio_GetInputIO(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
    return GetBit(((uint32_t)&M0P_GPIO->PAIN + enPort), enPin);
}

/**
 *******************************************************************************
 ** \brief GPIO IO Port??????
 **
 ** \param [in]  enPort          IO Port
 **
 ** \retval boolean_t            IO Port??
 ******************************************************************************/
uint16_t    Gpio_GetInputData(en_gpio_port_t enPort)
{
    return (uint16_t)(*((uint32_t *)((uint32_t)&M0P_GPIO->PAIN + enPort)));
}

/**
 *******************************************************************************
 ** \brief GPIO IO?????
 **
 ** \param [in]  enPort          IO Port?
 ** \param [in]  enPin           IO Pin?
 ** \param [out] bVal            ???
 **
 ** \retval en_result_t          Ok      ????
 **                              ???  ????
 ******************************************************************************/
en_result_t Gpio_WriteOutputIO(en_gpio_port_t enPort, en_gpio_pin_t enPin, boolean_t bVal)
{
    SetBit(((uint32_t)&M0P_GPIO->PAOUT + enPort), enPin, bVal);

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO IO?????
 **
 ** \param [in]  enPort          IO Port?
 ** \param [in]  enPin           IO Pin?
 **
 ** \retval boolean_t            IO????
 ******************************************************************************/
boolean_t   Gpio_ReadOutputIO(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
    return GetBit(((uint32_t)&M0P_GPIO->PAOUT + enPort), enPin);
}

/**
 *******************************************************************************
 ** \brief GPIO IO Port??,???????Port????PIN
 **
 ** \param [in]  enPort          IO Port
 ** \param [in]  u16ValMsk       ?Port?16?PIN???,??????PIN???bit?1??
 **
 ** \retval boolean_t            IO Port??
 ******************************************************************************/
en_result_t Gpio_SetPort(en_gpio_port_t enPort, uint16_t u16ValMsk)
{
    *((uint16_t*)(((uint32_t)&(M0P_GPIO->PABSET)) + enPort)) = u16ValMsk;
    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO IO??
 **
 ** \param [in]  enPort          IO Port?
 ** \param [in]  enPin           IO Pin?
 **
 ** \retval en_result_t          Ok      ????
 **                              ???  ????
 ******************************************************************************/
en_result_t Gpio_SetIO(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
    SetBit(((uint32_t)&M0P_GPIO->PABSET + enPort), enPin, TRUE);

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO IO Port??,???????Port????PIN
 **
 ** \param [in]  enPort          IO Port
 ** \param [in]  u16ValMsk       ?Port?16?PIN???,??????PIN???bit?1??
 **
 ** \retval boolean_t            IO Port??
 ******************************************************************************/
en_result_t Gpio_ClrPort(en_gpio_port_t enPort, uint16_t u16ValMsk)
{
    *((uint16_t*)(((uint32_t)&(M0P_GPIO->PABCLR)) + enPort)) = u16ValMsk;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO IO??
 **
 ** \param [in]  enPort          IO Port?
 ** \param [in]  enPin           IO Pin?
 **
 ** \retval en_result_t          Ok      ????
 **                              ???  ????
 ******************************************************************************/
en_result_t Gpio_ClrIO(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
    SetBit(((uint32_t)&M0P_GPIO->PABCLR + enPort), enPin, TRUE);

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO IO Port??/??,?????/????Port????PIN
 **
 ** \param [in]  enPort       IO Port
 ** \param [in]  u32ValMsk    ?16bits???Port?16?PIN?????,
 **                           ?16bits???Port?16?PIN?????,
 **                           ??????PIN???bit?1,???PIN??????1,??PIN???
 **
 ** \retval en_result_t       Ok      ????
 **                           ???  ????
 ******************************************************************************/
en_result_t Gpio_SetClrPort(en_gpio_port_t enPort, uint32_t u32ValMsk)
{
    *((uint32_t*)(((uint32_t)&(M0P_GPIO->PABSETCLR)) + enPort)) = u32ValMsk;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO IO?????????
 **
 ** \param [in]  enPort          IO Port?
 ** \param [in]  enPin           IO Pin?
 **
 ** \retval Ok         ????
 **         ???     ????
 ******************************************************************************/
en_result_t Gpio_SetAnalogMode(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
    SetBit((uint32_t)&M0P_GPIO->PAADS + enPort, enPin, TRUE);

    return Ok;
}

/**
 *******************************************************************************
** \brief GPIO IO??????
 **
 ** \param [in]  enPort    IO Port?
 ** \param [in]  enPin     IO Pin?
 ** \param [in]  enAf      ??????????
 ** \retval Ok             ????
 **         ???         ????
 ******************************************************************************/
en_result_t Gpio_SetAfMode(en_gpio_port_t enPort, en_gpio_pin_t enPin, en_gpio_af_t enAf)
{
    *((uint32_t*)(((uint32_t)(&(M0P_GPIO->PA00_SEL)) + enPort) + (((uint32_t)enPin)<<2))) = enAf;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO IO????
 **
 ** \param [in]  enPort          IO Port?
 ** \param [in]  enPin           IO Pin?
 ** \param [in]  enType          ??????
 **
 ** \retval      Ok            ????
 ******************************************************************************/
en_result_t Gpio_EnableIrq(en_gpio_port_t enPort, en_gpio_pin_t enPin, en_gpio_irqtype_t enType)
{
    uint32_t u32PieAddr;

    u32PieAddr = ((uint32_t)((&M0P_GPIO->PAHIE) + enType)) + enPort;

    SetBit(u32PieAddr, enPin, TRUE);

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO IO????
 **
 ** \param [in]  enPort          IO Port?
 ** \param [in]  enPin           IO Pin?
 ** \param [in]  enType          ??????
 **
 ** \retval      Ok            ????
 ******************************************************************************/
en_result_t Gpio_DisableIrq(en_gpio_port_t enPort, en_gpio_pin_t enPin, en_gpio_irqtype_t enType)
{
    uint32_t u32PieAddr;

    u32PieAddr = ((uint32_t)((&M0P_GPIO->PAHIE) + enType)) + enPort;

    SetBit(u32PieAddr, enPin, FALSE);

    return Ok;
}


/**
 *******************************************************************************
 ** \brief GPIO ??IO????
 **
 ** \param [in]  u8Port          IO Port?
 ** \param [in]  u8Pin           IO Pin?
 **
 ** \retval      IO??????
 ******************************************************************************/
boolean_t Gpio_GetIrqStatus(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
    return GetBit((uint32_t)&M0P_GPIO->PA_STAT + enPort, enPin);
}

/**
 *******************************************************************************
 ** \brief GPIO ??IO????
 **
 ** \param [in]  u8Port          IO Port?
 ** \param [in]  u8Pin           IO Pin?
 **
 ** \retval    Ok       ????
 ******************************************************************************/
en_result_t Gpio_ClearIrq(en_gpio_port_t enPort, en_gpio_pin_t enPin)
{
    SetBit((uint32_t)&M0P_GPIO->PA_ICLR + enPort, enPin, FALSE);

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO ????????——??????
 **
 ** \param [in]  enIrqMode          ??????(??????????)
 **
 ** \retval    Ok       ????
 ******************************************************************************/
en_result_t Gpio_SfIrqModeCfg(en_gpio_sf_irqmode_t enIrqMode)
{
    M0P_GPIO->CTRL0_f.IESEL = enIrqMode;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO ????????——IR??????
 **
 ** \param [in]  enIrPolMode          IR????????
 **
 ** \retval    Ok       ????
 ******************************************************************************/
en_result_t Gpio_SfIrPolCfg(en_gpio_sf_irpol_t enIrPolMode)
{
    M0P_GPIO->CTRL1_f.IR_POL = enIrPolMode;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO ????????——HCLK????
 **
 ** \param [in]  enGate         HCLK????
 ** \param [in]  enDiv          ???????
 **
 ** \retval    Ok       ????
 ******************************************************************************/
en_result_t Gpio_SfHClkOutputCfg(en_gpio_sf_hclkout_g_t enGate, en_gpio_sf_hclkout_div_t enDiv)
{
    M0P_GPIO->CTRL1_f.HCLK_EN  = enGate;
    M0P_GPIO->CTRL1_f.HCLK_SEL = enDiv;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO ????????——PCLK????
 **
 ** \param [in]  enGate         PCLK????
 ** \param [in]  enDiv          ???????
 **
 ** \retval    Ok       ????
 ******************************************************************************/
en_result_t Gpio_SfPClkOutputCfg(en_gpio_sf_pclkout_g_t enGate, en_gpio_sf_pclkout_div_t enDiv)
{
    M0P_GPIO->CTRL1_f.PCLK_EN  = enGate;
    M0P_GPIO->CTRL1_f.PCLK_SEL = enDiv;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO ????????——??????????
 **
 ** \param [in]  enExtClk         ????????????
 **
 ** \retval    Ok       ????
 ******************************************************************************/
en_result_t Gpio_SfExtClkCfg(en_gpio_sf_ssn_extclk_t enExtClk)
{
    M0P_GPIO->CTRL1_f.EXT_CLK_SEL  = enExtClk;

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO ????????——SSN ????????
 **
 ** \param [in]  enSpi         SSN SPI??????
 ** \param [in]  enSsn         SSN ????????
 **
 ** \retval    Ok       ????
 ******************************************************************************/
en_result_t Gpio_SfSsnCfg(en_gpio_sf_ssnspi_t enSpi, en_gpio_sf_ssn_extclk_t enSsn)
{
    //SPI0
    if(enSpi == GpioSpi0)
    {
        M0P_GPIO->CTRL1_f.SSN0_SEL = enSsn;
    }
    //SPI1
    if(enSpi == GpioSpi1)
    {
        M0P_GPIO->CTRL2_f.SSN1_SEL = enSsn;
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO ????????——Timer ??????
 **
 ** \param [in]  enTimG       Timer??????
 ** \param [in]  enSf         Timer????????
 **
 ** \retval    Ok       ????
 ******************************************************************************/
en_result_t Gpio_SfTimGCfg(en_gpio_sf_tim_g_t enTimG, en_gpio_sf_t enSf)
{
    if(enTimG&0x20u)
    {
        enTimG &= ~0x20u;
        M0P_GPIO->PCAS &= (uint32_t)(~(0x07U<<enTimG));
        M0P_GPIO->PCAS |= (uint32_t)(enSf<<enTimG);
    }
    else
    {
        M0P_GPIO->TIMGS &= (uint32_t)(~(0x07U<<enTimG));
        M0P_GPIO->TIMGS |= (uint32_t)(enSf<<enTimG);
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO ????????——Timer ETR????
 **
 ** \param [in]  enTimE       Timer??????
 ** \param [in]  enSf         Timer????????
 **
 ** \retval    Ok       ????
 ******************************************************************************/
en_result_t Gpio_SfTimECfg(en_gpio_sf_tim_e_t enTimE, en_gpio_sf_t enSf)
{
    if(enTimE&0x20u)
    {
        enTimE &= ~0x20u;
        M0P_GPIO->PCAS &= (uint32_t)(~(0x07U<<enTimE));
        M0P_GPIO->PCAS |= (uint32_t)(enSf<<enTimE);
    }
    else
    {
        M0P_GPIO->TIMES &= (uint32_t)(~(0x07U<<enTimE));
        M0P_GPIO->TIMES |= (uint32_t)(enSf<<enTimE);
    }

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO ????????——Timer ????????
 **
 ** \param [in]  enTimC       Timer??????
 ** \param [in]  enSf         Timer????????
 **
 ** \retval    Ok       ????
 ******************************************************************************/
en_result_t Gpio_SfTimCCfg(en_gpio_sf_tim_c_t enTimC, en_gpio_sf_t enSf)
{
    M0P_GPIO->TIMCPS &= (uint32_t)(~(0x07u<<enTimC));
    M0P_GPIO->TIMCPS |= (uint32_t)(enSf<<enTimC);

    return Ok;
}

/**
 *******************************************************************************
 ** \brief GPIO ????????——PCA ????????
 **
 ** \param [in]  enPca        PCA??????
 ** \param [in]  enSf         PCA????????
 **
 ** \retval    Ok       ????
 ******************************************************************************/
en_result_t Gpio_SfPcaCfg(en_gpio_sf_pca_t enPca, en_gpio_sf_t enSf)
{
    M0P_GPIO->PCAS &= (uint32_t)(~(0x07u<<enPca));
    M0P_GPIO->PCAS |= (uint32_t)(enSf<<enPca);

    return Ok;
}

//@} // GpioGroup


/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

