 

#ifndef PLATFORM_FLASH_INTERFACE_H_
#define PLATFORM_FLASH_INTERFACE_H_

#include <stdint.h>

int8_t flash_sendData(void* txBuffer, uint8_t* rxBuffer, uint32_t numberOfFrames);
uint32_t flash_msTick(void);
#endif /* PLATFORM_FLASH_INTERFACE_H_ */
