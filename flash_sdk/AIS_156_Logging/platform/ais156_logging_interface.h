 
#ifndef AIS156_LOGGING_INTERFACE_H_
#define AIS156_LOGGING_INTERFACE_H_

#include "ringfs.h"
#include "stdbool.h"

//#define NOR_FLASH  128

#if NOR_FLASH == 128
/* Data record format. */
#define LOG_ENTRY_VERSION 4
#elif NOR_FLASH == 16
/* Data record format. */
#define LOG_ENTRY_VERSION 3
#endif

/* AIS_LOGGING SECTORS count*/
// #define AIS_LOGGING_SECTORS				512

#if (NOR_FLASH < 16)
#define LOG_ENTRY_VERSION 4
	//#error "AIS Logging Sectors count not correct"
#endif

 #define DEFAULT_LOG_INTERVAL 60

/*Data logging interval in seconds*/

#if LOG_ENTRY_VERSION == 1
typedef struct __attribute__((__packed__)){
	bool thermalRunaway			:	1;
	bool voltageMismatch		:	1;
	bool utCellDischarge_status	:	1;
	bool utCellCharge_status	:	1;
	bool utInternal				:	1;
	bool otCellDischarge_status	:	1;
	bool otCellCharge_status	:	1;
	bool otInternal				:	1;
	bool mosfetError			:	1;
	bool sc_status				:	1;
	bool ov_status				:	1;
	bool uv_status				:	1;
	bool otMosfet_status		:	1;
	bool utMosfet_status		:	1;
	bool occ_status				:	1;
	bool ocd_status				:	1;
}bms_faultsLog;


typedef struct __attribute__((__packed__)){
    uint32_t tick;
    uint16_t minVoltage;
    uint16_t maxVoltage;
    int16_t minTemp;
    int16_t maxTemp;
    int16_t fetTemp;
    int16_t pchgTemp;
    int16_t current;
    uint8_t soc;
    uint8_t soh;
    uint8_t soe;
    bms_faultsLog faults;
    uint8_t dsgStat  : 1;
    uint8_t chgStat	 : 1;
    uint8_t pchgStat : 1;
    uint8_t customData[6];
}log_entry;

#elif LOG_ENTRY_VERSION == 2

typedef struct __attribute__((__packed__)){
	bool thermalRunaway			:	1;
	bool voltageMismatch		:	1;
	bool utCellDischarge_status	:	1;
	bool utCellCharge_status	:	1;
	bool otCellDischarge_status	:	1;
	bool otCellCharge_status	:	1;
	bool prechargeError_Status	:	1;
	bool commError_Status		:	1;
	bool mosfetError			:	1;
	bool sc_status				:	1;
	bool ov_status				:	1;
	bool uv_status				:	1;
	bool otMosfet_status		:	1;
	bool utMosfet_status		:	1;
	bool occ_status				:	1;
	bool ocd_status				:	1;
}bms_faultsLog;


typedef struct __attribute__((__packed__)){
    uint32_t tick;
    uint16_t minVoltage;
    uint16_t maxVoltage;
    int16_t minTemp;
    int16_t maxTemp;
    int16_t fetTemp;
    int16_t pchgTemp;
    int16_t current;
    uint8_t soc;
    uint8_t soh;
    uint8_t soe;
    bms_faultsLog faults;
    uint8_t dsgStat  : 1;
    uint8_t chgStat	 : 1;
    uint8_t pchgStat : 1;
    uint8_t customData[6];
}log_entry;

#elif LOG_ENTRY_VERSION == 3

typedef struct __attribute__((__packed__)){
	bool thermalRunaway			:	1;
	bool voltageMismatch		:	1;
	bool utCellDischarge_status	:	1;
	bool utCellCharge_status	:	1;
	bool otCellDischarge_status	:	1;
	bool otCellCharge_status	:	1;
	bool prechargeError_Status	:	1;
	bool commError_Status		:	1;
	bool mosfetError			:	1;
	bool sc_status				:	1;
	bool ov_status				:	1;
	bool uv_status				:	1;
	bool otMosfet_status		:	1;
	bool utMosfet_status		:	1;
	bool occ_status				:	1;
	bool ocd_status				:	1;
}bms_faultsLog;

typedef struct __attribute__((__packed__)){
    uint32_t tick;
    uint16_t minVoltage;
    uint16_t maxVoltage;
    int16_t minTemp;
    int16_t maxTemp;
    int16_t fetTemp;
    int16_t pchgTemp;
    int16_t current;
    uint8_t soc;
    uint8_t soh;
    uint8_t soe;
    bms_faultsLog faults;
    uint8_t dsgStat  : 1;
    uint8_t chgStat	 : 1;
    uint8_t pchgStat : 1;
	uint16_t stackVoltage;
	uint8_t minVoltage_cellno;
	uint8_t maxVoltage_cellno;
	uint8_t minTemp_NTCno;
	uint8_t maxTemp_NTCno;
    //uint8_t customData[6];
}log_entry;

#elif LOG_ENTRY_VERSION == 4

typedef struct __attribute__((__packed__)){
	bool thermalRunaway			:	1;
	bool voltageMismatch		:	1;
	bool utCellDischarge_status	:	1;
	bool utCellCharge_status	:	1;
	bool otCellDischarge_status	:	1;
	bool otCellCharge_status	:	1;
	bool prechargeError_Status	:	1;
	bool commError_Status		:	1;
	bool mosfetError			:	1;
	bool sc_status				:	1;
	bool ov_status				:	1;
	bool uv_status				:	1;
	bool otMosfet_status		:	1;
	bool utMosfet_status		:	1;
	bool occ_status				:	1;
	bool ocd_status				:	1;
	bool afe_ot_status			:	1;
	bool afe_ut_status			:	1;
	bool ahCutoff_status		:	1;
	bool cellOpenWire_status	:	1;
	bool ntcOpen_status			:	1;
	bool ntcShort_status		:	1;
	bool packOV_status			:	1;
	bool packUV_status			:	1;
}bms_faultsLog;

//typedef struct __attribute__((__packed__)){
//    uint32_t tick;
//    uint16_t minVoltage;
//    uint16_t maxVoltage;
//    int16_t minTemp;
//    int16_t maxTemp;
//    int16_t fetTemp;
//    int16_t pchgTemp;
//    int32_t current;
//    uint8_t soc;
//    uint8_t soh;
//    uint8_t soe;
//    bms_faultsLog faults;
//    uint8_t dsgStat  					: 1;
//    uint8_t chgStat	 					: 1;
//    uint8_t pchgStat 					: 1;
//	uint8_t interlockStatus				: 1;
//	uint32_t stackVoltage;
//	uint8_t minVoltage_cellno;
//	uint8_t maxVoltage_cellno;
//	uint8_t minTemp_NTCno;
//	uint8_t maxTemp_NTCno;
//	float mahGained;
//	float mahSpent;
//	int16_t afeTemp[2];
//}log_entry;


typedef struct __attribute__((__packed__)){
    uint32_t tick;
    uint32_t time;
    uint32_t time2;
    uint16_t faults;
    uint16_t packVolt;
    int16_t packCurrent;
    uint8_t RmCap;
    uint8_t FcCap;
    uint16_t maxCellVoltage;
	  uint8_t maxVoltage_cellno;
    uint16_t minCellVoltage;
   	uint8_t minVoltage_cellno;
    int16_t maxTemp;
    int16_t minTemp;
    uint8_t mosFet;
}log_entry;


#endif


typedef struct 
{
    uint8_t ais156_moveCursor;
    uint8_t ais156_g_exitSendRoutine;
    uint8_t ais156_formatMem ;
    volatile uint8_t ais156_logAckRecvd;
}ais156_Flags_Struct;

typedef struct 
{    
    uint32_t ais156_totalLogs;
    uint32_t ais156_logsToRetrieve;
    uint32_t ais156_specificLogNumToRetrieve;
    uint32_t ais156_logsSent;
    uint32_t ais156_logsRead;
    uint32_t ais156_startingLog;
    uint8_t ais156_retreiveLogCount;
    uint8_t ais156_logsFailed;    
}ais156_Log_Handle_Struct;

int op_sector_erase(struct ringfs_flash_partition *flash, int address);
signed int op_program(struct ringfs_flash_partition *flash, int address, const void *data, size_t size);
signed int op_read(struct ringfs_flash_partition *flash, int address, void *data, size_t size);
int ais156_canInterface(uint32_t canID, uint8_t * dataBuff, uint8_t length);
uint8_t ais156_getBmsStatus(void);

#endif /* PLATFORM_AIS156_LOGGING_INTERFACE_H_ */
