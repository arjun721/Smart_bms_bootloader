

#include "ais156_logging_interface.h"
#include "nor_flash.h"
 
/**
 * Erases a sector of the flash.
 *
 * @param flash The flash partition structure.
 * @param address The address of the sector to erase.
 *
 * @returns None
 */
int op_sector_erase(struct ringfs_flash_partition *flash, int address)
{
    (void) flash;
    flashSectorErase(address);
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
signed int op_program(struct ringfs_flash_partition *flash, int address, const void *data, size_t size)
{
    (void) flash;
    flash_programPage(address, (uint8_t *)data, size);
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
signed int op_read(struct ringfs_flash_partition *flash, int address, void *data, size_t size)
{
    (void) flash;
    flash_readData(address, data, size);
    return size;
}


/**
 * Transmits a CAN message.
 *
 * @param canID The CAN ID of the message.
 * @param dataBuff The data buffer of the message.
 * @param length The length of the data buffer.
 *
 * @returns 0 on success, -1 on failure.
 */
int ais156_canInterface(uint32_t canID, uint8_t * dataBuff, uint8_t length)
{
	return 0;//mcu_fdcan1Transmit(canID, dataBuff, length);
}

uint8_t ais156_getBmsStatus(void)
{
    return 0;//bms_dataAccess.bms_logicState.batteryMode;
}

