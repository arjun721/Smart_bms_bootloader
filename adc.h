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
 * @file   adc.h
 *
 * @brief  Header file for ADC functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

#ifndef __ADC_H__
#define __ADC_H__


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
 ** \defgroup AdcGroup AD Converter (ADC)
  **
 ******************************************************************************/
 
//@{

/******************************************************************************
 * Global definitions
 ******************************************************************************/
#define ADC_SCAN_CH0_EN     (0x1u)           /*!< SCAN????ADC CH0 */
#define ADC_SCAN_CH1_EN     (0x1u<<1)        /*!< SCAN????ADC CH1 */
#define ADC_SCAN_CH2_EN     (0x1u<<2)        /*!< SCAN????ADC CH2 */
#define ADC_SCAN_CH3_EN     (0x1u<<3)        /*!< SCAN????ADC CH3 */
#define ADC_SCAN_CH4_EN     (0x1u<<4)        /*!< SCAN????ADC CH4 */
#define ADC_SCAN_CH5_EN     (0x1u<<5)        /*!< SCAN????ADC CH5 */
#define ADC_SCAN_CH6_EN     (0x1u<<6)        /*!< SCAN????ADC CH6 */
#define ADC_SCAN_CH7_EN     (0x1u<<7)        /*!< SCAN????ADC CH7 */


/******************************************************************************
 ** Global type definitions
 *****************************************************************************/

 /**
 ******************************************************************************
 ** \brief ADC????
 *****************************************************************************/
typedef enum en_adc_mode
{
    AdcSglMode  = 0u,           /*!< ??????????? */
    AdcScanMode = 1u,           /*!< ???????/????????*/   
}en_adc_mode_t;

/**
 ******************************************************************************
 ** \brief ADC??????
 *****************************************************************************/
typedef enum en_adc_clk_sel
{
    AdcMskClkDiv1  = 0u<<2,        /*!< PCLK */
    AdcMskClkDiv2  = 1u<<2,        /*!< 1/2 PCLK */
    AdcMskClkDiv4  = 2u<<2,        /*!< 1/4 PCLK */
    AdcMskClkDiv8  = 3u<<2,        /*!< 1/8 PCLK */
} en_adc_clk_div_t;

/**
 ******************************************************************************
 ** \brief ADC????
 *****************************************************************************/
typedef enum en_adc_ref_vol_sel
{
    AdcMskRefVolSelInBgr1p5 = 0u<<9,        /*!<??????1.5V(SPS<=200kHz)*/
    AdcMskRefVolSelInBgr2p5 = 1u<<9,        /*!<??????2.5V(avdd>3V,SPS<=200kHz)*/
    AdcMskRefVolSelExtern1  = 2u<<9,        /*!<????(max avdd)   PB01*/
    AdcMskRefVolSelAVDD     = 3u<<9,        /*!<AVDD*/
}en_adc_ref_vol_sel_t;

/**
 ******************************************************************************
 ** \brief ADC??????
 *****************************************************************************/
typedef enum en_adc_samp_ch_sel
{                                  /*!<CHMAP = 0*/      /*!<CHMAP = 1*/
    AdcExInputCH0    =  0u,        /*!<??PA00*/       /*!<??PD08*/
    AdcExInputCH1    =  1u,        /*!<??PA01*/       /*!<??PD09*/
    AdcExInputCH2    =  2u,        /*!<??PA02*/       /*!<??PD10*/
    AdcExInputCH3    =  3u,        /*!<??PA03*/       /*!<??PD11*/
    AdcExInputCH4    =  4u,        /*!<??PA04*/       /*!<??PA04*/
    AdcExInputCH5    =  5u,        /*!<??PA05*/       /*!<??PA05*/
    AdcExInputCH6    =  6u,        /*!<??PA06*/       /*!<??PE08*/
    AdcExInputCH7    =  7u,        /*!<??PA07*/       /*!<??PE09*/
    AdcExInputCH8    =  8u,        /*!<??PB00*/       /*!<??PE10*/
    AdcExInputCH9    =  9u,        /*!<??PB01*/       /*!<??PB01*/
    AdcExInputCH10   =  10u,       /*!<??PC00*/       /*!<??PE11*/
    AdcExInputCH11   =  11u,       /*!<??PC01*/       /*!<??PE12*/
    AdcExInputCH12   =  12u,       /*!<??PC02*/       /*!<??PE13*/
    AdcExInputCH13   =  13u,       /*!<??PC03*/       /*!<??PE14*/
    AdcExInputCH14   =  14u,       /*!<??PC04*/       /*!<??PC04*/
    AdcExInputCH15   =  15u,       /*!<??PC05*/       /*!<??PC05*/
    AdcExInputCH16   =  16u,       /*!<??PB02*/       /*!<??PB02*/
    AdcExInputCH17   =  17u,       /*!<??PB10*/       /*!<??PB10*/
    AdcExInputCH18   =  18u,       /*!<??PB11*/       /*!<??PB11*/
    AdcExInputCH19   =  19u,       /*!<??PB12*/       /*!<??PB12*/
    AdcExInputCH20   =  20u,       /*!<??PB13*/       /*!<??PB13*/
    AdcExInputCH21   =  21u,       /*!<??PB14*/       /*!<??PB14*/
    AdcExInputCH22   =  22u,       /*!<??PB15*/       /*!<??PB15*/
    AdcExInputCH23   =  23u,       /*!<??PE15*/       /*!<??PE15*/
    AdcExInputCH24   =  24u,       /*!<??PC07*/       /*!<??PC07*/

    AdcDac0Input     =  25u,       /*!<??DAC1??(????????)*/
    AdcDac1Input     =  26u,       /*!<??DAC1??(????????)*/                    
    AdcAVccdiv3Input =  27u,       /*!<??1/3 AVCC(????????)*/
    AdcAiTsInput     =  28u,       /*!<?????????BGR_TS(????????)*/
    AdcVref1_2Input  =  29u,       /*!<??????1.2V(????????)*/

}en_adc_samp_ch_sel_t;

 /**
 ******************************************************************************
 ** \brief ADC?????????
 *****************************************************************************/
typedef enum en_adc_op_buf
{
    AdcMskBufEnable  = 1u<<11,          /*!< ?????BUF */
    AdcMskBufDisable = 0u,              /*!< ?????BUF */  
} en_adc_op_buf_t;

/**
 ******************************************************************************
 ** \brief ADC??????
 *****************************************************************************/
typedef enum en_adc_samp_cycle_sel
{
    AdcMskSampCycle4Clk  =  0u<<12,        /*!<4?????*/
    AdcMskSampCycle6Clk  =  1u<<12,        /*!<6?????*/
    AdcMskSampCycle8Clk  =  2u<<12,        /*!<8?????*/
    AdcMskSampCycle12Clk =  3u<<12,        /*!<12?????*/

}en_adc_samp_cycle_sel_t;

 /**
 ******************************************************************************
 ** \brief ADC??????????
 *****************************************************************************/
typedef enum en_adc_in_ref
{
    AdcMskInRefEnable  = 1u<<14,          /*!< ???????? */
    AdcMskInRefDisable = 0u,              /*!< ???????? */  
}en_adc_in_ref_t;

/**
 ******************************************************************************
 ** \brief ADC?????????
 *****************************************************************************/
typedef enum en_adc_trig_sel
{
    AdcMskTrigTimer0     =  1u<<0,         /*!<??timer0???,????ADC??*/
    AdcMskTrigTimer1     =  1u<<1,         /*!<??timer1???,????ADC??*/
    AdcMskTrigTimer2     =  1u<<2,         /*!<??timer2???,????ADC??*/
    AdcMskTrigTimer3     =  1u<<3,         /*!<??timer3???,????ADC??*/
    AdcMskTrigTimer4     =  1u<<4,         /*!<??timer4???,????ADC??*/
    AdcMskTrigTimer5     =  1u<<5,         /*!<??timer5???,????ADC??*/
    AdcMskTrigTimer6     =  1u<<6,         /*!<??timer6???,????ADC??*/
    AdcMskTrigUart0      =  1u<<7,         /*!<??uart0???,????ADC??*/
    AdcMskTrigUart1      =  1u<<8,         /*!<??uart1???,????ADC??*/
    AdcMskTrigLpuart0    =  1u<<9,         /*!<??lpuart0???,????ADC??*/
    AdcMskTrigLpuart1    =  1u<<10,        /*!<??lpuart1???,????ADC??*/
    AdcMskTrigVC0        =  1u<<11,        /*!<??VC0???,????ADC??*/
    AdcMskTrigVC1        =  1u<<12,        /*!<??VC1???,????ADC??*/
    AdcMskTrigRTC        =  1u<<13,        /*!<??RTC???,????ADC??*/
    AdcMskTrigPCA        =  1u<<14,        /*!<??PCA???,????ADC??*/
    AdcMskTrigSPI0       =  1u<<15,        /*!<??SPI0???,????ADC??*/
    AdcMskTrigSPI1       =  1u<<16,        /*!<??SPI1???,????ADC??*/
    AdcMskTrigDMA        =  1u<<17,        /*!<??DMA???,????ADC??*/
    AdcMskTrigPA03       =  1u<<18,        /*!<??PA03???,????ADC??*/
    AdcMskTrigPB03       =  1u<<19,        /*!<??PB03???,????ADC??*/
    AdcMskTrigPC03       =  1u<<20,        /*!<??PC03???,????ADC??*/
    AdcMskTrigPD03       =  1u<<21,        /*!<??PD03???,????ADC??*/
    AdcMskTrigPA07       =  1u<<22,        /*!<??PA07???,????ADC??*/
    AdcMskTrigPB07       =  1u<<23,        /*!<??PB07???,????ADC??*/
    AdcMskTrigPC07       =  1u<<24,        /*!<??PC07???,????ADC??*/
    AdcMskTrigPD07       =  1u<<25,        /*!<??PD07???,????ADC??*/
    AdcMskTrigPA11       =  1u<<26,        /*!<??PA11???,????ADC??*/
    AdcMskTrigPB11       =  1u<<27,        /*!<??PB11???,????ADC??*/
    AdcMskTrigPC11       =  1u<<28,        /*!<??PC11???,????ADC??*/
    AdcMskTrigPA15       =  1u<<29,        /*!<??PA15???,????ADC??*/
    AdcMskTrigPB15       =  1u<<30,        /*!<??PB15???,????ADC??*/
    AdcMskTrigPC15       =  1u<<31,        /*!<??PC15???,????ADC??*/
}en_adc_trig_sel_t;

/**
 ******************************************************************************
 ** \brief ADC??????????
 *****************************************************************************/
typedef enum en_adc_ext_trig_sel
{
    AdcExtTrig0     =  0u,         /*!<????????? ??????????*/
    AdcExtTrig1     =  1u,         /*!<?????? ??????????*/
}en_adc_ext_trig_sel_t;

/**
 ******************************************************************************
 ** \brief ADC??????
 *****************************************************************************/
typedef enum en_adc_sqr_chmux
{
    AdcSQRCH0MUX     =  0u,         /*!<??????????0*/
    AdcSQRCH1MUX     =  1u,         /*!<??????????1*/
    AdcSQRCH2MUX     =  2u,         /*!<??????????2*/
    AdcSQRCH3MUX     =  3u,         /*!<??????????3*/
    AdcSQRCH4MUX     =  4u,         /*!<??????????4*/
    AdcSQRCH5MUX     =  5u,         /*!<??????????5*/
    AdcSQRCH6MUX     =  6u,         /*!<??????????6*/
    AdcSQRCH7MUX     =  7u,         /*!<??????????7*/
    AdcSQRCH8MUX     =  8u,         /*!<??????????8*/
    AdcSQRCH9MUX     =  9u,         /*!<??????????9*/
    AdcSQRCH10MUX    =  10u,        /*!<??????????10*/
    AdcSQRCH11MUX    =  11u,        /*!<??????????11*/
    AdcSQRCH12MUX    =  12u,        /*!<??????????12*/
    AdcSQRCH13MUX    =  13u,        /*!<??????????13*/
    AdcSQRCH14MUX    =  14u,        /*!<??????????14*/
    AdcSQRCH15MUX    =  15u,        /*!<??????????15*/
}en_adc_sqr_chmux_t;

/**
 ******************************************************************************
 ** \brief ADC??????
 *****************************************************************************/
typedef enum en_adc_jqr_chmux
{
    AdcJQRCH0MUX     =  0u,         /*!<????0*/
    AdcJQRCH1MUX     =  1u,         /*!<????1*/
    AdcJQRCH2MUX     =  2u,         /*!<????2*/
    AdcJQRCH3MUX     =  3u,         /*!<????3*/
}en_adc_jqr_chmux_t;
/**
 ******************************************************************************
 ** \brief ADC??????
 *****************************************************************************/
typedef enum en_adc_align
{
     AdcAlignRight  = 0u,
     AdcAlignLeft   = 1u,
}en_adc_align_t;

/**
 ******************************************************************************
 ** \brief ADC??????????
 *****************************************************************************/
typedef enum en_adc_result_acc
{
     AdcResultAccEnable  = 1u,
     AdcResultAccDisable = 0u,
}en_adc_result_acc_t;

/**
 ******************************************************************************
 ** \brief ADC??????
 *****************************************************************************/
typedef enum en_adc_irq_type
{
    AdcMskIrqJqr =  1u<<5,        /*!<ADC????????*/
    AdcMskIrqSqr =  1u<<4,        /*!<ADC????????*/
    AdcMskIrqReg =  1u<<3,        /*!<ADC?????????*/
    AdcMskIrqHt  =  1u<<2,        /*!<ADC??????HT*/
    AdcMskIrqLt  =  1u<<1,        /*!<ADC??????LT*/
    AdcMskIrqSgl =  1u<<0,        /*!<ADC??????*/

}en_adc_irq_type_t;

/**
 ******************************************************************************
 ** \brief OPA?ADC????
 *****************************************************************************/
typedef enum en_adc_channel_remap_type
{
    AdcMskMuxMap =  0u,        /*!<ADC??SGLMUX???????*/
    AdcMskOpaMap =  1u,        /*!<ADC???????OPA??*/
}en_adc_channel_remap_type_t;

/******************************************************************************
 ** Extern type definitions ('typedef')
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief  ADC????????
 *****************************************************************************/
typedef struct stc_adc_cfg
{
    en_adc_mode_t           enAdcMode;                  /*! ADC????*/
    
    en_adc_clk_div_t        enAdcClkDiv;                /*! ADC????*/
    en_adc_samp_cycle_sel_t enAdcSampCycleSel;          /*! ADC??????*/
    en_adc_ref_vol_sel_t    enAdcRefVolSel;             /*! ADC??????*/
    en_adc_op_buf_t         enAdcOpBuf;                 /*! ADC???????????*/
    en_adc_in_ref_t         enInRef;                    /*! ADC????????*/
    
    en_adc_align_t          enAdcAlign;                 /*! ADC????????*/
}stc_adc_cfg_t;


/**
 ******************************************************************************
 ** \brief  ADC???????????
 *****************************************************************************/
typedef struct stc_adc_sqr_cfg
{
    uint8_t                 u8SqrCnt;                   /*! ADC????????*/
    en_adc_result_acc_t     enResultAcc;                /*! ADC??????????*/
    boolean_t               bSqrDmaTrig;                /*! ADC????????DMA????*/
}stc_adc_sqr_cfg_t;

/**
 ******************************************************************************
 ** \brief  ADC???????????
 *****************************************************************************/
typedef struct stc_adc_jqr_cfg
{
    uint8_t                 u8JqrCnt;                   /*! ADC????????*/
    boolean_t               bJqrDmaTrig;                /*! ADC????????DMA????*/    
}stc_adc_jqr_cfg_t;


/**
 ******************************************************************************
 ** \brief  ADC?????????
 *****************************************************************************/
typedef struct stc_adc_threshold_cfg
{
    boolean_t bAdcRegCmp ;                  /*!ADC????*/ 
    boolean_t bAdcHtCmp ;                   /*!ADC???????*/ 
    boolean_t bAdcLtCmp ;                   /*!ADC???????*/
    
    uint32_t u32AdcHighThd;                 /*!ADC?????*/
    uint32_t u32AdcLowThd;                  /*!ADC?????*/ 
    
    en_adc_samp_ch_sel_t    enSampChSel;    /*!ADC??????*/
    
}stc_adc_threshold_cfg_t;


/******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
//ADC ???
en_result_t Adc_Init(stc_adc_cfg_t* pstcAdcCfg);

//ADC ????
void Adc_EnableIrq(void);
//ADC ????
void Adc_DisableIrq(void);

//ADC ??/????????
boolean_t Adc_GetIrqStatus(en_adc_irq_type_t enAdcIrq);
//ADC ??/????????
void Adc_ClrIrqStatus(en_adc_irq_type_t enAdcIrq);

//ADC ??
void Adc_Enable(void);
//ADC ??
void Adc_Disable(void);

//ADC ????????
en_result_t Adc_SqrModeCfg(stc_adc_sqr_cfg_t* pstcAdcSqrCfg);
//ADC ????????
en_result_t Adc_JqrModeCfg(stc_adc_jqr_cfg_t* pstcAdcJqrCfg);

//ADC Sgl ???????????? 
en_result_t Adc_CfgSglChannel( en_adc_samp_ch_sel_t enstcAdcSampCh);
//ADC SQR ?????????????? 
en_result_t Adc_CfgSqrChannel(en_adc_sqr_chmux_t enstcAdcSqrChMux, en_adc_samp_ch_sel_t enstcAdcSampCh);
//ADC JQR ?????????????? 
en_result_t Adc_CfgJqrChannel(en_adc_jqr_chmux_t enstcAdcJqrChMux, en_adc_samp_ch_sel_t enstcAdcSampCh);

///<ADC ???????????
void Adc_SglExtTrigCfg(en_adc_trig_sel_t enAdcTrigSel, boolean_t bValue);
///<ADC ?????????????
void Adc_SqrExtTrigCfg(en_adc_trig_sel_t enAdcTrigSel, boolean_t bValue);
///<ADC ?????????????
void Adc_JqrExtTrigCfg(en_adc_trig_sel_t enAdcTrigSel, boolean_t bValue);

//ADC ????????
void Adc_ThresholdCfg(stc_adc_threshold_cfg_t* pstcAdcThrCfg);

//ADC ????????
void Adc_SGL_Start(void);
//ADC ????????
void Adc_SGL_Stop(void);

//ADC ??????????????
void Adc_SGL_Always_Start(void);
//ADC ??????????????
void Adc_SGL_Always_Stop(void);

//ADC ??????????
void Adc_SQR_Start(void);
//ADC ??????????
void Adc_SQR_Stop(void);

//ADC ??????????
void Adc_JQR_Start(void);
//ADC ??????????
void Adc_JQR_Stop(void);

//?????????
uint32_t Adc_GetSglResult(void);
//?????????
uint32_t Adc_GetSqrResult(en_adc_sqr_chmux_t enstcAdcSqrChMux);
//?????????
uint32_t Adc_GetJqrResult(en_adc_jqr_chmux_t enstcAdcJqrChMux);

//???????
uint32_t Adc_GetAccResult(void);
//clear ADC ?????????
void Adc_ClrAccResult(void);

///< ADC ?????
void Adc_ChannelRemap(en_adc_channel_remap_type_t enChMap);

//@}
#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */
/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
