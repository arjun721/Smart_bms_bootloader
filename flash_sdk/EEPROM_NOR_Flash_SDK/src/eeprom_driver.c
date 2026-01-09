#include "eeprom_driver.h"
#include "ringfs.h"
#include <string.h>
//#include "module_log.h"

bool eeprom_potentialError = 0;

static bool eepromInitialized = 0;

static bool eepromChangesCommited = 0;

static eeprom_image_Struct_t eeprom_Image = {0};

static uint8_t eeprom_potentialErrorCounter = 0;

int eeflash_sector_erase(struct ringfs_flash_partition *flash, int address);
signed int eeflash_program(struct ringfs_flash_partition *flash, int address, const void *data, size_t size);
signed int eeflash_read(struct ringfs_flash_partition *flash, int address, void *data, size_t size);

/**
 * Erases a sector of the flash.
 *
 * @param flash The flash partition structure.
 * @param address The address of the sector to erase.
 *
 * @returns None
 */
int eeflash_sector_erase(struct ringfs_flash_partition *flash, int address)
{
	if(eepromInitialized != 1){
		return -1;
	}

    (void) flash;
	
	if (FLASH_STATUS_OK != flashSectorErase(address)){
		return -1;
	}
    return 0;
}

/**
 * Writes data to the flash.
 *
 * @param flash The flash partition structure.
 * @param address The address to write to.
 * @param data The data to write.
 * @param size The size of the data to write.
 *
 * @returns The number of bytes written.
 */
signed int eeflash_program(struct ringfs_flash_partition *flash, int address, const void *data, size_t size)
{
	if(eepromInitialized != 1){
		return -1;
	}

    (void) flash;
	if(size > PAGE_SIZE){
		uint32_t loops = 0;
		loops = ((size/PAGE_SIZE) + (size%PAGE_SIZE != 0));
		for(uint8_t wPage = 0; wPage < loops; wPage++){
			uint32_t sizeToWrite = PAGE_SIZE;
			if((size%PAGE_SIZE != 0) && (wPage == loops-1)){
				sizeToWrite = size%PAGE_SIZE;
			}
			__asm("nop");
			if(FLASH_STATUS_OK != flash_programPage((address + (PAGE_SIZE*wPage)), (data + (PAGE_SIZE*wPage)), sizeToWrite)){
				return -1;
			}
			__asm("nop");
		}
	}
	else{
		if(FLASH_STATUS_OK != flash_programPage(address, (uint8_t *)data, size)){
			return -1;
		}
	}
    return size;
}

/**
 * Reads data from the flash.
 *
 * @param flash The flash partition structure
 * @param address The address to read from.
 * @param data The data to read.
 * @param size The size of the data to read.
 *
 * @returns The number of bytes read.
 */
signed int eeflash_read(struct ringfs_flash_partition *flash, int address, void *data, size_t size)
{
	if(eepromInitialized != 1){
		return -1;
	}

	(void) flash;
	if(size > PAGE_SIZE){
		uint32_t loops = 0;
		loops = ((size/PAGE_SIZE) + (size%PAGE_SIZE != 0));
		for(uint8_t rPage = 0; rPage < loops; rPage++){
			uint32_t sizeToRead = PAGE_SIZE;
			if((size%PAGE_SIZE != 0) && (rPage == loops-1)){
				sizeToRead = size%PAGE_SIZE;
			}
			if(FLASH_STATUS_OK != flash_readData((address + (PAGE_SIZE*rPage)), (data + (PAGE_SIZE*rPage)), sizeToRead)){
				return -1;
			}
			__asm("nop");
		}
	}
	else{
		if(FLASH_STATUS_OK != flash_readData(address, data, size)){
			return -1;
		}
	}
    return size;
}

/**
 * @brief Structure for ringfs partition
 * @details Struct ringfs_flash_partition flash contains details related to the flash partition used for RingFS. \n
 * 			It includes the sector size, sector offset, sector count and the flash operations functions such as sector_erase, program and read.
 */
static struct ringfs_flash_partition eeflash_partition = {
    .sector_size = SECTOR_SIZE,
    .sector_offset = PARTITION_OFFSET,
    .sector_count = (EEPROM_PARTITION - PARTITION_OFFSET),

    .sector_erase = eeflash_sector_erase,
    .program = eeflash_program,
    .read = eeflash_read,
};

static struct ringfs eeflash_fs;

/**
 * Reads an eeprom object from the logged data.
 *
 * @param eepObj The log entry to read.
 *
 * @returns 0 on success, -1 on failure.
 */
static N_EEPROM_StatusTypeDef ee_readFlash(eeprom_image_Struct_t * eepObj){
	if(eepromInitialized != 1){
		return N_EEPROM_INITIALIZE_ERROR;
	}
	if(ringfs_fetch(&eeflash_fs, eepObj) == 0){
		ringfs_rewind(&eeflash_fs);
		return N_EEPROM_OK;
	}
	ringfs_rewind(&eeflash_fs);
	return N_EEPROM_READ_ERROR;
}

/**
 * Appends an eeprom object to the flash.
 *
 * @param eepObj The object to append.
 *
 * @returns 0 on success, -1 on failure.
 */
static N_EEPROM_StatusTypeDef ee_appendFlash(const eeprom_image_Struct_t * eepObj){
	if(eepromInitialized != 1){
		return N_EEPROM_INITIALIZE_ERROR;
	}
	if(ringfs_append(&eeflash_fs, eepObj) == 0){
		__asm("nop");
		return N_EEPROM_OK;
	}
	return N_EEPROM_WRITE_ERROR;
}

/**
 * @brief 
 * 
 * @param changesDetected 
 * @return N_EEPROM_StatusTypeDef 
 */
static N_EEPROM_StatusTypeDef ee_detectChange(bool * changesDetected){

	eeprom_image_Struct_t eeprom_tmpObj;
	memset(&eeprom_tmpObj, 0, sizeof(eeprom_tmpObj));
	*changesDetected = 0;
	if(ee_readFlash(&eeprom_tmpObj) == N_EEPROM_OK){
		for(uint32_t byte = 0; byte < sizeof(eeprom_tmpObj); byte++){
			if(eeprom_tmpObj.eeprom[byte] != eeprom_Image.eeprom[byte]){
				*changesDetected = 1;
				break;
			}
		}
		return N_EEPROM_OK;
	}
	return N_EEPROM_READ_ERROR;
}

/**
 * @brief 
 * 
 * @param slotsToDiscard 
 * @return int8_t 
 */
static int ee_discardExtraSlots(int16_t slotsToDiscard){
	
	if(slotsToDiscard < 0){
		return -1;
	}
    for(uint8_t slot = 0; slot < slotsToDiscard; slot++){
		int discardRetStatus = 0;
        discardRetStatus = ringfs_item_discard(&eeflash_fs);
		if(discardRetStatus == -1){
			return -1;
		}
    }
	ringfs_rewind(&eeflash_fs);
	int slotsRemaining = ringfs_count_exact(&eeflash_fs);
	return slotsRemaining;
}

/**
 * @brief 
 * 
 * @param ip_logToUse 
 * @param ip_totalLogs 
 * @param op_validSlots 
 * @param op_totalSlots 
 * @param op_logToUse 
 * @param useImage 
 * @return N_EEPROM_StatusTypeDef 
 */
static N_EEPROM_StatusTypeDef ee_fixBadSlots(uint32_t ip_logToUse, int ip_totalLogs, int * op_validSlots, int * op_totalSlots, uint32_t * op_logToUse, bool useImage){

	/*Discard slots from current read to actual valid slot*/
	if(ee_discardExtraSlots(ip_logToUse - 1) == -1){
		return N_EEPROM_ERROR;
	}
	eeprom_image_Struct_t eeprom_tmpObj;
	if(useImage == 0){
		memset(&eeprom_tmpObj, 0, sizeof(eeprom_tmpObj));	
		if(ee_readFlash(&eeprom_tmpObj) != N_EEPROM_OK){						/*Read actual valid slot*/
			__asm("nop");
			return N_EEPROM_FIXING_FAILED_READ;
		}
	}
	else{
		memcpy(&eeprom_tmpObj, &eeprom_Image, sizeof(eeprom_image_Struct_t));	/*Copy RAM image to local buffer*/
	}

	if(ee_appendFlash(&eeprom_tmpObj) != N_EEPROM_OK){	/*Write the slot data to new slot*/
		__asm("nop");
		return N_EEPROM_FIXING_FAILED_WRITE;
	}
	/*Discard slots from actual valid to new valid slot*/
	*op_validSlots = ee_discardExtraSlots((ip_totalLogs - ip_logToUse) + 1);
	if(*op_validSlots == -1){
		__asm("nop");
		return N_EEPROM_ERROR;
	}

	/*Sanity check*/
	*op_totalSlots = ringfs_getTotalSlotsandLatesValidSlot(&eeflash_fs, op_logToUse);
	if(*op_totalSlots == -1){
		__asm("nop");
		return N_EEPROM_ERROR;
	}

	__asm("nop");

	return N_EEPROM_OK;
}
/***************************************************************************************************************************************************/
/***************************************************************************************************************************************************/
/***************************************************************************************************************************************************/

/**
 * @brief Initializes the EEPROM driver.
 * @param instance The EEPROM instance to initialize.
 * 
 * @details This function is used to initialize the EEPROM instance by setting start and end addresses. \n
 * 			If the instance is already initialized, it will return N_EEPROM_INITIALIZED, \n 
 * 			else it will erase the EEPROM and write the necessary data to the addresses and return N_EEPROM_INITIALIZED. \n
 * 			If the initialization fails, it will return N_EEPROM_INITIALIZE_ERROR
 *
 * @returns A status code indicating whether or not the initialization was successful.
 */
N_EEPROM_StatusTypeDef ee_driverInit(void){
	int retVal = -1;
	retVal = ringfs_init(&eeflash_fs, &eeflash_partition, EEPROM_VERSION, sizeof(eeprom_Image));
	if(retVal == 0){
		eepromInitialized = 1;

		int scanTries = 3;
		int scanRetStat = -1;
		int dataPoints = 0;
		do{
			scanRetStat = ringfs_scan(&eeflash_fs);
			if(scanRetStat == 0){
				dataPoints = ringfs_count_exact(&eeflash_fs);
				if(dataPoints > 0){
					break;
				}
				else{
					__asm("nop");
					scanRetStat = 2;
				}
				//DELAY_MS(100);
				__asm("nop");
			}
			scanTries--;
		}while(scanTries > 0);
		if (scanRetStat == 0) {
			int valid_dataSets_Available = 0;
			__asm("nop");
			valid_dataSets_Available = ringfs_count_exact(&eeflash_fs);
			if(valid_dataSets_Available < 0){
				return N_EEPROM_INITIALIZE_ERROR;
			}
			__asm("nop");
			uint32_t logToUse = 0;
			int totalLogs = 0;
			totalLogs = ringfs_getTotalSlotsandLatesValidSlot(&eeflash_fs, &logToUse);
			if(totalLogs < 0){
				return N_EEPROM_INITIALIZE_ERROR;
			}
			if((totalLogs >= valid_dataSets_Available) && (totalLogs > 1 || valid_dataSets_Available > 1) && (logToUse >= 1) && (valid_dataSets_Available >= 1)){
				N_EEPROM_StatusTypeDef tempStatus = N_EEPROM_ERROR;
				tempStatus = ee_fixBadSlots(logToUse, totalLogs, &valid_dataSets_Available, &totalLogs, &logToUse, false);
				__asm("nop");
				if(N_EEPROM_OK != tempStatus){
					__asm("nop");
					return tempStatus;
				}
			}
			__asm("nop");
			if(valid_dataSets_Available == 1 && totalLogs == 1 && logToUse == 1){
				__asm("nop");
				if(ee_readFlash(&eeprom_Image) == N_EEPROM_OK){
					return N_EEPROM_INITIALIZED;		/*INIT SUCCESSFULL*/
				}
			}
			__asm("nop");
			return N_EEPROM_INITIALIZE_ERROR;
    	}
		else if(scanRetStat == 2){
			__asm("nop");
			return N_EEPROM_EMPTY_ERROR;
		}
    	else{
			uint8_t rxBuff[PAGE_SIZE] = {0};
			flashStatus_t flashRetStat = FLASH_STATUS_ERROR;
			bool newFlash = true;
			for(uint32_t page = 0; page < PAGES_PER_SECTOR; page++){
				flashRetStat = flash_readData(PAGE_ADDR_FROM_NUM(page), rxBuff, sizeof(rxBuff));
				if(flashRetStat != FLASH_STATUS_OK){
					return N_EEPROM_READ_ERROR;		/*FLASH READ FAILED*/
				}
				for(uint32_t byte = 0; byte < PAGE_SIZE; byte++){
					if(0xFF != rxBuff[byte]){
						__asm("nop");
						newFlash = false;
						break;
					}
				}
				if(newFlash == false){
					break;
				}
			}
			__asm("nop");
			if(newFlash == true){
				if(ringfs_format(&eeflash_fs, (void*)0) == 0){ //mcu_wwdgRefresh
					if(ringfs_scan(&eeflash_fs) == 0){
						memset(&eeprom_Image, 0, sizeof(eeprom_Image));
						if(ee_appendFlash(&eeprom_Image) == N_EEPROM_OK){
							bool changes = 1;
							if(ee_detectChange(&changes) != N_EEPROM_OK){
								return N_EEPROM_INITIALIZE_ERROR;
							}
							if(changes == 0){
								eepromChangesCommited = true;
								return N_EEPROM_INITIALIZED;	/*INIT SUCCESSFULL*/
							}
						}
					}
					return N_EEPROM_INITIALIZE_ERROR;	/*UNABLE TO WRITE FIRST EEPROM SLOT TO FLASH -> FAIL*/
				}
				return N_EEPROM_INITIALIZE_ERROR;		/*UNABLE TO WRITE RING_FS FS TO FLASH -> FAIL*/
			}
			return N_EEPROM_GARBAGE_FLASH;				/*USED FLASH FOUND -> FAIL*/
    	}
	}
	__asm("nop");
	return N_EEPROM_INITIALIZE_ERROR;					/*UNABLE TO INIT EE_FS STRUCTURE -> FAIL*/
}

/**
 * Reads data from the EEPROM.
 *
 * @param instance The EEPROM instance to use.
 * @param address The address to start reading from.
 * @param dataBuff The buffer to store the data in.
 * @param size The number of bytes to read.
 * 
 * @details This function is used to read data from the EEPROM instance in the given address \n
 * 			and store it in the given data buffer. It checks the instance and address given are valid before reading. \n
 * 			It returns N_EEPROM_OK on success and N_EEPROM_ERROR on failure.
 *
 * @returns N_EEPROM_OK if the read was successful, N_EEPROM_ERROR otherwise.
 */
N_EEPROM_StatusTypeDef ee_readData(uint32_t address, uint8_t * dataBuff, uint8_t size){

	N_EEPROM_StatusTypeDef ee_retStat = N_EEPROM_ERROR;
	ee_retStat = ee_checkAddress(address, size);
	if(ee_retStat != N_EEPROM_OK){
		return ee_retStat;
	}
	memcpy(dataBuff, &eeprom_Image.eeprom[address], size);	//Copy data from eeprom image to data buffer	
	return ee_retStat;
}

/**
 * Writes data to the EEPROM.
 *
 * @param instance The EEPROM instance to write to.
 * @param address The address to start writing to.
 * @param dataBuff The data to write.
 * @param size The number of bytes to write.
 * 
 * @details Writes data to an EEPROM. It first checks if the driver is ready, then checks that the address and size are valid. \n
 * 			If both the checks are valid, it proceeds to program the page in the EEPROM with the given data buffer. \n
 * 			If the program is successful, it returns N_EEPROM_OK. Otherwise, it returns N_EEPROM_ERROR.
 *
 * @returns N_EEPROM_OK if the write was successful, N_EEPROM_ERROR otherwise.
 */
N_EEPROM_StatusTypeDef ee_writeData(uint32_t address, uint8_t * const dataBuff, uint8_t size){

	N_EEPROM_StatusTypeDef ee_retStat = N_EEPROM_ERROR;

	ee_retStat = ee_checkAddress(address, size);
	if(ee_retStat != N_EEPROM_OK){
		return ee_retStat;
	}

	__asm("nop");
	bool changesFound = 0;
	if(eepromChangesCommited == true){
		eepromChangesCommited = false;
		if(ee_detectChange(&changesFound) == N_EEPROM_OK){
			__asm("nop");
			if(changesFound && eeprom_potentialErrorCounter<=3){
				eeprom_potentialErrorCounter++;
				if(eeprom_potentialErrorCounter > 3) {
				eeprom_potentialError = 1;
				eeprom_potentialErrorCounter = 0;
				}
			}
		}
	}
	
	memcpy(&eeprom_Image.eeprom[address], dataBuff, size);	//Copy data from data buffer to the eeprom image
	return ee_retStat;
}

/**
 * Checks if the address is within the bounds of the EEPROM instance.
 *
 * @param instance The EEPROM instance to check.
 * @param address The address to check.
 * @param size The size of the data to check.
 * 
 * @details This function checks the given address and size for the specified eeprom instance to see if it is a valid address. \n
 * 			It returns N_EEPROM_OK if the address is valid, and N_EEPROM_ADDR_OOB if the address is out of bounds.
 *
 * @returns N_EEPROM_OK if the address is within the bounds of the EEPROM instance.
 *          N_EEPROM_ADDR_OOB if the address is out of bounds.
 */
N_EEPROM_StatusTypeDef ee_checkAddress(uint32_t address, uint8_t size){
	if(address < EEPROM_START_ADDR || ((address+size) > (EEPROM_REQUIRED_BYTES))){
		return N_EEPROM_ADDR_OOB;
	}
	return N_EEPROM_OK;
}

/**
 * @brief This function is to be called periodically
 * 		  to update the EEPROM to latest data and switch partitions
 *		  based on endurance of pages
 * 
 * @return N_EEPROM_StatusTypeDef 
 */
N_EEPROM_StatusTypeDef ee_updateEEPROM(void){
	N_EEPROM_StatusTypeDef ee_retStat = N_EEPROM_ERROR;

	bool updateEEPROM = 0;
	ee_retStat = ee_detectChange(&updateEEPROM);

	if(ee_retStat == N_EEPROM_OK){
		if(updateEEPROM == 1){
			ee_retStat = ee_appendFlash(&eeprom_Image);
			if(ee_retStat == N_EEPROM_OK){
				ringfs_advance_slot_specific(&eeflash_fs, 1);
				ee_retStat = ee_detectChange(&updateEEPROM);
				if(ee_retStat == N_EEPROM_OK && updateEEPROM == 0){
					int retDiscardStat = 0;
					retDiscardStat = ringfs_item_discard(&eeflash_fs);
					if(retDiscardStat == -1){
						ee_retStat = N_EEPROM_WRITE_ERROR;
					}
					else{
						eepromChangesCommited = true;
						ee_retStat = N_EEPROM_UPDATED;
					}
					ringfs_rewind(&eeflash_fs);
				}
				else{
					ee_retStat = N_EEPROM_WRITE_ERROR;
					if(eeprom_potentialErrorCounter<=3){
						eeprom_potentialErrorCounter++;
						if(eeprom_potentialErrorCounter > 3) {
							eeprom_potentialError = 1;
							eeprom_potentialErrorCounter = 0;
						}
					}
				}
			}
			/*************************************EEPROM_Integrity_checking_and_fixing************************************/
			
			if(ee_retStat == N_EEPROM_WRITE_ERROR){
				uint32_t latestSlot = 0;
				int validSlots = 0;
				validSlots = ringfs_count_exact(&eeflash_fs);
				if(validSlots < 0){
					return N_EEPROM_ERROR;
				}

				int totalSlots = 0;
				totalSlots = ringfs_getTotalSlotsandLatesValidSlot(&eeflash_fs, &latestSlot);
				if(totalSlots < 0){
					return N_EEPROM_ERROR;
				}

				if((totalSlots >= validSlots) && (totalSlots > 1 || validSlots > 1) && latestSlot >= 1 && (validSlots >= 1)){
					__asm("nop");
					//Try to fix the slots buffers
					if(ee_fixBadSlots(latestSlot, totalSlots, &validSlots, &totalSlots, &latestSlot, true) != N_EEPROM_OK){
						eeprom_potentialError = true;
						return N_EEPROM_ERROR;
					}

					//Check if fixing was successfull
					if(validSlots != 1 || totalSlots != 1 || latestSlot != 1){
						eeprom_potentialError = true;
						return N_EEPROM_ERROR;
					}

					updateEEPROM = 0;
					ee_retStat = ee_detectChange(&updateEEPROM);
					if(N_EEPROM_OK != ee_retStat || (updateEEPROM == 1)){
						eeprom_potentialError = true;
						return N_EEPROM_ERROR;
					}

					ee_retStat = N_EEPROM_FIXED;
				}
			}
			
			/************************************************************************************************************/
		}
		else{
			ee_retStat = N_EEPROM_NO_CHANGE;
		}
	}
	else {
		__asm("nop");
		if(eeprom_potentialErrorCounter<=3){
			eeprom_potentialErrorCounter++;
			if(eeprom_potentialErrorCounter > 3) {
				eeprom_potentialError = 1;
				eeprom_potentialErrorCounter = 0;
			}
		}
	}
	return ee_retStat;
}

/**
 * @brief 
 * 
 * @param integrity 
 * @return N_EEPROM_StatusTypeDef 
 */
N_EEPROM_StatusTypeDef ee_integrityEEPROM(bool * integrity){
	*integrity = eeprom_potentialError;
}

/**
 * @brief This function formats the complete sectors of EEPROM
 * 
 * @return N_EEPROM_StatusTypeDef 
 */
N_EEPROM_StatusTypeDef ee_formatEEPROM(void){

	N_EEPROM_StatusTypeDef retStat = N_EEPROM_FORMATTED;

	for (int32_t sector = 0; sector < EEPROM_PARTITION; sector++){
	
		flashStatus_t flashRetStat = FLASH_STATUS_ERROR;
	
		flashRetStat = flashSectorErase(SECTOR_ADDR(sector));
	
		if(flashRetStat != FLASH_STATUS_OK){
			retStat = N_EEPROM_FORMAT_FAILED;
			break;
		}
	}
	return retStat;
}

/**
 * @brief Retrieves data from EEPROM Image in RAM.
 * 
 * @return N_EEPROM_StatusTypeDef 
 */
N_EEPROM_StatusTypeDef ee_writeEEPROMImage(uint8_t* buff)
{
	memcpy(&eeprom_Image.eeprom[0],buff,EEPROM_REQUIRED_BYTES);

	return N_EEPROM_OK;
}