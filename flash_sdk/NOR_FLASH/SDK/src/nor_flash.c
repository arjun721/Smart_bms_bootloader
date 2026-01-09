 
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "nor_flash.h"
#include "mcal_spi_interface.h"

#define NOR_TIMEOUT		50
#define ERASE_TIMEOUT		400
#define BMS_CONFIG_ADDRESS   0x000000  // Starting address for test
  

static flashStatus_t flash_wEnable(void);
static flashStatus_t flash_wDisable(void);
static flashStatus_t flash_readStatusReg1(flash_statusReg_Struct* statusReg);
static flashStatus_t flash_writeSetup(void);
static flashStatus_t flash_readWIP(bool* WIP);
static flashStatus_t flash_writeToPage(uint8_t* txBuff, uint8_t* rxBuff, uint16_t length);
//static flashStatus_t flash_eraseCommands(uint32_t address, flash_eraseCmds command);

 BmsParameters_t readCfg;
/**
 * Send write enable command to NOR Flash
 *
 * @returns A status code indicating the result of the operation.
 */
static flashStatus_t flash_wEnable(void){
	uint8_t rxBuff[1] = {0};
	uint8_t txBuff[1] = {0};
	txBuff[0] = WriteEnable;
	flashStatus_t retStat = FLASH_STATUS_ERROR;
	retStat = flash_sendData(txBuff, rxBuff, 1);

	/*Variable to check Write in progress bit*/
	bool wip = 0;

	/*Do while loop as WIP is to be read at-least once*/
		uint32_t startTime = flash_msTick();
		do{
			/*Read WIP bit*/
			retStat = flash_readWIP(&wip);
			if(retStat != FLASH_STATUS_OK){
				break;
			}
			if((flash_msTick() - startTime) > NOR_TIMEOUT){
				__asm("nop");
				retStat = FLASH_STATUS_WRITE_BLOCKED;
				break;
			}
		}while(wip != 0 );
		
	__asm("nop");
	return retStat;
}

/**
 * Send write disable command to NOR Flash.
 *
 * @returns A status code indicating the result of the operation.
 */
static flashStatus_t flash_wDisable(void){
	uint8_t rxBuff[1] = {0};
	uint8_t txBuff[1] = {0};
	txBuff[0] = WriteDisable;
	flashStatus_t retStat = FLASH_STATUS_ERROR;
	retStat = flash_sendData(txBuff, rxBuff, 1);
	return retStat;
}

/**
 * Reads the status register 1 of the flash.
 *
 * @returns The status register 1 of the flash.
 */
static flashStatus_t flash_readStatusReg1(flash_statusReg_Struct* statusReg){
	uint8_t rxBuff[2] = {0};
	uint8_t txBuff[2] = {0};

	txBuff[0] = STAT_REG_1;
	flashStatus_t retStat = FLASH_STATUS_ERROR;
	retStat = flash_sendData(txBuff, rxBuff, 2);
	__asm("nop");
	if(retStat == FLASH_STATUS_OK){
		memcpy(statusReg, &rxBuff[1], sizeof(uint8_t));
	}
	return retStat;
}

/**
 * Setup the NOR Flash for a write/erase.
 *
 * @returns A status code indicating the success of the function call.
 */
static flashStatus_t flash_writeSetup(void){
	/*Temporary  structure for status register*/
	flash_statusReg_Struct tmp_flash_statusRegister  = {0};

	/*Variable for transaction status check*/
	flashStatus_t retStat = FLASH_STATUS_ERROR;

	/*Send write enable command*/
	retStat = flash_wEnable();

	/*If all transactions are successful*/
	if(retStat == FLASH_STATUS_OK){
		/*Read status register 1, as status register 2 is not of importance here*/
		retStat = flash_readStatusReg1(&tmp_flash_statusRegister);
		if(retStat == FLASH_STATUS_OK){
			if(tmp_flash_statusRegister.WEL == 1)	/*Write Enable Latch Bit is set i.e write is allowed*/
				return FLASH_STATUS_WRITE_ALLOWED;
			else
				return FLASH_STATUS_WRITE_BLOCKED;
		}
	}
	return retStat;
}

/**
 * Read the Write in progress bit of status register 1
 *
 * @param statusRegister The status register of the flash.
 *
 * @returns The status of the operation.
 */
static flashStatus_t flash_readWIP(bool* WIP){
	/*Temporary  structure for status register*/
	flash_statusReg_Struct tmp_flash_statusRegister  = {0};

	/*Variable for transaction status check*/
	flashStatus_t retStat = FLASH_STATUS_ERROR;

	/*Read status register 1, as status register 2 is not of importance here*/
	retStat = flash_readStatusReg1(&tmp_flash_statusRegister);

	/*If all transactions are successful*/
	if(retStat == FLASH_STATUS_OK){
		*WIP = (bool)tmp_flash_statusRegister.WIP;
	}
	return retStat;
}


/**
 * Writes data to the flash memory.
 *
 * @param txBuff The buffer containing the data to be written.
 * @param rxBuff The buffer to receive the data.
 * @param length The number of bytes to be written.
 *
 * @returns The status of the write operation.
 */
static flashStatus_t flash_writeToPage(uint8_t* txBuff, uint8_t* rxBuff, uint16_t length){
	/*Variable for transaction status*/
	flashStatus_t retStat = FLASH_STATUS_ERROR;

	/*setup write before Page Erase*/
	retStat = flash_writeSetup();

	if(retStat == FLASH_STATUS_WRITE_ALLOWED){
		__asm("nop");
		retStat = flash_sendData(txBuff, rxBuff, length);
		if(retStat != FLASH_STATUS_OK){
			__asm("nop");
			return retStat;
		}

		/*Variable to check Write in progress bit*/
		bool wip = 0;

		/*Do while loop as WIP is to be read at-least once*/
		uint32_t startTime = flash_msTick();
		do{
			/*Read WIP bit*/
			retStat = flash_readWIP(&wip);
			if(retStat != FLASH_STATUS_OK){
				break;
			}
			if((flash_msTick() - startTime) > NOR_TIMEOUT){
				__asm("nop");
				retStat = FLASH_STATUS_WRITE_BLOCKED;
				break;
			}
		}while(wip != 0);

	}
	return retStat;
}


/*----------------------------------------------------------------------------------------------------*/
/*________________________________________________API_________________________________________________*/
/*----------------------------------------------------------------------------------------------------*/

/**
 * Erases the flash memory.
 *
 * @param address The address to start erasing.
 * @param command The erase command to use.
 *
 * @returns The status of the erase operation.
 */
flashStatus_t flash_eraseCommands(uint32_t address, flash_eraseCmds command){
	/*Variable for transaction status*/
	flashStatus_t retStat = FLASH_STATUS_ERROR;

	static volatile uint32_t startTime = 0;
	static volatile uint32_t timeNow = 0;

	/*setup write before Page Erase*/
	retStat = flash_writeSetup();

	/*If write is allowed*/
	if(retStat == FLASH_STATUS_WRITE_ALLOWED){
		uint8_t txBuff[4] = {0};
		uint8_t rxBuff[4] = {0};

		/*Fill the Tx buffer*/
		txBuff[0] = command;				/*Erase Command*/
		txBuff[1] = (address >> 16) & 0xFF;	/*MSB of address*/
		txBuff[2] = (address >> 8) & 0xFF;	/*Mid part of address*/
		txBuff[3] = (address >> 0) & 0xFF;	/*LSB of address*/
		__asm("nop");
		/*SPI Transaction*/
		retStat = flash_sendData(txBuff, rxBuff, 4);
		if(retStat != FLASH_STATUS_OK){
			__asm("nop");
			return retStat;
		}

		/*Variable to check Write in progress bit*/
		bool wip = 0;

		startTime =  flash_msTick();

		do{
			/*Read WIP bit*/
			retStat = flash_readWIP(&wip);
			if(retStat != FLASH_STATUS_OK){
				break;
			}
			timeNow = flash_msTick();
			if((timeNow - startTime) > ERASE_TIMEOUT){

				__asm("nop");
				retStat = FLASH_STATUS_WRITE_BLOCKED;
				break;
			}
		}while(wip != 0 );
	}

	timeNow++;
	startTime++;

	__asm("nop");

	return retStat;
}

/********************************************************************************************************************/
/**
 * Reads the unique ID of the flash chip.
 *
 * @param uID The unique ID of the flash chip.
 *
 * @returns The status of the operation.
 */
flashStatus_t flash_getUID(uint8_t* uID){
	uint8_t rxBuff[21] = {0};
	uint8_t txBuff[21] = {0};
	txBuff[0] = UID;

	flashStatus_t retStat = FLASH_STATUS_ERROR;
	retStat = flash_sendData(txBuff, rxBuff, 21);
	__asm("nop");
	if(retStat == FLASH_STATUS_OK){
		memcpy(uID, &rxBuff[5], 16*sizeof(uint8_t));
	}
	return retStat;
}

/**
 * Gets the manufacturer ID and device ID of the flash.
 *
 * @param mID The manufacturer ID.
 * @param dID The device ID.
 *
 * @returns The status of the operation.
 */
flashStatus_t flash_getMID(uint8_t* mID, uint8_t* dID){
	uint8_t rxBuff[6] = {0};
	uint8_t txBuff[6] = {0};

	txBuff[0] = DEVICE_ID;
	txBuff[1] = 0xFF;
	txBuff[2] = 0xFF;
	txBuff[3] = 0;

	flashStatus_t retStat = FLASH_STATUS_ERROR;
	retStat = flash_sendData(txBuff, rxBuff, 6);
	if(retStat == FLASH_STATUS_OK){
		memcpy(mID, &rxBuff[4], sizeof(uint8_t));
		memcpy(dID, &rxBuff[5], sizeof(uint8_t));
	}
	return retStat;
}

/**
 * Read the Manufacturer ID, and ID of the flash (???)
 *
 * @param mID The manufacturer ID.
 * @param id The device ID.
 *
 * @returns The status of the operation.
 */
flashStatus_t flash_getRID(uint8_t* mID, uint16_t* id){

	#define l_BUFF_SIZE 4

	uint8_t rxBuff[l_BUFF_SIZE] = {0};
	uint8_t txBuff[l_BUFF_SIZE] = {0};

	txBuff[0] = READ_ID;

	flashStatus_t retStat = FLASH_STATUS_ERROR;
	retStat = flash_sendData(txBuff, rxBuff, l_BUFF_SIZE);
	if(retStat == FLASH_STATUS_OK){
		memcpy(mID, &rxBuff[1], sizeof(uint8_t));
		uint8_t tempBuff[2] = {0};
		for (int ijk = 0; ijk < (l_BUFF_SIZE-2); ijk++){
			memcpy((tempBuff + ijk), &rxBuff[(l_BUFF_SIZE - 1) - ijk], 1);
		}
		memcpy(id, tempBuff, sizeof(tempBuff));
	}

	#undef l_BUFF_SIZE

	return retStat;
}


/**
 * Writes data to a page of flash.
 *
 * @param address The address of the page to write to.
 * @param data The data to write to the page.
 * @param length The length of the data to write.
 *
 * @returns The status of the write operation.
 */
flashStatus_t flash_programPage(uint32_t address, const uint8_t *data, uint16_t length){
	/*Variable for transaction status*/
	flashStatus_t retStat = FLASH_STATUS_ERROR;

	if(length > MAX_RW_ALLOWED){
		return FLASH_STATUS_ERROR;
	}

	uint8_t txBuff[MAX_RW_ALLOWED + 4] = {0};
	// uint8_t rxBuff[MAX_RW_ALLOWED + 4] = {0};

	uint32_t pageStart = PAGE_ADDRESS(address);
	uint32_t pageEnd = pageStart + 0xFF;

	if((address + length) <= pageEnd+1)
	{

		/*Fill the Tx buffer*/
		txBuff[0] = PageProgram;					/*Page program command*/
		txBuff[1] = (address >> 16) & 0xFF;			/*MSB of address*/
		txBuff[2] = (address >> 8) & 0xFF;			/*Mid part of address*/
		txBuff[3] = (address >> 0) & 0xFF;			/*LSB of address*/
		memcpy(&txBuff[4], data, length);			/*Data bytes to program*/

		/*SPI Transaction*/
		__asm("nop");
		retStat = flash_writeToPage(txBuff, NULL, length + 4);
	}
	else
	{
		__asm("nop");
		uint8_t breakLength = (pageEnd - address) + 1;
		/*Fill the Tx buffer*/
		txBuff[0] = PageProgram;						/*Page program command*/
		txBuff[1] = (address >> 16) & 0xFF;			/*MSB of address*/
		txBuff[2] = (address >> 8) & 0xFF;			/*Mid part of address*/
		txBuff[3] = (address >> 0) & 0xFF;			/*LSB of address*/
		memcpy(&txBuff[4], data, breakLength);			/*Data bytes to program*/

		/*SPI Transaction*/
		__asm("nop");
		retStat = flash_writeToPage(txBuff, NULL, breakLength + 4);

		/*Fill the Tx buffer*/
		txBuff[0] = PageProgram;												/*Page program command*/
		txBuff[1] = ((pageEnd+1) >> 16) & 0xFF;										/*MSB of address*/
		txBuff[2] = ((pageEnd+1) >> 8) & 0xFF;										/*Mid part of address*/
		txBuff[3] = ((pageEnd+1) >> 0) & 0xFF;										/*LSB of address*/
		memcpy(&txBuff[4], (uint8_t*)&data[breakLength], length - breakLength);			/*Data bytes to program*/

		/*SPI Transaction*/
		__asm("nop");
		retStat |= flash_writeToPage(txBuff, NULL, (length - breakLength) + 4);

	}

	if(retStat != FLASH_STATUS_OK){
		__asm("nop");
	}
	return retStat;
}

/**
 * Reads data from the flash.
 *
 * @param address The address to read from.
 * @param data The data read from the flash.
 * @param length The length of the data to read.
 *
 * @returns The status of the read operation.
 */
flashStatus_t flash_readData(uint32_t address, uint8_t *data, uint16_t length){
	/*Variable for transaction status*/
	flashStatus_t retStat = FLASH_STATUS_ERROR;

	if(length > MAX_RW_ALLOWED){
		return FLASH_STATUS_ERROR;
	}

	/*Variable to check Write in progress bit*/
	bool wip = 0;

	/*Do while loop as WIP is to be read at-least once*/
		uint32_t startTime = flash_msTick();
		do{
			/*Read WIP bit*/
			retStat = flash_readWIP(&wip);
			if(retStat != FLASH_STATUS_OK){
				break;
			}
			if((flash_msTick() - startTime) > NOR_TIMEOUT){
				retStat = FLASH_BUSY_IN_OPERATION;
				break;
			}
		}while(wip != 0 );
		
	__asm("nop");

	uint8_t txBuff[MAX_RW_ALLOWED + 4] = {0};
	uint8_t rxBuff[MAX_RW_ALLOWED + 4] = {0};

	/*Fill the Tx buffer*/
	txBuff[0] = ReadData;
	txBuff[1] = (address >> 16) & 0xFF;
	txBuff[2] = (address >> 8 ) & 0xFF;
	txBuff[3] = (address >> 0 ) & 0xFF;

	retStat = flash_sendData(txBuff, rxBuff, length+4);
	__asm("nop");
	memcpy(data, &rxBuff[4], length);
	return retStat;
}



flashStatus_t flash_writeBmsParameters(const BmsParameters_t *params)
{
    flashStatus_t status;

    // Erase sector first
    status = flashSectorErase(BMS_CONFIG_ADDRESS);
    if (status != FLASH_STATUS_OK)
        return status;

    // Program structure
    return flash_programPage(BMS_CONFIG_ADDRESS, (uint8_t*)params, sizeof(BmsParameters_t));
}

flashStatus_t flash_readBmsParameters(BmsParameters_t *params)
{
    return flash_readData(BMS_CONFIG_ADDRESS, (uint8_t*)params, sizeof(BmsParameters_t));
}


//for loding the perivous th from nor
extern uint16_t l_cell_under_volt;
extern uint16_t l_cell_over_volt;
extern uint16_t l_pack_overVolt;
extern uint16_t l_pack_underVolt;
extern uint16_t cellundervoltagerecovery;
extern uint16_t cellovervoltagerecovery;
extern uint32_t packOverVoltRecovery;
extern uint32_t packUnderVoltRecovery;

extern uint16_t  l_cell_balance_start;
extern uint16_t  l_cell_balance_delta;
extern uint16_t  l_cell_balance_diff;

extern uint32_t l_doc2;
extern uint32_t l_disChgCurrent; // 65 A
extern uint32_t l_ChgCurrent;    // 40 A

extern int16_t  l_ntc_under_temp;
extern uint16_t l_max_chg_temp;
extern uint16_t l_max_dis_temp;
extern uint16_t l_max_chg_temp_recovery;
extern uint16_t l_max_dis_temp_recovery;
extern void Send_All_Config_OverCAN(void);
extern BmsParameters_t writeCfg;

void flash_testBmsParameters(void)
{
   // printf("Reading BMS parameters from Flash...\n");
     flash_readBmsParameters(&readCfg);
     writeCfg = readCfg;
  
     l_cell_over_volt = readCfg.BasicParameterConfig.Cell_OV_P.set;
     cellovervoltagerecovery =  readCfg.BasicParameterConfig.Cell_OV_P.release;
    
     l_cell_under_volt =  readCfg.BasicParameterConfig.Cell_UV_P.set;
     cellundervoltagerecovery =  readCfg.BasicParameterConfig.Cell_UV_P.release;
     
     l_pack_overVolt = readCfg.BasicParameterConfig.Pack_OV_P.set;
     packOverVoltRecovery = readCfg.BasicParameterConfig.Pack_OV_P.release;
     
     l_pack_underVolt =  readCfg.BasicParameterConfig.Pack_UV_P.set;
     packUnderVoltRecovery = readCfg.BasicParameterConfig.Pack_UV_P.release;
  
     l_cell_balance_start  =  readCfg.BalanceConfig.CellOpenVoltage;
     l_cell_balance_delta   =  readCfg.BalanceConfig.BalanceDriftVoltage;
     l_cell_balance_diff   =  readCfg.BalanceConfig.IPS_Off_Delay;
  
     l_ntc_under_temp  = readCfg.BasicParameterConfig.Chg_UT_P.set * 100;
     l_max_chg_temp= readCfg.BasicParameterConfig.Chg_OT_P.set* 100;
     l_max_dis_temp= readCfg.BasicParameterConfig.Dsg_OT_P.set* 100;
     
     l_max_chg_temp_recovery= ((readCfg.BasicParameterConfig.Chg_OT_P.release* 100) - 500); 
     l_max_dis_temp_recovery= ((readCfg.BasicParameterConfig.Dsg_OT_P.release* 100) - 500);
     
     l_doc2= readCfg.BasicParameterConfig.Dsg_OC_P.release* 1000;
     l_disChgCurrent= readCfg.BasicParameterConfig.Dsg_OC_P.set* 1000;
     l_ChgCurrent= readCfg.BasicParameterConfig.Dsg_OC_P.set* 1000;
     
}


#define FLASH_TEST_ADDRESS   0x000000  // Starting address for test
    uint8_t writeData[100];
    uint8_t readData[100];


void flash_testWriteRead(void) {
    flashStatus_t status;
 

    // 1?? Fill test data
    for(uint8_t i = 0; i < 100; i++) {
        writeData[i] = i;   // Example pattern 0..99
    }

    // 2?? Erase the page containing our address
    status = flashPageErase(FLASH_TEST_ADDRESS);
    if(status != FLASH_STATUS_OK) {
        
        return;
    }

    // 3?? Program the 100 bytes
    status = flash_programPage(FLASH_TEST_ADDRESS, writeData, 100);
    if(status != FLASH_STATUS_OK) {
        
        return;
    }

    // 4?? Read back the 100 bytes
    memset(readData, 0, sizeof(readData));
    status = flash_readData(FLASH_TEST_ADDRESS, readData, 100);

}