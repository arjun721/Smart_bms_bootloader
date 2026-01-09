 
#ifndef INC_NOR_FLASH_H_
#define INC_NOR_FLASH_H_

#include "SDK/inc/flash_types.h"
//#include "mcu_interface.h"


 typedef struct __attribute__((packed))
{
    uint16_t set;
    uint16_t release;
    uint16_t delay;
} Threshold_t;

/*--------------- 1. Basic Parameter Config ---------------*/
typedef struct __attribute__((packed))
{
    Threshold_t Cell_OV_P;
    Threshold_t Cell_UV_P;
    Threshold_t Pack_OV_P;
    Threshold_t Pack_UV_P;
    Threshold_t Chg_UT_P;
    Threshold_t Dsg_OT_P;
    Threshold_t Chg_OT_P;
    Threshold_t Chg_OC_P;
    Threshold_t Dsg_OC_P;
} BasicParameterConfig_t;

/*--------------- 2. Advanced Protection Config ---------------*/
typedef struct __attribute__((packed))
{
    Threshold_t Dsg_OC2_P;
    Threshold_t ShortCircuit;
    Threshold_t H_Cell_OV_P;
    Threshold_t L_Cell_UV_P;
} AdvancedProtectionConfig_t;

/*--------------- 3. Function Config ---------------*/
typedef struct __attribute__((packed))
{
    bool Load_EN;
    bool Balance_EN;
    bool CHG_Balance;
    bool NTC1;
    bool NTC2;
    bool NTC3;
    bool NTC4;
} FunctionConfig_t;

/*--------------- 4. Balance Config ---------------*/
typedef struct __attribute__((packed))
{
    uint16_t CellOpenVoltage;
    uint16_t BalanceDriftVoltage;
    uint16_t IPS_Off_Delay;
} BalanceConfig_t;

/*--------------- 5. Other Config ---------------*/
typedef struct __attribute__((packed))
{
    uint8_t  RS485Address;
    uint8_t  BatteryNumber;
    char     SerialNumber[16];   // Up to 15 chars + null terminator
} OtherConfig_t;

/*--------------- 6. Capacity Config ---------------*/
typedef struct __attribute__((packed))
{
    uint32_t NominalCapacity;
    uint32_t CycleCapacity;
    uint16_t FullSetVoltage;
    uint16_t EndOfVoltage;
    uint8_t  SelfDischargeRate;
} CapacityConfig_t;

/*--------------- Main BMS Config ---------------*/
typedef struct __attribute__((packed))
{
    BasicParameterConfig_t      BasicParameterConfig;
    AdvancedProtectionConfig_t  AdvancedProtectionConfig;
    FunctionConfig_t            FunctionConfig;
    BalanceConfig_t             BalanceConfig;
    OtherConfig_t               OtherConfig;
    CapacityConfig_t            CapacityConfig;
    uint32_t                    crc32;   // Optional checksum
} BmsParameters_t;

/*------------------ EEPROM Partition ------------------*/
#if NOR_FLASH == 16
#define EEPROM_PARTITION     13          // In sectors
#elif NOR_FLASH == 32
#define EEPROM_PARTITION     32          // Example: 32 sectors for 32 Mbit flash
#elif NOR_FLASH == 128
#define EEPROM_PARTITION     181         // In sectors
#else
#error "EEPROM_PARTITION not defined for this flash"
#endif

#define APPLOADER_SECTORS   256            // Same for all

/*------------------ AIS Logging ------------------*/
#if NOR_FLASH == 16
#define AIS_LOGGING_SECTORS 512
#elif NOR_FLASH == 32
#define AIS_LOGGING_SECTORS 1024          // Example: half of flash sectors
#elif NOR_FLASH == 128
#define AIS_LOGGING_SECTORS 1024
#else
#error "AIS_LOGGING_SECTORS not defined for this flash"
#endif


#define MAX_RW_ALLOWED 		PAGE_SIZE	//In Bytes

flashStatus_t flash_eraseCommands(uint32_t address, flash_eraseCmds command);
flashStatus_t flash_programPage(uint32_t address, const uint8_t *data, uint16_t length);
flashStatus_t flash_readData(uint32_t address, uint8_t *data, uint16_t length);

flashStatus_t flash_getUID(uint8_t* uID);
flashStatus_t flash_getMID(uint8_t* mID, uint8_t* dID);
flashStatus_t flash_getRID(uint8_t* mID, uint16_t* id);
void flash_testWriteRead(void);

flashStatus_t flash_writeBmsParameters(const BmsParameters_t *params);
flashStatus_t flash_readBmsParameters(BmsParameters_t *params);
void flash_testBmsParameters(void);

#define flashPageErase(address)				flash_eraseCommands(address, FLASH_PAGE_ERASE)
#define flashSectorErase(address)			flash_eraseCommands(address, FLASH_SECTOR_ERASE)
#define flashBlockErase(address)			flash_eraseCommands(address, FLASH_BLOCK_ERASE)
#define flashChipErase(address)				flash_eraseCommands(address, FLASH_CHIP_ERASE)


#endif /* INC_NOR_FLASH_H_ */
