
#ifndef INC_NOR_FLASH_H_
#define INC_NOR_FLASH_H_

#include "SDK/inc/flash_types.h"

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

#define flashPageErase(address)				flash_eraseCommands(address, FLASH_PAGE_ERASE)
#define flashSectorErase(address)			flash_eraseCommands(address, FLASH_SECTOR_ERASE)
#define flashBlockErase(address)			flash_eraseCommands(address, FLASH_BLOCK_ERASE)
#define flashChipErase(address)				flash_eraseCommands(address, FLASH_CHIP_ERASE)


#endif /* INC_NOR_FLASH_H_ */
