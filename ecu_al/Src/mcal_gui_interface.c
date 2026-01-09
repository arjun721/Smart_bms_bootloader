#include "mcal_gui_interface.h"
 #include "mcal_gpio_interface.h"
#include <stdio.h>
#include <string.h>   // for strlen()
#include <stdint.h>   // for uint8_t, int types
//#include "rs485_app.h"
#include "string.h"
#include "stdbool.h"
#include "main.h"
 

uint8_t uart_count;
 
#define RX_BUFFER_SIZE 256
#define BUF_LEN 256

char rxBuffer[RX_BUFFER_SIZE];
volatile uint16_t rxIndex = 0;

volatile uint8_t u8RxData[8];
volatile uint8_t u8RxCnt = 0;

 
volatile uint8_t rxBuf[BUF_LEN];
volatile uint8_t rxCnt = 0;
volatile uint8_t frameReady = 0;
 

bleBmsData_t blebmsData = {0};  // initialize all values to 0
volatile uint8_t enable_flag = 0;
volatile uint8_t rxActive = 0;
uint8_t anyProtectFlag = false;

extern BmsConfig_t bmsCfg;


void UART0_2_IRQHandler(void)
{
    if (Uart_GetStatus(M0P_UART2, UartRC))
    {
        uint8_t b = Uart_ReceiveData(M0P_UART2);
        Uart_ClrStatus(M0P_UART2, UartRC);

        // Ignore CR/LF
        if (b == '\r' || b == '\n')
            return;

        // Start of frame
        if (b == '{')
        {
            rxCnt = 0;
            rxActive = 1;
        }

        if (rxActive && rxCnt < BUF_LEN - 1)
        {
            rxBuf[rxCnt++] = b;
        }

        // End of frame
        if (b == '}' && rxActive)
        {
            rxBuf[rxCnt] = '\0';   // proper null termination
            rxActive = 0;

            char key[20];
            int value;

            if (sscanf((char*)rxBuf, "{%19[^:]:%d}", key, &value) == 2)
            {
                if (strcmp(key, "COP") == 0)      { bmsCfg.Cell_OV_set = value; anyProtectFlag = true; }
                else if (strcmp(key, "COR") == 0) { bmsCfg.Cell_OV_rel = value; anyProtectFlag = true; }
                else if (strcmp(key, "COD") == 0) { bmsCfg.Cell_OV_delay = value; anyProtectFlag = true;}
                else if (strcmp(key, "CUP") == 0) { bmsCfg.Cell_UV_set = value; anyProtectFlag = true; }
                else if (strcmp(key, "CUR") == 0) { bmsCfg.Cell_UV_rel = value; anyProtectFlag = true; }
                else if (strcmp(key, "CUD") == 0) { bmsCfg.Cell_UV_delay = value; anyProtectFlag = true;}
                else if (strcmp(key, "POP") == 0) { bmsCfg.Pack_OV_set = value; anyProtectFlag = true; }
                else if (strcmp(key, "POR") == 0) { bmsCfg.Pack_OV_rel  = value; anyProtectFlag = true; }
                else if (strcmp(key, "POD") == 0) { bmsCfg.Pack_OV_delay  = value; anyProtectFlag = true;}
                else if (strcmp(key, "PUP") == 0) { bmsCfg.Pack_UV_set  = value; anyProtectFlag = true; }
                else if (strcmp(key, "PUR") == 0) { bmsCfg.Pack_UV_rel  = value; anyProtectFlag = true; }
                else if (strcmp(key, "PUD") == 0) { bmsCfg.Pack_UV_delay  = value; anyProtectFlag = true;}
    
                else if (strcmp(key, "CO1") == 0) { bmsCfg.Chg_OT_set = value; anyProtectFlag = true; }
                else if (strcmp(key, "CO2") == 0) { bmsCfg.Chg_OT_rel = value; anyProtectFlag = true; }
                else if (strcmp(key, "CO3") == 0) { bmsCfg.Chg_OT_delay = value; anyProtectFlag = true;}
                else if (strcmp(key, "CU1") == 0) { bmsCfg.Chg_UT_set = value; anyProtectFlag = true; }
                else if (strcmp(key, "CU2") == 0) { bmsCfg.Chg_UT_rel = value; anyProtectFlag = true; }
                else if (strcmp(key, "CU3") == 0) { bmsCfg.Chg_UT_delay = value; anyProtectFlag = true;}
                else if (strcmp(key, "DO1") == 0) { bmsCfg.Dsg_OT_set = value; anyProtectFlag = true; }
                else if (strcmp(key, "DO2") == 0) { bmsCfg.Dsg_OT_rel  = value; anyProtectFlag = true; }
                else if (strcmp(key, "DO3") == 0) { bmsCfg.Dsg_OT_delay  = value; anyProtectFlag = true;}
                else if (strcmp(key, "DU1") == 0) { bmsCfg.Dsg_UT_set  = value; anyProtectFlag = true; }
                else if (strcmp(key, "DU2") == 0) { bmsCfg.Dsg_UT_rel  = value; anyProtectFlag = true; }
                else if (strcmp(key, "DU3") == 0) { bmsCfg.Dsg_UT_delay  = value; anyProtectFlag = true;}
                
               
                else if (strcmp(key, "CCP") == 0) { bmsCfg.Chg_OC_set = value; anyProtectFlag = true; }
                else if (strcmp(key, "CCR") == 0) { bmsCfg.Chg_OC_rel = value; anyProtectFlag = true; }
                else if (strcmp(key, "CCD") == 0) { bmsCfg.Chg_OC_delay = value; anyProtectFlag = true;}
                
                else if (strcmp(key, "DCP") == 0) { bmsCfg.Dsg_OC_set = value; anyProtectFlag = true; }
                else if (strcmp(key, "DCR") == 0) { bmsCfg.Dsg_OC_rel = value; anyProtectFlag = true; }
                else if (strcmp(key, "DCD") == 0) { bmsCfg.Dsg_OC_delay = value; anyProtectFlag = true;}
                
                else if (strcmp(key, "D2P") == 0) { bmsCfg.Dsg_OC2_set = value; anyProtectFlag = true; }
                else if (strcmp(key, "D2D") == 0) { bmsCfg.Dsg_OC2_delay  = value; anyProtectFlag = true; }
                
                else if (strcmp(key, "SCP") == 0) { bmsCfg.SC_set  = value; anyProtectFlag = true;}
                else if (strcmp(key, "SCR") == 0) { bmsCfg.SC_rel  = value; anyProtectFlag = true; }
                else if (strcmp(key, "SCD") == 0) { bmsCfg.SC_delay  = value; anyProtectFlag = true; }
                
                else if (strcmp(key, "BSV") == 0) { bmsCfg.cellOpenVoltage = value; anyProtectFlag = true; }
                else if (strcmp(key, "BDV") == 0) { bmsCfg.balanceDriftVoltage = value; anyProtectFlag = true; }
                else if (strcmp(key, "DCV") == 0) { bmsCfg.LCell_UV_set = value; anyProtectFlag = true;}
                else if (strcmp(key, "DDV") == 0) { bmsCfg.LCell_UV_delay = value; anyProtectFlag = true; }
                
//                else if (strcmp(key, "NC") == 0) { bmsCfg.nominalCapacity  = value; anyProtectFlag = true; }
//                else if (strcmp(key, "CC") == 0) { bmsCfg.cycleCapacity  = value; anyProtectFlag = true;}
                //else if (strcmp(key, "CN") == 0) { bmsCfg.  = value; anyProtectFlag = true; }
                //else if (strcmp(key, "SCD") == 0) { bmsCfg.SC_delay  = value; anyProtectFlag = true; }
                
                else if (strcmp(key, "VOL") == 0) enable_flag = 1; //OK
                else if (strcmp(key, "TMP") == 0) enable_flag = 2; //TMP
                else if (strcmp(key, "UUR") == 0) enable_flag = 8;
                else if (strcmp(key, "ORG") == 0) enable_flag = 4;
                else if (strcmp(key, "ADV") == 0) enable_flag = 5;
                else if (strcmp(key, "LIV") == 0) enable_flag = 9; //OK
            }

            memset(rxBuf, 0, BUF_LEN);
            rxCnt = 0;
        }
    }
}

// void UART0_2_IRQHandler(void)
//{
//	if (Uart_GetStatus(M0P_UART2, UartRC))
//	{
//		uint8_t b = Uart_ReceiveData(M0P_UART2);
//		Uart_ClrStatus(M0P_UART2, UartRC);

//		if (rxCnt < BUF_LEN)
//			rxBuf[rxCnt++] = b;

//		// End of JSON-like frame
//		if (b == '}')
//		{
//			char key[20];
//			int value;

//			rxBuf[11] = '\0';   // ensure null-termination
//      
//			if (sscanf((char*)&rxBuf[1], "{%19[^:]:%d}", key, &value) == 2)
//			{
//				// Map keys to struct directly
//				if      (strcmp(key, "COP") == 0) blebmsData.COP = value;
//				else if (strcmp(key, "COR") == 0) blebmsData.COR = value;
//				else if (strcmp(key, "COD") == 0) blebmsData.COD = value;
//        
//				else if (strcmp(key, "CUP") == 0) blebmsData.CUP = value;
//				else if (strcmp(key, "CUR") == 0) blebmsData.CUR = value;
//				else if (strcmp(key, "CUD") == 0) blebmsData.CUD = value;

//				else if (strcmp(key, "POP")  == 0) blebmsData.POP  = value;
//				else if (strcmp(key, "POR")  == 0) blebmsData.POR  = value;
//				else if (strcmp(key, "POD") == 0) blebmsData.POD = value;
//        
//				else if (strcmp(key, "PUP") == 0) blebmsData.PUP = value;
//				else if (strcmp(key, "PUR") == 0) blebmsData.PUR = value;
//				else if (strcmp(key, "PUD") == 0) blebmsData.PUD = value;
//       else if (strcmp(key, "VOL") == 0)  { enable_flag = 1; }
//       else if (strcmp(key, "LIV") == 0)  { enable_flag = 9; }
//			}

//			// Reset buffer for next frame
//			rxCnt = 0;
//			memset(rxBuf, 0, BUF_LEN);
//		}
//	}
//}



 

#pragma pack(push,1)
typedef struct
{
    uint16_t Cell_OV_set, Cell_OV_rel, Cell_OV_delay;
    uint16_t Cell_UV_set, Cell_UV_rel, Cell_UV_delay;
    uint16_t Pack_OV_set, Pack_OV_rel, Pack_OV_delay;
    uint16_t Pack_UV_set, Pack_UV_rel, Pack_UV_delay;

//    int16_t  Chg_UT_set, Chg_UT_rel;
//    uint16_t Chg_UT_delay;

//    uint16_t Dsg_OT_set, Dsg_OT_rel, Dsg_OT_delay;

//    int16_t  Dsg_UT_set, Dsg_UT_rel;
//    uint16_t Dsg_UT_delay;

//    uint16_t Chg_OT_set, Chg_OT_rel, Chg_OT_delay;
//    uint16_t Chg_OC_set, Chg_OC_rel, Chg_OC_delay;
//    uint16_t Dsg_OC_set, Dsg_OC_rel, Dsg_OC_delay;

//    uint16_t Dsg_OC2_set, Dsg_OC2_rel, Dsg_OC2_delay;
//    uint16_t SC_set, SC_rel, SC_delay;
//    uint16_t HCell_OV_set, HCell_OV_rel, HCell_OV_delay;
//    uint16_t LCell_UV_set, LCell_UV_rel, LCell_UV_delay;

//    uint8_t  load_en;
//    uint8_t  balance_en;
//    uint8_t  chg_balance;
//    uint8_t  ntc1, ntc2, ntc3, ntc4;

//    uint16_t cellOpenVoltage;
//    uint16_t balanceDriftVoltage;
//    uint16_t ipsOffDelay;

//    uint32_t nominalCapacity;
//    uint32_t cycleCapacity;
//    uint16_t fullSetVoltage;
//    uint16_t endOfVoltage;
//    uint8_t  selfDischargeRate;

} BmsConfig_b;
#pragma pack(pop)

BmsConfig_b g_bmsConfig;

 

// ===================== UART ISR =====================
//void UART0_2_IRQHandler(void)
//{
//    if (Uart_GetStatus(M0P_UART2, UartRC))
//    {
//        uint8_t b = Uart_ReceiveData(M0P_UART2);
//        Uart_ClrStatus(M0P_UART2, UartRC);

//        if (rxCnt < BUF_LEN)
//        {
//            rxBuf[rxCnt++] = b;

//            if (rxCnt == CFG_SIZE)
//            {
//                cfgReady = 1; // full struct received
//            }
//        }
//        else
//        {
//            rxCnt = 0; // overflow protection
//        }
//    }
//}


//void UART0_2_IRQHandler(void)
//{
//    if (Uart_GetStatus(M0P_UART2, UartRC))
//    {
//        uint8_t b = Uart_ReceiveData(M0P_UART2);
//        Uart_ClrStatus(M0P_UART2, UartRC);

//        if (rxCnt < 50)
//        {
//          rxBuf[rxCnt++] = b;
//          cfgReady = 1;   // indicate data received
//        }
//        else
//        {
//            rxCnt = 0;
//        }
//    }
//}

//void SendBmsConfig(M0P_UART_TypeDef* UARTx, BmsConfig_b* cfg)
//{
//    App_UartTransmitPoll(UARTx, (uint8_t*)cfg, sizeof(BmsConfig_b));
//}

//volatile uint8_t rxBuf[50];
//volatile uint8_t rxCnt = 0;
//volatile uint8_t cfgReady = 0;

 


//void UART0_2_IRQHandler(void)
//{
//    if (Uart_GetStatus(M0P_UART2, UartRC))
//    {
//        uint8_t b = Uart_ReceiveData(M0P_UART2);
//        Uart_ClrStatus(M0P_UART2, UartRC);

//        if (rxCnt < 5)
//        {
//            rxBuf[rxCnt++] = b;

//            /* Decode only when 2 bytes received */
//            if (rxCnt == 3)
//            {
//                if (rxBuf[1] == 0xAF && rxBuf[2] == 0x06)
//                {
//                    enable_flag = rxBuf[2];   // enable = 1
// 
//                }
//                else if (rxBuf[1] == 0xAB && rxBuf[2] == 0x02)
//                {
//                    enable_flag = rxBuf[2]; 
// 
//                }
//                else if (rxBuf[1] == 0xAC && rxBuf[2] == 0x03)
//                {
//                    enable_flag = rxBuf[2]; 
//                
//                } 
//                else if (rxBuf[1] == 0xAD && rxBuf[2] == 0x04)
//                {
//                    enable_flag = rxBuf[2]; 
//                
//                }
//                else if (rxBuf[1] == 0xAE && rxBuf[2] == 0x05)
//                {
//                    enable_flag = rxBuf[2];
//               
//                }                
//                else
//                {
//                    enable_flag = 0;          // invalid command
//                }

//                cfgReady = 1;                 // command ready

//                /* Clear buffer & counter for next command */
//                memset(rxBuf, 0, sizeof(rxBuf));
//                rxCnt = 0;
//            }
//        }
//        else
//        {
//            rxCnt = 0;   // overflow safety
//        }
//    }
//}

 

#define FRAME_H1 0xAA
#define FRAME_H2 0x55

void SendBmsConfig(M0P_UART_TypeDef *uart, BmsConfig_b *cfg)
{
    uint16_t payload_len = sizeof(BmsConfig_b);
    uint16_t frame_len   = payload_len + 5; // AA 55 + len(2) + crc

    static uint8_t txBuf[200];
    uint16_t idx = 0;
    uint8_t crc = 0;

    txBuf[idx++] = FRAME_H1;
    txBuf[idx++] = FRAME_H2;
    txBuf[idx++] = payload_len & 0xFF;
    txBuf[idx++] = (payload_len >> 8) & 0xFF;

    uint8_t *p = (uint8_t *)cfg;
    for (uint16_t i = 0; i < payload_len; i++) {
        txBuf[idx++] = p[i];
        crc ^= p[i];
    }

    txBuf[idx++] = crc;

    // ?? SEND IN ONE SHOT (CRITICAL)
    App_UartTransmitPoll(uart, txBuf, idx);
}


 
//void fun(void)
//{
//      if (cfgReady)
//        {
//            // Disable interrupts while copying
//            __disable_irq();
// 
//            memcpy(&g_bmsConfig, rxBuf, CFG_SIZE);
//            rxCnt = 0;
//            cfgReady = 0;

//            __enable_irq();
//        }  
//}



void App_UartPortInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    DDL_ZERO_STRUCT(stcGpioCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE); 
    
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(GpioPortA,GpioPin9,&stcGpioCfg);
    Gpio_SetAfMode(GpioPortA,GpioPin9,GpioAf1); //PA09-UART0 TX
  
    stcGpioCfg.enDir = GpioDirIn;
    Gpio_Init(GpioPortA,GpioPin10,&stcGpioCfg);
    Gpio_SetAfMode(GpioPortA,GpioPin10,GpioAf1); //PA10-UART0 RX
}


void App_UartBlePortInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    DDL_ZERO_STRUCT(stcGpioCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE); 
    
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(GpioPortC,GpioPin3,&stcGpioCfg);
    Gpio_SetAfMode(GpioPortC,GpioPin3,GpioAf5); //PC3-UART2 TX
  
    stcGpioCfg.enDir = GpioDirIn;
    Gpio_Init(GpioPortC,GpioPin2,&stcGpioCfg);
    Gpio_SetAfMode(GpioPortC,GpioPin2,GpioAf4); //PC2-UART2 RX
}

void App_UartRfidPortInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    DDL_ZERO_STRUCT(stcGpioCfg);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE); 
    
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(GpioPortA,GpioPin2,&stcGpioCfg);
    Gpio_SetAfMode(GpioPortA,GpioPin2,GpioAf1); //PA02-UART1 TX
  
    stcGpioCfg.enDir = GpioDirIn;
    Gpio_Init(GpioPortA,GpioPin3,&stcGpioCfg);
    Gpio_SetAfMode(GpioPortA,GpioPin3,GpioAf1); //PA3-UART1 RX
}


void App_UartRfidInit(void)
{
    App_UartRfidPortInit();
  
    stc_uart_cfg_t  stcCfg;
    stc_uart_multimode_t stcMulti;
    stc_uart_baud_t stcBaud;

    DDL_ZERO_STRUCT(stcCfg);
    DDL_ZERO_STRUCT(stcMulti);
    DDL_ZERO_STRUCT(stcBaud);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralUart1,TRUE);//UART0 
 
    stcCfg.enRunMode   = UartMskMode1;       // 8-bit UART, variable baud
    stcCfg.enStopBit   = UartMsk1bit;        // 1 stop bit
    stcCfg.enMmdorCk   = UartMskDataOrAddr;  // No parity
    stcCfg.stcBaud.u32Baud   = 9600;
    stcCfg.stcBaud.enClkDiv  = UartMsk8Or16Div;
    stcCfg.stcBaud.u32Pclk   = Sysctrl_GetPClkFreq();
    Uart_Init(M0P_UART1, &stcCfg);       //?????
}

  
void App_UartBleInit(void)
{
    App_UartBlePortInit();
  
    stc_uart_cfg_t  stcCfg;
    stc_uart_multimode_t stcMulti;
    stc_uart_baud_t stcBaud;

    DDL_ZERO_STRUCT(stcCfg);
    DDL_ZERO_STRUCT(stcMulti);
    DDL_ZERO_STRUCT(stcBaud);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralUart2,TRUE);//UART0 
    
    stcCfg.enRunMode   = UartMskMode1;       // 8-bit UART, variable baud
    stcCfg.enStopBit   = UartMsk1bit;        // 1 stop bit
    stcCfg.enMmdorCk   = UartMskDataOrAddr;  // No parity
    stcCfg.stcBaud.u32Baud   = 9600;
    stcCfg.stcBaud.enClkDiv  = UartMsk8Or16Div;
    stcCfg.stcBaud.u32Pclk   = Sysctrl_GetPClkFreq();
    Uart_Init(M0P_UART2, &stcCfg);       //?????

    Uart_ClrStatus(M0P_UART2,UartRC);    //?????
    Uart_ClrStatus(M0P_UART2,UartTC);    //?????
    Uart_EnableIrq(M0P_UART2,UartRxIrq); //????????
    EnableNvic(UART0_2_IRQn, IrqLevel3, TRUE);       ///<??????
}


void App_UartInit(void)
{
    App_UartPortInit(); // for soc meter
  
    stc_uart_cfg_t  stcCfg;
    stc_uart_multimode_t stcMulti;
    stc_uart_baud_t stcBaud;

    DDL_ZERO_STRUCT(stcCfg);
    DDL_ZERO_STRUCT(stcMulti);
    DDL_ZERO_STRUCT(stcBaud);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralUart0,TRUE);//UART0 
    
    stcCfg.enRunMode   = UartMskMode1;       // 8-bit UART, variable baud
    stcCfg.enStopBit   = UartMsk1bit;        // 1 stop bit
    stcCfg.enMmdorCk   = UartMskDataOrAddr;  // No parity
    stcCfg.stcBaud.u32Baud   = 9600;
    stcCfg.stcBaud.enClkDiv  = UartMsk8Or16Div;
    stcCfg.stcBaud.u32Pclk   = Sysctrl_GetPClkFreq();
    Uart_Init(M0P_UART0, &stcCfg);       //?????
}

 

void App_UartTransmitPoll(M0P_UART_TypeDef* UARTx, uint8_t* data, uint16_t length)
{
    for (uint16_t i = 0; i < length; i++)
    {
        Uart_SendDataPoll(UARTx, data[i]);  // send each byte
    }
}


 

 
void Uart_ReceiveBuffer(M0P_UART_TypeDef* UARTx, uint8_t* buf, uint16_t len)
{
    for (uint16_t i = 0; i < len; i++)
    {
              // Wait until RX flag indicates data available
        while (UARTx->ISR_f.RC == 0);

        buf[i] = Uart_ReceiveData(UARTx);

        // Clear RC flag after reading (if required by hardware)
        UARTx->ISR_f.RC = 0;
    }
}



 
//#include "mcal_rs485_interface.h"
//#include "mcal_gui_interface.h"

 
volatile rs485_basicInfo_Struct rs485_basicInfo_obj = {0};
volatile rs485_cellVoltages_Struct rs485_cellInfo_obj = {0};
volatile rs485_SOCFaults_Struct rs485_SOCFaults_obj = {0};

#define READ_BUFF_SIZE 1

volatile master_readRequest_RS485 readRequestBuff[READ_BUFF_SIZE] = {0};
volatile master_writeRequest_RS485 writeRequestBuff[READ_BUFF_SIZE] = {0};

event_received_Struct event_Received = {0};

static bool read_req_received = 0;
static bool write_req_received = 0;

static bool event_received = 0;

// static int8_t read_reqBuff_tail = 0;
// static int8_t read_reqBuff_head = -1;

// static int8_t write_reqBuff_tail = 0;
// static int8_t write_reqBuff_head = -1;

uint32_t total_readAttempts = 0;
uint32_t reqReceived = 0;

int32_t l_tickNow = 0;

/*
uint32_t basicReqServed = 0;
uint32_t voltageReqServed = 0;
uint32_t hwReqServed = 0;
uint32_t totalReqServed = 0;
*/

/***********************************Static Declarations****************************/
  uint16_t rs485_getChecksum(uint8_t *ptr, uint8_t len);
static RS485_STAT_t rs485_send_basicInfo(void);
static RS485_STAT_t rs485_send_cellInfo(void);
static RS485_STAT_t rs485_send_hwVesrion(uint8_t * hwVersion, uint8_t len);
static void hextoASCII(uint8_t *hex, char* ascii, uint8_t* len);
/**********************************************************************************/

void revMemcpy(void * destination, void * source, uint32_t length);

void revMemcpy(void * destination, void * source, uint32_t length) {
    char *charDest = (char *) destination;
    char *charSrc = (char *) source;
    for (int ijk = 0; ijk < (int) length; ijk++)
        charDest[ijk] = charSrc[(length - 1) - ijk];
}


/**********************************************Static Definitions*************************************/

/** 
 * @brief Calculates the checksum for the given data packet. 
 * @param ptr Pointer to the data packet. 
 * @param len Length of the data packet. 
 * @return The calculated checksum. 
 */
  uint16_t rs485_getChecksum(uint8_t *ptr, uint8_t len)
{
	int8_t i;
	uint16_t checksum = 0;

	for(i=0; i<len; i++)
		checksum += ptr[i];

	__asm("nop");
	checksum = (0xffff & ~checksum) + 1;

	return(checksum);
}

	// uint8_t checksumDataBuff[2 + sizeof(rs485_basicInfo_Struct)] = {0};
	// checksumDataBuff[0] = sendObj.status;
	// checksumDataBuff[1] = sendObj.data_length;
	// memcpy(&checksumDataBuff[2], &sendObj.data_buff[0], sizeof(rs485_basicInfo_Struct));

	// uint16_t tmpChecksum = rs485_getChecksum(checksumDataBuff, sizeof(checksumDataBuff));


//  RS485_STAT_t rs485_send_basicInfo(void)
//    {
//	rs485_send_basicInfo_Struct sendObj;
//	sendObj.startByte = RS485_START_BYTE;
//	sendObj.command_code = BASIC_INFO_CMD;
//	sendObj.status = RS485_STATUS_OK;
//      
//     
//	sendObj.data_length = (uint8_t)(sizeof(rs485_basicInfo_Struct));
//      rs485_basicInfo_Struct filldata;
//      
//      filldata.balanceStatus_0
//      
//	memcpy(&sendObj.data_buff[0], (void *)&rs485_basicInfo_obj, sizeof(rs485_basicInfo_Struct));

//	uint16_t tmpChecksum = rs485_getChecksum(&sendObj.status, (2 + sendObj.data_length));

//	revMemcpy(&sendObj.checksum, &tmpChecksum, sizeof(sendObj.checksum));

//	sendObj.endByte = RS485_END_BYTE;

// 
//   App_UartTransmitPoll(M0P_UART3, (uint8_t *)(&sendObj), sizeof(rs485_send_basicInfo_Struct));
// 
//	return 0;
//}
    
   
extern struct oz3717_chip oz3717_data_y;
extern bool pack_uv_flag;
extern bool cell_ov_flag;
extern bool cell_uv_flag;
extern bool pack_ov_flag;
extern bool pack_uv_flag;
extern bool chg_ot_flag;
extern bool chg_ut_flag;
extern bool dsg_ot_flag;
extern bool dsg_ut_flag;
extern bool chg_oc_flag;
extern bool dsg_oc_flag;
extern bool sc_flag;
extern bool afe_error_flag;
extern bool soft_lock_flag;
extern bool hg_mos_damage_flag;
extern bool mos_ot_flag;
extern bool sg_mos_damage_flag;   // reserved
 
 RS485_STAT_t rs485_send_basicInfo(void)
{
     
  
    rs485_send_basicInfo_Struct sendObj;

    sendObj.startByte    = RS485_START_BYTE;
    sendObj.command_code = BASIC_INFO_CMD;
    sendObj.status       = RS485_STATUS_OK;

    sendObj.data_length = (uint8_t)(sizeof(rs485_basicInfo_Struct));

    // Fill default data
 
    rs485_basicInfo_Struct filldata = {
        .packVoltage_10mV        = 0,
        .packCurrent_10mA        = 0,
        .remainingCapacity_10mAh = 50000,
        .nominalCapacity_10mAh   = 50000,
        .cycles                  = 10,
        .productionDate          = 0x2024,

        .balanceStatus_0 = {0},
        .balanceStatus_1 = {0},

//chg_oc_flag
        .protectionStatus = {1,dsg_oc_flag,sc_flag,afe_error_flag,soft_lock_flag,hg_mos_damage_flag,mos_ot_flag,
        sg_mos_damage_flag,cell_ov_flag,cell_uv_flag,pack_ov_flag,pack_uv_flag,chg_ot_flag,chg_ut_flag,dsg_ot_flag,dsg_ut_flag},

        .softwareVersion = 1,
        .remainingSOC    = 50,
        .fetStatus       = {1, 1},

        .cellsInSeries   = 16,
        .ntcNumber       = 4,
        .ntcReading      = {250, 250, 250, 250, 0, 0}
    };

    // Write to active data object
    rs485_basicInfo_obj = filldata;

    // Copy into send buffer
    memcpy(&sendObj.data_buff[0], &rs485_basicInfo_obj, sizeof(rs485_basicInfo_Struct));

    // Calculate checksum
    uint16_t tmpChecksum = rs485_getChecksum(&sendObj.status, (2 + sendObj.data_length));
    revMemcpy(&sendObj.checksum, &tmpChecksum, sizeof(sendObj.checksum));

    sendObj.endByte = RS485_END_BYTE;

    App_UartTransmitPoll(M0P_UART3, (uint8_t *)(&sendObj), sizeof(rs485_send_basicInfo_Struct));

    return RS485_STATUS_OK;
}


	//RS485_STAT_t retVal = RS485_ERROR;
	//retVal = (RS485_STAT_t)App_TransmitBuffer((uint8_t *)(&sendObj), sizeof(rs485_send_basicInfo_Struct));
	//RS485_STAT_t retVal = RS485_ERROR;
	//retVal = (RS485_STAT_t)App_TransmitBuffer((uint8_t *)(&sendObj), sizeof(rs485_send_basicInfo_Struct));


 

//static RS485_STAT_t rs485_send_cellInfo(void){
//  
//	rs485_send_cellVoltages_Struct sendObj;
//  
//	sendObj.startByte = RS485_START_BYTE;
//	sendObj.command_code = CELL_VOLTAGES_CMD;
//	sendObj.status = RS485_STATUS_OK;
//	sendObj.data_length = (uint8_t)(sizeof(rs485_cellVoltages_Struct));
//	memcpy(&sendObj.data_buff[0], (void *)&rs485_cellInfo_obj, (uint8_t)sizeof(rs485_cellVoltages_Struct));

//	uint8_t checksumDataBuff[2 + sizeof(rs485_cellVoltages_Struct)] = {0};
//	checksumDataBuff[0] = sendObj.status;
//	checksumDataBuff[1] = sendObj.data_length;
//	memcpy(&checksumDataBuff[2], &sendObj.data_buff[0], (uint8_t)sizeof(sendObj.data_buff));

//	uint16_t tmpChecksum = rs485_getChecksum(checksumDataBuff, sizeof(checksumDataBuff));
//	revMemcpy(&sendObj.checksum, &tmpChecksum, sizeof(sendObj.checksum));

//	sendObj.endByte = RS485_END_BYTE;

//  
//   App_UartTransmitPoll(M0P_UART3, (uint8_t *)(&sendObj), sizeof(rs485_send_basicInfo_Struct));

////	RS485_STAT_t retVal = RS485_ERROR;
////	retVal = (RS485_STAT_t)App_TransmitBuffer((uint8_t *)(&sendObj), sizeof(rs485_send_cellVoltages_Struct));
//	return 0;
//}



 

static RS485_STAT_t rs485_send_cellInfo(void)
{
    rs485_send_cellVoltages_Struct sendObj;

    sendObj.startByte = RS485_START_BYTE;
    sendObj.command_code = CELL_VOLTAGES_CMD;
    sendObj.status = RS485_STATUS_OK;

    sendObj.data_length = (uint8_t)(sizeof(rs485_cellVoltages_Struct));

  
  rs485_cellVoltages_Struct rs485_cellInfo_obj = {
    .rs485_cellVoltages = {
        3500, 3501, 3498, 3502, 3499, 3500, 3497, 3503,
        3501, 3500, 3499, 3502, 3498, 3501, 3500, 3499
    },
};
    // Copy all 16 cell voltages
    memcpy(sendObj.data_buff,
           &rs485_cellInfo_obj,
           sizeof(rs485_cellVoltages_Struct));

    // --------------------------------------------
    // Prepare checksum buffer = status + length + data
    // --------------------------------------------
    uint8_t checksumDataBuff[2 + sizeof(rs485_cellVoltages_Struct)];

    checksumDataBuff[0] = sendObj.status;
    checksumDataBuff[1] = sendObj.data_length;

    memcpy(&checksumDataBuff[2],
           sendObj.data_buff,
           sizeof(rs485_cellVoltages_Struct));

    // --------------------------------------------
    // Calculate checksum
    // --------------------------------------------
    uint16_t tmpChecksum =
        rs485_getChecksum(checksumDataBuff,
                          sizeof(checksumDataBuff));

    revMemcpy(&sendObj.checksum,
              &tmpChecksum,
              sizeof(sendObj.checksum));

    sendObj.endByte = RS485_END_BYTE;

    // --------------------------------------------
    // Transmit entire object
    // --------------------------------------------
    App_UartTransmitPoll(M0P_UART3,
                         (uint8_t *)&sendObj,
                         sizeof(rs485_send_cellVoltages_Struct));

    return RS485_STATUS_OK;
}


static RS485_STAT_t rs485_send_hwVesrion(uint8_t * hwVersion, uint8_t len){
	uint8_t dataBuff[38];
	memset(dataBuff, 0, sizeof(dataBuff));
	
	uint8_t hwVersion_length = (len > 31? 31 :len);

	dataBuff[0] = RS485_START_BYTE;
	dataBuff[1] = HW_VERSION;
	dataBuff[2] = RS485_OK;
	dataBuff[3] = hwVersion_length;
	memcpy(&dataBuff[4], (uint8_t *)hwVersion, hwVersion_length);


	uint16_t tmpChecksum = rs485_getChecksum(&dataBuff[2], 2 + hwVersion_length);
	revMemcpy(&dataBuff[4 + hwVersion_length], &tmpChecksum, sizeof(tmpChecksum));

	dataBuff[4 + hwVersion_length + (uint8_t)sizeof(tmpChecksum)] = RS485_END_BYTE;

	RS485_STAT_t retVal = RS485_ERROR;
	retVal = (RS485_STAT_t)App_TransmitBufferv(&dataBuff[0], sizeof(dataBuff));
	// hwReqServed++;
	return retVal;
}

static RS485_STAT_t rs485_send_SOCFaults(void)
{
	rs485_send_SOCFaults_Struct sendObj;

	sendObj.startByte = RS485_START_BYTE;
	sendObj.command_code = SOC_FAULT_CMD;
	sendObj.status = RS485_STATUS_OK;
	sendObj.data_length = ((uint8_t)sizeof(rs485_SOCFaults_Struct));
	memcpy(&sendObj.data_buff[0], (void *)&rs485_SOCFaults_obj, sizeof(rs485_SOCFaults_Struct));

	uint16_t tmpChecksum = rs485_getChecksum(&sendObj.status, (2 + sendObj.data_length));

	revMemcpy(&sendObj.checksum, &tmpChecksum, sizeof(sendObj.checksum));

	sendObj.endByte = RS485_END_BYTE;

	RS485_STAT_t retVal = RS485_ERROR;
	retVal = (RS485_STAT_t)App_TransmitBufferv((uint8_t *)(&sendObj), sizeof(rs485_send_SOCFaults_Struct));

	return retVal;
}
/********************************************Static Definitions End****************************************/


RS485_STAT_t rs485_init(){
	 //App_Rs485UartCfg();
	return 0;
}


RS485_STAT_t rs485_serve_requests(uint8_t * serialNumber, uint8_t length, uint8_t * dsgStatus, uint8_t * chgStatus, uint8_t * deviceStatus){

	char convertedSerialNumber[17];
	// totalReqServed = basicReqServed + voltageReqServed + hwReqServed;

	RS485_STAT_t retVal = RS485_IDLE;

	uint16_t calcChecksum = 0;

	if(event_received == 1){
		switch(event_Received.event_code){
			case EVENT_HEARTBEAT:
				calcChecksum = rs485_getChecksum((uint8_t *)(&event_Received.event_code), 4*sizeof(uint8_t));
				if(calcChecksum == event_Received.checksum){
					if(event_Received.data[0] == 0){
						hextoASCII(serialNumber, convertedSerialNumber,&length);
						retVal = rs485_send_hwVesrion(convertedSerialNumber, length);
					}
				}
				*deviceStatus = HEARTBEAT_RECV;
				break;
			default:
				break;
		}

		event_received = 0;
	}

	if(read_req_received == 1){
		calcChecksum = rs485_getChecksum((uint8_t *)(&readRequestBuff[0].command_code), 2*sizeof(uint8_t));
		if(calcChecksum == readRequestBuff[0].checksum){
			*deviceStatus = VALID_DATA_RECV;
			RS485_CMD_t command_recv = readRequestBuff[0].command_code;
			switch(command_recv){
				case BASIC_INFO_CMD:
					retVal = rs485_send_basicInfo();
					break;
				case CELL_VOLTAGES_CMD:
					retVal = rs485_send_cellInfo();
					break;
				case HW_VERSION:
					retVal = rs485_send_hwVesrion(serialNumber, length);
					break;
				default:
					*deviceStatus = INVALID_DATA_RECV;
					retVal = RS485_UNKNOWN_CMD;
					break;
			}
		}
		else{
			*deviceStatus = INVALID_DATA_RECV;
			retVal = RS485_CHKSUM_FAIL;
		}
		read_req_received = 0;
	}

	if(write_req_received == 1){
		calcChecksum = rs485_getChecksum((uint8_t *)(&writeRequestBuff[0].command_code), 4*sizeof(uint8_t));

		if(calcChecksum == writeRequestBuff[0].checksum){
			*deviceStatus = VALID_DATA_RECV;
			RS485_CMD_t command_recv = writeRequestBuff[0].command_code;
			if(FET_CONOTROL_CMD == command_recv){
				switch(writeRequestBuff[0].data[1]){
					case RS485_FETS_ON:
						*dsgStatus = ON;
						*chgStatus = ON;
						break;
					case RS485_CHG_OFF:
						*dsgStatus = ON;
						*chgStatus = OFF;
						break;
					case RS485_DSG_OFF:
						*dsgStatus = OFF;
						*chgStatus = ON;
						break;
					case RS485_FETS_OFF:
						*dsgStatus = OFF;
						*chgStatus = OFF;
						break;
					default:
						*deviceStatus = INVALID_DATA_RECV;
						break;
				}
			}
			else{
				*deviceStatus = INVALID_DATA_RECV;
				retVal = RS485_UNKNOWN_CMD;
			}
		}
		else{
			*deviceStatus = INVALID_DATA_RECV;
			retVal = RS485_CHKSUM_FAIL;
		}
		write_req_received = 0;
	}
 
#if BLE == 1	
	if(GET_TICK() - l_tickNow > 0)
	{
		if((GET_TICK() - l_tickNow >= 1000))
		{
			retVal = rs485_send_SOCFaults();
			l_tickNow = GET_TICK();
		}
	}
#endif
	return retVal;
}

uint32_t u32currTick = 0;
uint8_t u8SetCurrTick = 0;
RS485_STAT_t rs485_recv_data(void)
{
	uint8_t data_buffer[25] = {0};
	int8_t buff_length = 0;
	volatile uint8_t Buffsize = 0;
	if(4 >= READ_REQ_LENGTH){
		//Buffsize = rs485_uart_available_Data();
		if(u8SetCurrTick == 0)
        {
            u32currTick = 0;
            u8SetCurrTick = 1;
        }
        else
        {
            if((0 - u32currTick) > 23)
            {
				u32currTick = 0;
                u8SetCurrTick = 0;
				total_readAttempts++;
				//buff_length = rs485_uart_ReceiveData(data_buffer, Buffsize);
				if((data_buffer[0] == 0xB5) && (data_buffer[1] == 0x63)&& (buff_length > 14))
				{
				//	CantoUart_CopyDatatoQueue(data_buffer, buff_length);
				}
				else if(buff_length >= WRITE_REQ_LENGTH){
				//	rs485_reset_buffer();
				}
				else if(data_buffer[1] == RS485_READ_STATUS && buff_length >= READ_REQ_LENGTH){
				//	rs485_reset_buffer();
				}
				else{
					buff_length = 0;
				}
			}
		}
	}
	if(buff_length > 0){
		__asm("nop");
		if(data_buffer[0] == RS485_START_BYTE){
			switch(data_buffer[1]){
				case RS485_READ_STATUS:
						memcpy((master_readRequest_RS485 *)&readRequestBuff[0], &data_buffer[0], sizeof(master_readRequest_RS485));
						revMemcpy((uint8_t *)(&readRequestBuff[0].checksum), &data_buffer[4], sizeof(uint16_t));
						read_req_received = 1;
						reqReceived++;
					break;
				case RS485_WRITE_STATUS:
						memcpy((master_writeRequest_RS485 *)&writeRequestBuff[0], &data_buffer[0], sizeof(master_writeRequest_RS485));
						revMemcpy((uint8_t *)(&writeRequestBuff[0].checksum), &data_buffer[6], sizeof(uint16_t));
						write_req_received = 1;
					break;
				case UART_EVENT:
						memcpy((event_received_Struct *)&event_Received, &data_buffer[0], sizeof(event_received_Struct));
						revMemcpy((uint8_t *)(&event_Received.checksum), &data_buffer[6], sizeof(uint16_t));
						event_received = 1;
					break;
				default:
					break;
			}
		}
	}
	// vframe data handling
	//CantoUart_recv_data();
}

static void hextoASCII(uint8_t *hex, char* ascii, uint8_t* len){
	uint8_t cnt = 0;
	for(char i=0;i<8;i++){
		if (hex[i]>9){
			//nibble 2
			if (((hex[i]>>4)&0xF)==0xA){
				ascii[cnt] = 'A';
				cnt++;
			}
			else if (((hex[i]>>4)&0xF)==0xB){
				ascii[cnt] = 'B';
				cnt++;
			}
			else if (((hex[i]>>4)&0xF)==0xC){
				ascii[cnt] = 'C';
				cnt++;
			}
			else if (((hex[i]>>4)&0xF)==0xD){
				ascii[cnt] = 'D';
				cnt++;
			}
			else if (((hex[i]>>4)&0xF)==0xE){
				ascii[cnt] = 'E';
				cnt++;
			}
			else if (((hex[i]>>4)&0xF)==0xF){
				ascii[cnt] = 'F';
				cnt++;
			}
			else{
				ascii[cnt] = ((hex[i]>>4)&0x0F) + 48;
				cnt++;
			}

			//nibble 1
			if ((hex[i]&0xF)==0xA){
				ascii[cnt] = 'A';
				cnt++;
			}
			else if ((hex[i]&0xF)==0xB){
				ascii[cnt] = 'B';
				cnt++;
			}
			else if ((hex[i]&0xF)==0xC){
				ascii[cnt] = 'C';
				cnt++;
			}
			else if ((hex[i]&0xF)==0xD){
				ascii[cnt] = 'D';
				cnt++;
			}
			else if ((hex[i]&0xF)==0xE){
				ascii[cnt] = 'E';
				cnt++;
			}
			else if ((hex[i]&0xF)==0xF){
				ascii[cnt] = 'F';
				cnt++;
			}
			else{
				ascii[cnt] = (hex[i]&0x0F) + 48;
				cnt++;
			}

		}
		else{
			ascii[cnt] = ((hex[i]>>4)&0xF)+48;
			cnt++;
			
			ascii[cnt] = (hex[i]&0xF)+48;
			cnt++;
		}
	}
	*len=16;
	ascii[cnt] = 0;
	__asm("nop");
}

 void send_data_s(void)
 {
   
   rs485_send_cellInfo();
  // rs485_send_basicInfo();
 
 }

 
 uint8_t App_TransmitBufferv(uint8_t *buf, uint16_t len)
 {
    //App_BmsRs485TxMode();

    for(uint16_t i = 0; i < len; i++)
    {
        Uart_SendDataPoll(M0P_UART3, buf[i]);
        while(Uart_GetStatus(M0P_UART3, UartTC) == FALSE);
    }

  // App_BmsRs485RxMode();
    return 0;
}
 

 


 

 
 