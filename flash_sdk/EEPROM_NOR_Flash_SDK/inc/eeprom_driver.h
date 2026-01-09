#ifndef INC_EEPROM_DRIVER_H_
#define INC_EEPROM_DRIVER_H_

#include "nor_flash.h"
#include "eeprom_types.h"

#if EEPROM_REQUIRED_BYTES != 0x300
	#error "Currently supports only 0x300 bytes"
#endif

#if ENDURANCE_TARGET_YEARS < 10
	#error "Endurance target time too low"
#endif

#define PARTITION_OFFSET	1

#define EEPROM_START_ADDR	0

#define MAX_ENDURANCE_CYCLES	70000	/*!< This number indicates number of data updations possible in a page*/

#define TOTAL_CYCLES		(MAX_ENDURANCE_CYCLES*(EEPROM_PARTITION-PARTITION_OFFSET))

#define CYCLES_PER_YEAR		((uint32_t)(TOTAL_CYCLES/ENDURANCE_TARGET_YEARS))
#define CYCLES_PER_DAY		((uint32_t)(CYCLES_PER_YEAR/365))
#define CYCLES_PER_HOUR		((uint32_t)(CYCLES_PER_DAY/24))


#define SECONDS_PER_CYCLE	((uint32_t)(3600/CYCLES_PER_HOUR))	//Commented for now defaulted to 10 minutes

#if NOR_FLASH == 16
#define SECONDS_PER_CYCLE	(60*15)	//15 minutes by default
#elif NOR_FLASH == 128
#define SECONDS_PER_CYCLE	60	// 1 minute 
#endif

typedef struct{
	uint8_t eeprom[EEPROM_REQUIRED_BYTES];
}eeprom_image_Struct_t;

N_EEPROM_StatusTypeDef ee_driverInit();
N_EEPROM_StatusTypeDef ee_driverDeInit();
N_EEPROM_StatusTypeDef ee_driverReady();
N_EEPROM_StatusTypeDef ee_eraseEEPROM();
N_EEPROM_StatusTypeDef ee_readData(uint32_t address, uint8_t * dataBuff, uint8_t size);
N_EEPROM_StatusTypeDef ee_writeData(uint32_t address, uint8_t * const dataBuff, uint8_t size);
N_EEPROM_StatusTypeDef ee_checkAddress(uint32_t address, uint8_t size);
N_EEPROM_StatusTypeDef ee_updateEEPROM(void);
N_EEPROM_StatusTypeDef ee_formatEEPROM(void);
N_EEPROM_StatusTypeDef ee_integrityEEPROM(bool * integrity);
N_EEPROM_StatusTypeDef ee_writeEEPROMImage(uint8_t* buff);
#endif /* INC_EEPROM_DRIVER_H_ */
