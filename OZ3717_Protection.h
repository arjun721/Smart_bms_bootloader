/*****************************************************************************
* Copyright(c) O2Micro, 2019. All rights reserved.
*       
* O2Micro [OZ3717] Source Code Reference Design
* File: oz3717_Protection.h
*       
* This Source Code Reference Design for O2MICRO [OZ3717] access 
* ("Reference Design") is solely for the use of PRODUCT INTEGRATION REFERENCE ONLY, 
* and contains confidential and privileged information of O2Micro International 
* Limited. O2Micro shall have no liability to any PARTY FOR THE RELIABILITY, 
* SERVICEABILITY FOR THE RESULT OF PRODUCT INTEGRATION, or results from: (i) any 
* modification or attempted modification of the Reference Design by any party, or 
* (ii) the combination, operation or use of the Reference Design with non-O2Micro 
* Reference Design. Use of the Reference Design is at user's discretion to qualify 
* the final work result.
*****************************************************************************/

 #ifndef _OZ3717_PROTECTION_H_
 #define _OZ3717_PROTECTION_H_
 
 #include "config.h"

/*****************************************************************************
 * Protection setting
 * add const #define here
 ****************************************************************************/ 
 //Constant setting
//UVSTDN 0x4F
#define UVSTDN_DLY      OZ3717_UVSTDN_9CYCLE
#define UVSTDN_TH       UV_SHUTDOWN_TH_VAL 

//OVP 0x50
#define OVP_DLY        OZ3717_OVP_DLY_4CYCLE
#define OVP_TH         OVER_VOLTAGE_TH 
#define OVP_RELEASE    OVER_VOLTAGE_RECOVERY 

//COCP 0x52
#define TYPE_CFG        OZ3717_COCP_TYPE_CFG_2_TERMINAL
#define COCP_DLY        OZ3717_COCP_DELAY_6_CYCLE
#define COCP_TH         OVER_CHG_CURRENT_TH_A 

//DOC1P 0x51
#define DOC1_TH         OVER_DSCHG_CURRENT_TH_A  //65
#define EFETC           OZ3717_DOC1P_EFETC_MODE_CHG_DSG_OFF_EFETC_HIGH
#define DOC1P_DLY       OZ3717_DOC1P_DELAY_16_CYCLE

//DOCSCP 0x53
#define SCPTH          OZ3717_DOC2SC_SCP_TH_6X_DOC2
#define INT_TYPE       OZ3717_DOC2SC_INTR_TYPE_LOW_LEVEL
#define DOC2P_DLY      16 //ms
#define SCPP_DLY       135 //us
#define DOC2P_TH       OVER_SC_CURRNT_TH_A //A

//IDLE 0x55
#define DSG_TH   IDEL_DSG_TH_MA    
#define CHG_TH   IDEL_CHG_TH_MA    

//DOCSCP 0x55
#define SPIWDT   OZ3717_SPIWDT_DISABLE


/*
#define UVSTDN_DLY OZ3717_UVSTDN_9CYCLE
#define UVSTDN_TH  0 //mV

//OVP 0x50
#define OVP_DLY OZ3717_OVP_DLY_4CYCLE
#define OVP_TH  4200 //mV
#define OVP_RELEASE  100 //mV

//DOC1P 0x51

#define DOC1_TH   65 //A
#define EFETC        OZ3717_DOC1P_EFETC_MODE_CHG_DSG_OFF_EFETC_HIGH
#define DOC1P_DLY OZ3717_DOC1P_DELAY_2_CYCLE

//COCP 0x52
#define TYPE_CFG OZ3717_COCP_TYPE_CFG_2_TERMINAL
#define COCP_DLY OZ3717_COCP_DELAY_6_CYCLE
#define COCP_TH  2 //A

//DOCSCP 0x53
#define SCPTH      OZ3717_DOC2SC_SCP_TH_6X_DOC2
#define INT_TYPE   OZ3717_DOC2SC_INTR_TYPE_LOW_LEVEL
#define DOC2P_DLY  16 //ms
#define SCPP_DLY   135 //us
#define DOC2P_TH   DOC1_TH + 5 //A

//DOCSCP 0x55
#define SPIWDT OZ3717_SPIWDT_DISABLE

//IDLE 0x55
#define DSG_TH  1000 //mA
#define CHG_TH  500 //mA
*/

#endif

