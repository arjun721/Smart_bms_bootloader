 

#ifndef INC_EEPROM_API_H_
#define INC_EEPROM_API_H_

#include "../inc/eeprom_types.h"

/**
 * @brief Initializes the EEPROM interface.
 *
 * @returns EEPROM_OK if the EEPROM was successfully initialized.
 */
N_EEPROM_StatusTypeDef eepromInterface_eepromInit(void);

/**
 * @brief Deinitializes the EEPROM interface.
 *
 * @returns EEPROM_OK if the EEPROM interface was successfully deinitialized.
 * Otherwise, returns EEPROM_ERROR.
 */
N_EEPROM_StatusTypeDef eepromInterface_eepromDeinit(void);

/**
 * @brief Checks if the EEPROM is ready.
 *
 * @returns EEPROM_OK if the EEPROM is ready, EEPROM_BUSY otherwise.
 */
N_EEPROM_StatusTypeDef eepromInterface_eepromReady(void);

/**
 * @brief Reads a word from the EEPROM.
 *
 * @param address The address to read from.
 * @param pData A pointer to the data to be read.
 *
 * @returns EEPROM_OK if the read was successful.
 */
N_EEPROM_StatusTypeDef eepromInterface_readWord(uint32_t address, uint32_t *pData);

/**
 * @brief Reads a word from the EEPROM in reverse order.
 * @note For legacy purposes
 * 
 * @param address The address of the word to read.
 * @param pData A pointer to the data to be read.
 *
 * @returns A status code indicating the success of the operation.
 */
N_EEPROM_StatusTypeDef eepromInterface_readWordReverse(uint32_t address, uint32_t *pData);

/**
 * @brief Reads a byte from the EEPROM.
 *
 * @param address The address of the byte to read.
 * @param pData A pointer to the data to be read.
 *
 * @returns EEPROM_OK if the read was successful, EEPROM_ERROR otherwise.
 */
N_EEPROM_StatusTypeDef eepromInterface_readByte(uint32_t address, uint8_t *pData);

/**
 * @brief Writes a value to the EEPROM.
 *
 * @param address The address to write to.
 * @param val The value to write.
 * @param size The size of the value to write.
 *
 * @returns The status of the write operation.
 */
N_EEPROM_StatusTypeDef eepromInterface_write(uint32_t address, int32_t val, uint8_t size);

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
N_EEPROM_StatusTypeDef eepromInterface_writeReverse(uint32_t address, int32_t val, uint8_t byteorword);

/**
 * @brief Checks if an address is valid for the EEPROM.
 *
 * @param address The address to check.
 * @param size The size of the data to check.
 *
 * @returns EEPROM_OK if the address is valid, EEPROM_ERROR otherwise.
 */
N_EEPROM_StatusTypeDef eepromInterface_checkAddress(uint32_t address, uint8_t size);

/**
 * @brief This function updates the eeprom in regular interval if specified time has elapsed since last updation
 * 		  and changes are detected
 * 
 * @note To be called periodically, ideally in a fast task/routine
 * 
 * @param time_ms 
 * @return N_EEPROM_StatusTypeDef 
 */
N_EEPROM_StatusTypeDef eepromInterface_updateEEPROM(uint32_t time_ms, bool forceUpdate, bool *eepromError);

/**
 * @brief Returns the lock status of the EEPROM.
 *
 * @returns The lock status of the EEPROM.
 */
N_EEPROM_StatusTypeDef eepromInterface_lockStatus(void);

/**
 * @brief Locks the EEPROM.
 *
 * @returns EEPROM_OK if the EEPROM was successfully locked.
 */
N_EEPROM_StatusTypeDef eepromInterface_lockEEPROM(void);

/**
 * @brief Unlocks the EEPROM.
 *
 * @returns EEPROM_OK if the EEPROM was unlocked successfully.
 */
N_EEPROM_StatusTypeDef eepromInterface_unlockEEPROM(void);
/**
 * @brief Erases the EEPROM.
 *
 * @returns EEPROM_OK if the EEPROM was successfully erased.
 */
N_EEPROM_StatusTypeDef eepromInterface_eraseEEPROM(void);

/**
 * @brief Writes a byte to the EEPROM at the specified address and then reads it and verifies it.
 * 
 * @param address The address to write the byte to.
 * @param val The byte to write.
 *
 * @returns The status of the write operation.
 */
N_EEPROM_StatusTypeDef eepromInterface_writeByteNVerify(uint32_t address, uint8_t val);

/**
 * @brief Retrieves data if data structure matches previous one.
 *
 * @returns EEPROM_OK if previous data structure retrieval is successful.
 */
N_EEPROM_StatusTypeDef ee_eepromPreinit(uint8_t* rxData);

#endif /* INC_EEPROM_API_H_ */
