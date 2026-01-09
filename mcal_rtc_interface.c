#include "mcal_rtc_interface.h"

uint8_t raw_time[7];
uint8_t decoded_time[7]; 


#define DS3231_ADDRESS 0xDE

#define SDA_PIN GpioPin10
#define SDA_PORT GpioPortA
#define SCL_PIN GpioPin9
#define SCL_PORT GpioPortA

//#define BMS_RTC_SCL_PORT   GpioPortB
//#define BMS_RTC_SCL_PIN    GpioPin3
//#define BMS_RTC_SDA_PORT   GpioPortB
//#define BMS_RTC_SDA_PIN    GpioPin4


#define SDA_HIGH() Gpio_WriteOutputIO(SDA_PORT, SDA_PIN, 1)
#define SDA_LOW()  Gpio_WriteOutputIO(SDA_PORT, SDA_PIN, 0)
#define SCL_HIGH() Gpio_WriteOutputIO(SCL_PORT, SCL_PIN, 1)
#define SCL_LOW()  Gpio_WriteOutputIO(SCL_PORT, SCL_PIN, 0)
#define SDA_READ() Gpio_GetInputIO(SDA_PORT, SDA_PIN)

#define SDA_INPUT()  { stc_gpio_cfg_t cfg;\
                      cfg.enDir = GpioDirIn;\
                      cfg.enPu = GpioPuDisable;\
                      cfg.enPd = GpioPdDisable;\
                      Gpio_Init(SDA_PORT, SDA_PIN, &cfg);}

#define SDA_OUTPUT() { stc_gpio_cfg_t cfg;\
                      cfg.enDir = GpioDirOut;\
                      cfg.enPu = GpioPuDisable;\
                      cfg.enPd = GpioPdDisable;\
                      cfg.enOD = GpioOdEnable;\
                      Gpio_Init(SDA_PORT, SDA_PIN, &cfg);}


void I2C_Delay(void)
{
    for (volatile int i = 0; i < 40; i++) { // 100
        __NOP(); // Each NOP takes 1 cycle ˜ 125 ns at 8 MHz
    }
}
void I2C_Start(void) {
    SDA_OUTPUT();
    SDA_HIGH();
    SCL_HIGH();
    I2C_Delay();
    SDA_LOW();
    I2C_Delay();
    SCL_LOW();
}

void I2C_Stop(void) {
    SDA_OUTPUT();
    SCL_LOW();
    SDA_LOW();
    I2C_Delay();
    SCL_HIGH();
    I2C_Delay();
    SDA_HIGH();
    I2C_Delay();
}

void I2C_SendByte(uint8_t byte) {
    SDA_OUTPUT();
    for (int i = 0; i < 8; i++) {
        if (byte & 0x80) SDA_HIGH(); else SDA_LOW();
        I2C_Delay();
        SCL_HIGH();
        I2C_Delay();
        SCL_LOW();
        I2C_Delay();
        byte <<= 1;
    }
}

uint8_t I2C_ReadByte(uint8_t ack) {
    uint8_t byte = 0;
    SDA_INPUT();
    for (int i = 0; i < 8; i++) {
        SCL_HIGH();
        I2C_Delay();
        byte <<= 1;
        if (SDA_READ()) byte |= 0x01;
        SCL_LOW();
        I2C_Delay();
    }
    SDA_OUTPUT();
    if (ack) SDA_LOW(); else SDA_HIGH();
    I2C_Delay();
    SCL_HIGH();
    I2C_Delay();
    SCL_LOW();
    SDA_HIGH();
    return byte;
}

uint8_t I2C_WaitAck(void) {
    SDA_INPUT();
    I2C_Delay();
    SCL_HIGH();
    I2C_Delay();
    uint8_t ack = SDA_READ();
    SCL_LOW();
    SDA_OUTPUT();
    return ack;
}

uint8_t ISL1208_ReadStatusRegister(void) {
    uint8_t status = 0;
    I2C_Start();
    I2C_SendByte(0xDE);  // ISL1208 Write
    if (I2C_WaitAck()) return 0xFF;

    I2C_SendByte(0x07);  // Status register address
    if (I2C_WaitAck()) return 0xFF;

    I2C_Start();
    I2C_SendByte(0xDF);  // ISL1208 Read
    if (I2C_WaitAck()) return 0xFF;

    status = I2C_ReadByte(0);
    I2C_Stop();
    return status;
}


void ISL1208_WriteStatusRegister(uint8_t data)
{
    I2C_Start();
    I2C_SendByte(0xDE);        // ISL1208 Write address
    if (I2C_WaitAck()) goto stop;

    I2C_SendByte(0x07);        // Status Register
    if (I2C_WaitAck()) goto stop;

    I2C_SendByte(data);        // Data: WRTC | PAE (e.g., 0x11)
    if (I2C_WaitAck()) goto stop;

stop:
    I2C_Stop();
}


void ISL1208_ReadTime(uint8_t *raw_time)
{
    I2C_Start();
    I2C_SendByte(0xDE);        // ISL1208 Write address
    if (I2C_WaitAck()) goto stop;

    I2C_SendByte(0x00);        // Start from Seconds register
    if (I2C_WaitAck()) goto stop;

    I2C_Start();               // Repeated start
    I2C_SendByte(0xDF);        // ISL1208 Read address
    if (I2C_WaitAck()) goto stop;

    for (int i = 0; i < 6; i++) {
        raw_time[i] = I2C_ReadByte(1);  // Send ACK after each byte
    }
    raw_time[6] = I2C_ReadByte(0);      // Last byte (Year), send NACK

stop:
    I2C_Stop();
}


uint8_t bcdToDec(uint8_t val) {
    return (val >> 4) * 10 + (val & 0x0F);
}


void App_xPortInit(void)
{
    stc_gpio_cfg_t cfg;
    DDL_ZERO_STRUCT(cfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio, TRUE); // Enable GPIO peripheral
    
    cfg.enDir = GpioDirOut;
    cfg.enPu = GpioPuDisable;
    cfg.enOD = GpioOdEnable;       // Open-drain for I2C
    cfg.enPd = GpioPdDisable;
    Gpio_Init(SDA_PORT, SDA_PIN|SCL_PIN, &cfg);  

   SDA_HIGH();
   SCL_HIGH();  
}


 