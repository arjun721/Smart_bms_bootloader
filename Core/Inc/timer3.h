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
 * @file   timer3.h
 *
 * @brief  Header file for TIMER3 functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

#ifndef __TIMER3_H__
#define __TIMER3_H__

/*****************************************************************************
 * Include files
 *****************************************************************************/
#include "ddl.h"


#ifdef __cplusplus
extern "C"
{
#endif

/**
 ******************************************************************************
 ** \defgroup Tim3Group Timer3 (TIM3)
  **
 ******************************************************************************/
//@{
    
/******************************************************************************/
/* Global pre-processor symbols/macros ('#define')                            */
/******************************************************************************/

/******************************************************************************
 * Global type definitions
 ******************************************************************************/

/**
 ******************************************************************************
 ** \brief Timer3 ????
 *****************************************************************************/
typedef enum en_tim3_channel
{
    Tim3CH0  = 0u,        ///< Timer3??0
    Tim3CH1  = 1u,        ///< Timer3??1
    Tim3CH2  = 2u,        ///< Timer3??2
}en_tim3_channel_t;

/**
 ******************************************************************************
 ** \brief ????????????? (MODE)(??0/1/23)
 *****************************************************************************/
typedef enum en_tim3_work_mode
{
    Tim3WorkMode0  = 0u,         ///< ?????
    Tim3WorkMode1  = 1u,         ///< PWC??
    Tim3WorkMode2  = 2u,         ///< ?????
    Tim3WorkMode3  = 3u,         ///< ?????
}en_tim3_work_mode_t;

/**
 ******************************************************************************
 ** \brief ??????????? (GATE_P)(??0)
 *****************************************************************************/
typedef enum en_tim3_m0cr_gatep
{
    Tim3GatePositive = 0u,            ///< ?????
    Tim3GateOpposite = 1u,            ///< ?????
}en_tim3_m0cr_gatep_t;

/**
 ******************************************************************************
 ** \brief TIM3 ????? (PRS)(??0/1/23)
 *****************************************************************************/
typedef enum en_tim3_cr_timclkdiv
{
    Tim3PCLKDiv1   = 0u,           ///< Div 1
    Tim3PCLKDiv2   = 1u,           ///< Div 2
    Tim3PCLKDiv4   = 2u,           ///< Div 4
    Tim3PCLKDiv8   = 3u,           ///< Div 8
    Tim3PCLKDiv16  = 4u,           ///< Div 16
    Tim3PCLKDiv32  = 5u,           ///< Div 32
    Tim3PCLKDiv64  = 6u,           ///< Div 64
    Tim3PCLKDiv256 = 7u,           ///< Div 256
}en_tim3_cr_timclkdiv_t;

/**
 ******************************************************************************
 ** \brief ??/?????????????? (CT)(??0/1/23)
 *****************************************************************************/
typedef enum en_tim3_cr_ct
{
    Tim3Timer   = 0u,            ///< ?????,???????PCLK
    Tim3Counter = 1u,            ///< ?????,???????ETR
}en_tim3_cr_ct_t;


/**
 ******************************************************************************
 ** \brief ?????????????? (MD)(??0)
 *****************************************************************************/
typedef enum en_tim3_m0cr_md
{
    Tim332bitFreeMode  = 0u,         ///< 32????/???
    Tim316bitArrMode  = 1u,         ///< ?????16????/???
}en_tim3_m0cr_md_t;

/**
 ******************************************************************************
** \brief TIM3???????????(??0/1/23)
 *****************************************************************************/
typedef enum en_tim3_irq_type
{
    Tim3UevIrq   = 0u,         ///< ??/??????
    Tim3CA0Irq   = 2u,         ///< CH0A??/????(???1/23??)
    Tim3CA1Irq   = 3u,         ///< CH1A??/????(???23??)
    Tim3CA2Irq   = 4u,         ///< CH2A??/????(???23??)
    Tim3CB0Irq   = 5u,         ///< CH0B??/????(???23??)
    Tim3CB1Irq   = 6u,         ///< CH1B??/????(???23??)
    Tim3CB2Irq   = 7u,         ///< CH2B??/????(???23??)
    Tim3CA0E     = 8u,         ///< CH0A????????(???23??)(????)
    Tim3CA1E     = 9u,         ///< CH1A????????(???23??)(????)
    Tim3CA2E     = 10u,        ///< CH2A????????(???23??)(????)
    Tim3CB0E     = 11u,        ///< CH0B????????(???23??)(????)
    Tim3CB1E     = 12u,        ///< CH1B????????(???23??)(????)
    Tim3CB2E     = 13u,        ///< CH2B????????(???23??)(????)
    Tim3BkIrq    = 14u,        ///< ????(???23??)
    Tim3TrigIrq  = 15u,        ///< ????(???23??)   
}en_tim3_irq_type_t;

/**
 ******************************************************************************
 ** \brief ????????????? (Edg1stEdg2nd)(??1)
 *****************************************************************************/
typedef enum en_tim3_m1cr_Edge
{
    Tim3PwcRiseToRise  = 0u,            ///< ???????(??)
    Tim3PwcFallToRise  = 1u,            ///< ???????(???)
    Tim3PwcRiseToFall  = 2u,            ///< ???????(???)
    Tim3PwcFallToFall  = 3u,            ///< ???????(??)
}en_tim3_m1cr_Edge_t;

/**
 ******************************************************************************
 ** \brief PWC??????????????? (Oneshot)(??1)
 *****************************************************************************/
typedef enum en_tim3_m1cr_oneshot
{
    Tim3PwcCycleDetect    = 0u,         ///< PWC????
    Tim3PwcOneShotDetect  = 1u,         ///< PWC????
}en_tim3_m1cr_oneshot_t;

/**
 ******************************************************************************
 ** \brief PWC IA0????????? (IA0S)(??1)
 *****************************************************************************/
typedef enum en_tim3_m1_mscr_ia0s
{
    Tim3IA0Input    = 0u,         ///< IAO??
    Tim3XORInput    = 1u,         ///< IA0 ETR GATE XOR(TIM0/1/2)/IA0 IA1 IA2 XOR(TIM3)
}en_tim3_m1_mscr_ia0s_t;

/**
 ******************************************************************************
 ** \brief PWC IB0????????? (IA0S)(??1)
 *****************************************************************************/
typedef enum en_tim3_m1_mscr_ib0s
{
    Tim3IB0Input    = 0u,         ///< IBO??
    Tim3TsInput     = 1u,         ///< ????TS????
}en_tim3_m1_mscr_ib0s_t;

/**
 ******************************************************************************
 ** \brief ????????? ??????? (CCPA0/CCPB0/ETP/BKP)(??1/23)
 *****************************************************************************/
typedef enum en_tim3_port_polarity
{
    Tim3PortPositive      = 0u,         ///< ??????
    Tim3PortOpposite      = 1u,         ///< ??????
}en_tim3_port_polarity_t;

/**
 ******************************************************************************
 ** \brief ??????????? (FLTET/FLTA0/FLAB0)(??1/23)
 *****************************************************************************/
typedef enum en_tim3_flt
{
    Tim3FltNone          = 0u,         ///< ???
    Tim3FltPCLKCnt3      = 4u,         ///< PCLK 3?????
    Tim3FltPCLKDiv4Cnt3  = 5u,         ///< PCLK/4 3?????
    Tim3FltPCLKDiv16Cnt3 = 6u,         ///< PCLK/16 3?????
    Tim3FltPCLKDiv64Cnt3 = 7u,         ///< PCLK/64 3?????
}en_tim3_flt_t;

/**
 ******************************************************************************
 ** \brief ?????? ??????? (OCMA/OCMB)(??23)
 *****************************************************************************/
typedef enum en_tim3_m23_fltr_ocm
{
    Tim3ForceLow      = 0u,         ///< ???0
    Tim3ForceHigh     = 1u,         ///< ???1
    Tim3CMPForceLow   = 2u,         ///< ????????0
    Tim3CMPForceHigh  = 3u,         ///< ????????1
    Tim3CMPInverse    = 4u,         ///< ?????????
    Tim3CMPOnePrdHigh = 5u,         ///< ?????????????????
    Tim3PWMMode1      = 6u,         ///< ?????PWM mode 1
    Tim3PWMMode2      = 7u,         ///< ?????PWM mode 2
}en_tim3_m23_fltr_ocm_t;

/**
 ******************************************************************************
 ** \brief ????TS??????? (TS)(??1/23)
 *****************************************************************************/
typedef enum en_tim3_mscr_ts
{
    Tim3Ts0ETR        = 0u,         ///< ETR??????????????
    Tim3Ts1TIM0TRGO   = 1u,         ///< Timer0?TRGO????
    Tim3Ts2TIM1TRGO   = 2u,         ///< Timer1?TRGO????
    Tim3Ts3TIM2TRGO   = 3u,         ///< Timer2?TRGO????
    Tim3Ts4TIM3TRGO   = 4u,         ///< Timer3?TRGO????
    //Tim3Ts5IA0ED    = 5u,         ///< ??
    Tim3Ts6IAFP       = 6u,         ///< CH0A ???????????????
    Tim3Ts7IBFP       = 7u,         ///< CH0B ??????????????
}en_tim3_mscr_ts_t;

/**
 ******************************************************************************
 ** \brief PWM????????????? (COMP)(??23)
 *****************************************************************************/
typedef enum en_tim3_m23cr_comp
{
    Tim3IndependentPWM      = 0u,         ///< ??PWM??
    Tim3ComplementaryPWM    = 1u,         ///< ??PWM??
}en_tim3_m23cr_comp_t;

/**
 ******************************************************************************
 ** \brief ????????????? (DIR)(??23)
 *****************************************************************************/
typedef enum en_tim3_m23cr_dir
{
    Tim3CntUp      = 0u,         ///< ????
    Tim3CntDown    = 1u,         ///< ????
}en_tim3_m23cr_dir_t;

/**
 ******************************************************************************
 ** \brief ????????????? (PWM2S)(??23)
 *****************************************************************************/
typedef enum en_tim3_m23cr_pwm2s
{
    Tim3DoublePointCmp      = 0u,         ///< ??????,??CCRA,CCRB????OCREFA??
    Tim3SinglePointCmp      = 1u,         ///< ??????,??CCRA????OCREFA??
}en_tim3_m23cr_pwm2s_t;

/**
 ******************************************************************************
 ** \brief GATE?PWM???????????? ????????? (CSG)(??23)
 *****************************************************************************/
typedef enum en_tim3_m23cr_csg
{
     Tim3PWMCompGateCmpOut    = 0u,         ///< ?PWM?????,Gate??????
     Tim3PWMCompGateCapIn     = 1u,         ///< ?PWM?????,Gate??????
}en_tim3_m23cr_csg_t;


/**
 ******************************************************************************
 ** \brief ??????? ??????? (CCR0A,CCR0B)(??23)
 *****************************************************************************/
typedef enum en_tim3_m23_ccrx
{
     Tim3CCR0A    = 0u,         ///< CCR0A???????
     Tim3CCR0B    = 1u,         ///< CCR0B???????
     Tim3CCR1A    = 2u,         ///< CCR1A???????
     Tim3CCR1B    = 3u,         ///< CCR1B???????
     Tim3CCR2A    = 4u,         ///< CCR2A???????
     Tim3CCR2B    = 5u,         ///< CCR2B???????
}en_tim3_m23_ccrx_t;

/**
 ******************************************************************************
 ** \brief  OCREF??? ????????? (OCCS)(??23)
 *****************************************************************************/
typedef enum en_tim3_m23ce_occs
{
     Tim3OC_Ref_Clr  = 0u,         ///< ??VC?OC_Ref_Clr
     Tim3ETRf        = 1u,         ///< ??ETRf
}en_tim3_m23ce_occs_t;

/**
 ******************************************************************************
 ** \brief  ???????? ????????? (CIS/CISB)(??23)
 *****************************************************************************/
typedef enum en_tim3_m23_cisa_cisb
{
     Tim3CmpIntNone        = 0u,         ///< ???????
     Tim3CmpIntRise        = 1u,         ///< ?????????
     Tim3CmpIntFall        = 2u,         ///< ?????????
     Tim3CmpIntRiseFall    = 3u,         ///< ????????????
}en_tim3_m23_cisa_cisb_t;

/**
 ******************************************************************************
 ** \brief TIM3???? - ???CHx??????(BKSA/BKSB)(??23)
 **
 ** \note
 ******************************************************************************/
typedef enum en_tim3_m23_crchx_bks
{
    Tim3CHxBksHiZ         = 0u,         ///< ?????,CHx???????
    Tim3CHxBksNorm        = 1u,         ///< ?????,CHx??????
    Tim3CHxBksLow         = 2u,         ///< ?????,CHx???????
    Tim3CHxBksHigh        = 3u,         ///< ?????,CHx???????
}en_tim3_m23_crchx_bks_t;

/**
 ******************************************************************************
** \brief TIM3???? - CHx????????(CRx/CFx)(??23)
 **
 ** \note
 ******************************************************************************/
typedef enum en_tim3_m23_crch0_cfx_crx
{
    Tim3CHxCapNone        = 0u,         ///< CHx??????
    Tim3CHxCapRise        = 1u,         ///< CHx?????????
    Tim3CHxCapFall        = 2u,         ///< CHx?????????
    Tim3CHxCapFallRise    = 3u,         ///< CHx?????????????
}en_tim3_m23_crch0_cfx_crx_t;

/**
 ******************************************************************************
** \brief TIM3???? - CHx??????(CSA/CSB)(??23)
 **
 ** \note
 ******************************************************************************/
typedef enum en_tim3_m23_crch0_csa_csb
{
    Tim3CHxCmpMode        = 0u,         ///< CHx?????????
    Tim3CHxCapMode        = 1u,         ///< CHx?????????
}en_tim3_m23_crch0_csa_csb_t;

/**
 ******************************************************************************
 ** \brief  ????? DMA?????? ??????? (CCDS)(??23)
 *****************************************************************************/
typedef enum en_tim3_m23_mscr_ccds
{
     Tim3CmpTrigDMA  = 0u,         ///< ??????DMA
     Tim3UEVTrigDMA  = 1u,         ///< ????????????DMA
}en_tim3_m23_mscr_ccds_t;

/**
 ******************************************************************************
 ** \brief  ?????? ??????? (MSM)(??23)
 *****************************************************************************/
typedef enum en_tim3_m23_mscr_msm
{
     Tim3SlaveMode    = 0u,         ///< ???
     Tim3MasterMode   = 1u,         ///< ???
}en_tim3_m23_mscr_msm_t;

/**
 ******************************************************************************
 ** \brief  ???????? ??????? (MMS)(??23)
 *****************************************************************************/
typedef enum en_tim3_m23_mscr_mms
{
     Tim3MasterUG         = 0u,         ///< UG(????)?
     Tim3MasterCTEN       = 1u,         ///< CTEN?
     Tim3MasterUEV        = 2u,         ///< UEV???
     Tim3MasterCMPSO      = 3u,         ///< ?????????
     Tim3MasterOCA0Ref    = 4u,         ///< OCA0_Ref?
     Tim3MasterOCB0Ref    = 5u,         ///< OCB0_Ref?
     //Tim3MasterOCB0Ref  = 6u,
     //Tim3MasterOCB0Ref  = 7u,
}en_tim3_m23_mscr_mms_t;

/**
 ******************************************************************************
 ** \brief  ??????? ??????? (SMS)(??23)
 *****************************************************************************/
typedef enum en_tim3_m23_mscr_sms
{
     Tim3SlaveIClk        = 0u,         ///< ??????
     Tim3SlaveResetTIM    = 1u,         ///< ????
     Tim3SlaveTrigMode    = 2u,         ///< ????
     Tim3SlaveEClk        = 3u,         ///< ??????
     Tim3SlaveCodeCnt1    = 4u,         ///< ????????1
     Tim3SlaveCodeCnt2    = 5u,         ///< ????????2
     Tim3SlaveCodeCnt3    = 6u,         ///< ????????3
     Tim3SlaveGateCtrl    = 7u,         ///< ????
}en_tim3_m23_mscr_sms_t;

/**
 ******************************************************************************
 ** \brief ?????????????? (CTEN)
 *****************************************************************************/
typedef enum en_tim3_start
{
    Tim3CTENDisable = 0u,            ///< ??
    Tim3CTENEnable  = 1u,            ///< ??
}en_tim3_start_t;

/**
 ******************************************************************************
 ** \brief TIM3 mode0 ???????(??0)
 *****************************************************************************/
typedef struct stc_tim3_mode0_cfg
{
    en_tim3_work_mode_t     enWorkMode;       ///< ??????
    en_tim3_m0cr_gatep_t    enGateP;          ///< ??????
    boolean_t               bEnGate;          ///< ????
    en_tim3_cr_timclkdiv_t  enPRS;            ///< ?????
    boolean_t               bEnTog;           ///< ??????
    en_tim3_cr_ct_t         enCT;             ///< ??/??????
    en_tim3_m0cr_md_t       enCntMode;        ///< ??????
}stc_tim3_mode0_cfg_t;

/**
 ******************************************************************************
 ** \brief TIM3 mode1 ???????(??1)
 *****************************************************************************/
typedef struct stc_tim3_mode1_cfg
{
    en_tim3_work_mode_t     enWorkMode;       ///< ??????
    en_tim3_cr_timclkdiv_t  enPRS;            ///< ?????
    en_tim3_cr_ct_t         enCT;             ///< ??/??????
    en_tim3_m1cr_oneshot_t  enOneShot;        ///< ????/??????
}stc_tim3_mode1_cfg_t;

/**
 ******************************************************************************
 ** \brief PWC?????????(??1)
 *****************************************************************************/
typedef struct stc_tim3_pwc_input_cfg
{
    en_tim3_mscr_ts_t          enTsSel;       ///< ???????
    en_tim3_m1_mscr_ia0s_t     enIA0Sel;      ///< CHA0????
    en_tim3_m1_mscr_ib0s_t     enIB0Sel;      ///< CHB0????
    en_tim3_port_polarity_t    enETRPhase;    ///< ETR????
    en_tim3_flt_t              enFltETR;      ///< ETR????
    en_tim3_flt_t              enFltIA0;      ///< CHA0????
    en_tim3_flt_t              enFltIB0;      ///< CHB0????
}stc_tim3_pwc_input_cfg_t;

/**
 ******************************************************************************
 ** \brief TIM3 mode23 ???????(??23)
 *****************************************************************************/
typedef struct stc_tim3_mode23_cfg
{
    en_tim3_work_mode_t         enWorkMode;       ///< ??????
    en_tim3_m23cr_dir_t         enCntDir;         ///< ????
    en_tim3_cr_timclkdiv_t      enPRS;            ///< ???????
    en_tim3_cr_ct_t             enCT;             ///< ??/??????
    en_tim3_m23cr_comp_t        enPWMTypeSel;     ///< PWM????(??/??)
    en_tim3_m23cr_pwm2s_t       enPWM2sSel;       ///< OCREFA????????
    boolean_t                   bOneShot;         ///< ????????/??
    boolean_t                   bURSSel;          ///< ?????
}stc_tim3_mode23_cfg_t;

/**
 ******************************************************************************
 ** \brief GATE?PWM???????????? ???????(??23)
 *****************************************************************************/
typedef struct stc_tim3_m23_gate_cfg
{
    en_tim3_m23cr_csg_t        enGateFuncSel;      ///< Gate?????????
    boolean_t                  bGateRiseCap;       ///< GATE???????,????????
    boolean_t                  bGateFallCap;       ///< GATE???????,????????
}stc_tim3_m23_gate_cfg_t;

/**
 ******************************************************************************
 ** \brief CHA/CHB?????? ???????(??23)
 *****************************************************************************/
typedef struct stc_tim3_m23_compare_cfg
{
    en_tim3_m23_crch0_csa_csb_t  enCHxACmpCap;     ///< CH0A??/??????
    en_tim3_m23_fltr_ocm_t       enCHxACmpCtrl;    ///< CH0A??????
    en_tim3_port_polarity_t      enCHxAPolarity;   ///< CH0A??????
    boolean_t                    bCHxACmpBufEn;    ///< ??A???? ??/??
    en_tim3_m23_cisa_cisb_t      enCHxACmpIntSel;  ///< CHA????????
    
    en_tim3_m23_crch0_csa_csb_t  enCHxBCmpCap;     ///< CH0B??/??????
    en_tim3_m23_fltr_ocm_t       enCHxBCmpCtrl;    ///< CH0B??????
    en_tim3_port_polarity_t      enCHxBPolarity;   ///< CH0B??????
    boolean_t                    bCHxBCmpBufEn;    ///< ??B???? ??/??
    en_tim3_m23_cisa_cisb_t      enCHxBCmpIntSel;  ///< CHB0????????
}stc_tim3_m23_compare_cfg_t;

/**
 ******************************************************************************
 ** \brief CHA/CHB?????? ???????(??23)
 *****************************************************************************/
typedef struct stc_tim3_m23_input_cfg
{
    en_tim3_m23_crch0_csa_csb_t  enCHxACmpCap;     ///< CH0A??/??????
    en_tim3_m23_crch0_cfx_crx_t  enCHxACapSel;     ///< CH0A??????
    en_tim3_flt_t                enCHxAInFlt;      ///< CH0A????????
    en_tim3_port_polarity_t      enCHxAPolarity;   ///< CH0A????
    
    en_tim3_m23_crch0_csa_csb_t  enCHxBCmpCap;     ///< CH0A??/??????
    en_tim3_m23_crch0_cfx_crx_t  enCHxBCapSel;     ///< CH0B??????
    en_tim3_flt_t                enCHxBInFlt;      ///< CH0B????????
    en_tim3_port_polarity_t      enCHxBPolarity;   ///< CH0B????
    
}stc_tim3_m23_input_cfg_t;   

/**
 ******************************************************************************
 ** \brief ETR?????????????(??23)
 *****************************************************************************/
typedef struct stc_tim3_m23_etr_input_cfg
{
    en_tim3_port_polarity_t     enETRPolarity;  ///< ETR??????
    en_tim3_flt_t               enETRFlt;       ///< ETR????
}stc_tim3_m23_etr_input_cfg_t;

/**
 ******************************************************************************
 ** \brief ??BK?????????????(??23)
 *****************************************************************************/
typedef struct stc_tim3_m23_bk_input_cfg
{
    boolean_t                   bEnBrake;         ///< ????
    boolean_t                   bEnVCBrake;       ///< ??VC??
    boolean_t                   bEnSafetyBk;      ///< ??safety??
    boolean_t                   bEnBKSync;        ///< TIM0/TIM1/TIM2??????
    en_tim3_m23_crchx_bks_t     enBkCH0AStat;     ///< ???CHA??????
    en_tim3_m23_crchx_bks_t     enBkCH0BStat;     ///< ???CHB??????
    en_tim3_m23_crchx_bks_t     enBkCH1AStat;     ///< ???CHA??????
    en_tim3_m23_crchx_bks_t     enBkCH1BStat;     ///< ???CHB??????
    en_tim3_m23_crchx_bks_t     enBkCH2AStat;     ///< ???CHA??????
    en_tim3_m23_crchx_bks_t     enBkCH2BStat;     ///< ???CHB??????
    en_tim3_port_polarity_t     enBrakePolarity;  ///< ??BK??????
    en_tim3_flt_t               enBrakeFlt;       ///< ??BK????
}stc_tim3_m23_bk_input_cfg_t;

/**
 ******************************************************************************
** \brief ???????????(??23)
 *****************************************************************************/
typedef struct stc_tim3_m23_dt_cfg
{
    boolean_t       bEnDeadTime;      ///< ???CHA??????
    uint8_t         u8DeadTimeValue;  ///< ???CHA??????
}stc_tim3_m23_dt_cfg_t;

/**
 ******************************************************************************
 ** \brief ??ADC???????(??23)
 *****************************************************************************/
typedef struct stc_tim3_m23_adc_trig_cfg
{
    boolean_t     bEnTrigADC;          ///< ??ADC????
    boolean_t     bEnUevTrigADC;       ///< ??????ADC
    boolean_t     bEnCH0ACmpTrigADC;   ///< CH0A??????ADC
    boolean_t     bEnCH0BCmpTrigADC;   ///< CH0B??????ADC
    boolean_t     bEnCH1ACmpTrigADC;   ///< CH0A??????ADC
    boolean_t     bEnCH1BCmpTrigADC;   ///< CH0B??????ADC
    boolean_t     bEnCH2ACmpTrigADC;   ///< CH0A??????ADC
    boolean_t     bEnCH2BCmpTrigADC;   ///< CH0B??????ADC
}stc_tim3_m23_adc_trig_cfg_t;

/**
 ******************************************************************************
 ** \brief  DMA?? ???????(??23)
 *****************************************************************************/
typedef struct stc_tim3_m23_trig_dma_cfg
{
    boolean_t                bUevTrigDMA;      ///< ?? ??DMA??
    boolean_t                bTITrigDMA;       ///< Trig ??DMA??
    boolean_t                bCmpA0TrigDMA;    ///< CH0A??????DMA??
    boolean_t                bCmpB0TrigDMA;    ///< CH0B??????DMA??
    boolean_t                bCmpA1TrigDMA;    ///< CH1A??????DMA??
    boolean_t                bCmpB1TrigDMA;    ///< CH1B??????DMA??
    boolean_t                bCmpA2TrigDMA;    ///< CH2A??????DMA??
    boolean_t                bCmpB2TrigDMA;    ///< CH2B??????DMA??
    en_tim3_m23_mscr_ccds_t  enCmpUevTrigDMA;  ///< ?????DMA??????
}stc_tim3_m23_trig_dma_cfg_t;

/**
 ******************************************************************************
 ** \brief  ???? ???????(??23)
 *****************************************************************************/
typedef struct stc_tim3_m23_master_slave_cfg
{
    en_tim3_m23_mscr_msm_t      enMasterSlaveSel;  ///< ??????
    en_tim3_m23_mscr_mms_t      enMasterSrc;       ///< ????????
    en_tim3_m23_mscr_sms_t      enSlaveModeSel;    ///< ?????
    en_tim3_mscr_ts_t           enTsSel;           ///< ???????
}stc_tim3_m23_master_slave_cfg_t;

/**
 ******************************************************************************
 ** \brief  OCREF???? ???????(??23)
 *****************************************************************************/
typedef struct stc_tim3_m23_OCREF_Clr_cfg
{
    en_tim3_m23ce_occs_t       enOCRefClrSrcSel;  ///< OCREF?????
    boolean_t                  bVCClrEn;          ///< ??????VC?OCREF_Clr
}stc_tim3_m23_OCREF_Clr_cfg_t;

/******************************************************************************
 * Global variable declarations ('extern', definition in C source)
 *****************************************************************************/

/******************************************************************************
 * Global function prototypes (definition in C source)
 *****************************************************************************/
//??????

//??????
boolean_t Tim3_GetIntFlag(en_tim3_irq_type_t enTim3Irq);
//??????
en_result_t Tim3_ClearIntFlag(en_tim3_irq_type_t enTim3Irq);
//????????
en_result_t Tim3_ClearAllIntFlag(void);
//??0????
en_result_t Tim3_Mode0_EnableIrq(void);
//??1????
en_result_t Tim3_Mode1_EnableIrq (en_tim3_irq_type_t enTim3Irq);
//??2????
en_result_t Tim3_Mode23_EnableIrq (en_tim3_irq_type_t enTim3Irq);
//??0????
en_result_t Tim3_Mode0_DisableIrq(void);
//??1????
en_result_t Tim3_Mode1_DisableIrq (en_tim3_irq_type_t enTim3Irq);
//??2????
en_result_t Tim3_Mode23_DisableIrq (en_tim3_irq_type_t enTim3Irq);


//??0??????????

//timer??????
en_result_t Tim3_Mode0_Init(stc_tim3_mode0_cfg_t* pstcCfg);
//timer ??/??
en_result_t Tim3_M0_Run(void);
en_result_t Tim3_M0_Stop(void);
//?????
en_result_t Tim3_M0_ARRSet(uint16_t u16Data);
//16??????/??
en_result_t Tim3_M0_Cnt16Set(uint16_t u16Data);
uint16_t Tim3_M0_Cnt16Get(void);
//32??????/??
en_result_t Tim3_M0_Cnt32Set(uint32_t u32Data);
uint32_t Tim3_M0_Cnt32Get(void);
//??????/????
en_result_t Tim3_M0_Enable_Output(boolean_t bEnOutput);
//????/??(???)??
en_result_t Tim3_M0_EnTOG(boolean_t bEnTOG);

//??1??????????

//timer??????
en_result_t Tim3_Mode1_Init(stc_tim3_mode1_cfg_t* pstcCfg);
//PWC ????
en_result_t Tim3_M1_Input_Cfg(stc_tim3_pwc_input_cfg_t* pstcCfg);
//PWC??????????
en_result_t Tim3_M1_PWC_Edge_Sel(en_tim3_m1cr_Edge_t enEdgeSel);
//timer ??/??
en_result_t Tim3_M1_Run(void);
en_result_t Tim3_M1_Stop(void);
//16??????/??
en_result_t Tim3_M1_Cnt16Set(uint16_t u16Data);
uint16_t Tim3_M1_Cnt16Get(void);
//????????????
uint16_t Tim3_M1_PWC_CapValueGet(void);


//??23??????????

//timer??????
en_result_t Tim3_Mode23_Init(stc_tim3_mode23_cfg_t* pstcCfg);
//timer ??/??
en_result_t Tim3_M23_Run(void);
en_result_t Tim3_M23_Stop(void);
//PWM????
en_result_t Tim3_M23_EnPWM_Output(boolean_t bEnOutput, boolean_t bEnAutoOutput);
//?????
en_result_t Tim3_M23_ARRSet(uint16_t u16Data, boolean_t bArrBufEn);
//16??????/??
en_result_t Tim3_M23_Cnt16Set(uint16_t u16Data);
uint16_t Tim3_M23_Cnt16Get(void);
//???????CCR0A/CCR0B??/??
en_result_t Tim3_M23_CCR_Set(en_tim3_m23_ccrx_t enCCRSel, uint16_t u16Data);
uint16_t Tim3_M23_CCR_Get(en_tim3_m23_ccrx_t enCCRSel);
//PWM???????,GATE????
en_result_t Tim3_M23_GateFuncSel(stc_tim3_m23_gate_cfg_t* pstcCfg);
//??????
en_result_t Tim3_M23_MasterSlave_Set(stc_tim3_m23_master_slave_cfg_t* pstcCfg);
//CH0A/CH0B??????
en_result_t Tim3_M23_PortOutput_Cfg(en_tim3_channel_t enTim3Chx, stc_tim3_m23_compare_cfg_t* pstcCfg);
//CH0A/CH0B????
en_result_t Tim3_M23_PortInput_Cfg(en_tim3_channel_t enTim3Chx, stc_tim3_m23_input_cfg_t* pstcCfg);
//ERT????
en_result_t Tim3_M23_ETRInput_Cfg(stc_tim3_m23_etr_input_cfg_t* pstcCfg);
//??BK????
en_result_t Tim3_M23_BrakeInput_Cfg(stc_tim3_m23_bk_input_cfg_t* pstcBkCfg);
//??ADC??
en_result_t Tim3_M23_TrigADC_Cfg(stc_tim3_m23_adc_trig_cfg_t* pstcCfg);
//????
en_result_t Tim3_M23_DT_Cfg(stc_tim3_m23_dt_cfg_t* pstcCfg);
//??????
en_result_t Tim3_M23_SetValidPeriod(uint8_t u8ValidPeriod);
//OCREF????
en_result_t Tim3_M23_OCRefClr(stc_tim3_m23_OCREF_Clr_cfg_t* pstcCfg);
//??DMA??
en_result_t Tim3_M23_EnDMA(stc_tim3_m23_trig_dma_cfg_t* pstcCfg);
//????A????
en_result_t Tim3_M23_EnSwTrigCapCmpA(en_tim3_channel_t enTim3Chx);
//????B????
en_result_t Tim3_M23_EnSwTrigCapCmpB(en_tim3_channel_t enTim3Chx);
//??????
en_result_t Tim3_M23_EnSwUev(void);
//??????
en_result_t Tim3_M23_EnSwTrig(void);
//??????
en_result_t Tim3_M23_EnSwBk(void);


//@} // Tim3Group

#ifdef __cplusplus
#endif


#endif /* __BT_H__ */
/******************************************************************************
 * EOF (not truncated)
 *****************************************************************************/


