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
 * @file   can.h
 *
 * @brief  Header file for CAN functions
 *
 * @author MADS Team 
 *
 ******************************************************************************/

#ifndef __CAN_H__
#define __CAN_H__

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
 ** \defgroup CanGroup Controller Area Network(CAN)
 **
 ******************************************************************************/
//@{

/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/**
 *******************************************************************************
 ** \brief  CAN ????.
 ******************************************************************************/
typedef enum
{
    NO_ERROR        = 0U,       ///< ???
    BIT_ERROR       = 1U,       ///< ???
    FORM_ERROR      = 2U,       ///< ????
    STUFF_ERROR     = 3U,       ///< ????
    ACK_ERROR       = 4U,       ///< ????
    CRC_ERROR       = 5U,       ///< CRC??
    UNKOWN_ERROR    = 6U,       ///< ????
}en_can_error_t;

/**
 *******************************************************************************
 ** \brief  CAN???????. (TCMD)
 ******************************************************************************/
typedef enum
{
    CanPTBSel       = 0U,        ///< ??????
    CanSTBSel       = 1U,        ///< ??????
}en_can_buffer_sel_t;

/**
 *******************************************************************************
 ** \brief  CAN ????.(AFWL)
 ******************************************************************************/
typedef struct stc_can_warning_limit
{
    uint8_t CanWarningLimitVal;         ///< ?????????
    uint8_t CanErrorWarningLimitVal;    ///< ??????
}stc_can_warning_limit_t;

/**
 *******************************************************************************
 ** \brief  ?????.(ACF)
 ******************************************************************************/
typedef enum en_can_acf_format_en
{
    CanStdFrames        = 0x02u,   ///< ??????
    CanExtFrames        = 0x03u,   ///< ??????
    CanAllFrames        = 0x00u,   ///< ?????????
}en_can_acf_format_en_t;

/**
 *******************************************************************************
 ** \brief  ??????. (ACFEN)
 ******************************************************************************/
typedef enum en_can_filter_sel
{
    CanFilterSel1        = 0u,   ///< ????1??
    CanFilterSel2        = 1u,   ///< ????2??
    CanFilterSel3        = 2u,   ///< ????3??
    CanFilterSel4        = 3u,   ///< ????4??
    CanFilterSel5        = 4u,   ///< ????5??
    CanFilterSel6        = 5u,   ///< ????6??
    CanFilterSel7        = 6u,   ///< ????7??
    CanFilterSel8        = 7u,   ///< ????8??
}en_can_filter_sel_t;

/**
 *******************************************************************************
 ** \brief  CAN ??????.(IE)
 ******************************************************************************/
typedef enum
{
    //<<Can Rx or Tx Irq En
    CanRxIrqEn              = 0x00000080,   ///< ??????
    CanRxOverIrqEn          = 0x00000040,   ///< ????????
    CanRxBufFullIrqEn       = 0x00000020,   ///< ??????????
    CanRxBufAlmostFullIrqEn = 0x00000010,   ///< ???????????
    CanTxPrimaryIrqEn       = 0x00000008,   ///< PTB??????
    CanTxSecondaryIrqEn     = 0x00000004,   ///< STB??????
    CanErrorIrqEn           = 0x00000002,   ///< ??????

    //<<Can Error Irq En
    CanErrorPassiveIrqEn    = 0x00200000,   ///< ????????
    CanArbiLostIrqEn        = 0x00080000,   ///< ????????
    CanBusErrorIrqEn        = 0x00020000,   ///< ????????

}en_can_irq_type_t;

/**
 *******************************************************************************
 ** \brief  CAN????.(IF)
 ******************************************************************************/
typedef enum
{
    CanTxBufFullIrqFlg          = 0x00000001,   ///< ????????
    CanRxIrqFlg                 = 0x00008000,   ///< ??????
    CanRxOverIrqFlg             = 0x00004000,   ///< ????????
    CanRxBufFullIrqFlg          = 0x00002000,   ///< ??????????
    CanRxBufAlmostFullIrqFlg    = 0x00001000,   ///< ???????????
    CanTxPrimaryIrqFlg          = 0x00000800,   ///< PTB??????
    CanTxSecondaryIrqFlg        = 0x00000400,   ///< STB??????
    CanErrorIrqFlg              = 0x00000200,   ///< ??????
    CanAbortIrqFlg              = 0x00000100,   ///< ????????

    CanErrorWarningIrqFlg       = 0x00800000,   ///< ??????????
    CanErrorPassivenodeIrqFlg   = 0x00400000,   ///< ????????
    CanErrorPassiveIrqFlg       = 0x00100000,   ///< ????????
    CanArbiLostIrqFlg           = 0x00040000,   ///< ????????
    CanBusErrorIrqFlg           = 0x00010000,   ///< ????????
}en_can_irq_flag_type_t;

/**
 *******************************************************************************
 ** \brief  CAN ??.(CFG_STAT)
 ******************************************************************************/
typedef enum
{
    CanExternalLoopBackMode  = 0x40u,        ///< ??????
    CanInternalLoopBackMode  = 0x20u,        ///< ??????
    CanTxSignalPrimaryMode   = 0x10u,        ///< PTB??????
    CanTxSignalSecondaryMode = 0x08u,        ///< STB??????
    CanListenOnlyMode        = 0xFFu,        ///< ????
}en_can_mode_t;

/**
 *******************************************************************************
 ** \brief  CAN ??.(STAT)
 ******************************************************************************/
typedef enum
{
    CanRxActive = 0x04,        ///< ?????
    CanTxActive = 0x02,        ///< ?????
    CanBusoff   = 0x01,        ///< ??????
}en_can_status_t;

/**
 *******************************************************************************
 ** \brief  CAN ????.(TCMD)
 ******************************************************************************/
typedef enum
{
    CanPTBTxCmd      = 0x10,        ///< PTB????
    CanPTBTxAbortCmd = 0x08,        ///< PTB??????
    CanSTBTxOneCmd   = 0x04,        ///< STB??????
    CanSTBTxAllCmd   = 0x02,        ///< STB?????
    CanSTBTxAbortCmd = 0x01,        ///< STB??????
}en_can_tx_cmd_t;

/**
 *******************************************************************************
 ** \brief  CAN ??????????.(TCTRL)
 ******************************************************************************/
typedef enum
{
    CanSTBFifoMode    = 0,        ///< FIFO??
    CanSTBPrimaryMode = 1,        ///< ?????
}en_can_stb_mode_t;

/**
 *******************************************************************************
 ** \brief  CAN ???.(RCTRL)
 ******************************************************************************/
typedef enum
{
    CanSelfAckDisable = 0,        ///< ????
    CanSelfAckEnable  = 1,        ///< ?????(LBME=1)
}en_can_self_ack_en_t;

/**
 *******************************************************************************
 ** \brief  ?????????.(RCTRL)
 ******************************************************************************/
typedef enum
{
    CanRxBufOverwritten = 0,        ///< ???????????
    CanRxBufNotStored   = 1,        ///< ????????????
}en_can_rx_buf_mode_en_t;

/**
 *******************************************************************************
 ** \brief  ???????????.(RCTRL)
 ******************************************************************************/
typedef enum
{
    CanRxNormal = 0,        ///< ????
    CanRxAll    = 1,        ///< ????(????)??
}en_can_rx_buf_all_t;

/**
 *******************************************************************************
 ** \brief  CAN ???????.(RSTAT)
 ******************************************************************************/
typedef enum
{
    CanRxBufEmpty          = 0,        ///< ?
    CanRxBufnotAlmostFull  = 1,        ///< ??????????
    CanRxBufAlmostFull     = 2,        ///< ??????????
    CanRxBufFull           = 3,        ///< ?(??)
}en_can_rx_buf_status_t;

/**
 *******************************************************************************
 ** \brief  CAN ???????.(TSSTAT)
 ******************************************************************************/
typedef enum
{
    CanTxBufEmpty        = 0,       ///< ?
    CanTxBufnotHalfFull  = 1,       ///< ??????
    CanTxBufHalfFull     = 2,       ///< ????
    CanTxBufFull         = 3,       ///< ?
}en_can_tx_buf_status_t;

/**
 *******************************************************************************
 ** \brief  CAN ???.
 ******************************************************************************/
typedef struct stc_can_filter
{
    uint32_t                u32CODE;        ///< CODE
    uint32_t                u32MASK;        ///< MASK
    en_can_filter_sel_t     enFilterSel;    ///< ??????
    en_can_acf_format_en_t  enAcfFormat;    ///< ?????.
}stc_can_filter_t;

/**
 *******************************************************************************
 ** \brief  CAN ??.
 ******************************************************************************/
typedef struct stc_can_bt
{
    uint8_t SEG_1;      ///< ??1??(Tseg_1 = (SEG_1 + 2)*TQ)
    uint8_t SEG_2;      ///< ??2??(Tseg_2 = (SEG_2 + 1)*TQ)
    uint8_t SJW;        ///< ?????????(Tsjw = (SJW + 1)*TQ)
    uint8_t PRESC;      ///< CAN?????(TQ)
}stc_can_bt_t;

/**
 *******************************************************************************
 ** \brief  CAN ???????.
 ******************************************************************************/
typedef struct
{
    uint32_t DLC                     : 4;        ///< Data length code
    uint32_t RESERVED0               : 2;        ///< Ignore
    uint32_t RTR                     : 1;        ///< Remote transmission request
    uint32_t IDE                     : 1;        ///< IDentifier extension
    uint32_t RESERVED1               : 24;       ///< Ignore
}stc_can_txcontrol_t;

/**
 *******************************************************************************
 ** \brief  CAN ?????.
 ******************************************************************************/
typedef struct stc_can_txframe
{
    union
    {
        uint32_t TBUF32_0;                  ///< Ignore
        uint32_t StdID;                     ///< Standard ID
        uint32_t ExtID;                     ///< Extended ID
    };
    union
    {
        uint32_t TBUF32_1;                  ///< Ignore
        stc_can_txcontrol_t Control_f;      ///< CAN Tx Control
    };
    union
    {
        uint32_t TBUF32_2[2];               ///< Ignore
        uint8_t  Data[8];                   ///< CAN data
    };
    en_can_buffer_sel_t     enBufferSel;    ///< CAN Tx buffer select

}stc_can_txframe_t;

/**
 *******************************************************************************
 ** \brief  CAN ???????.
 ******************************************************************************/
typedef struct
{
    uint8_t DLC          : 4;       ///< Data length code
    uint8_t RESERVED0    : 2;       ///< Ignore
    uint8_t RTR          : 1;       ///< Remote transmission request
    uint8_t IDE          : 1;       ///< IDentifier extension
}stc_can_rxcontrol_t;

/**
 *******************************************************************************
 ** \brief  CAN???????.
 ******************************************************************************/
typedef struct
{
    uint8_t RESERVED0    : 4;       ///< Ignore
    uint8_t TX           : 1;       ///< TX is set to 1 if the loop back mode is activated
    uint8_t KOER         : 3;       ///< Kind of error
}stc_can_status_t;

/**
 *******************************************************************************
 ** \brief  CAN ????????CYCTIM.
 ******************************************************************************/
typedef struct
{
    stc_can_rxcontrol_t Control_f;      ///< @ref stc_can_rxcontrol_t
    stc_can_status_t    Status_f;       ///< @ref stc_can_status_t
    uint16_t            CycleTime;      ///< TTCAN cycletime
}stc_can_cst_t;

/**
 *******************************************************************************
 ** \brief  CAN ?????.
 ******************************************************************************/
typedef struct stc_can_rxframe
{
    union
    {
        uint32_t RBUF32_0;              ///< Ignore
        uint32_t StdID;                 ///< Standard ID
        uint32_t ExtID;                 ///< Extended ID
    };
    union
    {
        uint32_t        RBUF32_1;       ///< Ignore
        stc_can_cst_t   Cst;            ///< @ref stc_can_cst_t
    };
    union
    {
        uint32_t RBUF32_2[2];           ///< Ignore
        uint8_t  Data[8];               ///< CAN data
    };

}stc_can_rxframe_t;

/**
 *******************************************************************************
 ** \brief  CAN ?????.
 ******************************************************************************/
typedef struct stc_can_init_config
{
    en_can_rx_buf_all_t     enCanRxBufAll;      ///< @ref en_can_rx_buf_all_t
    en_can_rx_buf_mode_en_t enCanRxBufMode;     ///< @ref en_can_rx_buf_mode_en_t
    en_can_stb_mode_t       enCanSTBMode;       ///< @ref en_can_stb_mode_t
    stc_can_bt_t            stcCanBt;           ///< @ref stc_can_bt_t
    stc_can_warning_limit_t stcWarningLimit;    ///< @ref stc_can_warning_limit_t
}stc_can_init_config_t;


/**
 *******************************************************************************
 ** \brief                         CAN TTCAN
 ******************************************************************************/
/**
 *******************************************************************************
 ** \brief  TTCAN ?????
 ******************************************************************************/
typedef enum
{
    CanTTcanPTBSel      = 0x00u,        ///< PTB
    CanTTcanSTB1Sel     = 0x01u,        ///< STB1
    CanTTcanSTB2Sel     = 0x02u,        ///< STB2
    CanTTcanSTB3Sel     = 0x03u,        ///< STB3
    CanTTcanSTB4Sel     = 0x04u,        ///< STB4
}en_can_ttcan_tbslot_t;

/**
 *******************************************************************************
 ** \brief  TTCAN ??????
 ******************************************************************************/
typedef enum
{
    CanTTcanTprescDiv1  = 0x00u,        ///< Div1
    CanTTcanTprescDiv2  = 0x01u,        ///< Div2
    CanTTcanTprescDiv3  = 0x02u,        ///< Div3
    CanTTcanTprescDiv4  = 0x03u,        ///< Div4
}en_can_ttcan_Tpresc_t;

/**
 *******************************************************************************
 ** \brief  TTCAN ????
 ******************************************************************************/
typedef enum
{
    CanTTcanImmediate   = 0x00,         ///< ????
    CanTTcanTime        = 0x01,         ///< ????
    CanTTcanSingle      = 0x02,         ///< ??????
    CanTTcanTransStart  = 0x03,         ///< ??????
    CanTTcanTransStop   = 0x04,         ///< ??????
}en_can_ttcan_trigger_type_t;

/**
 *******************************************************************************
 ** \brief  TTCAN ??????
 ******************************************************************************/
typedef enum
{
    CanTTcanWdtTriggerIrq   = 0x80,       ///< ????????
    CanTTcanErrorTriggerIrq = 0x10,       ///< ????????    
    CanTTcanTimTriggerIrq   = 0x10,       ///< ????????
}en_can_ttcan_irq_type_t;


typedef struct stc_can_ttcan_ref_msg
{
    uint8_t       u8IDE;                ///< Reference message IDE:1-Extended; 0-Standard;
    union                               ///< Reference message ID
    {
        uint32_t RefStdID;              ///< Reference standard ID
        uint32_t RefExtID;              ///< Reference Extended ID
    };
}stc_can_ttcan_ref_msg_t;

typedef struct stc_can_ttcan_trigger_config
{
    en_can_ttcan_tbslot_t       enTbSlot;           ///< Transmit trigger TB slot pointer
    en_can_ttcan_trigger_type_t enTrigType;         ///< Trigger type
    en_can_ttcan_Tpresc_t       enTpresc;           ///< Timer prescaler
    uint8_t                     u8Tew;              ///< Transmit enable window
    uint16_t                    u16TrigTime;        ///< TTCAN trigger time
    uint16_t                    u16WatchTrigTime;   ///< TTCAN watch trigger time register
}stc_can_ttcan_trigger_config_t;


/*******************************************************************************
 * Global pre-processor symbols/macros ('#define')
 ******************************************************************************/


/*******************************************************************************
 * Global variable definitions ('extern')
 ******************************************************************************/

/*******************************************************************************
 * Global function prototypes (definition in C source)
 ******************************************************************************/
///< CAN ????? 
void CAN_Init(stc_can_init_config_t *pstcCanInitCfg);
///< CAN ????
void CAN_DeInit(void);
///< CAN ????
void CAN_IrqCmd(en_can_irq_type_t enCanIrqType, boolean_t enNewState);
///< CAN ??????
boolean_t CAN_IrqFlgGet(en_can_irq_flag_type_t enCanIrqFlgType);
///< CAN ??????
void CAN_IrqFlgClr(en_can_irq_flag_type_t enCanIrqFlgType);
///< CAN ????
void CAN_ModeConfig(en_can_mode_t enMode, en_can_self_ack_en_t enCanSAck, boolean_t enNewState);
///< CAN ??????
en_can_error_t CAN_ErrorStatusGet(void);
///< CAN CAN????
boolean_t CAN_StatusGet(en_can_status_t enCanStatus);
///< CAN ?????
void CAN_FilterConfig(stc_can_filter_t *pstcFilter, boolean_t enNewState);
///< CAN ???????
void CAN_SetFrame(stc_can_txframe_t *pstcTxFrame);
///< CAN ???????
void CAN_TransmitCmd(en_can_tx_cmd_t enTxCmd);
///< CAN ????????
en_can_tx_buf_status_t CAN_TxBufStatusGet(void);
///< CAN ?????
void CAN_Receive(stc_can_rxframe_t *pstcRxFrame);
///< CAN ????????
en_can_rx_buf_status_t CAN_RxBufStatusGet(stc_can_rxframe_t *pstcRxFrame);

///< CAN ????
uint8_t CAN_ArbitrationLostCap(void);
///< CAN ?????????
uint8_t CAN_RxErrorCntGet(void);
///< CAN ?????????
uint8_t CAN_TxErrorCntGet(void);


//<< void CAN_TTCAN_Enable(void);
//<< void CAN_TTCAN_Disable(void);
//<< void CAN_TTCAN_IrqCmd(void);
//<< void CAN_TTCAN_ReferenceMsgSet(stc_can_ttcan_ref_msg_t *pstcRefMsg);
//<< void CAN_TTCAN_TriggerConfig(stc_can_ttcan_trigger_config_t *pstcTriggerCfg);

//@} // CanGroup

#ifdef __cplusplus
}
#endif

#endif /* __CAN_H__ */

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/

