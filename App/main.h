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
#include "FreeRTOS.h"
#include "task.h"
#include <math.h>
#include <stdbool.h> 
#include "nor_flash.h"
#include "eeprom_api.h"
 

#define ADC_ENABLE      1
#define SPI_ENABLE      1
#define FDCAN_ENABLE    1
#define GPIO_ENABLE     1
#define SYSCTRL_ENABLE  1
#define TIMER_ENBALE    1
#define RS485_ENABLE    1
#define TIMER_ENABLE    1

#define BMS_AFE_INT_IRQ_PORT      GpioPortB
#define BMS_AFE_INT_IRO_PIN       GpioPin7

#define BMS_CHG_DSG_FET_OFF   o2m_register_write(OZ3717_FETCTRL,OZ3717_FETCTRL_CHG_DISABLE|OZ3717_FETCTRL_DSG_DISABLE)
#define BMS_CHG_DSG_FET_ON    o2m_register_write(OZ3717_FETCTRL,OZ3717_FETCTRL_CHG_ENABLE|OZ3717_FETCTRL_DSG_ENABLE)
#define BMS_CHG_FET_ON        o2m_register_write(OZ3717_FETCTRL,OZ3717_FETCTRL_CHG_ENABLE)
#define BMS_DSG_FET_ON        o2m_register_write(OZ3717_FETCTRL,OZ3717_FETCTRL_DSG_ENABLE)
#define BMS_CHG_FET_OFF       o2m_register_write(OZ3717_FETCTRL,OZ3717_FETCTRL_CHG_DISABLE)
#define BMS_DSG_FET_OFF       o2m_register_write(OZ3717_FETCTRL,OZ3717_FETCTRL_DSG_DISABLE)

#define FAULT_OVERVOLTAGE         (1 << 0)
#define FAULT_UNDERVOLTAGE        (1 << 1)
#define FAULT_UNDERPACKVOLTAGE    (1 << 2)
#define FAULT_OVERPACKVOLTAGE     (1 << 3)
#define FAULT_OVERCHARGE          (1 << 4)
#define FAULT_OVERDISCHARGE       (1 << 5)
#define FAULT_THERMALRUNWAY       (1 << 6)
#define FAULT_CELLDIFF            (1 << 7)
#define FAULT_ONBOARD_UNDER_TEMP  (1 << 8)
#define FAULT_ONBOARD_OVER_TEMP   (1 << 9)
#define FAULT_EXT_OVER_TEMP_IN_CHARGE     (1 << 10)
#define FAULT_EXT_OVER_TEMP_IN_DISCHARGE  (1 << 11)
#define FAULT_TMP_DSG                      (1 << 12)
#define FAULT_SHORT_CKT                   (1 << 13)
#define FAULT_UNDER_TEMP                  (1 << 14)
#define FAULT_HARDWARE                    (1 << 15)
 
 
 

#define SOC_METER_CHARGE     (1<<0)
#define SOC_METER_DISCHARGE  (1<<1)
#define SOC_METER_STANDBY    (1<<2)
#define SOC_METER_ERROR      (1<<3)
#define SOC_METER_OT         (1<<4)
#define SOC_METER_OC         (1<<5)
#define SOC_METER_CHG_OC     (1<<6)
#define SOC_METER_DISCHG_OC  (1<<7)

#ifndef MAX
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#endif
#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif
 

typedef struct
{
    // (A) BASIC PROTECTION (9 blocks × HHH)
    uint16_t Cell_OV_set,    Cell_OV_rel,    Cell_OV_delay;
    uint16_t Cell_UV_set,    Cell_UV_rel,    Cell_UV_delay;
    uint16_t Pack_OV_set,    Pack_OV_rel,    Pack_OV_delay;
    uint16_t Pack_UV_set,    Pack_UV_rel,    Pack_UV_delay;
    uint16_t Chg_UT_set,     Chg_UT_rel,     Chg_UT_delay;
    uint16_t Dsg_OT_set,     Dsg_OT_rel,     Dsg_OT_delay;
    uint16_t Dsg_UT_set,     Dsg_UT_rel,     Dsg_UT_delay;
    uint16_t Chg_OT_set,     Chg_OT_rel,     Chg_OT_delay;
    uint16_t Chg_OC_set,     Chg_OC_rel,     Chg_OC_delay;
    uint16_t Dsg_OC_set,     Dsg_OC_rel,     Dsg_OC_delay;

    // (B) ADVANCED PROTECTION
    uint16_t Dsg_OC2_set,    Dsg_OC2_rel,    Dsg_OC2_delay;
    uint16_t SC_set,         SC_rel,         SC_delay;
    uint16_t HCell_OV_set,   HCell_OV_rel,   HCell_OV_delay;
    uint16_t LCell_UV_set,   LCell_UV_rel,   LCell_UV_delay;

    // (C) FUNCTION CONFIG 7 bytes
    uint8_t load_en;
    uint8_t balance_en;
    uint8_t chg_balance;
    uint8_t ntc1;
    uint8_t ntc2;
    uint8_t ntc3;
    uint8_t ntc4;

    // (D) BALANCE CONFIG
    uint16_t cellOpenVoltage;
    uint16_t balanceDriftVoltage;
    uint16_t ipsOffDelay;

    // (E) CAPACITY CONFIG
    uint32_t nominalCapacity;
    uint32_t cycleCapacity;
    uint16_t fullSetVoltage;
    uint16_t endOfVoltage;
    uint8_t  selfDischargeRate;
} BmsConfig_t;


typedef struct {
    double nominal_capacity_ah;   // design capacity in Ah
    double soc;                   // 0.0..1.0 (state of charge)
    double soh;                   // 0.0..1.0 (state of health)
    double coulomb_acc_ah;        // net coulombs counted since last reference (Ah)
    double last_avg_current_a;    // moving average current (A), negative=discharging
    double current_ma_window_s;   // time constant for current smoothing (seconds)

    // cycle measurement
    bool in_discharge_cycle;
    double cycle_discharge_ah;    // measured Ah discharged during this cycle
    double soh_ema_alpha;         // alpha for EMA of measured capacity -> soh
    unsigned long cycle_count;    // total number of full charge+discharge cycles
    double charged_ah_accum;      // accumulate charged Ah for cycle counting

    // voltage correction params
    double v_full_threshold;      // pack voltage threshold considered full
    double v_empty_threshold;     // pack voltage threshold considered empty
    double voltage_idle_time_s;   // time current must be near 0 to allow snap
    double idle_time_accum_s;     // accumulator for idle
    double current_snap_threshold_a; // current magnitude threshold considered idle
} SimpleBMS;

 
typedef enum
{
    CUR_IDLE = 0,
    CUR_DISCHARGE,
    CUR_CHARGE
} current_dir_t;

typedef enum
{
  UNKNOWN_MODE = -1,
  IDEL_MODE = 0,
  SLEEP_MODE = 1,
  WAKEUP_MODE = 2,
  SHUTDOWN_MODE = 3,
} Bms_ModeType_t;

typedef enum
{
  NONE_STATE = 0,
  AFE_CHARGE_STATE = 1,
  AFE_DISCHARGE_STATE = 2,
  AFE_IDEAL_STATE = 3,
} Bms_StateType_t;

 

 

void App_BmsInit(void);
void App_BmsFaultClear(void);
void App_BmsChgDsgFetOff(void);
void App_BmsChgDsgFetOn(void);
void App_BmsChgFetOn(void);
void App_BmsDsgFetOn(void);
void App_BmsChgFetOff(void);
void App_BmsDsgFetOff(void);
 
void App_BmsFaultMonitor(void);
uint8_t App_GetBmsState(void);
 
void App_CellBalancing(void);
 

void App_CanThresholdSetting(void);

void App_SendSoCMeterData(void);

void App_BleSendJsonData(void);
void App_BleBasicInfo(void);
void App_Bleoriginsettings(void) ;
//void App_Bleoriginsettings2(void);
void App_BleProtectionSettings(void);
void App_BleProtectioncount(void);
void App_BleCurrentProtectionSettings(void) ;
void App_BleTemperatureProtectionSettings(void);

void App_CellVoltProtectionTask(void);
void App_TemperatureProtectionTask(void);
void App_TemperatureProtectionc(void);
void App_ChgDisCurrentProtection(void);
 
 void App_TemperatureProtectionCombined(void);
 void Send_All_Config_OverCAN(void);
 
 void App_advanced_protection_config(void);
// Forward declarations for soc, soh
void bms_init(SimpleBMS *b, double nominal_capacity_ah,double initial_soc, double initial_soh, double v_full_threshold, double v_empty_threshold);
void bms_update(SimpleBMS *b, double current_a, double dt_s, double pack_voltage);
double bms_get_soc_percent(const SimpleBMS *b);
double bms_get_soh_percent(const SimpleBMS *b);
double bms_estimate_time_s(const SimpleBMS *b, double current_a);
double Get_SOC_From_Voltage(uint16_t voltage_mV);
 


 

/* Fault bits */
typedef enum {
    F_CELL_OV = 0,
    F_CELL_UV,
    F_PACK_OV,
    F_PACK_UV,
    F_CHG_OT,
    F_CHG_UT,
    F_DSG_OT,
    F_DSG_UT,
    F_CHG_OC,
    F_DSG_OC,
    F_SHORTCIRCUIT,
    F_AFE_ERROR,
    F_SOFT_LOCK,
    F_HG_MOS_DAMAGE,
    F_SG_MOS_DAMAGE,
    F_MOS_OT,
    F_FAULT_COUNT
} fault_id_t;

typedef uint32_t fault_mask_t;

/* Fault meta (tunable) */
typedef struct {
    uint32_t detect_debounce_ms;   /* must be continuously true for this duration to assert */
    uint32_t recovery_delay_ms;    /* after condition clears, wait this long to auto-clear (0 => immediate) */
    uint8_t  priority;             /* higher => more severe; used for diagnostics only */
    bool     sticky;               /* sticky/latching fault (requires manual clear) */
    const char *name;              /* human name */
} fault_meta_t;

/* main manager state - defined in source */
typedef struct {
    fault_mask_t active_mask;
    fault_mask_t latched_mask;
    uint32_t detect_start_ms[F_FAULT_COUNT];
    uint32_t recover_start_ms[F_FAULT_COUNT];
} bms_faults_snapshot_t;

/* Externs */
//extern bms_faults_snapshot_t g_bms_faults_snapshot;

/* HW hooks - implement these for your platform (weak stubs provided in .c) */

void bms_faults_init(void);

void bms_faults_periodic(void);

void Rs485_data_send(void);

void Rs485_data_send2(void);

void Rs485_data_send3(void);

void decode_rs485_config(void);

void App_getBleDta_config(void);

bool Load_All_Thresholds_From_EEPROM(void);//(uint8_t *buffer, uint8_t count);

void App_LoadNorData(void);

void Sleep_function(void);


void bms_soc_process(SimpleBMS *b,
                     int16_t current_scaled,
                     uint16_t volt_scaled,
                     uint8_t cell_uv_flag,
                     uint8_t cell_ov_flag);