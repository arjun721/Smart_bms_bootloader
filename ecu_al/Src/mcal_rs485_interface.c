#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>   // for atoi() and atol()
#include <string.h>   // for strncmp()
#include "mcal_rs485_interface.h"
#include "mcal_gpio_interface.h"
#include "mcal_rtc_interface.h"
 
#define RX_BUFFER_SIZE 180
 

uint8_t rxBuxf[180];
uint16_t rxCntx = 0;
uint8_t rxBufcc[128];
uint8_t rxCntd = 0;

uint8_t g_sec;
uint8_t g_min;
uint8_t g_hour;
uint8_t g_date;
uint8_t g_dow;
uint8_t g_month;
uint8_t g_year;

 
 
typedef struct {
    uint16_t cu;   // cell under
    uint16_t co;   // cell over
    uint32_t pu;   // pack under
    uint32_t po;   // pack over
    uint16_t cur;  // cell under recovery
    uint16_t cor;  // cell over recovery
    uint32_t pur;  // pack under recovery
    uint32_t por;  // pack over recovery
    uint16_t ut;
} VoltageData_t;

VoltageData_t voltageData;


volatile uint8_t rs485_rx_buf[180];
volatile uint16_t rs485_rx_cnt = 0;
volatile uint8_t rs485_frame_ready = 0;
volatile uint8_t frame_started = 0;
volatile uint8_t rs485_frame_started = 0;

volatile uint8_t rs485_rx_buf1[180];
volatile uint16_t rs485_rx_cnt1 = 0;
volatile uint8_t rs485_frame_ready1 = 0;
volatile uint8_t frame_started1 = 0;
volatile uint8_t rs485_frame_started1 = 0;

volatile uint8_t rs485_rx_buf2[180];
volatile uint16_t rs485_rx_cnt2 = 0;
volatile uint8_t rs485_frame_ready2 = 0;
volatile uint8_t frame_started2 = 0;
volatile uint8_t rs485_frame_started2 = 0;

volatile uint8_t rs485_rx_buf3[180];
volatile uint16_t rs485_rx_cnt3 = 0;
volatile uint8_t rs485_frame_ready3 = 0;
volatile uint8_t frame_started3 = 0;
volatile uint8_t rs485_frame_started3 = 0;

volatile uint8_t rs485_rx_buf4[180];
volatile uint16_t rs485_rx_cnt4 = 0;
volatile uint8_t rs485_frame_ready4 = 0;
volatile uint8_t frame_started4 = 0;
volatile uint8_t rs485_frame_started4 = 0;

volatile uint8_t rs485_rx_buf5[180];
volatile uint16_t rs485_rx_cnt5 = 0;
volatile uint8_t rs485_frame_ready5 = 0;
volatile uint8_t frame_started5 = 0;
volatile uint8_t rs485_frame_started5 = 0;
uint16_t all_log;
uint16_t static_calibration_flag=0;
uint16_t chg_calibration_flag=0;
uint16_t dsg_calibration_flag=0;


typedef struct
{
    int16_t offset;        // ADC zero offset
    float   gain_chg;      // gain for charging current
    float   gain_dsg;      // gain for discharging current
} current_cal_t;
typedef struct
{
    uint8_t  cell_no;     // 1 to 16
    uint16_t voltage_mv;  // cell voltage in mV
    uint8_t ntc_no;
    uint16_t temp_c;
} rs485_cell_data_t;

rs485_cell_data_t cell_data;
current_cal_t cur_cal;

int16_t measured_chg_current,measured_dsg_current;
static void rtc_config_byuser(void);
 


void UART1_3_IRQHandler(void)
{
    if (Uart_GetStatus(M0P_UART3, UartRC))
    {
        uint8_t b = Uart_ReceiveData(M0P_UART3);
        Uart_ClrStatus(M0P_UART3, UartRC);

        // ---------------------------------------------------
        // FOR Cell voltage Info
        // ---------------------------------------------------
        if (b == 0xAA)
        {
            rs485_frame_started = 1;
            rs485_rx_cnt = 0;
            for (uint16_t i = 0; i < RX_BUFFER_SIZE; i++) rs485_rx_buf[i] = 0;
        }
 
        if (rs485_frame_started)
        {
            if (rs485_rx_cnt < RX_BUFFER_SIZE)
            {
                rs485_rx_buf[rs485_rx_cnt++] = b;
            }
        }
        if (b == 0x55 && rs485_frame_started)
        {
            rs485_frame_ready = 1;   // tell main loop
            rs485_frame_started = 0; // stop receiving
        }
   
        
        // ---------------------------------------------------
        // for the basic info
        // ---------------------------------------------------   
        if (b == 0xBB)
        {
            rs485_frame_started1 = 1;
            rs485_rx_cnt1 = 0;
            for (uint16_t i = 0; i < RX_BUFFER_SIZE; i++) rs485_rx_buf1[i] = 0;
        }
 
        if (rs485_frame_started1)
        {
            if (rs485_rx_cnt1 < RX_BUFFER_SIZE)
            {
                rs485_rx_buf1[rs485_rx_cnt1++] = b;
            }
        }
        if (b == 0x66 && rs485_frame_started1)
        {
            rs485_frame_ready1 = 1;   // tell main loop
            rs485_frame_started1 = 0; // stop receiving
        }
        
        
        // ---------------------------------------------------
        // for  calibration info
        // ---------------------------------------------------   
        if (b == 0xCC)
        {
            rs485_frame_started2 = 1;
            rs485_rx_cnt2 = 0;
            for (uint16_t i = 0; i < RX_BUFFER_SIZE; i++) rs485_rx_buf2[i] = 0;
        }
 
        if (rs485_frame_started2)
        {
            if (rs485_rx_cnt2 < RX_BUFFER_SIZE)
            {
                rs485_rx_buf2[rs485_rx_cnt2++] = b;
            }
        }
        // here for static , charge , discharge  current caribration
        if (b == 0x77 && rs485_frame_started2)
        {
            rs485_frame_ready2 = 1;   // tell main loop
            rs485_frame_started2 = 0; // stop receiving
            static_calibration_flag  = 1;
        }
        
        if (b == 0x78 && rs485_frame_started2)
        {
            measured_chg_current  = (uint16_t)rs485_rx_buf2[4] |((uint16_t)rs485_rx_buf2[5] << 8);
            rs485_frame_ready2 = 1;   // tell main loop
            rs485_frame_started2 = 0; // stop receiving
            chg_calibration_flag = 1;
        }
        
        if (b == 0x79 && rs485_frame_started2)
        {
            measured_dsg_current   = (uint16_t)rs485_rx_buf2[4] |((uint16_t)rs485_rx_buf2[5] << 8);
            rs485_frame_ready2 = 1;   // tell main loop
            rs485_frame_started2 = 0; // stop receiving
            dsg_calibration_flag = 1;
        }
        
       if (b == 0x80 && rs485_frame_started2) // for cell and cell_no
        {
            cell_data.cell_no = rs485_rx_buf2[4] | ((uint16_t)rs485_rx_buf2[5] << 8);
            cell_data.voltage_mv = rs485_rx_buf2[6] | ((uint16_t)rs485_rx_buf2[7] << 8);
          
            rs485_frame_ready2 = 1;   // tell main loop
            rs485_frame_started2 = 0; // stop receiving
        }
        
        if (b == 0x81 && rs485_frame_started2) // for cell and cell_no
        {
            cell_data.ntc_no = rs485_rx_buf2[4] | ((uint16_t)rs485_rx_buf2[5] << 8);
            cell_data.temp_c = rs485_rx_buf2[6] | ((uint16_t)rs485_rx_buf2[7] << 8);
          
            rs485_frame_ready2 = 1;   // tell main loop
            rs485_frame_started2 = 0; // stop receiving
        }
        
       if (b == 0x82 && rs485_frame_started2) // for cell and cell_no
        {
          
            //rs485_frame_ready2 = 1;   // tell main loop
            /* Decode from RS485 buffer */
            g_sec   = rs485_rx_buf2[3];
            g_min   = rs485_rx_buf2[4];
            g_hour  = rs485_rx_buf2[6];
            g_date  = rs485_rx_buf2[8];
            g_dow   = rs485_rx_buf2[10];
            g_month = rs485_rx_buf2[12];
            g_year  = rs485_rx_buf2[14];
            rtc_config_byuser();
            rs485_frame_started2 = 0; // stop receiving
        }
        
        
        // ---------------------------------------------------
        // End of  calibration info
        // --------------------------------------------------- 
        
        // ---------------------------------------------------
        // for  WRITE PARAMETER
        // ---------------------------------------------------   
        if (b == 0xFD)
        {
            rs485_frame_started3 = 1;
            rs485_rx_cnt3 = 0;
            for (uint16_t i = 0; i < RX_BUFFER_SIZE; i++) rs485_rx_buf3[i] = 0;
        }
 
        if (rs485_frame_started3)
        {
            if (rs485_rx_cnt3 < RX_BUFFER_SIZE)
            {
                rs485_rx_buf3[rs485_rx_cnt3++] = b;
            }
        }
        if (b == 0x99 && rs485_frame_started3)
        {
            rs485_frame_ready3 = 1;   // tell main loop
            rs485_frame_started3 = 0; // stop receiving
        }  
        
        // ---------------------------------------------------
        // for  READ PARAMETER
        // ---------------------------------------------------   
        if (b == 0xFE)
        {
            rs485_frame_started4 = 1;
            rs485_rx_cnt4 = 0;
            for (uint16_t i = 0; i < RX_BUFFER_SIZE; i++) rs485_rx_buf4[i] = 0;
        }
 
        if (rs485_frame_started4)
        {
            if (rs485_rx_cnt4 < RX_BUFFER_SIZE)
            {
                rs485_rx_buf4[rs485_rx_cnt4++] = b;
            }
        }
        if (b == 0x98 && rs485_frame_started4)
        {
            rs485_frame_ready4 = 1;   // tell main loop
            rs485_frame_started4 = 0; // stop receiving
        }  
        
        
        
        // ---------------------------------------------------
        // for  log of nor flash
        // ---------------------------------------------------   
        if (b == 0xDD)
        {
            rs485_frame_started5 = 1;
            rs485_rx_cnt5 = 0;
            for (uint16_t i = 0; i < RX_BUFFER_SIZE; i++) rs485_rx_buf5[i] = 0;
        }
 
        if (rs485_frame_started5)
        {
            if (rs485_rx_cnt5 < RX_BUFFER_SIZE)
            {
                rs485_rx_buf4[rs485_rx_cnt5++] = b;
            }
        }
        if (b == 0x88 && rs485_frame_started5)
        {
            all_log = (uint16_t)rs485_rx_buf4[4] |
               ((uint16_t)rs485_rx_buf4[5] << 8);
            all_log = all_log *2;
         
            rs485_frame_ready5 = 1;   // tell main loop
            rs485_frame_started5 = 0; // stop receiving
        }
    }
}
  
void App_Rs485PortInit(void)
{
    stc_gpio_cfg_t stcGpioCfg;
    
    DDL_ZERO_STRUCT(stcGpioCfg); 
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);   
    
    stcGpioCfg.enDir = GpioDirOut;
    Gpio_Init(BMS_RS485_TX_PORT,BMS_RS485_TX_PIN,&stcGpioCfg);
    Gpio_SetAfMode(BMS_RS485_TX_PORT,BMS_RS485_TX_PIN,GpioAf6);              
    stcGpioCfg.enDir = GpioDirIn;
    Gpio_Init(BMS_RS485_RX_PORT,BMS_RS485_RX_PIN,&stcGpioCfg);
    Gpio_SetAfMode(BMS_RS485_RX_PORT,BMS_RS485_RX_PIN,GpioAf6);             
}

void App_Rs485UartCfg(void)
{
    App_Rs485PortInit();   // Initialize TX/RX pins, DE/RE pins
    
    stc_uart_cfg_t  stcCfg;
    stc_uart_multimode_t stcMulti;
    stc_uart_baud_t stcBaud;

    DDL_ZERO_STRUCT(stcCfg);
    DDL_ZERO_STRUCT(stcMulti);
    DDL_ZERO_STRUCT(stcBaud);
    
    Sysctrl_SetPeripheralGate(SysctrlPeripheralUart3, TRUE); // enable UART3 clock
 
    stcCfg.enRunMode   = UartMskMode1;       // 8-bit UART, variable baud
    stcCfg.enStopBit   = UartMsk1bit;        // 1 stop bit
    stcCfg.enMmdorCk   = UartMskDataOrAddr;  // No parity
    stcCfg.stcBaud.u32Baud   = 9600;       // UART baud rate
    stcCfg.stcBaud.enClkDiv  = UartMsk8Or16Div;
    stcCfg.stcBaud.u32Pclk   = Sysctrl_GetPClkFreq();
    Uart_Init(M0P_UART3, &stcCfg); 

    // clear status flags
    Uart_ClrStatus(M0P_UART3,UartRC);     
    Uart_EnableIrq(M0P_UART3,UartRxIrq);  
    EnableNvic(UART1_3_IRQn, IrqLevel3, TRUE); 
}


void App_TransmitDataPoll(uint8_t u8data)
{
App_BmsRs485TxMode();                
Uart_SendDataPoll(M0P_UART3, 10);   // send decimal 23 (0x17)
//App_BmsRs485RxMode();

}


uint8_t App_TransmitBuffer(uint8_t *buf, uint16_t len)
{
    App_BmsRs485TxMode();

    for(uint16_t i = 0; i < len; i++)
    {
        Uart_SendDataPoll(M0P_UART3, buf[i]);
        while(Uart_GetStatus(M0P_UART3, UartTC) == FALSE);
    }

    App_BmsRs485RxMode();
    return 0;
}


// Enable RS485 Transmit mode (RE/DE = HIGH)
void App_BmsRs485TxMode(void)
{
    App_BmsRs485EnOn();  
}

// Enable RS485 Receive mode (RE/DE = LOW)
void App_BmsRs485RxMode(void)
{
    App_BmsRs485EnOff();  
}



void rtc_config_byuser(void)
{
    Sysctrl_SetPeripheralGate(SysctrlPeripheralRtc, TRUE);
    Sysctrl_ClkSourceEnable(SysctrlClkRCL, TRUE);
  
    stc_rtc_initstruct_t RtcInitStruct;

    // 12-hour mode, AM
    RtcInitStruct.rtcAmpm      = RtcAm;        
    // Internal low-speed clock
    RtcInitStruct.rtcClksrc    = RtcClkRcl;    
    // 1-second periodic interrupt
    RtcInitStruct.rtcPrdsel.rtcPrdsel = RtcPrds;  
    RtcInitStruct.rtcPrdsel.rtcPrds    = Rtc1S;  
 
/* Load RTC structure (DEC ? BCD) */
RtcInitStruct.rtcTime.u8Second    = Dec2BCD(g_sec);
RtcInitStruct.rtcTime.u8Minute    = Dec2BCD(g_min);
RtcInitStruct.rtcTime.u8Hour      = Dec2BCD(g_hour);
RtcInitStruct.rtcTime.u8Day       = Dec2BCD(g_date);
RtcInitStruct.rtcTime.u8DayOfWeek = Dec2BCD(g_dow);
RtcInitStruct.rtcTime.u8Month     = Dec2BCD(g_month);
RtcInitStruct.rtcTime.u8Year      = Dec2BCD(g_year);

    // RTC compensation
    RtcInitStruct.rtcCompen     = RtcCompenEnable;  
    RtcInitStruct.rtcCompValue  = 0;               

    // Initialize RTC
    Rtc_Init(&RtcInitStruct);
    // Enable alarm interrupt (optional, here just 1-second periodic)
   // Rtc_AlmIeCmd(TRUE);  
   Rtc_Cmd(TRUE); 

}
 
 

 
