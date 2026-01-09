/*
 * flash_interface.c
 *
 *  Created on: 16-Jan-2023
 *      Author: Vecmocon Technology
 */

#include "flash_interface.h"
#include "mcu_interface.h"
/**
 * Transmits data over the SPI bus.
 *
 * @param txBuffer The buffer containing the data to transmit.
 * @param rxBuffer The buffer to store the received data.
 * @param numberOfFrames The number of frames to transmit.
 *
 * @returns The number of frames received.
 */
int8_t flash_sendData(void* txBuffer, uint8_t* rxBuffer, uint32_t numberOfFrames){
	return mcu_spiMasterTransmit(txBuffer, rxBuffer, numberOfFrames);
}

uint32_t flash_msTick(void){
	return GET_TICK();
}
