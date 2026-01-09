#ifndef _MCAL_GUI_INTERFACE_H_
#define _MCAL_GUI_INTERFACE_H_

#include <math.h>
#include <stdio.h>
#include "uart.h"
#include "gpio.h"
#include "reset.h"
#include "stdint.h"
#include "stdbool.h"
 

#define BMS_UART_TX_PORT   GpioPortA
#define BMS_UART_TX_PIN    GpioPin9
#define BMS_UART_RX_PORT   GpioPortA
#define BMS_UART_RX_PIN    GpioPin10

#define READ_REQ_LENGTH 7
#define WRITE_REQ_LENGTH 9


#define RS485_PROTO_VER 1

#define RS485_START_BYTE		0xDD
#define RS485_READ_STATUS		0xA5
#define RS485_WRITE_STATUS		0x5A
#define RS485_END_BYTE			0x77

#define UART_EVENT				0xA6


#define RS485_STATUS_OK			0 
#define RS485_STATUS_ERROR		0x80

#define RS485_NTC_COUNT			5
#define RS485_SERIES_COUNT		CELLCOUNT


typedef struct __attribute__((__packed__)){
	uint8_t balStatus_cell_09	:	1;
	uint8_t balStatus_cell_10	:	1;
	uint8_t balStatus_cell_11	:	1;
	uint8_t balStatus_cell_12	:	1;
	uint8_t balStatus_cell_13	:	1;
	uint8_t balStatus_cell_14	:	1;
	uint8_t balStatus_cell_15	:	1;
	uint8_t balStatus_cell_16	:	1;
	uint8_t balStatus_cell_01	:	1;
	uint8_t balStatus_cell_02	:	1;
	uint8_t balStatus_cell_03	:	1;
	uint8_t balStatus_cell_04	:	1;
	uint8_t balStatus_cell_05	:	1;
	uint8_t balStatus_cell_06	:	1;
	uint8_t balStatus_cell_07	:	1;
	uint8_t balStatus_cell_08	:	1;
}rs485_balancingStatus_Struct;


 

typedef struct __attribute__((__packed__)){
	uint8_t charge_overCurrent		:	1;
	uint8_t discharge_overCurrent	:	1;
	uint8_t shortCircuit			:	1;
	uint8_t afe_error				:	1;
  
	uint8_t sw_lock_MOS				:	1;
	uint8_t hg_mos_damage			:	1;
  uint8_t mos_ot				:	1;
  uint8_t sg_mos_damage   :	1;
	
	uint8_t cell_overVoltage		:	1;
	uint8_t cell_underVoltage		:	1;
	uint8_t pack_overVoltage		:	1;
	uint8_t pack_underVoltage		:	1;
	uint8_t charge_overTemp			:	1;
	uint8_t charge_underTemp		:	1;
	uint8_t discharge_overTemp		:	1;
	uint8_t discharge_underTemp		:	1;
}rs485_protectionStatus_Struct;

typedef struct __attribute__((__packed__)){
	uint8_t chargeFET_Status	:	1;
	uint8_t dischargeFET_Status	:	1;
	uint8_t reserved			:	6;
}rs485_fetStatus_Struct;

typedef struct __attribute__((__packed__)){
	uint16_t packVoltage_10mV;
	int16_t packCurrent_10mA;
	uint16_t remainingCapacity_10mAh;
	uint16_t nominalCapacity_10mAh;
	uint16_t cycles;
	uint16_t productionDate;
	rs485_balancingStatus_Struct balanceStatus_0;
	rs485_balancingStatus_Struct balanceStatus_1;
	rs485_protectionStatus_Struct	protectionStatus;
	uint8_t softwareVersion;
	uint8_t remainingSOC;
	rs485_fetStatus_Struct fetStatus;
	uint8_t cellsInSeries;
	uint8_t ntcNumber;
	uint16_t ntcReading[RS485_NTC_COUNT];
}rs485_basicInfo_Struct;

typedef struct{
	uint16_t rs485_cellVoltages[16];
  uint8_t fault_count[16];
}rs485_cellVoltages_Struct;

typedef struct{
	uint16_t afe_fault;
  uint16_t afe_fet_status;
  uint8_t soh;
}rs485_misinfo_Struct;

typedef struct{
    char faultType_ASCII;
	uint8_t faultNumber;
	uint8_t remainingSOC;
	uint8_t command;
}rs485_SOCFaults_Struct;

typedef enum{
	BASIC_INFO_CMD		= 0x05,
	CELL_VOLTAGES_CMD	= 0x04,
	HW_VERSION			= 0x07,
	SOC_FAULT_CMD       = 0x06,
	FET_CONOTROL_CMD	= 0xE1
}RS485_CMD_t;

typedef enum{
	RS485_OK 			= 0,
	RS485_ERROR 		= 1,
	RS485_IDLE			= 2,
	RS485_CHKSUM_FAIL	= 3,
	RS485_UNKNOWN_CMD	= 4
}RS485_STAT_t;

typedef enum{
	RS485_FETS_ON	=	0,
	RS485_CHG_OFF	=	1,
	RS485_DSG_OFF	=	2,
	RS485_FETS_OFF  =	3
}RS485_FET_CMD_t;

typedef enum{
	EVENT_HEARTBEAT = 0xBB
}EVENT_t;

typedef enum{
	HEARTBEAT_RECV		=	1,
	VALID_DATA_RECV		=	2,
	INVALID_DATA_RECV	=	3
}DATA_RECV_t;

typedef struct
{
    uint16_t COP, COR, COD, POP, POR, POD;
    uint16_t PUP, PUR, PUD, CUP, CUR, CUD;
} bleBmsData_t;
 
 

typedef struct __attribute__((__packed__)){
	uint8_t startByte;
	uint8_t command_code;
	uint8_t status;
	uint8_t data_length;
	uint8_t data_buff[sizeof(rs485_basicInfo_Struct)];
	uint16_t checksum;
	uint8_t endByte;
}rs485_send_basicInfo_Struct;

typedef struct{
	uint8_t startByte;
	uint8_t command_code;
	uint8_t status;
	uint8_t data_length;
	uint8_t data_buff[sizeof(rs485_cellVoltages_Struct)];
	uint16_t checksum;
	uint8_t endByte;
}rs485_send_cellVoltages_Struct;


typedef struct __attribute__((__packed__))
{
    // (A) BASIC PROTECTION (9 blocks × HHH)
    uint16_t Cell_OV_set,    Cell_OV_rel,    Cell_OV_delay;
    uint16_t Cell_UV_set,    Cell_UV_rel,    Cell_UV_delay;
    uint16_t Pack_OV_set,    Pack_OV_rel,    Pack_OV_delay;
    uint16_t Pack_UV_set,    Pack_UV_rel,    Pack_UV_delay;
    uint16_t Chg_UT_set,     Chg_UT_rel,     Chg_UT_delay;
    uint16_t Dsg_UT_set,     Dsg_UT_rel,     Dsg_UT_delay; //
    uint16_t Dsg_OT_set,     Dsg_OT_rel,     Dsg_OT_delay;
    uint16_t Chg_OT_set,     Chg_OT_rel,     Chg_OT_delay;
    uint16_t Chg_OC_set,     Chg_OC_rel,     Chg_OC_delay;
    uint16_t Dsg_OC_set,     Dsg_OC_rel,     Dsg_OC_delay;

    // (B) ADVANCED PROTECTION
    uint16_t Dsg_OC2_set,    Dsg_OC2_rel,    Dsg_OC2_delay;
    uint16_t SC_set,         SC_rel,         SC_delay;
    uint16_t HCell_OV_set,   HCell_OV_rel,   HCell_OV_delay;
    uint16_t LCell_UV_set,   LCell_UV_rel,   LCell_UV_delay;

    // (C) FUNCTION CONFIG 7 bytes
    bool load_en;
    bool balance_en;
    bool chg_balance;
    bool ntc1;
    bool ntc2;
    bool ntc3;
    bool ntc4;

    // (D) BALANCE CONFIG
    uint16_t cellOpenVoltage;
    uint16_t balanceDriftVoltage;
    uint16_t ipsOffDelay;

    // (E) CAPACITY CONFIG
    uint32_t nominalCapacity;
    uint32_t cycleCapacity;
    uint16_t fullSetVoltage;
    uint16_t endOfVoltage;
    uint8_t  selfDischargeRate;

} BmsConfig_struct;



typedef struct{
	uint8_t startByte;
	uint8_t command_code;
	uint8_t status;
	uint8_t data_length;
	uint8_t data_buff[sizeof(BmsConfig_struct)];
	uint16_t checksum;
	uint8_t endByte;
}rs485_send_mis_Struct;


typedef struct{
	uint8_t startByte;
	uint8_t command_code;
	uint8_t status;
	uint8_t data_length;
	uint8_t data_buff[sizeof(rs485_SOCFaults_Struct)];
	uint16_t checksum;
	uint8_t endByte;
}rs485_send_SOCFaults_Struct;

typedef struct{
	uint8_t start_byte;
	uint8_t status_byte;
	uint8_t command_code;
	uint8_t length;
	uint16_t checksum;
	uint8_t end_byte;
}master_readRequest_RS485;

typedef struct{
	uint8_t start_byte;
	uint8_t status_byte;
	uint8_t command_code;
	uint8_t length;
	uint8_t data[2];
	uint16_t checksum;
	uint8_t end_byte;
}master_writeRequest_RS485;

typedef struct{
	uint8_t start_byte;
	uint8_t status_code;
	uint8_t event_code;
	uint8_t length;
	uint8_t data[2];
	uint16_t checksum;
	uint8_t end_byte;
}event_received_Struct;


#define ON	1
#define OFF	0

extern volatile rs485_basicInfo_Struct rs485_basicInfo_obj;
extern volatile rs485_cellVoltages_Struct rs485_cellInfo_obj;
extern volatile rs485_SOCFaults_Struct rs485_SOCFaults_obj;

RS485_STAT_t rs485_init();
RS485_STAT_t rs485_serve_requests(uint8_t * serialNumber, uint8_t length, uint8_t * dsgStatus, uint8_t * chgStatus, uint8_t * deviceStatus);
RS485_STAT_t rs485_recv_data(void);



uint8_t App_TransmitBufferv(uint8_t *buf, uint16_t len);
 
void fun(void);

void
send_data_s(void);
void 
App_UartInit(void);
  
void 
App_UartPortInit(void);

void 
App_UartRfidInit(void);

void
App_UartRfidPortInit(void);
  
void 
App_UartBleInit(void);
 
void 
App_UartBlePortInit(void);

 
void 
App_UartTransmitPoll(M0P_UART_TypeDef* UARTx, uint8_t* data, uint16_t length);

void 
Uart_ReceiveBuffer(M0P_UART_TypeDef* UARTx, uint8_t* buf, uint16_t len);

#endif