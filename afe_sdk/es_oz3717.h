/*****************************************************************************
* Copyright(c) O2Micro, 2019. All rights reserved.
*       
* O2Micro [OZ3717] Source Code Reference Design
* File: es_oz3717.h
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

 #ifndef _OZ3717_DRIVER_H_
 #define _OZ3717_DRIVER_H_

/*****************************************************************************
 * #include section
 * add #include here
 *****************************************************************************/
#include "oz3717_regdef.h"
#include "oz3717_protection.h"
#include "mcal_spi_interface.h"

#ifndef	NULL
#define	NULL	(void*)0
#endif

#define ES_SPI_STATUS_TIMEOUT     0xFE

#ifndef BYTE
typedef unsigned char 	BYTE;
#endif
#ifndef WORD
typedef unsigned short 	WORD;
#endif
#ifndef DWORD
typedef unsigned long 	DWORD;
#endif
#ifndef CHAR
typedef char 			CHAR;
#endif
#ifndef SHORT
typedef short 			SHORT;
#endif
#ifndef LONG
typedef long 			LONG;
#endif
#ifndef FLOAT32
typedef double 			FLOAT32;
#endif

 
 #define OZ3717_REG_VPACK       0x1A
#define OZ3717_VPACK_LSB_mV    2.5f  // 2.5 mV per LSB

 
  
//Basic configuration
 #define	RSENSE									  (0.1)  //mohm
 #define 	CELL_NUM  			           16
 
 #define 	OZ3717_TYPE_CFG   					OZ3717_COCP_TYPE_CFG_2_TERMINAL
 #define	OZ3717_CELL_VOLTAGE_LSB					0.15625
 #define	OZ3717_CURRENT_LSB						  0.0078125
 #define	OZ3717_TEMP_VOLT_LSB					  0.078125
 
 #define	OZ3717_CELL_BATT_VOLTAGE_LSB	 (0.15625*16) 
 #define	WR_REG_CMD								       0xC5
 #define	RD_REG_CMD								       0x30


/*****************************************************************************
 * Cell Balance setting
 * add const #define here
 *****************************************************************************/
 
#define CELL_BALANCE_START             3400 //mV
#define CELL_BALANCE_DELTA              20 //mV

#define CELL_BALANCE_DISABLE           0x00
#define CELL_BALANCE_CHARGE_ENABLE     0x01
#define CELL_BALANCE_IDLE_ENABLE       0x02
#define CELL_BALANCE_IDLE_CHARGE_ENABLE  (CELL_BALANCE_CHARGE_ENABLE | CELL_BALANCE_IDLE_ENABLE)
#define CELL_BALANCE_OPTION        CELL_BALANCE_CHARGE_ENABLE


/*****************************************************************************
 * Protection setting
 * add const #define here
 *****************************************************************************/
 
//PRE 0x59
#define PRE_SET_VAL        0x50//
#define PRE_SET          OZ3717_FETCTRL_SEL_PRE_DISABLE
#define SW_LOAD_DETECT   OZ3717_FETCTRL_LOAD_DETECT_ENABLE


/****************************************************User configure DATA prepare************************************************************/
extern uint16_t doc2_p , doc2_delay; 
extern uint16_t sc_delay,sc_p;
/**********Under voltage protection OVP 0x50*************/
#if(UVSTDN_TH < 1500)
#define UVSTDN_TH_REG 0
#else
#define UVSTDN_TH_REG (((UVSTDN_TH-OZ3717_UVSTDN_TH_BASE)/OZ3717_UVSTDN_TH_LSB)+1)
#endif
#define UVSTDN_DLY_REG UVSTDN_DLY

/**********Over voltage protection OVP 0x50***************/
#define OVP_TH_REG  (((OVP_TH-OZ3717_OVP_TH_BASE)/OZ3717_OVP_TH_LSB))
#define OVP_REL_REG ((OVP_RELEASE/OZ3717_OVP_REL_HYS_LSB) << OZ3717_OVP_REL_HYS_START)
#define OVP_DLY_REG  OVP_DLY


/**********Discharging over current protection DOC1P 0x51***************/ 
#define DOC1P_TH_MV      ((RSENSE * 80)-OZ3717_DOC1P_TH_BASE)  //this is working fine  //78 // 60 --ok
#define CELL_NUMBER_REG  ((CELL_NUM-10) << OZ3717_DOC1P_CELL_NUMBER_START)
#define DOC1P_TH_REG     (WORD)(DOC1P_TH_MV/OZ3717_DOC1P_TH_LSB)
#define EFETC_REG        EFETC
#define DOC1P_DLY_REG    DOC1P_DLY


/**********Discharging over current protection COCP 0x52***************/  
#define COCP_TH_MV      ((RSENSE * 80)-OZ3717_COCP_TH_BASE)   //minimum detectable current based on a 0.1 mO
#define COCP_TH_REG   (BYTE)(COCP_TH_MV/OZ3717_COCP_TH_LSB)
#define TYPE_CFG_REG TYPE_CFG
#define COCP_DLY_REG COCP_DLY
 
//ustmp = (sc_p<< 14)|((BYTE)((sc_delay-62.5)/62.5)<<4) |(BYTE)((((0.1) * doc2_p)-10)/10)|(doc2_delay << 8)|(0x00 << 13); 
/**********Discharging over current-2 protection DOCSCP 0x53***************/ 
//#define DOC2P_TH_MV    ((RSENSE * 50)-OZ3717_DOC2_TH_BASE)  
//#define SCPTH_REG       SCPTH      
//#define INT_TYPE_REG    INT_TYPE   
//#define DOC2P_DLY_REG  (DOC2P_DLY << OZ3717_DOC2_DELAY_START)
//#define DOC2P_TH_REG   (BYTE)(DOC2P_TH_MV/OZ3717_DOC2_TH_LSB)
//#define SCPP_DLY_REG   ((BYTE)((SCPP_DLY-OZ3717_SCP_DLY_BASE)/OZ3717_SCP_DLY_LSB)<<OZ3717_SC_DELAY_START)


#define DOC2P_TH_MV    ((RSENSE * doc2_p)-OZ3717_DOC2_TH_BASE)   //doc2_p = 50
#define SCPTH_REG       (sc_p << 14)  // sc_p = 0 for 2x     
#define INT_TYPE_REG    INT_TYPE   
#define DOC2P_DLY_REG  (doc2_delay << OZ3717_DOC2_DELAY_START) //doc2_delay = 20 ms
#define DOC2P_TH_REG   (BYTE)(DOC2P_TH_MV/OZ3717_DOC2_TH_LSB)
#define SCPP_DLY_REG   ((BYTE)((sc_delay-OZ3717_SCP_DLY_BASE)/OZ3717_SCP_DLY_LSB)<<OZ3717_SC_DELAY_START) // sc_delay = 100

// Compute shunt voltage in mV
//#define DOC2_VSHUNT_MV   ( (RSENSE) * (TARGET_CURRENT) * 1000.0f )
// Clamp into [10, 160] mV
//#define DOC2_VSHUNT_CLAMP  ( (DOC2_VSHUNT_MV < DOC2_TH_MIN) ? DOC2_TH_MIN : \
                            ((DOC2_VSHUNT_MV > DOC2_TH_MAX) ? DOC2_TH_MAX : DOC2_VSHUNT_MV) )
// Register value = (V/10) - 1
//#define DOC2P_TH_REG     ( (BYTE)((DOC2_VSHUNT_CLAMP / DOC2_TH_STEP) - 1.0f) )

/**********Idel charging and discharging current IDLETH 0x55***************/ 
#define DSG_TH_MV    ((RSENSE * 1)-OZ3717_IDLE_TH_DSG_BASE)
#define DSG_TH_REG   ((WORD)(DSG_TH_MV/OZ3717_IDLE_TH_DSG_LSB) << OZ3717_IDLE_TH_DSG_START)

#define CHG_TH_MV    ((RSENSE * 1)-OZ3717_IDLE_TH_CHG_BASE)
#define CHG_TH_REG   ((BYTE)(CHG_TH_MV/OZ3717_IDLE_TH_CHG_LSB))

//SPIWDT 0x54
#define SPIWDT_REG SPIWDT

/*********************************************************End of threshold settings************************************************************/ 


/**************************************************************
* OZ3717 chip id on Eagle senior
**************************************************************/
 #define AFE_ID_OZ3717 0x7717
/**************************************************************
* SDK EAGLE SENOIR VERSION
**************************************************************/
 #define ES_OZ3717_VER	0x0008
 
 #define  ES_STATUS_OK 						    					0x00	
 #define  ES_SPI_STATUS_PECERR 									0x81
 #define  ES_SPI_STATUS_EETIMEOUT 							0x82
 #define  ES_SPI_STATUS_READ_DATA_NUMBER_ERROR	0x83
 #define  ES_INVALID_PARAMETER          				0x84

 #define SPI_READ_DATA_MAX	16 //(In words)
 
 #define PACK_CAPACITY_AH   60.0f
#define CC_SAMPLE_TIME_S   1.0f   // function runs every 1 second
/**************************************************************
* ADT section
* add Abstract Data Type definition here
**************************************************************/
/**************************************************************
* AFE Data Struct
**************************************************************/
 struct oz3717_chip {
    uint16_t     cell_volt[CELL_NUM]; //ok 0x110
    int          system_current;
    int16_t      internal_temperature;
    int16_t      external_temperature[5];
    int16_t      max_temperature_data;
    int16_t      min_temperature_data;
    int16_t      onBoard_tempertaure_data;
    uint8_t      which_cell_max;
    uint16_t     max_cell_data;
    uint8_t      which_cell_min;
    uint16_t     min_cell_data;
    uint16_t     status1;
		uint16_t     status2;
	  uint16_t     fetCtrl;
	 	uint16_t     cbsel1;
	 	uint16_t     cbsel2;
    float        vbat;
    float        vpack;
}; 

// Example OCV table for LFP (avg cell voltage vs SOC %)
 
/*****************************************************************************
 * function prototype section
 * add function prototype here
 *****************************************************************************/
 

uint8_t App_AfeInit(void);
uint8_t App_BmsMonitor(void);
uint8_t afe_register_read(uint8_t index, uint8_t read_number, uint16_t *buf);
uint8_t afe_register_write(uint8_t index, uint16_t buf);

uint8_t o2m_register_read(uint8_t index, uint16_t *buf);
uint8_t o2m_register_write(uint8_t index, uint16_t buf);

uint8_t crc8_calc(uint8_t * pdata, uint16_t n);

uint16_t o2_es_version(void);
uint16_t o2_es_id(void);
uint8_t oz3717_convert_temperature(unsigned int data,signed int *temperature);
uint8_t oz3717_cb_function(void);
uint8_t oz3717_cb_set(uint16_t channel);
uint8_t oz3717_read_balance_status(uint16_t *cbsel1, uint16_t *cbsel2);
uint8_t oz3717_init(void);
uint8_t oz3717_PowerMode(uint16_t pmode);
uint8_t oz3717_read_cell_volt_ascan(uint8_t channel,uint16_t *volt);
uint8_t oz3717_read_cell_volt_tscan(uint8_t channel,uint16_t *volt);
uint8_t oz3717_read_max_volt_cell(uint8_t *cell_num,uint16_t *volt);
uint8_t oz3717_read_min_volt_cell(uint8_t *cell_num,uint16_t *volt);
uint8_t oz3717_read_pack_current_ascan(float *curr);
uint8_t oz3717_read_pack_current_tscan(float *curr);

uint8_t oz3717_read_cadc_current_conse(float *current_data);
uint8_t oz3717_read_cadc_current_trigger(float *current_data);
uint8_t oz3717_coulomb_count_setting(float current,float chg_threshold,float dsg_threshold);
uint8_t oz3717_read_coulomb_count(float *cc_data);
uint8_t oz3717_read_temperature(void);
uint8_t oz3717_read_status(uint16_t *status1, uint16_t *status2);

uint8_t oz3717_PreDischarge_Disable(void);
uint8_t oz3717_PreDischarge_Enable(void);
uint8_t oz3717_Pre_Set_Modify(uint8_t bpreset);

uint8_t spi_access(uint8_t bout_number_bytes,uint8_t * dataout,uint8_t bin_number_bytes,uint8_t * datain);
 
float App_BmsVbatVoltage(void);
void App_GetTempPoll(void);
void App_GetTemp(int16_t* temp);
void App_AdcTemp(uint32_t u32adcresult[5], int16_t* tempC);
 
void App_SendCell(void);
void App_FindMinMaxTemperature(void);
float App_BmsVpackVoltage(void);

//soc
// Linear interpolation helper
float interpolate_ocv(float voltage);
void update_soc(struct oz3717_chip *chip);


void App_advance_Doc2_setting(void);
 
#endif

