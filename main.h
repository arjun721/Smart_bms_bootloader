#include "mcal_sysctrl_interface.h"
#include "mcal_gpio_interface.h"
#include "mcal_spi_interface.h"
#include "mcal_can_interface.h"
#include "mcal_timer_interface.h"
#include "mcal_adc_interface.h"
#include "mcal_rs485_interface.h"
#include "mcal_rtc_interface.h"
#include "mcal_gui_interface.h"
#include "es_oz3717.h"
#include "string.h"

#define ADC_ENABLE      1
#define SPI_ENABLE      1
#define FDCAN_ENABLE    1
#define GPIO_ENABLE     1
#define SYSCTRL_ENABLE  1
#define TIMER_ENBALE    1
#define RS485_ENABLE    1
#define TIMER_ENABLE    1


typedef enum {
    BMS_STATE_INIT,
    BMS_STATE_IDLE,
    BMS_STATE_CHARGING,
    BMS_STATE_DISCHARGING,
    BMS_STATE_BALANCING,
    BMS_STATE_FAULT,
    BMS_STATE_SLEEP
} BmsState_t;



//BmsState_t currentState = BMS_STATE_INIT;

//void Bms_StateMachine(void) {
//    switch (currentState) {
//        case BMS_STATE_INIT:
//            // Initialize hardware, check battery config
//            if (InitializationSuccessful())
//                currentState = BMS_STATE_IDLE;
//            else
//                currentState = BMS_STATE_FAULT;
//            break;

//        case BMS_STATE_IDLE:
//            if (FaultDetected())
//                currentState = BMS_STATE_FAULT;
//            else if (ShouldCharge())
//                currentState = BMS_STATE_CHARGING;
//            else if (ShouldDischarge())
//                currentState = BMS_STATE_DISCHARGING;
//            else if (ShouldSleep())
//                currentState = BMS_STATE_SLEEP;
//            break;

//        case BMS_STATE_CHARGING:
//            if (FaultDetected())
//                currentState = BMS_STATE_FAULT;
//            else if (ChargeComplete())
//                currentState = BMS_STATE_IDLE;
//            break;

//        case BMS_STATE_DISCHARGING:
//            if (FaultDetected())
//                currentState = BMS_STATE_FAULT;
//            else if (DischargeComplete())
//                currentState = BMS_STATE_IDLE;
//            break;

//        case BMS_STATE_BALANCING:
//            if (FaultDetected())
//                currentState = BMS_STATE_FAULT;
//            else if (BalancingComplete())
//                currentState = BMS_STATE_IDLE;
//            break;

//        case BMS_STATE_FAULT:
//            HandleFault();
//            break;

//        case BMS_STATE_SLEEP:
//            if (WakeUpConditionMet())
//                currentState = BMS_STATE_IDLE;
//            break;
//    }
//}

void 
App_BmsInit(void);

void
App_BmsTempControl(void);
 

 