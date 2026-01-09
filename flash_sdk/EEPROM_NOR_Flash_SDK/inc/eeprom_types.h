#ifndef INC_EEPROM_TYPES_H_
#define INC_EEPROM_TYPES_H_

#include "stdint.h"
#include "stdbool.h"

#define EEPROM_VERSION 0xEEEE

#define ENDURANCE_TARGET_YEARS	10 		/*!< This macro defines the target time to EEPROM in years*/

#define EEPROM_REQUIRED_BYTES	0x300	/*!< This macro defines the number of bytes required for the EEPROM memory.*/


#define EEPROM_ALLOCATED_BYTES (SECTOR_SIZE*EEPROM_PARTITION)	/*!<  This defines the size of the allocated bytes for the EEPROM partition in bytes, where SECTOR_SIZE is the sector size of the EEPROM and EEPROM_PARTITION is the number of sectors allocated for the EEPROM */


/**
 * @brief The N_EEPROM_StatusTypeDef enum is used to define the status of operations when interacting with the EEPROM
 * 
 */
typedef enum{
	N_EEPROM_OK						=	0x0,	/*!< EEPROM Command executed successfully*/
	N_EEPROM_ERROR					=	0x1,	/*!< Error occured in execution of EEPROM command*/
	N_EEPROM_ADDR_OOB				=	0x2,	/*!< Address is out of bounds*/
	N_EEPROM_INITIALIZED			=	0x3,	/*!< EEPROM Initialized successfuly*/
	N_EEPROM_INITIALIZE_ERROR 		=	0x4,	/*!< EEPROM Initialization error*/
	N_EEPROM_GARBAGE_FLASH			=	0x5,	/*!< No file system found on flash*/
	N_EEPROM_ADDR_RSVD				=	0x6,	/*!< The EEPROM address is reserved*/
	N_EEPROM_READ_ERROR				=	0x7,	/*!< EEPROM Read error*/
	N_EEPROM_WRITE_ERROR			=	0x8,	/*!< EEPROM Write error*/
	N_EEPROM_FORMATTED				=	0x9,	/*!< EEPROM Formatted*/
	N_EEPROM_FORMAT_FAILED			=	0xA,	/*!< EEPROM Fomrat failed*/
	N_EEPROM_UPDATED				=	0xB,	/*!< EEPROM Updated*/
	N_EEPROM_NO_CHANGE				=	0xC,	/*!< No change found in RAM and EEPROM*/
	N_EEPROM_PREINIT_FAILED			=   0xD,	/*!< EEPROM old data structure retrieval failed*/
	N_EEPROM_FIXED					=	0xE,	/*!< EEPROM Fixed in runtime on abnormality detection*/
	N_EEPROM_FIXING_FAILED_WRITE	=	0xD,	/*!< EEPROM Fixing failed while writing*/
	N_EEPROM_FIXING_FAILED_READ		=	0xF,	/*!< EEPROM Fixing failed while reading*/
	N_EEPROM_EMPTY_ERROR			=	0x10	/*!< EEPROM File System exists but empty*/
}N_EEPROM_StatusTypeDef;

#if 0 /*Below code commented out*/
/**
 * @brief EEPROM Information structure
 * @details This structure stores the information related to an EEPROM memory address. \n
 * 			It contains the eeprom address, the start address, the end address \n
 * 			and the number of pages within the memory.
 */
typedef struct{
	uint32_t address;		/*!< EEPROM Address*/			
	uint32_t startAddress;	/*!< EEPROM Start Address*/
	uint32_t endAddress;	/*!< EEPROM End Address*/
	uint32_t pages;			/*!< Number of pages in EEPROM*/
}eeprom_address;

/**
 * @brief EEPROM Instances enumerator
 * @details This enum is used to specify the type of EEPROM instance. \n
 * 			PRIME_EEPROM is used to refer to the primary EEPROM \n
 * 			and MIRROR_EEPROM is used to refer to the mirrored EEPROM.
 * 
 */
typedef enum{
	PRIME_EEPROM	= 0,
	MIRROR_EEPROM	= 1,
}eepromInstance_t;
#endif
#endif /* INC_EEPROM_TYPES_H_ */
