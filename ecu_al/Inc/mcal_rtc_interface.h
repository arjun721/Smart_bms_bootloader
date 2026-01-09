#ifndef _MCAL_RTC_INTERFACE_H_
#define _MCAL_RTC_INTERFACE_H_

#include <math.h>
#include <stdio.h>
#include "gpio.h"
#include "reset.h"
#include "rtc.h"

uint8_t BCD2Dec(uint8_t bcd);
uint8_t Dec2BCD(uint8_t dec);

void RTC_Cfg(void);

























 
//void 
//App_xPortInit(void);

//void I2C_Delay(void);
// 
//void I2C_Start(void)  ;

//void I2C_Stop(void)  ;

//void I2C_SendByte(uint8_t byte);  

//uint8_t I2C_ReadByte(uint8_t ack);  

//uint8_t I2C_WaitAck(void);  

//uint8_t ISL1208_ReadStatusRegister(void);  


//void ISL1208_WriteStatusRegister(uint8_t data);
// 

//void ISL1208_ReadTime(uint8_t *raw_time);
// 

//uint8_t bcdToDec(uint8_t val);  
// 


#endif