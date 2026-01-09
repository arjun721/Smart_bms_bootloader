
#ifndef _MCAL_SPI_INTERFACE_H_
#define _MCAL_SPI_INTERFACE_H_

#include "gpio.h"
#include "spi.h"
#include "reset.h"


typedef enum
{
    SpiCh0 = 0,
    SpiCh1 = 1
} SpiChannel;

void 
App_SpiGpioInit(void);

void 
App_Spi1GpioInit(void);

void
App_SpiInit(M0P_SPI_TypeDef* SPIx, SpiChannel spiCh);  


void 
App_SpiAfeInit(void);
 

void 
Spi_ReadJedecID(uint8_t *pData, uint8_t len);

uint8_t 
Spi_ReadStatusRegister(void);

void
Spi_ResetDevice(void);


#endif