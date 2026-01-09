#include "mcal_spi_interface.h"

extern uint32_t app_getTick(void);

void 
App_SpiGpioInit(void)  
{
    stc_gpio_cfg_t GpioInitStruct;
    DDL_ZERO_STRUCT(GpioInitStruct);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
 
    GpioInitStruct.enDrv = GpioDrvH;
    GpioInitStruct.enDir = GpioDirOut;   

    Gpio_Init(GpioPortA, GpioPin4, &GpioInitStruct);
    Gpio_SetAfMode(GpioPortA, GpioPin4, GpioAf1);             ///<??SPI0_CS
                                                               
    Gpio_Init(GpioPortA, GpioPin5, &GpioInitStruct);            
    Gpio_SetAfMode(GpioPortA, GpioPin5, GpioAf1);           ///<??SPI0_SCK
    
    GpioInitStruct.enDir = GpioDirOut;    
    Gpio_Init(GpioPortA, GpioPin7, &GpioInitStruct);           
    Gpio_SetAfMode(GpioPortA, GpioPin7, GpioAf1);         ///<??SPI0_MOSI
    
    GpioInitStruct.enDir = GpioDirIn;   
    Gpio_Init(GpioPortA, GpioPin6, &GpioInitStruct);            
    Gpio_SetAfMode(GpioPortA, GpioPin6, GpioAf1);         
}

void 
App_Spi1GpioInit(void)  
{
    stc_gpio_cfg_t GpioInitStruct;
    DDL_ZERO_STRUCT(GpioInitStruct);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);
 
    GpioInitStruct.enDrv = GpioDrvH;
    GpioInitStruct.enDir = GpioDirOut;   

    Gpio_Init(GpioPortB, GpioPin12, &GpioInitStruct);
    Gpio_SetAfMode(GpioPortB, GpioPin12, GpioAf1);             //SPI1_CS
                                                               
    Gpio_Init(GpioPortB, GpioPin13, &GpioInitStruct);            
    Gpio_SetAfMode(GpioPortB, GpioPin13, GpioAf1);             //SPI0_SCK
    
    GpioInitStruct.enDir = GpioDirOut;    
    Gpio_Init(GpioPortB, GpioPin15, &GpioInitStruct);           
    Gpio_SetAfMode(GpioPortB, GpioPin15, GpioAf1);             //SPI0_MOSI
    
    GpioInitStruct.enDir = GpioDirIn;   
    Gpio_Init(GpioPortB, GpioPin14, &GpioInitStruct);            
    Gpio_SetAfMode(GpioPortB, GpioPin14, GpioAf1);         
}


void
App_SpiInit(M0P_SPI_TypeDef* SPIx, SpiChannel spiCh)
{
    if (spiCh == SpiCh0)
    {
      App_SpiGpioInit();
    } 
    else if (spiCh == SpiCh1)
    {
       App_Spi1GpioInit();
    }
    
    stc_spi_cfg_t SpiInitStruct;
    DDL_ZERO_STRUCT(SpiInitStruct);

    Sysctrl_SetPeripheralGate(spiCh == SpiCh0 ? SysctrlPeripheralSpi0 : SysctrlPeripheralSpi1, TRUE);
    Reset_RstPeripheral0(spiCh == SpiCh0 ? ResetMskSpi0 : ResetMskSpi1);

    SpiInitStruct.enSpiMode = SpiMskMaster;
    SpiInitStruct.enPclkDiv = SpiClkMskDiv32;  //--> 500kbps
    
    if(spiCh == SpiCh0)         // MODE - 0
    {
      SpiInitStruct.enCPHA    = SpiMskCphafirst;
      SpiInitStruct.enCPOL    = SpiMskcpollow;
    }
    else if (spiCh == SpiCh1)   // MODE -3 
    {
      SpiInitStruct.enCPHA    = SpiMskCphasecond;
      SpiInitStruct.enCPOL    = SpiMskcpolhigh;
    }
    Spi_Init(SPIx, &SpiInitStruct);
}


void
Spi_ReadJedecID(uint8_t *pData, uint8_t len)
{
    if (pData == NULL || len == 0)
        return;

    Spi_SetCS(M0P_SPI0, FALSE);               // Pull CS low
    Spi_RWByte(M0P_SPI0, 0x9F);               // Send JEDEC ID command (0x9F)

    for (uint8_t i = 0; i < len; i++)
    {
        pData[i] = Spi_RWByte(M0P_SPI0, 0x00);  // Read response byte
    }
    Spi_SetCS(M0P_SPI0, TRUE);                // Pull CS high
}

uint8_t 
Spi_ReadStatusRegister(void)
{
    uint8_t status;
    Spi_SetCS(M0P_SPI0, FALSE);
    Spi_RWByte(M0P_SPI0, 0x05);           // Read Status Register command
    status = Spi_RWByte(M0P_SPI0, 0x00);  // Dummy byte to receive response
    Spi_SetCS(M0P_SPI0, TRUE);
    return status;
}

void 
Spi_ResetDevice(void)
{
    Spi_SetCS(M0P_SPI0, FALSE);
    Spi_RWByte(M0P_SPI0, 0x66); // Enable Reset command
    Spi_SetCS(M0P_SPI0, TRUE);

    Spi_SetCS(M0P_SPI0, FALSE);
    Spi_RWByte(M0P_SPI0, 0x99); // Reset Device command
    Spi_SetCS(M0P_SPI0, TRUE);
}


void App_SpiAfeInit(void)
{
    App_SpiInit(M0P_SPI1, 1);  // Provide required arguments here
}


void App_SpiNorInit(void)
{
    App_SpiInit(M0P_SPI0, 0);  // Provide required arguments here
}


int8_t flash_sendData(void* txBuffer, uint8_t* rxBuffer, uint32_t numberOfFrames) {
    if(numberOfFrames == 0) return -1;

    uint8_t* txBuf = (uint8_t*)txBuffer;

    // Activate CS (low)
    Spi_SetCS(M0P_SPI0, FALSE); // Assuming SPI0, adjust as needed

    // Transmit & receive
    for(uint32_t i = 0; i < numberOfFrames; i++) {
        uint8_t dataToSend = txBuf ? txBuf[i] : 0xFF;  // Send 0xFF if txBuffer is NULL
        uint8_t received = Spi_RWByte(M0P_SPI0, dataToSend);  // send & read simultaneously
        if(rxBuffer) rxBuffer[i] = received;
    }
    // Deactivate CS (high)
    Spi_SetCS(M0P_SPI0, TRUE);
 
    return 0;
}

uint32_t flash_msTick(void){
	return app_getTick(); //GET_TICK();xTaskGetTickCount();
}
 