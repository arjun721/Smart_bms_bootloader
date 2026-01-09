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
 * @file   gpio.h
 *
 * @brief  Header file for GPIO functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

#ifndef __GPIO_H__
#define __GPIO_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"


/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif

/**
 *******************************************************************************
 ** \defgroup GpioGroup General Purpose I/O (GPIO)
 **
 **
 ******************************************************************************/
//@{
#define GPIO_GPSZ       (0x40u)

/*******************************************************************************
 * Global type definitions
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief GPIO PORT????
 ******************************************************************************/
typedef enum en_gpio_port
{
    GpioPortA = 0x00u,                 ///< GPIO PORT A
    GpioPortB = 0x40u,                 ///< GPIO PORT B
    GpioPortC = 0x80u,                 ///< GPIO PORT C
    GpioPortD = 0xc0u,                 ///< GPIO PORT D
    GpioPortE = 0x1000u,               ///< GPIO PORT E
    GpioPortF = 0x1040u,               ///< GPIO PORT F
}en_gpio_port_t;

/**
 *******************************************************************************
 ** \brief GPIO PIN????
 ******************************************************************************/
typedef enum en_gpio_pin
{
    GpioPin0  = 0u,                 ///< GPIO PIN0
    GpioPin1  = 1u,                 ///< GPIO PIN1
    GpioPin2  = 2u,                 ///< GPIO PIN2
    GpioPin3  = 3u,                 ///< GPIO PIN3
    GpioPin4  = 4u,                 ///< GPIO PIN4
    GpioPin5  = 5u,                 ///< GPIO PIN5
    GpioPin6  = 6u,                 ///< GPIO PIN6
    GpioPin7  = 7u,                 ///< GPIO PIN7
    GpioPin8  = 8u,                 ///< GPIO PIN8
    GpioPin9  = 9u,                 ///< GPIO PIN9
    GpioPin10 = 10u,                ///< GPIO PIN10
    GpioPin11 = 11u,                ///< GPIO PIN11
    GpioPin12 = 12u,                ///< GPIO PIN12
    GpioPin13 = 13u,                ///< GPIO PIN13
    GpioPin14 = 14u,                ///< GPIO PIN14
    GpioPin15 = 15u,                ///< GPIO PIN15
}en_gpio_pin_t;

/**
 *******************************************************************************
 ** \brief GPIO ??????(AF-Alternate function)????
 ** \note  ??????????????GPIO??????
 ******************************************************************************/
typedef enum en_gpio_af
{
    GpioAf0  = 0u,      ///< GPIO??
    GpioAf1  = 1u,      ///< GPIO AF1:????1
    GpioAf2  = 2u,      ///< GPIO AF2:????2
    GpioAf3  = 3u,      ///< GPIO AF3:????3
    GpioAf4  = 4u,      ///< GPIO AF4:????4
    GpioAf5  = 5u,      ///< GPIO AF5:????5
    GpioAf6  = 6u,      ///< GPIO AF6:????6
    GpioAf7  = 7u,      ///< GPIO AF7:????7
}en_gpio_af_t;

/**
 *******************************************************************************
 ** \brief GPIO????????????
 ******************************************************************************/
typedef enum en_gpio_dir
{
    GpioDirOut = 0u,                ///< GPIO ??
    GpioDirIn  = 1u,                ///< GPIO ??
}en_gpio_dir_t;

/**
 *******************************************************************************
 ** \brief GPIO????????????
 ******************************************************************************/
typedef enum en_gpio_pu
{
    GpioPuDisable = 0u,                ///< GPIO???
    GpioPuEnable  = 1u,                ///< GPIO??
}en_gpio_pu_t;

/**
 *******************************************************************************
 ** \brief GPIO????????????
 ******************************************************************************/
typedef enum en_gpio_pd
{
    GpioPdDisable = 0u,                ///< GPIO???
    GpioPdEnable  = 1u,                ///< GPIO??
}en_gpio_pd_t;
/**
 *******************************************************************************
 ** \brief GPIO????????????????
 ******************************************************************************/
typedef enum en_gpio_drv
{
    GpioDrvH = 0u,                  ///< GPIO?????
    GpioDrvL = 1u,                  ///< GPIO?????
}en_gpio_drv_t;

/**
 *******************************************************************************
 ** \brief GPIO??????????????
 ******************************************************************************/
typedef enum en_gpio_od
{
    GpioOdDisable = 0u,             ///< GPIO??????
    GpioOdEnable  = 1u,             ///< GPIO??????
}en_gpio_od_t;

/**
 *******************************************************************************
 ** \brief GPIO????/??????????????
 ******************************************************************************/
typedef enum en_gpio_ctrl_mode
{
    GpioFastIO = 0u,             ///< FAST IO ??????
    GpioAHB  = 1u,               ///< AHB ??????
}en_gpio_ctrl_mode_t;

/**
 *******************************************************************************
 ** \brief GPIO??????????
 ******************************************************************************/
typedef enum en_gpio_irqtype
{
    GpioIrqHigh     = 0u,           ///< GPIO?????
    GpioIrqLow      = 1u,           ///< GPIO?????
    GpioIrqRising   = 2u,           ///< GPIO?????
    GpioIrqFalling  = 3u,           ///< GPIO?????
}en_gpio_irqtype_t;

/**
 *******************************************************************************
 ** \brief GPIO ????(SF-Secondary Function)??????????
 ******************************************************************************/
typedef enum en_gpio_sf_irqmode
{
    GpioSfIrqDpslpMode  = 1u,        ///< Deep Sleep??
    GpioSfIrqActSlpMode = 0u,        ///< Active/Sleep??
}en_gpio_sf_irqmode_t;

/**
 *******************************************************************************
 ** \brief GPIO ????(SF-Secondary Function)HCLK????????
 ******************************************************************************/
typedef enum en_gpio_sf_hclkout_g
{
    GpioSfHclkOutDisable = 0u,       ///< HCLK??????
    GpioSfHclkOutEnable  = 1u,       ///< HCLK??????
}en_gpio_sf_hclkout_g_t;

/**
 *******************************************************************************
 ** \brief GPIO ????(SF-Secondary Function)HCLK??????????
 ******************************************************************************/
typedef enum en_gpio_sf_hclkout_div
{
    GpioSfHclkOutDiv1 = 0u,         ///< HCLK
    GpioSfHclkOutDiv2 = 1u,         ///< HCLK/2
    GpioSfHclkOutDiv4 = 2u,         ///< HCLK/4
    GpioSfHclkOutDiv8 = 3u,         ///< HCLK/8
}en_gpio_sf_hclkout_div_t;
/**
 *******************************************************************************
 ** \brief GPIO ????(SF-Secondary Function)PCLK????????
 ******************************************************************************/
typedef enum en_gpio_sf_pclkout_g
{
    GpioSfPclkOutDisable = 0u,       ///< PCLK??????
    GpioSfPclkOutEnable  = 1u,       ///< PCLK??????
}en_gpio_sf_pclkout_g_t;

/**
 *******************************************************************************
 ** \brief GPIO ????(SF-Secondary Function)PCLK??????????
 ******************************************************************************/
typedef enum en_gpio_sf_pclkout_div
{
    GpioSfPclkOutDiv1 = 0u,         ///< PCLK
    GpioSfPclkOutDiv2 = 1u,         ///< PCLK/2
    GpioSfPclkOutDiv4 = 2u,         ///< PCLK/4
    GpioSfPclkOutDiv8 = 3u,         ///< PCLK/8
}en_gpio_sf_pclkout_div_t;

/**
 *******************************************************************************
 ** \brief GPIO ????(SF-Secondary Function)IR??????????
 ******************************************************************************/
typedef enum en_gpio_sf_irpol
{
    GpioSfIrP = 0u,                  ///< IR????
    GpioSfIrN = 1u,                  ///< IR????
}en_gpio_sf_irpol_t;

/**
 *******************************************************************************
 ** \brief GPIO ????(SF-Secondary Function)SSN??????
 ******************************************************************************/
typedef enum en_gpio_sf_ssnspi
{
    GpioSpi0 = 0u,                   ///< SPI0 SSN
    GpioSpi1 = 1u,                   ///< SPI1 SSN
}en_gpio_sf_ssnspi_t;

/**
 *******************************************************************************
 ** \brief GPIO ????(SF-Secondary Function)SSN????????????????
 ******************************************************************************/
typedef enum en_gpio_sf_ssn_extclk
{
    GpioSfSsnExtClkH    = 0u,        ///< ???
    GpioSfSsnExtClkPA03 = 1u,        ///< PA03
    GpioSfSsnExtClkPA04 = 2u,        ///< PA04
    GpioSfSsnExtClkPA06 = 3u,        ///< PA06
    GpioSfSsnExtClkPA08 = 4u,        ///< PA08
    GpioSfSsnExtClkPA09 = 5u,        ///< PA09
    GpioSfSsnExtClkPA12 = 6u,        ///< PA12
    GpioSfSsnExtClkPA15 = 7u,        ///< PA15
    GpioSfSsnExtClkPB01 = 8u,        ///< PB01
    GpioSfSsnExtClkPB02 = 9u,        ///< PB02
    GpioSfSsnExtClkPB05 = 10u,       ///< PB05
    GpioSfSsnExtClkPB06 = 11u,       ///< PB06
    GpioSfSsnExtClkPB09 = 12u,       ///< PB09
    GpioSfSsnExtClkPB10 = 13u,       ///< PB10
    GpioSfSsnExtClkPB12 = 14u,       ///< PB12
    GpioSfSsnExtClkPB14 = 15u,       ///< PB14
}en_gpio_sf_ssn_extclk_t;

/**
 *******************************************************************************
 ** \brief GPIO ????(SF-Secondary Function)?????????????
 ** \note  ??????????????GPIO???????
 ******************************************************************************/
typedef enum en_gpio_sf
{
    GpioSf0 = 0u,       ///< SF0:PX_SEL?????
    GpioSf1 = 1u,       ///< SF1:????1
    GpioSf2 = 2u,       ///< SF2:????2
    GpioSf3 = 3u,       ///< SF3:????3
    GpioSf4 = 4u,       ///< SF4:????4
    GpioSf5 = 5u,       ///< SF5:????5
    GpioSf6 = 6u,       ///< SF6:????6
    GpioSf7 = 7u,       ///< SF7:????7
}en_gpio_sf_t;

/**
 *******************************************************************************
 ** \brief GPIO ????(SF-Secondary Function)???????????????
 ******************************************************************************/
typedef enum en_gpio_sf_tim_g
{
    GpioSfTim0G   = 0u,                   ///<Tim0???GATE????
    GpioSfTim1G   = 3u,                   ///<Tim1???GATE????
    GpioSfTim2G   = 6u,                   ///<Tim2???GATE????
    GpioSfTim3G   = 9u,                   ///<Tim3???GATE????
    GpioSfLpTim0G = 12u,                  ///<LPTim0???GATE????
    GpioSfLpTim1G = 38u,                  ///<LPTim1???GATE????
}en_gpio_sf_tim_g_t;

/**
 *******************************************************************************
 ** \brief GPIO ????(SF-Secondary Function)???ETR??????????
 ******************************************************************************/
typedef enum en_gpio_sf_tim_e
{
    GpioSfTim0E   = 0u,                   ///<Tim0???ETR????
    GpioSfTim1E   = 3u,                   ///<Tim1???ETR????
    GpioSfTim2E   = 6u,                   ///<Tim2???ETR????
    GpioSfTim3E   = 9u,                   ///<Tim3???ETR????
    GpioSfLpTim0E = 12u,                  ///<LPTim0???ETR????
    GpioSfLpTim1E = 41u,                  ///<LPTim1???ETR????
}en_gpio_sf_tim_e_t;

/**
 *******************************************************************************
 ** \brief GPIO ????(SF-Secondary Function)?????????????????
 ******************************************************************************/
typedef enum en_gpio_sf_tim_c
{
    GpioSfTim0CA = 0u,                   ///<Tim0???CHA????
    GpioSfTim1CA = 3u,                   ///<Tim1???CHA????
    GpioSfTim2CA = 6u,                   ///<Tim2???CHA????
    GpioSfTim3CA = 9u,                   ///<Tim3???CH0A????
    GpioSfTim3CB = 12u,                  ///<Tim3???CH0B????
}en_gpio_sf_tim_c_t;

/**
 *******************************************************************************
 ** \brief GPIO ????(SF-Secondary Function)PCA??????????
 ******************************************************************************/
typedef enum en_gpio_sf_pca
{
    GpioSfPcaCH0 = 0u,                   ///<PCA_CH0???????
    GpioSfPcaECI = 3u,                   ///<PCA ECI??????
}en_gpio_sf_pca_t;

/**
 *******************************************************************************
 ** \brief GPIO ????(SF-Secondary Function)PCNT????????????
 ******************************************************************************/
typedef enum en_gpio_sf_pcnt
{
    GpioSfPcntS0 = 0u,                   ///<PCNT_S0????
    GpioSfPcntS1 = 3u,                   ///<PCNT_S1????
}en_gpio_sf_pcnt_t;

/**
 *******************************************************************************
 ** \brief GPIO ?????????
 ******************************************************************************/
typedef struct
{
    boolean_t           bOutputVal;      ///< ????????
    en_gpio_dir_t       enDir;           ///< ??????
    en_gpio_drv_t       enDrv;           ///< ????????
    en_gpio_pu_t        enPu;            ///< ??????
    en_gpio_pd_t        enPd;            ///< ??????
    en_gpio_od_t        enOD;            ///< ????????
    en_gpio_ctrl_mode_t enCtrlMode;      ///< ????/??????????????
}stc_gpio_cfg_t;

/*******************************************************************************
 * Global definitions
 ******************************************************************************/
 
/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/******************************************************************************
  Global function prototypes (definition in C source)
*******************************************************************************/
 ///< GPIO IO???/????
en_result_t Gpio_Init(en_gpio_port_t enPort, en_gpio_pin_t enPin, stc_gpio_cfg_t *pstcGpioCfg);
 
///< GPIO ????????
boolean_t   Gpio_GetInputIO(en_gpio_port_t enPort, en_gpio_pin_t enPin);
uint16_t    Gpio_GetInputData(en_gpio_port_t enPort);

///< GPIO ??????
///< GPIO ???????????
en_result_t Gpio_WriteOutputIO(en_gpio_port_t enPort, en_gpio_pin_t enPin, boolean_t bVal);
boolean_t   Gpio_ReadOutputIO(en_gpio_port_t enPort, en_gpio_pin_t enPin);
///< GPIO ??/????????
en_result_t Gpio_SetPort(en_gpio_port_t enPort, uint16_t u16ValMsk);
en_result_t Gpio_SetIO(en_gpio_port_t enPort, en_gpio_pin_t enPin);
///< GPIO ??/????????
en_result_t Gpio_ClrPort(en_gpio_port_t enPort, uint16_t u16ValMsk);
en_result_t Gpio_ClrIO(en_gpio_port_t enPort, en_gpio_pin_t enPin);
///< GPIO ?????????????
en_result_t Gpio_SetClrPort(en_gpio_port_t enPort, uint32_t u32ValMsk);

///< GPIO ?????????
en_result_t Gpio_SetAnalogMode(en_gpio_port_t enPort, en_gpio_pin_t enPin);

///< GPIO ???????????
en_result_t Gpio_SetAfMode(en_gpio_port_t enPort, en_gpio_pin_t enPin, en_gpio_af_t enAf);

///< GPIO ??????????/??
en_result_t Gpio_EnableIrq(en_gpio_port_t enPort, en_gpio_pin_t enPin, en_gpio_irqtype_t enType);
en_result_t Gpio_DisableIrq(en_gpio_port_t enPort, en_gpio_pin_t enPin, en_gpio_irqtype_t enType);
///< GPIO ??????
boolean_t   Gpio_GetIrqStatus(en_gpio_port_t enPort, en_gpio_pin_t enPin);
///< GPIO ??????
en_result_t Gpio_ClearIrq(en_gpio_port_t enPort, en_gpio_pin_t enPin);

///< GPIO ????????
///< GPIO ??????
en_result_t Gpio_SfIrqModeCfg(en_gpio_sf_irqmode_t enIrqMode);
///< GPIO IR??????
en_result_t Gpio_SfIrPolCfg(en_gpio_sf_irpol_t enIrPolMode);
///< GPIO HCLK????
en_result_t Gpio_SfHClkOutputCfg(en_gpio_sf_hclkout_g_t enGate, en_gpio_sf_hclkout_div_t enDiv);
///< GPIO PCLK????
en_result_t Gpio_SfPClkOutputCfg(en_gpio_sf_pclkout_g_t enGate, en_gpio_sf_pclkout_div_t enDiv);
///< GPIO ????????
en_result_t Gpio_SfExtClkCfg(en_gpio_sf_ssn_extclk_t enExtClk);
///< GPIO SPI SSN????
en_result_t Gpio_SfSsnCfg(en_gpio_sf_ssnspi_t enSpi, en_gpio_sf_ssn_extclk_t enSsn);
///< GPIO Timer ??????
en_result_t Gpio_SfTimGCfg(en_gpio_sf_tim_g_t enTimG, en_gpio_sf_t enSf);
///< GPIO Timer ETR????
en_result_t Gpio_SfTimECfg(en_gpio_sf_tim_e_t enTimE, en_gpio_sf_t enSf);
///< GPIO Timer ??????
en_result_t Gpio_SfTimCCfg(en_gpio_sf_tim_c_t enTimC, en_gpio_sf_t enSf);
///< GPIO PCA??????
en_result_t Gpio_SfPcaCfg(en_gpio_sf_pca_t enPca, en_gpio_sf_t enSf);
///< GPIO PCNT??????
en_result_t Gpio_SfPcntCfg(en_gpio_sf_pcnt_t enPcnt, en_gpio_sf_t enSf);

//@} // GpioGroup

#ifdef __cplusplus
}
#endif

#endif /* __GPIO_H__ */
/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
