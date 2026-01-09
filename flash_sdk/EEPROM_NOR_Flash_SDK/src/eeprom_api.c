#include "../inc/eeprom_api.h"
#include <string.h>
#include "../inc/eeprom_driver.h"

#define ADDR_ADJUST(address) (address - 0x4000)   /*!<This macro subtracts 0x4000 from the given address to adjust it to the physical address of the EEPROM.*/

#define EEPROM_WORD_SIZE	4
#define EEPROM_BYTE_SIZE	1

static void eeprom_revMemcpy(void * destination, void * source, uint32_t length);

static void eeprom_revMemcpy(void * destination, void * source, uint32_t length) {
	char *charDest = (char *) destination;
	char *charSrc = (char *) source;
	for (int ijk = 0; ijk < (int) length; ijk++)
		charDest[ijk] = charSrc[(length - 1) - ijk];
}

static uint32_t eeprom_updateTime_ms = 0;
/***********************************************************************************************************/
/**************************************************EEPROM Functions*****************************************/
/***********************************************************************************************************/

/**
 * @brief Initializes the EEPROM interface.
 *
 * @returns EEPROM_OK if the EEPROM was successfully initialized.
 */
N_EEPROM_StatusTypeDef eepromInterface_eepromInit(void){
	N_EEPROM_StatusTypeDef retStatus = N_EEPROM_ERROR;

//#ifdef STDCAN
	/*Variable for receiving data*/
	uint8_t rxData[EEPROM_REQUIRED_BYTES] = {0};
	uint8_t preInitFlag=0;
	retStatus = ee_eepromPreinit(rxData);
	if (retStatus == N_EEPROM_OK){
		preInitFlag=1;
	}
//#endif

	retStatus = ee_driverInit();
#ifdef TEST_DEBUG
	MODULE_DEBUG("");
#endif
	if(retStatus == N_EEPROM_INITIALIZED){
		return retStatus;
	}
	else if(retStatus == N_EEPROM_GARBAGE_FLASH || retStatus == N_EEPROM_EMPTY_ERROR){
		retStatus = ee_formatEEPROM();
		if(retStatus == N_EEPROM_FORMATTED){
			retStatus = ee_driverInit();
			__asm("nop");
			// #ifdef STDCAN
			if(retStatus == N_EEPROM_INITIALIZED){
				if (preInitFlag==1){
					ee_writeEEPROMImage(rxData);
					eepromInterface_updateEEPROM(0, 1, NULL);
				}
			}
			// #endif
		}
		else{
			retStatus = N_EEPROM_GARBAGE_FLASH;
		}
		__asm("nop");

		return retStatus;
	}
	else{
		return retStatus;
	}
}

/**
 * Reads a word from the EEPROM.
 *
 * @param address The address to read from.
 * @param pData A pointer to the data to be read.
 *
 * @returns EEPROM_OK if the read was successful.
 */
N_EEPROM_StatusTypeDef eepromInterface_readWord(uint32_t address, uint32_t *pData){
	uint8_t rxBuff[EEPROM_WORD_SIZE] = {0};
	N_EEPROM_StatusTypeDef retStatus = N_EEPROM_ERROR;
	retStatus = ee_readData(ADDR_ADJUST(address), rxBuff, EEPROM_WORD_SIZE);
	if(retStatus == N_EEPROM_OK){
		memcpy(pData, rxBuff, sizeof(rxBuff));
	}
	return retStatus;
}

/**
 * @brief Reads a word from the EEPROM in reverse order.
 * @note For legacy purposes
 * 
 * @param address The address of the word to read.
 * @param pData A pointer to the data to be read.
 *
 * @returns A status code indicating the success of the operation.
 */
N_EEPROM_StatusTypeDef eepromInterface_readWordReverse(uint32_t address, uint32_t *pData){
	uint8_t rxBuff[EEPROM_WORD_SIZE] = {0};
	N_EEPROM_StatusTypeDef retStatus = N_EEPROM_ERROR;
	retStatus = ee_readData(ADDR_ADJUST(address), rxBuff, EEPROM_WORD_SIZE);
	if(retStatus == N_EEPROM_OK){
		eeprom_revMemcpy(pData, rxBuff, sizeof(rxBuff));
	}
	return retStatus;
}

/**
 * @brief Reads a byte from the EEPROM.
 *
 * @param address The address of the byte to read.
 * @param pData A pointer to the data to be read.
 *
 * @returns EEPROM_OK if the read was successful, EEPROM_ERROR otherwise.
 */
N_EEPROM_StatusTypeDef eepromInterface_readByte(uint32_t address, uint8_t *pData){
	uint8_t rxBuff[EEPROM_BYTE_SIZE] = {0};
	N_EEPROM_StatusTypeDef retStatus = N_EEPROM_ERROR;
	retStatus = ee_readData(ADDR_ADJUST(address), rxBuff, EEPROM_BYTE_SIZE);
	if(retStatus == N_EEPROM_OK){
		memcpy(pData, rxBuff, sizeof(rxBuff));
	}
	return retStatus;
}

/**
 * @brief Writes a value to the EEPROM.
 *
 * @param address The address to write to.
 * @param val The value to write.
 * @param size The size of the value to write.
 *
 * @returns The status of the write operation.
 */
N_EEPROM_StatusTypeDef eepromInterface_write(uint32_t address, int32_t val, uint8_t size){
	uint8_t txBuff[EEPROM_WORD_SIZE] = {0};
	N_EEPROM_StatusTypeDef retStatus = N_EEPROM_ERROR;

	if(size > EEPROM_WORD_SIZE){
		return retStatus;
	}
	memcpy(txBuff, &val, size);
	retStatus = ee_writeData(ADDR_ADJUST(address), txBuff, size);

	return retStatus;
}

/**
 * @brief Writes a 32-bit value to the EEPROM in reverse order.
 * @note For legacy purposes
 * 
 * @param address The address to write to.
 * @param val The value to write.
 * @param byteorword The number of bytes to write.
 *
 * @returns The status of the write operation.
 */
N_EEPROM_StatusTypeDef eepromInterface_writeReverse(uint32_t address, int32_t val, uint8_t byteorword){
	uint8_t txBuff[EEPROM_WORD_SIZE] = {0};
	N_EEPROM_StatusTypeDef retStatus = N_EEPROM_ERROR;

	if(byteorword > EEPROM_WORD_SIZE){
		return retStatus;
	}
	eeprom_revMemcpy(txBuff, &val, byteorword);
	retStatus = ee_writeData(ADDR_ADJUST(address), txBuff, byteorword);

	return retStatus;
}

/**
 * @brief Writes a byte to the EEPROM at the specified address and then reads it and verifies it.
 * 
 * @param address The address to write the byte to.
 * @param val The byte to write.
 *
 * @returns The status of the write operation.
 */
N_EEPROM_StatusTypeDef eepromInterface_writeByteNVerify(uint32_t address, uint8_t val){
	N_EEPROM_StatusTypeDef retStatus = N_EEPROM_ERROR;
	uint8_t wBuff[1] = {0};
	memcpy(&wBuff[0], &val, sizeof(val));
	retStatus = eepromInterface_write(address, wBuff[0], 1);
	if(retStatus == N_EEPROM_OK){
		uint8_t rBuff[1] = {0};
		retStatus = eepromInterface_readByte(address, &rBuff[0]);
		if(retStatus != N_EEPROM_OK){
			return retStatus;
		}
		if(rBuff[0] == wBuff[0])
		{
			return N_EEPROM_OK;
		}
	}
	return N_EEPROM_ERROR;
}

/**
 * @brief Checks if an address is valid for the EEPROM.
 *
 * @param address The address to check.
 * @param size The size of the data to check.
 *
 * @returns EEPROM_OK if the address is valid, EEPROM_ERROR otherwise.
 */
N_EEPROM_StatusTypeDef eepromInterface_checkAddress(uint32_t address, uint8_t size){
	return ee_checkAddress(ADDR_ADJUST(address), size);
}

/**
 * @brief This function updates the eeprom in regular interval if specified time has elapsed since last updation
 * 		  and changes are detected
 * 
 * @note To be called periodically, ideally in a fast task/routine
 * 
 * @param time_ms 	time_stamp in milli seconds
 * @param forceUpdate 	force update flag (send true to forcefully update flash if change)
 * @return N_EEPROM_StatusTypeDef 
 */
// uint32_t nFlash_writeTest_Var = 0;
N_EEPROM_StatusTypeDef eepromInterface_updateEEPROM(uint32_t time_ms, bool forceUpdate, bool * eeError){
	int32_t timeDelta_s = (int32_t)((time_ms - eeprom_updateTime_ms)/1000);
	N_EEPROM_StatusTypeDef retStat = N_EEPROM_OK;
	if(timeDelta_s >= SECONDS_PER_CYCLE || forceUpdate == true){
		eeprom_updateTime_ms = time_ms;
		// uint8_t dataBuff[4] = {0};
		// uint32_t testWrite = 0;
		// ee_readData(0x168, dataBuff, sizeof(dataBuff));
		// memcpy(&testWrite, dataBuff, sizeof(dataBuff));
		// testWrite++;
		// memset(dataBuff, 0, sizeof(dataBuff));
		// memcpy(dataBuff, &testWrite, sizeof(dataBuff));
		// nFlash_writeTest_Var = testWrite;
		// ee_writeData(0x168, dataBuff, sizeof(dataBuff));
		retStat = ee_updateEEPROM();
		if(retStat == N_EEPROM_UPDATED || retStat == N_EEPROM_NO_CHANGE || retStat == N_EEPROM_FIXED){
			retStat = N_EEPROM_OK;
			__asm("Nop");
		}
	}
	if(eeError!=NULL){
		ee_integrityEEPROM(eeError);
	}
  
	return retStat;
}

/**
 * @brief Retrieves data if data structure matches previous one.
 *
 * @returns EEPROM_OK if previous data structure retrieval is successful.
 */
N_EEPROM_StatusTypeDef ee_eepromPreinit(uint8_t* rxData)
{
	flashStatus_t retStat = FLASH_STATUS_ERROR;

	uint8_t rxData_1[2] = {0};

	/*NOR Flash transaction*/
	retStat = flash_readData(0, &rxData_1[0], 1);
	retStat |= flash_readData(4095, &rxData_1[1], 1);

	if (rxData_1[0] == 0xAA && rxData_1[1] == 0x55){

		/*ECUID*/
		retStat |= flash_readData(256, &rxData[132], 20);

		/*Gain and Offset*/
		retStat |= flash_readData(276, &rxData[216], 8);

		// /*Partition 1*/
		// retStat = flash_readData(4, &rxData[4], 132);

		// retStat |= flash_readData(256, &rxData[132], 20);

		// retStat |= flash_readData(152, &rxData[152], 96);


		// /*Partition 2*/
		// retStat |= flash_readData(1028, &rxData[260], 96);

		// /*Partiton 3*/
		// retStat |= flash_readData(1024, &rxData[516], 4);
		// __asm("nop");
		// retStat |= flash_readData(300, &rxData[520], 56);
		// __asm("nop");
		// retStat |= flash_readData(300, &rxData[612], 56);
		// __asm("nop");

		return N_EEPROM_OK;
	}
	return N_EEPROM_PREINIT_FAILED;
}

/***********************************************************************************************************/
/************************************************DUMMY DEFINES**********************************************/
/***********************************************************************************************************/
N_EEPROM_StatusTypeDef eepromInterface_eepromDeinit(void){
	return N_EEPROM_OK;
}

N_EEPROM_StatusTypeDef eepromInterface_eepromReady(void){
	return N_EEPROM_OK;
}
N_EEPROM_StatusTypeDef eepromInterface_lockStatus(void){
	return N_EEPROM_OK;
}
N_EEPROM_StatusTypeDef eepromInterface_lockEEPROM(void){
	return N_EEPROM_OK;
}

N_EEPROM_StatusTypeDef eepromInterface_unlockEEPROM(void){
	return N_EEPROM_OK;
}

N_EEPROM_StatusTypeDef eepromInterface_eraseEEPROM(void){
	return N_EEPROM_OK;
}
/***********************************************************************************************************/