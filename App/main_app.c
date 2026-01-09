#include "main.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>    // for snprintf()
#include <string.h>   // for strlen()
#include <stdint.h>   // for uint8_t, int types
#include <stdarg.h>
#include <stdlib.h>  // for abs()



static int convert(int x);

static int convert(int x) {
    if (x >= 201 && x <= 400) {
        // 201?-1, 202?-2, ..., 400?-200
        return -(x - 200);
    }
    else if (x >= 0 && x <= 199) {
        // Return same value
        return x;
    }
    else if (x == 200) {
        return 0;   // Optional: defined value for 200
    }

    return 0; // default for out-of-range
}

#define LFP_TABLE_SIZE (sizeof(lfp_soc_table) / sizeof(lfp_soc_table[0]))
extern void App_advance_Doc2_setting(void);
typedef void (*InitFunc)(void);
fault_meta_t fault_table[F_FAULT_COUNT];
 
static volatile Bms_StateType_t bmsStateMode_t = NONE_STATE;
volatile Bms_ModeType_t bmsPorMode_t = UNKNOWN_MODE;
 
extern uint8_t g_struct_fill_flag; 
extern volatile uint8_t g_sleepModeEnabel;
extern struct oz3717_chip oz3717_data;
extern int8_t g_externalTempValue[5];
 
extern bleBmsData_t blebmsData; 
extern uint32_t time_remaining_s;
volatile uint16_t fault_flags;
 
extern uint8_t g_can_conneted;
extern float soc_percent;
uint8_t g_soc_percent, g_soc_status,soc_init ,soc_volt,g_main_init_done;

extern float g_systemCurrent,g_packVolt; 
uint8_t fault_tigger_flag =0;
uint16_t volt_scaled;
int16_t current_scaled;
bool balancing_ckeck;
extern uint16_t chg_calibration_flag;
extern uint16_t dsg_calibration_flag;
extern int16_t measured_chg_current,measured_dsg_current;
uint8_t fCounts[16] = {0};
fault_mask_t active, latched;
uint32_t cycle_count_3 = 28;
extern uint16_t CC_soc;
    
uint16_t  l_cell_balance_start = 3400;
uint16_t  l_cell_balance_delta = 500;
uint16_t  l_cell_balance_diff = 1000;
uint16_t cell_diff_volt = 1000, cell_diff_volt_rel = 500;

 
uint16_t l_cell_over_volt = 3650;
uint16_t cellovervoltagerecovery = 3500;
uint16_t l_cov_delay = 1000;

uint16_t l_cell_under_volt = 1000;
uint16_t cellundervoltagerecovery = 1200;

uint16_t l_cuv_delay_e = 123;  // in use 

uint16_t l_cuv_delay = 500;

uint32_t packOverVoltRecovery = 58000;
uint16_t l_pack_overVolt = 60000;
uint16_t l_pov_delay;

uint32_t packUnderVoltRecovery= 30000;
uint16_t l_pack_underVolt = 20000;
uint16_t l_puv_delay;

uint32_t l_doc2 = 90;
uint32_t l_disChgCurrent = 70000; // 65 A
uint32_t l_ChgCurrent = 30000;    // 40 A

int16_t  l_ntc_under_temp = 1;
uint16_t l_max_chg_temp = 6500;
uint16_t l_max_dis_temp = 6000;
uint16_t   l_max_chg_temp_delay;
uint16_t   l_max_dis_temp_delay ;

uint16_t l_max_chg_temp_recovery = 54;
uint16_t l_max_dis_temp_recovery = 53;

int16_t   l_min_chg_temp;// = bmsCfg.Chg_UT_set;
int16_t   l_min_chg_temp_recovery;// = bmsCfg.Chg_UT_rel;
uint16_t   l_min_chg_temp_delay;//  = bmsCfg.Chg_UT_delay;
  
int16_t  l_min_dis_temp;// = bmsCfg.Dsg_UT_set;
int16_t  l_min_dis_temp_recovery;// = bmsCfg.Dsg_UT_rel;
uint16_t   l_min_dis_temp_delay;// = bmsCfg.Dsg_UT_delay;

uint16_t h_cell_ov_p = 3900, l_cell_ov_p = 2000;
uint16_t h_cell_ov_p_delay, l_cell_ov_p_delay;

uint16_t doc2_p = 50, doc2_delay = 20;  //( 60--> x2 , delay  1 to 32)
uint16_t sc_p = 0, sc_r = 2, sc_delay = 100;

extern BmsParameters_t readCfg;
extern BmsParameters_t writeCfg;
  
uint32_t app_getTick(void);

typedef struct {
    uint16_t bmsId;   // byte 0-1
    uint16_t BattType;  // byte 2-3
    uint16_t NoOfCell;   // byte 4-5
    uint16_t BattAh;  // byte 6-7
} BmsInfoCanData_t;

typedef struct {
    uint16_t cellOverVolt;   // byte 0-1
    uint16_t cellUnderVolt;  // byte 2-3
    uint16_t packOverVolt;   // byte 4-5
    uint16_t packUnderVolt;  // byte 6-7
} CellSettingCanData_t;

typedef struct {
    uint16_t COVRecovery;   // byte 0-1
    uint16_t CUVRecovery;  // byte 2-3
    uint16_t POVRecovery;   // byte 4-5
    uint16_t PUVRecovery;  // byte 6-7
} CellRecoveryCanData_t;

typedef struct {
    uint16_t maxChgTemp;   // byte 0-1
    uint16_t maxDisTemp;  // byte 2-3
    uint16_t minTemp;
    uint16_t HysTemp;   // byte 4-5
} TempSettingCanData_t;

typedef struct {
    uint32_t MaxChgCurrent;   // byte 0-1
    uint32_t MaxDisChgCurrent;  // byte 2-3
    uint32_t MaxPackCurrent;   // byte 4-5
    uint32_t resv;  // byte 6-7
} CurrentSettingCanData_t;

typedef struct {
    uint16_t CellBalanceVolt;   // byte 0-1
    uint16_t CellBalanceDelta;  // byte 2-3
    uint16_t resv1;   // byte 4-5
    uint16_t resv;  // byte 6-7
} CellBalanceSettingCanData_t;

 
SimpleBMS b;
 
BmsInfoCanData_t    bmsInfo_t;
CellSettingCanData_t cellSettings;
TempSettingCanData_t Tempsettings;
CurrentSettingCanData_t CurrentSettings;
CellBalanceSettingCanData_t CellBalanceSetting;
CellRecoveryCanData_t cellVoltRecovert_t;

/* ================== CURRENT DIRECTION ================== */

 

/* current_scaled:
 *  -1000 = -10.00 A
 *   1100 = +11.00 A
 * scale = 100 counts / amp
 */
current_dir_t get_current_direction(int16_t current_scaled)
{
    const int16_t DEAD_BAND = 90;   // ±0.20 A

    if (current_scaled > DEAD_BAND)
        return CUR_CHARGE;
    else if (current_scaled < -DEAD_BAND)
        return CUR_DISCHARGE;
    else
        return CUR_IDLE;
}


static uint16_t rs485_getChecksum1(uint8_t *ptr, uint8_t len)
{
	int8_t i;
	uint16_t checksum = 0;

	for(i=0; i<len; i++)
		checksum += ptr[i];

	__asm("nop");
	checksum = (0xffff & ~checksum) + 1;

	return(checksum);
}

static void revMemcpy1(void * destination, void * source, uint32_t length) {
    char *charDest = (char *) destination;
    char *charSrc = (char *) source;
    for (int ijk = 0; ijk < (int) length; ijk++)
        charDest[ijk] = charSrc[(length - 1) - ijk];
}

 

BmsConfig_t bmsCfg;

extern volatile uint8_t rs485_rx_buf3[180];
void bms_update_cycle_capacity(uint32_t newCycleCapacity);

void decode_rs485_config(void)
{
    uint16_t p = 4;        // payload starts at byte 4

    // ---------- Macros ----------
    #define U16() ( rs485_rx_buf3[p] | (rs485_rx_buf3[p+1] << 8) ); p+=2
    #define U8()  ( rs485_rx_buf3[p++] )

    // -------------------------------------------------------------
    // 1. BASIC PROTECTION (9 × HHH)
    // -------------------------------------------------------------
    bmsCfg.Cell_OV_set   = U16();   bmsCfg.Cell_OV_rel   = U16();   bmsCfg.Cell_OV_delay = U16();
    bmsCfg.Cell_UV_set   = U16();   bmsCfg.Cell_UV_rel   = U16();   bmsCfg.Cell_UV_delay = U16();
    bmsCfg.Pack_OV_set   = U16();   bmsCfg.Pack_OV_rel   = U16();   bmsCfg.Pack_OV_delay = U16();
    bmsCfg.Pack_UV_set   = U16();   bmsCfg.Pack_UV_rel   = U16();   bmsCfg.Pack_UV_delay = U16();
    bmsCfg.Chg_UT_set    = U16();   bmsCfg.Chg_UT_rel    = U16();   bmsCfg.Chg_UT_delay  = U16();
    bmsCfg.Dsg_OT_set    = U16();   bmsCfg.Dsg_OT_rel    = U16();   bmsCfg.Dsg_OT_delay  = U16();
    bmsCfg.Dsg_UT_set    = U16();   bmsCfg.Dsg_UT_rel    = U16();   bmsCfg.Dsg_UT_delay  = U16();
    bmsCfg.Chg_OT_set    = U16();   bmsCfg.Chg_OT_rel    = U16();   bmsCfg.Chg_OT_delay  = U16();
    bmsCfg.Chg_OC_set    = U16();   bmsCfg.Chg_OC_rel    = U16();   bmsCfg.Chg_OC_delay  = U16();
    bmsCfg.Dsg_OC_set    = U16();   bmsCfg.Dsg_OC_rel    = U16();   bmsCfg.Dsg_OC_delay  = U16();

    // -------------------------------------------------------------
    // 2. ADVANCED PROTECTION (4 × HHH)
    // -------------------------------------------------------------
    bmsCfg.Dsg_OC2_set   = U16();   bmsCfg.Dsg_OC2_rel   = U16();   bmsCfg.Dsg_OC2_delay = U16();
    bmsCfg.SC_set        = U16();   bmsCfg.SC_rel        = U16();   bmsCfg.SC_delay      = U16();
    bmsCfg.HCell_OV_set  = U16();   bmsCfg.HCell_OV_rel  = U16();   bmsCfg.HCell_OV_delay= U16();
    bmsCfg.LCell_UV_set  = U16();   bmsCfg.LCell_UV_rel  = U16();   bmsCfg.LCell_UV_delay= U16();

    // -------------------------------------------------------------
    // 3. FUNCTION CONFIG (7 bytes)
    // -------------------------------------------------------------
    bmsCfg.load_en       = U8();
    bmsCfg.balance_en    = U8();
    bmsCfg.chg_balance   = U8();
    bmsCfg.ntc1          = U8();
    bmsCfg.ntc2          = U8();
    bmsCfg.ntc3          = U8();
    bmsCfg.ntc4          = U8();

    // -------------------------------------------------------------
    // 4. BALANCE CONFIG (HHH)
    // -------------------------------------------------------------
    bmsCfg.cellOpenVoltage      = U16();
    bmsCfg.balanceDriftVoltage  = U16();
    bmsCfg.ipsOffDelay          = U16();

    // -------------------------------------------------------------
    // 5. CAPACITY CONFIG (IIHHB)
    // -------------------------------------------------------------
    bmsCfg.nominalCapacity = rs485_rx_buf3[p]
        | (rs485_rx_buf3[p+1] << 8)
        | (rs485_rx_buf3[p+2] << 16)
        | (rs485_rx_buf3[p+3] << 24);
    p += 4;

    bmsCfg.cycleCapacity =
          rs485_rx_buf3[p]
        | (rs485_rx_buf3[p+1] << 8)
        | (rs485_rx_buf3[p+2] << 16)
        | (rs485_rx_buf3[p+3] << 24);
    p += 4;
     bmsCfg.cycleCapacity =  bmsCfg.cycleCapacity /1000;

    bmsCfg.fullSetVoltage    = U16();
    bmsCfg.endOfVoltage      = U16();
    bmsCfg.selfDischargeRate = U8();


    // ===============================
    //  EEPROM WRITE + UPDATE
    // ===============================

eepromInterface_writeReverse(0x4000, bmsCfg.Cell_OV_set, 2);
eepromInterface_writeReverse(0x4002, bmsCfg.Cell_OV_rel, 2);
eepromInterface_writeReverse(0x4004, bmsCfg.Cell_OV_delay, 2);

eepromInterface_writeReverse(0x4006, bmsCfg.Cell_UV_set, 2);
eepromInterface_writeReverse(0x4008, bmsCfg.Cell_UV_rel, 2);
eepromInterface_writeReverse(0x400A, bmsCfg.Cell_UV_delay, 2);

eepromInterface_writeReverse(0x400C, bmsCfg.Pack_OV_set, 2);
eepromInterface_writeReverse(0x400E, bmsCfg.Pack_OV_rel, 2);
eepromInterface_writeReverse(0x4010, bmsCfg.Pack_OV_delay, 2);

eepromInterface_writeReverse(0x4012, bmsCfg.Pack_UV_set, 2);
eepromInterface_writeReverse(0x4014, bmsCfg.Pack_UV_rel, 2);
eepromInterface_writeReverse(0x4016, bmsCfg.Pack_UV_delay, 2);

eepromInterface_writeReverse(0x4018, bmsCfg.Chg_UT_set, 2);
eepromInterface_writeReverse(0x401A, bmsCfg.Chg_UT_rel, 2);
eepromInterface_writeReverse(0x401C, bmsCfg.Chg_UT_delay, 2);

eepromInterface_writeReverse(0x401E, bmsCfg.Dsg_OT_set, 2);
eepromInterface_writeReverse(0x4020, bmsCfg.Dsg_OT_rel, 2);
eepromInterface_writeReverse(0x4022, bmsCfg.Dsg_OT_delay, 2);

eepromInterface_writeReverse(0x4024, bmsCfg.Chg_OT_set, 2);
eepromInterface_writeReverse(0x4026, bmsCfg.Chg_OT_rel, 2);
eepromInterface_writeReverse(0x4028, bmsCfg.Chg_OT_delay, 2);

eepromInterface_writeReverse(0x402A, bmsCfg.Chg_OC_set, 2);
eepromInterface_writeReverse(0x402C, bmsCfg.Chg_OC_rel, 2);
eepromInterface_writeReverse(0x402E, bmsCfg.Chg_OC_delay, 2);

eepromInterface_writeReverse(0x4030, bmsCfg.Dsg_OC2_set, 2);
eepromInterface_writeReverse(0x4032, bmsCfg.Dsg_OC2_rel, 2);
eepromInterface_writeReverse(0x4034, bmsCfg.Dsg_OC2_delay, 2);

eepromInterface_writeReverse(0x4036, bmsCfg.SC_set, 2);
eepromInterface_writeReverse(0x4038, bmsCfg.SC_rel, 2);
eepromInterface_writeReverse(0x403A, bmsCfg.SC_delay, 2);

eepromInterface_writeReverse(0x403C, bmsCfg.HCell_OV_set, 2);
eepromInterface_writeReverse(0x403E, bmsCfg.HCell_OV_rel, 2);
eepromInterface_writeReverse(0x4040, bmsCfg.HCell_OV_delay, 2);

eepromInterface_writeReverse(0x4042, bmsCfg.LCell_UV_set, 2);
eepromInterface_writeReverse(0x4044, bmsCfg.LCell_UV_rel, 2);
eepromInterface_writeReverse(0x4046, bmsCfg.LCell_UV_delay, 2);

eepromInterface_writeReverse(0x4048, bmsCfg.load_en, 1);
eepromInterface_writeReverse(0x4049, bmsCfg.balance_en, 1);
eepromInterface_writeReverse(0x404A, bmsCfg.chg_balance, 1);
eepromInterface_writeReverse(0x404B, bmsCfg.ntc1, 1);
eepromInterface_writeReverse(0x404C, bmsCfg.ntc2, 1);
eepromInterface_writeReverse(0x404D, bmsCfg.ntc3, 1);
eepromInterface_writeReverse(0x404E, bmsCfg.ntc4, 1);

eepromInterface_writeReverse(0x4050, bmsCfg.cellOpenVoltage, 2);
eepromInterface_writeReverse(0x4052, bmsCfg.balanceDriftVoltage, 2);
eepromInterface_writeReverse(0x4054, bmsCfg.ipsOffDelay, 2);

eepromInterface_writeReverse(0x4056, bmsCfg.nominalCapacity, 4);
eepromInterface_writeReverse(0x405A, bmsCfg.cycleCapacity, 4);

eepromInterface_writeReverse(0x405E, bmsCfg.fullSetVoltage, 2);
eepromInterface_writeReverse(0x4060, bmsCfg.endOfVoltage, 2);
eepromInterface_writeReverse(0x4062, bmsCfg.selfDischargeRate, 1);


eepromInterface_writeReverse(0x4063, bmsCfg.Dsg_OC_set,   2);  // ? 0x4065
eepromInterface_writeReverse(0x4065, bmsCfg.Dsg_OC_rel,   2);  // ? 0x4067
eepromInterface_writeReverse(0x4067, bmsCfg.Dsg_OC_delay, 2);  // ? 0x4069

// ---------- Dsg_UT ----------
eepromInterface_writeReverse(0x4069, bmsCfg.Dsg_UT_set,   2);   // 0x4067 + 2
eepromInterface_writeReverse(0x406B, bmsCfg.Dsg_UT_rel,   2);   // 0x4069 + 2
eepromInterface_writeReverse(0x406D, bmsCfg.Dsg_UT_delay, 2);   // 0x406B + 2


    // FINAL EEPROM UPDATE
    eepromInterface_updateEEPROM(0, true, NULL);
    
   bms_update_cycle_capacity(bmsCfg.cycleCapacity);
    // write all the value  
    
    l_cell_over_volt = bmsCfg.Cell_OV_set;
   cellovervoltagerecovery = bmsCfg.Cell_OV_rel;
   fault_table[F_CELL_OV].recovery_delay_ms = (bmsCfg.Cell_OV_delay *1000)+5000;  // load runtime value

   l_cell_under_volt = bmsCfg.Cell_UV_set;
   cellundervoltagerecovery = bmsCfg.Cell_UV_rel;
   fault_table[F_CELL_UV].recovery_delay_ms = (bmsCfg.Cell_UV_delay *1000)+5000;  // load runtime value
   

   l_pack_overVolt =  bmsCfg.Pack_OV_set;
   packOverVoltRecovery = bmsCfg.Pack_OV_rel;
   fault_table[F_PACK_OV].recovery_delay_ms   = (bmsCfg.Pack_OV_delay*1000) + 9000;

   l_pack_underVolt = bmsCfg.Pack_UV_set;
   packUnderVoltRecovery = bmsCfg.Pack_UV_rel;
   fault_table[F_PACK_UV].recovery_delay_ms  = (bmsCfg.Pack_UV_delay *1000) + 9000;

   l_max_chg_temp =  (bmsCfg.Chg_OT_set*100);
   l_max_chg_temp_recovery =  (bmsCfg.Chg_OT_rel *100);
   fault_table[F_CHG_OT].recovery_delay_ms  = (bmsCfg.Chg_OT_delay *1000) +5000;
    
    if(bmsCfg.Chg_UT_set >= 0 && bmsCfg.Chg_UT_set <= 200)
    {
      l_min_chg_temp =(bmsCfg.Chg_UT_set*100);
    }
    else
    {
      l_min_chg_temp = convert((bmsCfg.Chg_UT_set));
      l_min_chg_temp = l_min_chg_temp *100;
    }
    
    if(bmsCfg.Chg_UT_rel >= 0 && bmsCfg.Chg_UT_rel <= 200)
    {
      l_min_chg_temp_recovery =(bmsCfg.Chg_UT_rel*100);
    }
    else
    {
      l_min_chg_temp_recovery = convert((bmsCfg.Chg_UT_rel));
      l_min_chg_temp_recovery = l_min_chg_temp_recovery*100;
    } 
    fault_table[F_CHG_UT].recovery_delay_ms  = (bmsCfg.Chg_UT_delay *1000)+5000;
   
    
   l_max_dis_temp = (bmsCfg.Dsg_OT_set *100);
   l_max_dis_temp_recovery = (bmsCfg.Dsg_OT_rel*100); 
   fault_table[F_DSG_OT].recovery_delay_ms  = (bmsCfg.Dsg_OT_delay *1000) +5000;
   
   
   if(bmsCfg.Dsg_UT_set >= 0 && bmsCfg.Dsg_UT_set <= 200)
   {
      l_min_dis_temp =(bmsCfg.Dsg_UT_set*100);
   }
    else
    {
      l_min_dis_temp = convert((bmsCfg.Dsg_UT_set));
      l_min_dis_temp = l_min_dis_temp*100;
    }
    
    if(bmsCfg.Dsg_UT_rel >= 0 && bmsCfg.Dsg_UT_rel <= 200)
    {
      l_min_dis_temp_recovery =(bmsCfg.Dsg_UT_rel*100);
    }
    else
    {
      l_min_dis_temp_recovery = convert((bmsCfg.Dsg_UT_rel));
      l_min_dis_temp_recovery = l_min_dis_temp_recovery*100;
    } 
   fault_table[F_DSG_UT].recovery_delay_ms  = (bmsCfg.Dsg_UT_delay *1000) + 5000;
    
   l_disChgCurrent = (bmsCfg.Dsg_OC_set *1000); // 65 A
   fault_table[F_DSG_OC].recovery_delay_ms  =  bmsCfg.Dsg_OC_rel + 5000;   
   fault_table[F_DSG_OC].detect_debounce_ms = (bmsCfg.Dsg_OC_delay*1000);
    
   l_ChgCurrent = (bmsCfg.Chg_OC_set * 1000);    // 40 A
   fault_table[F_CHG_OC].recovery_delay_ms  = bmsCfg.Chg_OC_rel + 5000;  
   fault_table[F_CHG_OC].detect_debounce_ms = (bmsCfg.Chg_OC_delay*1000);
 
   l_cell_balance_start = bmsCfg.cellOpenVoltage;
   l_cell_balance_delta= bmsCfg.balanceDriftVoltage;
   l_cell_balance_diff = bmsCfg.ipsOffDelay;
    
   h_cell_ov_p =  bmsCfg.HCell_OV_set;
   //l_cell_ov_p = bmsCfg.LCell_UV_set;
   cell_diff_volt =  bmsCfg.LCell_UV_set;
   cell_diff_volt_rel = bmsCfg.LCell_UV_delay;
    
   h_cell_ov_p_delay = (bmsCfg.HCell_OV_delay *1000);
   l_cell_ov_p_delay = (bmsCfg.LCell_UV_delay *1000);
   
   doc2_p =  bmsCfg.Dsg_OC2_set;
   doc2_delay = bmsCfg.Dsg_OC2_delay;
   
   sc_p =  bmsCfg.SC_set;
   fault_table[F_SHORTCIRCUIT].recovery_delay_ms = (bmsCfg.SC_rel *1000) +5000;
   sc_delay = bmsCfg.SC_delay; 
   
   if(doc2_p >= 50 || sc_p >= 2)
   {
     App_advance_Doc2_setting();
   }  
}


extern bleBmsData_t blebmsData;

void App_getBleDta_config(void)
{
eepromInterface_writeReverse(0x4000, bmsCfg.Cell_OV_set, 2);
eepromInterface_writeReverse(0x4002, bmsCfg.Cell_OV_rel, 2);
eepromInterface_writeReverse(0x4004, bmsCfg.Cell_OV_delay, 2);

eepromInterface_writeReverse(0x4006, bmsCfg.Cell_UV_set, 2);
eepromInterface_writeReverse(0x4008, bmsCfg.Cell_UV_rel, 2);
eepromInterface_writeReverse(0x400A, bmsCfg.Cell_UV_delay, 2);

eepromInterface_writeReverse(0x400C, bmsCfg.Pack_OV_set, 2);
eepromInterface_writeReverse(0x400E, bmsCfg.Pack_OV_rel, 2);
eepromInterface_writeReverse(0x4010, bmsCfg.Pack_OV_delay, 2);

eepromInterface_writeReverse(0x4012, bmsCfg.Pack_UV_set, 2);
eepromInterface_writeReverse(0x4014, bmsCfg.Pack_UV_rel, 2);
eepromInterface_writeReverse(0x4016, bmsCfg.Pack_UV_delay, 2);

eepromInterface_writeReverse(0x4018, bmsCfg.Chg_UT_set, 2);
eepromInterface_writeReverse(0x401A, bmsCfg.Chg_UT_rel, 2);
eepromInterface_writeReverse(0x401C, bmsCfg.Chg_UT_delay, 2);

eepromInterface_writeReverse(0x401E, bmsCfg.Dsg_OT_set, 2);
eepromInterface_writeReverse(0x4020, bmsCfg.Dsg_OT_rel, 2);
eepromInterface_writeReverse(0x4022, bmsCfg.Dsg_OT_delay, 2);

eepromInterface_writeReverse(0x4024, bmsCfg.Chg_OT_set, 2);
eepromInterface_writeReverse(0x4026, bmsCfg.Chg_OT_rel, 2);
eepromInterface_writeReverse(0x4028, bmsCfg.Chg_OT_delay, 2);

eepromInterface_writeReverse(0x402A, bmsCfg.Chg_OC_set, 2);
eepromInterface_writeReverse(0x402C, bmsCfg.Chg_OC_rel, 2);
eepromInterface_writeReverse(0x402E, bmsCfg.Chg_OC_delay, 2);

eepromInterface_writeReverse(0x4030, bmsCfg.Dsg_OC2_set, 2);
eepromInterface_writeReverse(0x4032, bmsCfg.Dsg_OC2_rel, 2);
eepromInterface_writeReverse(0x4034, bmsCfg.Dsg_OC2_delay, 2);

eepromInterface_writeReverse(0x4036, bmsCfg.SC_set, 2);
eepromInterface_writeReverse(0x4038, bmsCfg.SC_rel, 2);
eepromInterface_writeReverse(0x403A, bmsCfg.SC_delay, 2);

//eepromInterface_writeReverse(0x403C, bmsCfg.HCell_OV_set, 2);
//eepromInterface_writeReverse(0x403E, bmsCfg.HCell_OV_rel, 2);
//eepromInterface_writeReverse(0x4040, bmsCfg.HCell_OV_delay, 2);

eepromInterface_writeReverse(0x4042, bmsCfg.LCell_UV_set, 2);
eepromInterface_writeReverse(0x4044, bmsCfg.LCell_UV_rel, 2);
eepromInterface_writeReverse(0x4046, bmsCfg.LCell_UV_delay, 2);

//eepromInterface_writeReverse(0x4048, bmsCfg.load_en, 1);
//eepromInterface_writeReverse(0x4049, bmsCfg.balance_en, 1);
//eepromInterface_writeReverse(0x404A, bmsCfg.chg_balance, 1);
//eepromInterface_writeReverse(0x404B, bmsCfg.ntc1, 1);
//eepromInterface_writeReverse(0x404C, bmsCfg.ntc2, 1);
//eepromInterface_writeReverse(0x404D, bmsCfg.ntc3, 1);
//eepromInterface_writeReverse(0x404E, bmsCfg.ntc4, 1);

eepromInterface_writeReverse(0x4050, bmsCfg.cellOpenVoltage, 2);
eepromInterface_writeReverse(0x4052, bmsCfg.balanceDriftVoltage, 2);
//eepromInterface_writeReverse(0x4054, bmsCfg.ipsOffDelay, 2);

eepromInterface_writeReverse(0x4056, bmsCfg.nominalCapacity, 4);
eepromInterface_writeReverse(0x405A, bmsCfg.cycleCapacity, 4);

//eepromInterface_writeReverse(0x405E, bmsCfg.fullSetVoltage, 2);
//eepromInterface_writeReverse(0x4060, bmsCfg.endOfVoltage, 2);
//eepromInterface_writeReverse(0x4062, bmsCfg.selfDischargeRate, 1);


eepromInterface_writeReverse(0x4063, bmsCfg.Dsg_OC_set,   2);  // ? 0x4065
eepromInterface_writeReverse(0x4065, bmsCfg.Dsg_OC_rel,   2);  // ? 0x4067
eepromInterface_writeReverse(0x4067, bmsCfg.Dsg_OC_delay, 2);  // ? 0x4069

// ---------- Dsg_UT ----------
eepromInterface_writeReverse(0x4069, bmsCfg.Dsg_UT_set,   2);   // 0x4067 + 2
eepromInterface_writeReverse(0x406B, bmsCfg.Dsg_UT_rel,   2);   // 0x4069 + 2
eepromInterface_writeReverse(0x406D, bmsCfg.Dsg_UT_delay, 2);   // 0x406B + 2


    // FINAL EEPROM UPDATE
    eepromInterface_updateEEPROM(0, true, NULL);
    
   bms_update_cycle_capacity(bmsCfg.cycleCapacity);
   
   l_cell_over_volt = bmsCfg.Cell_OV_set;
   cellovervoltagerecovery = bmsCfg.Cell_OV_rel;
   fault_table[F_CELL_OV].recovery_delay_ms = (bmsCfg.Cell_OV_delay *1000) +5000;  // load runtime value

   l_cell_under_volt = bmsCfg.Cell_UV_set;
   cellundervoltagerecovery = bmsCfg.Cell_UV_rel;
   fault_table[F_CELL_UV].recovery_delay_ms = (bmsCfg.Cell_UV_delay *1000) +5000;  // load runtime value
   
   l_pack_overVolt =  bmsCfg.Pack_OV_set;
   packOverVoltRecovery = bmsCfg.Pack_OV_rel;
   fault_table[F_PACK_OV].recovery_delay_ms   = (bmsCfg.Pack_OV_delay*1000) + 9000;

   l_pack_underVolt = bmsCfg.Pack_UV_set;
   packUnderVoltRecovery = bmsCfg.Pack_UV_rel;
   fault_table[F_PACK_UV].recovery_delay_ms  = (bmsCfg.Pack_UV_delay *1000) + 9000;



//   l_max_chg_temp =  (bmsCfg.Chg_OT_set*100);
//   l_max_chg_temp_recovery =  (bmsCfg.Chg_OT_rel *100);
    if(bmsCfg.Chg_UT_set >= 0 && bmsCfg.Chg_UT_set <= 200)
    {
      l_min_chg_temp =(bmsCfg.Chg_UT_set*100);
    }
    else
    {
      l_min_chg_temp = convert((bmsCfg.Chg_UT_set));
      l_min_chg_temp = l_min_chg_temp *100;
    }
    
    if(bmsCfg.Chg_UT_rel >= 0 && bmsCfg.Chg_UT_rel <= 200)
    {
      l_min_chg_temp_recovery =(bmsCfg.Chg_UT_rel*100);
    }
    else
    {
      l_min_chg_temp_recovery = convert((bmsCfg.Chg_UT_rel));
      l_min_chg_temp_recovery = l_min_chg_temp_recovery*100;
    } 
   fault_table[F_CHG_OT].recovery_delay_ms  = (bmsCfg.Chg_OT_delay *1000)+5000;
    
    
//   l_min_chg_temp =(bmsCfg.Chg_UT_set*100);
//   l_min_chg_temp_recovery =(bmsCfg.Chg_UT_rel*100);
    
   if(bmsCfg.Chg_UT_set >= 0 && bmsCfg.Chg_UT_set <= 200)
    {
      l_min_chg_temp =(bmsCfg.Chg_UT_set*100);
    }
    else
    {
      l_min_chg_temp = convert((bmsCfg.Chg_UT_set));
      l_min_chg_temp = l_min_chg_temp *100;
    }
    
    if(bmsCfg.Chg_UT_rel >= 0 && bmsCfg.Chg_UT_rel <= 200)
    {
      l_min_chg_temp_recovery =(bmsCfg.Chg_UT_rel*100);
    }
    else
    {
      l_min_chg_temp_recovery = convert((bmsCfg.Chg_UT_rel));
      l_min_chg_temp_recovery = l_min_chg_temp_recovery*100;
    } 
   fault_table[F_CHG_UT].recovery_delay_ms  = (bmsCfg.Chg_UT_delay *1000) +5000;
  
   l_max_dis_temp = (bmsCfg.Dsg_OT_set *100);
   l_max_dis_temp_recovery = (bmsCfg.Dsg_OT_rel*100); 
   fault_table[F_DSG_OT].recovery_delay_ms  = (bmsCfg.Dsg_OT_delay *1000) +5000;
   
   l_min_dis_temp =(bmsCfg.Dsg_UT_set*100);
   l_min_dis_temp_recovery =(bmsCfg.Dsg_UT_rel*100);
   fault_table[F_DSG_UT].recovery_delay_ms  = (bmsCfg.Dsg_UT_delay *1000)+5000;
    
    
   l_cell_balance_start = bmsCfg.cellOpenVoltage;
   l_cell_balance_delta= bmsCfg.balanceDriftVoltage;
//   l_cell_balance_diff = bmsCfg.ipsOffDelay;
    
//   h_cell_ov_p =  bmsCfg.HCell_OV_set;
   cell_diff_volt =  bmsCfg.LCell_UV_set;
   cell_diff_volt_rel = bmsCfg.LCell_UV_delay;
    
//   h_cell_ov_p_delay = (bmsCfg.HCell_OV_delay *1000);
//   l_cell_ov_p_delay = (bmsCfg.LCell_UV_delay *1000);

   
   l_disChgCurrent = (bmsCfg.Dsg_OC_set *1000); // 65 A
   fault_table[F_DSG_OC].recovery_delay_ms  =  bmsCfg.Dsg_OC_rel +5000;   
   fault_table[F_DSG_OC].detect_debounce_ms = (bmsCfg.Dsg_OC_delay*1000);
    
   l_ChgCurrent = (bmsCfg.Chg_OC_set * 1000);    // 40 A
   fault_table[F_CHG_OC].recovery_delay_ms  = bmsCfg.Chg_OC_rel +5000;  
   fault_table[F_CHG_OC].detect_debounce_ms = (bmsCfg.Chg_OC_delay*1000);
   
   doc2_p =  bmsCfg.Dsg_OC2_set;
   doc2_delay = bmsCfg.Dsg_OC2_delay;
   
   sc_p =  bmsCfg.SC_set;
   fault_table[F_SHORTCIRCUIT].recovery_delay_ms = (bmsCfg.SC_rel *1000)+5000;
   sc_delay = bmsCfg.SC_delay; 
   
   if(doc2_p >= 50 || sc_p >= 2)
   {
     App_advance_Doc2_setting();
   }  
}
 


void bms_update_cycle_capacity(uint32_t newCycleCapacity)
{
    static uint8_t prevCycleCapacity = 0xFF;  // invalid initial value

    /* Accept only 1–99 */
    if (newCycleCapacity < 1 || newCycleCapacity > 99)
    {
        return;   // invalid value, ignore
    }

    if ((uint8_t)newCycleCapacity != prevCycleCapacity)
    {
        prevCycleCapacity = (uint8_t)newCycleCapacity;

        bmsCfg.cycleCapacity = prevCycleCapacity;

        /* Convert to fraction: 1–99 ? 0.01–0.99 */
        float x = bmsCfg.cycleCapacity / 100.0f;
       // float y =  bmsCfg.nominalCapacity/ 1000.0f;
      //bmsCfg.nominalCapacity

        /* Call init ONLY when changed */
        bms_init(&b,
                 73,     // nc  bmsCfg.nominalCapacity
                 x,      // soc  
                 b.soh,    // soh
                 56.0f,  // max voltage
                 48.0f); // min voltage
    }
}


 
bool Load_All_Thresholds_From_EEPROM(void)
{
    const int SIZE = 150;   // EXACT size of all parameters
    uint8_t buf[SIZE];

    // Read EXACT bytes written
    for (int i = 0; i < SIZE; i++)
    {
        if (eepromInterface_readByte(0x4000 + i, &buf[i]) != 0)
            return false;
    }
 
    // -------- First 16-bit groups -------
 bmsCfg.Cell_OV_set  = (uint16_t)((buf[0] << 8) | buf[1]);
 bmsCfg.Cell_OV_rel = (uint16_t)((buf[2] << 8) | buf[3]);
 bmsCfg.Cell_OV_delay = (uint16_t)((buf[4] << 8) | buf[5]);
   
 bmsCfg.Cell_UV_set = (uint16_t)((buf[6] << 8) | buf[7]);
 bmsCfg.Cell_UV_rel = (uint16_t)((buf[8] << 8) | buf[9]);
 bmsCfg.Cell_UV_delay = (uint16_t)((buf[10] << 8) | buf[11]);
 
 bmsCfg.Pack_OV_set= (uint16_t)((buf[12] << 8) | buf[13]);
 bmsCfg.Pack_OV_rel= (uint16_t)((buf[14] << 8) | buf[15]);
 bmsCfg.Pack_OV_delay= (uint16_t)((buf[16] << 8) | buf[17]);
 
 bmsCfg.Pack_UV_set= (uint16_t)((buf[18] << 8) | buf[19]);
 bmsCfg.Pack_UV_rel= (uint16_t)((buf[20] << 8) | buf[21]);
 bmsCfg.Pack_UV_delay= (uint16_t)((buf[22] << 8) | buf[23]);
    
 bmsCfg.Chg_UT_set= (uint16_t)((buf[24] << 8) | buf[25]);
 bmsCfg.Chg_UT_rel= (uint16_t)((buf[26] << 8) | buf[27]);
 bmsCfg.Chg_UT_delay= (uint16_t)((buf[28] << 8) | buf[29]);

 bmsCfg.Dsg_OT_set= (uint16_t)((buf[30] << 8) | buf[31]);
 bmsCfg.Dsg_OT_rel= (uint16_t)((buf[32] << 8) | buf[33]);
 bmsCfg.Dsg_OT_delay= (uint16_t)((buf[34] << 8) | buf[35]);


 bmsCfg.Chg_OT_set= (uint16_t)((buf[36] << 8) | buf[37]);
 bmsCfg.Chg_OT_rel= (uint16_t)((buf[38] << 8) | buf[39]);
 bmsCfg.Chg_OT_delay= (uint16_t)((buf[40] << 8) | buf[41]);


 bmsCfg.Chg_OC_set= (uint16_t)((buf[42] << 8) | buf[43]);
 bmsCfg.Chg_OC_rel= (uint16_t)((buf[44] << 8) | buf[45]);
 bmsCfg.Chg_OC_delay= (uint16_t)((buf[46] << 8) | buf[47]);
 
 bmsCfg.Dsg_OC2_set= (uint16_t)((buf[48] << 8) | buf[49]); 
 bmsCfg.Dsg_OC2_rel= (uint16_t)((buf[50] << 8) | buf[51]); 
 bmsCfg.Dsg_OC2_delay= (uint16_t)((buf[52] << 8) | buf[53]);

 bmsCfg.SC_set= (uint16_t)((buf[54] << 8) | buf[55]); 
 bmsCfg.SC_rel= (uint16_t)((buf[56] << 8) | buf[57]); 
 bmsCfg.SC_delay= (uint16_t)((buf[58] << 8) | buf[59]); 


 bmsCfg.HCell_OV_set = (uint16_t)((buf[60] << 8) | buf[61]);  
 bmsCfg.HCell_OV_rel = (uint16_t)((buf[62] << 8) | buf[63]);  
 bmsCfg.HCell_OV_delay  = (uint16_t)((buf[64] << 8) | buf[65]); 

 bmsCfg.LCell_UV_set = (uint16_t)((buf[66] << 8) | buf[67]); 
 bmsCfg.LCell_UV_rel = (uint16_t)((buf[68] << 8) | buf[69]); 
 bmsCfg.LCell_UV_delay = (uint16_t)((buf[70] << 8) | buf[71]);  
 


    // -------- 8-bit fields --------
     bmsCfg.load_en = buf[73]; //(72)
     bmsCfg.balance_en = buf[74];
     bmsCfg.chg_balance = buf[75];
     bmsCfg.ntc1 = buf[76];
     bmsCfg.ntc2 = buf[77];
     bmsCfg.ntc3 = buf[78];
     bmsCfg.ntc4 = buf[79];

    // -------- more 16-bit fields --------
    bmsCfg.cellOpenVoltage = (uint16_t)((buf[80] << 8) | buf[81]);
    bmsCfg.balanceDriftVoltage = (uint16_t)((buf[82] << 8) | buf[83]);
    bmsCfg.ipsOffDelay = (uint16_t)((buf[84] << 8) | buf[85]);

    // -------- 32-bit fields --------
    bmsCfg.nominalCapacity = ((uint32_t)buf[86] << 24) |
                 ((uint32_t)buf[87] << 16) |
                 ((uint32_t)buf[88] << 8)  |
                 (uint32_t)buf[89];
                 
   bmsCfg.cycleCapacity = ((uint32_t)buf[90] << 24) |
                 ((uint32_t)buf[91] << 16) |
                 ((uint32_t)buf[92] << 8)  |
                 (uint32_t)buf[93];
                 
    bmsCfg.cycleCapacity =  bmsCfg.cycleCapacity *1000;

    bmsCfg.fullSetVoltage = (uint16_t)((buf[94] << 8) | buf[95]);
    bmsCfg.endOfVoltage = (uint16_t)((buf[96] << 8) | buf[97]);
    bmsCfg.selfDischargeRate = buf[98];
    
 
    bmsCfg.Dsg_OC_set = (uint16_t)((buf[99] << 8) | buf[100]);
    bmsCfg.Dsg_OC_rel  = (uint16_t)((buf[101] << 8) | buf[102]);
    bmsCfg.Dsg_OC_delay  = (uint16_t)((buf[103] << 8) | buf[104]);
    
    bmsCfg.Dsg_UT_set= (uint16_t)((buf[105] << 8) | buf[106]);
    bmsCfg.Dsg_UT_rel= (uint16_t)((buf[107] << 8) | buf[108]);
    bmsCfg.Dsg_UT_delay= (uint16_t)((buf[109] << 8) | buf[110]);
 
   l_cell_over_volt = bmsCfg.Cell_OV_set;
   cellovervoltagerecovery = bmsCfg.Cell_OV_rel;
   fault_table[F_CELL_OV].recovery_delay_ms = (bmsCfg.Cell_OV_delay *1000)+5000;  // load runtime value

   l_cell_under_volt = bmsCfg.Cell_UV_set;
   cellundervoltagerecovery = bmsCfg.Cell_UV_rel;
   fault_table[F_CELL_UV].recovery_delay_ms = (bmsCfg.Cell_UV_delay *1000)+5000;  // load runtime value
   

   l_pack_overVolt =  bmsCfg.Pack_OV_set;
   packOverVoltRecovery = bmsCfg.Pack_OV_rel;
   fault_table[F_PACK_OV].recovery_delay_ms   = (bmsCfg.Pack_OV_delay*1000)+5000;

   l_pack_underVolt = bmsCfg.Pack_UV_set;
   packUnderVoltRecovery = bmsCfg.Pack_UV_rel;
   fault_table[F_PACK_UV].recovery_delay_ms  = (bmsCfg.Pack_UV_delay *1000)+5000;

   l_max_chg_temp =  (bmsCfg.Chg_OT_set*100);
   l_max_chg_temp_recovery =  (bmsCfg.Chg_OT_rel *100);
   fault_table[F_CHG_OT].recovery_delay_ms  = (bmsCfg.Chg_OT_delay *1000)+5000;
    
    if(bmsCfg.Chg_UT_set >= 0 && bmsCfg.Chg_UT_set <= 200)
    {
      l_min_chg_temp =(bmsCfg.Chg_UT_set*100);
    }
    else
    {
      l_min_chg_temp = convert((bmsCfg.Chg_UT_set));
      l_min_chg_temp = l_min_chg_temp *100;
    }
    
    if(bmsCfg.Chg_UT_rel >= 0 && bmsCfg.Chg_UT_rel <= 200)
    {
      l_min_chg_temp_recovery =(bmsCfg.Chg_UT_rel*100);
    }
    else
    {
      l_min_chg_temp_recovery = convert((bmsCfg.Chg_UT_rel));
      l_min_chg_temp_recovery = l_min_chg_temp_recovery*100;
    } 
    fault_table[F_CHG_UT].recovery_delay_ms  = (bmsCfg.Chg_UT_delay *1000)+5000;
   
    
   l_max_dis_temp = (bmsCfg.Dsg_OT_set *100);
   l_max_dis_temp_recovery = (bmsCfg.Dsg_OT_rel*100); 
   fault_table[F_DSG_OT].recovery_delay_ms  = (bmsCfg.Dsg_OT_delay *1000)+5000;
   
   
   if(bmsCfg.Dsg_UT_set >= 0 && bmsCfg.Dsg_UT_set <= 200)
   {
      l_min_dis_temp =(bmsCfg.Dsg_UT_set*100);
   }
    else
    {
      l_min_dis_temp = convert((bmsCfg.Dsg_UT_set));
      l_min_dis_temp = l_min_dis_temp*100;
    }
    
    if(bmsCfg.Dsg_UT_rel >= 0 && bmsCfg.Dsg_UT_rel <= 200)
    {
      l_min_dis_temp_recovery =(bmsCfg.Dsg_UT_rel*100);
    }
    else
    {
      l_min_dis_temp_recovery = convert((bmsCfg.Dsg_UT_rel));
      l_min_dis_temp_recovery = l_min_dis_temp_recovery*100;
    } 
   fault_table[F_DSG_UT].recovery_delay_ms  = (bmsCfg.Dsg_UT_delay *1000)+5000;
    
   l_disChgCurrent = (bmsCfg.Dsg_OC_set *1000); // 65 A
   fault_table[F_DSG_OC].recovery_delay_ms  =  bmsCfg.Dsg_OC_rel+5000;   
   fault_table[F_DSG_OC].detect_debounce_ms = (bmsCfg.Dsg_OC_delay*1000);
    
   l_ChgCurrent = (bmsCfg.Chg_OC_set * 1000);    // 40 A
   fault_table[F_CHG_OC].recovery_delay_ms  = bmsCfg.Chg_OC_rel+5000;  
   fault_table[F_CHG_OC].detect_debounce_ms = (bmsCfg.Chg_OC_delay*1000);
    
   l_cell_balance_start = bmsCfg.cellOpenVoltage;
   l_cell_balance_delta= bmsCfg.balanceDriftVoltage;
   l_cell_balance_diff = bmsCfg.ipsOffDelay;
   
    
   h_cell_ov_p =  bmsCfg.HCell_OV_set;
   cell_diff_volt =  bmsCfg.LCell_UV_set;
   cell_diff_volt_rel = bmsCfg.LCell_UV_delay;
    
   h_cell_ov_p_delay = (bmsCfg.HCell_OV_delay *1000);
   l_cell_ov_p_delay = (bmsCfg.LCell_UV_delay *1000);
  
   doc2_p =  bmsCfg.Dsg_OC2_set;
   doc2_delay = bmsCfg.Dsg_OC2_delay;
   
   
   sc_p =  bmsCfg.SC_set;
   fault_table[F_SHORTCIRCUIT].recovery_delay_ms = (bmsCfg.SC_rel *1000)+5000;
   sc_delay = bmsCfg.SC_delay; 
   
//   doc2_p =  bmsCfg.Dsg_OC2_set;
//   doc2_delay = bmsCfg.Dsg_OC2_delay;
   if(doc2_p >= 50 || sc_p >= 2)
   {
     App_advance_Doc2_setting();
   }
   

    rs485_send_mis_Struct sendObj;

    sendObj.startByte    = RS485_START_BYTE;
    sendObj.command_code = HW_VERSION;       // <--- BASIC_INFO_CMD was wrong
    sendObj.status       = RS485_STATUS_OK;

    sendObj.data_length  = (uint8_t)sizeof(BmsConfig_struct);

    // -----------------------------------------
    // Fill data (only if needed)
    // -----------------------------------------
 
     BmsConfig_struct rs485_basicInfo_t =
{
    // -----------------------------
    // (A) BASIC PROTECTION
    // -----------------------------
    .Cell_OV_set      = bmsCfg.Cell_OV_set,
    .Cell_OV_rel      = bmsCfg.Cell_OV_rel,
    .Cell_OV_delay    = bmsCfg.Cell_OV_delay,

    .Cell_UV_set      = bmsCfg.Cell_UV_set,
    .Cell_UV_rel      = bmsCfg.Cell_UV_rel,
    .Cell_UV_delay    = bmsCfg.Cell_UV_delay,

    .Pack_OV_set      = bmsCfg.Pack_OV_set,
    .Pack_OV_rel      = bmsCfg.Pack_OV_rel,
    .Pack_OV_delay    = bmsCfg.Pack_OV_delay,

    .Pack_UV_set      = bmsCfg.Pack_UV_set,
    .Pack_UV_rel      = bmsCfg.Pack_UV_rel,
    .Pack_UV_delay    = bmsCfg.Pack_UV_delay,

    .Chg_UT_set       = bmsCfg.Chg_UT_set,
    .Chg_UT_rel       = bmsCfg.Chg_UT_rel,
    .Chg_UT_delay     = bmsCfg.Chg_UT_delay,
    
    .Dsg_UT_set       = bmsCfg.Dsg_UT_set,
    .Dsg_UT_rel       = bmsCfg.Dsg_UT_rel,
    .Dsg_UT_delay     = bmsCfg.Dsg_UT_delay,

    .Dsg_OT_set       = bmsCfg.Dsg_OT_set,
    .Dsg_OT_rel       = bmsCfg.Dsg_OT_rel,
    .Dsg_OT_delay     = bmsCfg.Dsg_OT_delay,

    .Chg_OT_set       = bmsCfg.Chg_OT_set,
    .Chg_OT_rel       = bmsCfg.Chg_OT_rel,
    .Chg_OT_delay     = bmsCfg.Chg_OT_delay,

    .Chg_OC_set       = bmsCfg.Chg_OC_set,
    .Chg_OC_rel       = bmsCfg.Chg_OC_rel,
    .Chg_OC_delay     = bmsCfg.Chg_OC_delay,

    .Dsg_OC_set       = bmsCfg.Dsg_OC_set,
    .Dsg_OC_rel       = bmsCfg.Dsg_OC_rel,
    .Dsg_OC_delay     = bmsCfg.Dsg_OC_delay,

    // -----------------------------
    // (B) ADVANCED PROTECTION
    // -----------------------------
    .Dsg_OC2_set      = bmsCfg.Dsg_OC2_set,
    .Dsg_OC2_rel      = bmsCfg.Dsg_OC2_rel,
    .Dsg_OC2_delay    = bmsCfg.Dsg_OC2_delay,

    .SC_set           = bmsCfg.SC_set,
    .SC_rel           = bmsCfg.SC_rel,
    .SC_delay         = bmsCfg.SC_delay,

    .HCell_OV_set     = bmsCfg.HCell_OV_set,
    .HCell_OV_rel     = bmsCfg.HCell_OV_rel,
    .HCell_OV_delay   = bmsCfg.HCell_OV_delay,

    .LCell_UV_set     = bmsCfg.LCell_UV_set,
    .LCell_UV_rel     = bmsCfg.LCell_UV_rel,
    .LCell_UV_delay   = bmsCfg.LCell_UV_delay,

    // -----------------------------
    // (C) FUNCTION CONFIG (bools)
    // -----------------------------
    .load_en          = bmsCfg.load_en,
    .balance_en       = bmsCfg.balance_en,
    .chg_balance      = bmsCfg.chg_balance,
    .ntc1             = bmsCfg.ntc1,
    .ntc2             = bmsCfg.ntc2,
    .ntc3             = bmsCfg.ntc3,
    .ntc4             = bmsCfg.ntc4,

    // -----------------------------
    // (D) BALANCE CONFIG
    // -----------------------------
    .cellOpenVoltage      = bmsCfg.cellOpenVoltage,
    .balanceDriftVoltage  = bmsCfg.balanceDriftVoltage,
    .ipsOffDelay          = bmsCfg.ipsOffDelay,

    // -----------------------------
    // (E) CAPACITY CONFIG
    // -----------------------------
    .nominalCapacity      = bmsCfg.nominalCapacity,
    .cycleCapacity        = bmsCfg.cycleCapacity,
    .fullSetVoltage       = bmsCfg.fullSetVoltage,
    .endOfVoltage         = bmsCfg.endOfVoltage,
    .selfDischargeRate    = bmsCfg.selfDischargeRate
};

          
    // Copy into TX buffer
    memcpy(sendObj.data_buff,
           &rs485_basicInfo_t,                   // <--- fixed (was rs485_basicInfo_obj)
           sizeof(BmsConfig_struct));

    // -----------------------------------------
    // Prepare checksum source = status + length + data
    // -----------------------------------------
    uint8_t checksumBuff[2 + sizeof(BmsConfig_struct)];
    checksumBuff[0] = sendObj.status;
    checksumBuff[1] = sendObj.data_length;

    memcpy(&checksumBuff[2],
           sendObj.data_buff,
           sizeof(BmsConfig_struct));

    // -----------------------------------------
    // Compute checksum
    // -----------------------------------------
    uint16_t tmpChecksum =
        rs485_getChecksum1(checksumBuff,
                           sizeof(checksumBuff));

    revMemcpy1(&sendObj.checksum,
               &tmpChecksum,
               sizeof(sendObj.checksum));

    sendObj.endByte = 0x77;

    // -----------------------------------------
    // Transmit full packet
    // -----------------------------------------
    App_UartTransmitPoll(M0P_UART3,
                         (uint8_t *)&sendObj,
                         sizeof(rs485_send_mis_Struct));   // <--- FIXED!

    return true;
}

void App_LoadNorData(void)
{
      const int SIZE = 150;   // EXACT size of all parameters
    uint8_t buf[SIZE];

    // Read EXACT bytes written
    for (int i = 0; i < SIZE; i++)
    {
        if (eepromInterface_readByte(0x4000 + i, &buf[i]) != 0)
            return;
    }
 
    // -------- First 16-bit groups -------
 bmsCfg.Cell_OV_set  = (uint16_t)((buf[0] << 8) | buf[1]);
 bmsCfg.Cell_OV_rel = (uint16_t)((buf[2] << 8) | buf[3]);
 bmsCfg.Cell_OV_delay = (uint16_t)((buf[4] << 8) | buf[5]);
   
 bmsCfg.Cell_UV_set = (uint16_t)((buf[6] << 8) | buf[7]);
 bmsCfg.Cell_UV_rel = (uint16_t)((buf[8] << 8) | buf[9]);
 bmsCfg.Cell_UV_delay = (uint16_t)((buf[10] << 8) | buf[11]);
 
 bmsCfg.Pack_OV_set= (uint16_t)((buf[12] << 8) | buf[13]);
 bmsCfg.Pack_OV_rel= (uint16_t)((buf[14] << 8) | buf[15]);
 bmsCfg.Pack_OV_delay= (uint16_t)((buf[16] << 8) | buf[17]);
 
 bmsCfg.Pack_UV_set= (uint16_t)((buf[18] << 8) | buf[19]);
 bmsCfg.Pack_UV_rel= (uint16_t)((buf[20] << 8) | buf[21]);
 bmsCfg.Pack_UV_delay= (uint16_t)((buf[22] << 8) | buf[23]);
    
 bmsCfg.Chg_UT_set= (uint16_t)((buf[24] << 8) | buf[25]);
 bmsCfg.Chg_UT_rel= (uint16_t)((buf[26] << 8) | buf[27]);
 bmsCfg.Chg_UT_delay= (uint16_t)((buf[28] << 8) | buf[29]);

 bmsCfg.Dsg_OT_set= (uint16_t)((buf[30] << 8) | buf[31]);
 bmsCfg.Dsg_OT_rel= (uint16_t)((buf[32] << 8) | buf[33]);
 bmsCfg.Dsg_OT_delay= (uint16_t)((buf[34] << 8) | buf[35]);


 bmsCfg.Chg_OT_set= (uint16_t)((buf[36] << 8) | buf[37]);
 bmsCfg.Chg_OT_rel= (uint16_t)((buf[38] << 8) | buf[39]);
 bmsCfg.Chg_OT_delay= (uint16_t)((buf[40] << 8) | buf[41]);


 bmsCfg.Chg_OC_set= (uint16_t)((buf[42] << 8) | buf[43]);
 bmsCfg.Chg_OC_rel= (uint16_t)((buf[44] << 8) | buf[45]);
 bmsCfg.Chg_OC_delay= (uint16_t)((buf[46] << 8) | buf[47]);
 
 bmsCfg.Dsg_OC2_set= (uint16_t)((buf[48] << 8) | buf[49]); 
 bmsCfg.Dsg_OC2_rel= (uint16_t)((buf[50] << 8) | buf[51]); 
 bmsCfg.Dsg_OC2_delay= (uint16_t)((buf[52] << 8) | buf[53]);

 bmsCfg.SC_set= (uint16_t)((buf[54] << 8) | buf[55]); 
 bmsCfg.SC_rel= (uint16_t)((buf[56] << 8) | buf[57]); 
 bmsCfg.SC_delay= (uint16_t)((buf[58] << 8) | buf[59]); 


 bmsCfg.HCell_OV_set = (uint16_t)((buf[60] << 8) | buf[61]);  
 bmsCfg.HCell_OV_rel = (uint16_t)((buf[62] << 8) | buf[63]);  
 bmsCfg.HCell_OV_delay  = (uint16_t)((buf[64] << 8) | buf[65]); 

 bmsCfg.LCell_UV_set = (uint16_t)((buf[66] << 8) | buf[67]); 
 bmsCfg.LCell_UV_rel = (uint16_t)((buf[68] << 8) | buf[69]); 
 bmsCfg.LCell_UV_delay = (uint16_t)((buf[70] << 8) | buf[71]);  
 


    // -------- 8-bit fields --------
     bmsCfg.load_en = buf[73]; //(72)
     bmsCfg.balance_en = buf[74];
     bmsCfg.chg_balance = buf[75];
     bmsCfg.ntc1 = buf[76];
     bmsCfg.ntc2 = buf[77];
     bmsCfg.ntc3 = buf[78];
     bmsCfg.ntc4 = buf[79];

    // -------- more 16-bit fields --------
    bmsCfg.cellOpenVoltage = (uint16_t)((buf[80] << 8) | buf[81]);
    bmsCfg.balanceDriftVoltage = (uint16_t)((buf[82] << 8) | buf[83]);
    bmsCfg.ipsOffDelay = (uint16_t)((buf[84] << 8) | buf[85]);

    // -------- 32-bit fields --------
    bmsCfg.nominalCapacity = ((uint32_t)buf[86] << 24) |
                 ((uint32_t)buf[87] << 16) |
                 ((uint32_t)buf[88] << 8)  |
                 (uint32_t)buf[89];
                 
   bmsCfg.cycleCapacity = ((uint32_t)buf[90] << 24) |
                 ((uint32_t)buf[91] << 16) |
                 ((uint32_t)buf[92] << 8)  |
                 (uint32_t)buf[93];
                 
    bmsCfg.cycleCapacity =  bmsCfg.cycleCapacity *1000;

    bmsCfg.fullSetVoltage = (uint16_t)((buf[94] << 8) | buf[95]);
    bmsCfg.endOfVoltage = (uint16_t)((buf[96] << 8) | buf[97]);
    bmsCfg.selfDischargeRate = buf[98];
    
 
    bmsCfg.Dsg_OC_set = (uint16_t)((buf[99] << 8) | buf[100]);
    bmsCfg.Dsg_OC_rel  = (uint16_t)((buf[101] << 8) | buf[102]);
    bmsCfg.Dsg_OC_delay  = (uint16_t)((buf[103] << 8) | buf[104]);
    
    bmsCfg.Dsg_UT_set= (uint16_t)((buf[105] << 8) | buf[106]);
    bmsCfg.Dsg_UT_rel= (uint16_t)((buf[107] << 8) | buf[108]);
    bmsCfg.Dsg_UT_delay= (uint16_t)((buf[109] << 8) | buf[110]);
 
   l_cell_over_volt = bmsCfg.Cell_OV_set;
   cellovervoltagerecovery = bmsCfg.Cell_OV_rel;
   fault_table[F_CELL_OV].recovery_delay_ms = (bmsCfg.Cell_OV_delay *1000)+5000;  // load runtime value

   l_cell_under_volt = bmsCfg.Cell_UV_set;
   cellundervoltagerecovery = bmsCfg.Cell_UV_rel;
   fault_table[F_CELL_UV].recovery_delay_ms = (bmsCfg.Cell_UV_delay *1000)+5000;  // load runtime value
   

   l_pack_overVolt =  bmsCfg.Pack_OV_set;
   packOverVoltRecovery = bmsCfg.Pack_OV_rel;
   fault_table[F_PACK_OV].recovery_delay_ms   = (bmsCfg.Pack_OV_delay*1000)+5000;

   l_pack_underVolt = bmsCfg.Pack_UV_set;
   packUnderVoltRecovery = bmsCfg.Pack_UV_rel;
   fault_table[F_PACK_UV].recovery_delay_ms  = (bmsCfg.Pack_UV_delay *1000)+5000;

   l_max_chg_temp =  (bmsCfg.Chg_OT_set*100);
   l_max_chg_temp_recovery =  (bmsCfg.Chg_OT_rel *100);
   fault_table[F_CHG_OT].recovery_delay_ms  = (bmsCfg.Chg_OT_delay *1000)+5000;
    
    if(bmsCfg.Chg_UT_set >= 0 && bmsCfg.Chg_UT_set <= 200)
    {
      l_min_chg_temp =(bmsCfg.Chg_UT_set*100);
    }
    else
    {
      l_min_chg_temp = convert((bmsCfg.Chg_UT_set));
      l_min_chg_temp = l_min_chg_temp *100;
    }
    
    if(bmsCfg.Chg_UT_rel >= 0 && bmsCfg.Chg_UT_rel <= 200)
    {
      l_min_chg_temp_recovery =(bmsCfg.Chg_UT_rel*100);
    }
    else
    {
      l_min_chg_temp_recovery = convert((bmsCfg.Chg_UT_rel));
      l_min_chg_temp_recovery = l_min_chg_temp_recovery*100;
    } 
    fault_table[F_CHG_UT].recovery_delay_ms  = (bmsCfg.Chg_UT_delay *1000)+5000;
   
    
   l_max_dis_temp = (bmsCfg.Dsg_OT_set *100);
   l_max_dis_temp_recovery = (bmsCfg.Dsg_OT_rel*100); 
   fault_table[F_DSG_OT].recovery_delay_ms  = (bmsCfg.Dsg_OT_delay *1000)+5000;
   
   
   if(bmsCfg.Dsg_UT_set >= 0 && bmsCfg.Dsg_UT_set <= 200)
   {
      l_min_dis_temp =(bmsCfg.Dsg_UT_set*100);
   }
    else
    {
      l_min_dis_temp = convert((bmsCfg.Dsg_UT_set));
      l_min_dis_temp = l_min_dis_temp*100;
    }
    
    if(bmsCfg.Dsg_UT_rel >= 0 && bmsCfg.Dsg_UT_rel <= 200)
    {
      l_min_dis_temp_recovery =(bmsCfg.Dsg_UT_rel*100);
    }
    else
    {
      l_min_dis_temp_recovery = convert((bmsCfg.Dsg_UT_rel));
      l_min_dis_temp_recovery = l_min_dis_temp_recovery*100;
    } 
   fault_table[F_DSG_UT].recovery_delay_ms  = (bmsCfg.Dsg_UT_delay *1000)+5000;
    
   l_disChgCurrent = (bmsCfg.Dsg_OC_set *1000); // 65 A
   fault_table[F_DSG_OC].recovery_delay_ms  =  bmsCfg.Dsg_OC_rel+5000;   
   fault_table[F_DSG_OC].detect_debounce_ms = (bmsCfg.Dsg_OC_delay*1000);
    
   l_ChgCurrent = (bmsCfg.Chg_OC_set * 1000);    // 40 A
   fault_table[F_CHG_OC].recovery_delay_ms  = bmsCfg.Chg_OC_rel+5000;  
   fault_table[F_CHG_OC].detect_debounce_ms = (bmsCfg.Chg_OC_delay*1000);
    
   l_cell_balance_start = bmsCfg.cellOpenVoltage;
   l_cell_balance_delta= bmsCfg.balanceDriftVoltage;
   l_cell_balance_diff = bmsCfg.ipsOffDelay;
   
    
   h_cell_ov_p =  bmsCfg.HCell_OV_set;
   cell_diff_volt =  bmsCfg.LCell_UV_set;
   cell_diff_volt_rel = bmsCfg.LCell_UV_delay;
    
   h_cell_ov_p_delay = (bmsCfg.HCell_OV_delay *1000);
   l_cell_ov_p_delay = (bmsCfg.LCell_UV_delay *1000);
  
   doc2_p =  bmsCfg.Dsg_OC2_set;
   doc2_delay = bmsCfg.Dsg_OC2_delay;
   
   
   sc_p =  bmsCfg.SC_set;
   fault_table[F_SHORTCIRCUIT].recovery_delay_ms = (bmsCfg.SC_rel *1000)+5000;
   sc_delay = bmsCfg.SC_delay; 
   
//   doc2_p =  bmsCfg.Dsg_OC2_set;
//   doc2_delay = bmsCfg.Dsg_OC2_delay;
   if(doc2_p >= 50 || sc_p >= 2)
   {
     App_advance_Doc2_setting();
   }
}
 

InitFunc init_table[] = {

    #if SYSCTRL_ENABLE == 1
        App_ClkInit,
        App_AdcInit,
        App_AdcSQRCfg,
    #endif

    #if GPIO_ENABLE == 1
        App_GpioInit,
        App_GpioIsrInit,
        App_BmsPorEnOn,
        App_BmsEnable,
        App_BmsNtcEnOn,
    #endif

    #if SPI_ENABLE == 1
        App_SpiAfeInit,
        App_SpiNorInit,
        App_BmsEnable,
    #endif

    #if TIMER_ENABLE == 1
        App_TimerInit,
        App_TimerStart,
    #endif

    #if FDCAN_ENABLE == 1
         App_CanInit,
    #endif

    #if RS485_ENABLE == 1
        App_UartInit,
        App_UartBleInit,
        App_Rs485UartCfg,
        App_UartRfidInit,  // rs485
    #endif

    NULL
};

//System init function
void App_BmsInit(void)
{
    for (int i = 0; init_table[i] != NULL; i++) {
        init_table[i]();
    }
}


//void App_BmsStateMachine(void)
//{
//  //bms start , it will check whether bms - in charging, discharging, ble?,can?,any fault
//  //after the afe data is read and updated then we have to check  this (till this need to toggle led by every 1 sec,
//  if(fault_flags == 0 && g_can_conneted == 0)
//  {
//    if(g_sleepModeEnabel > 60)
//    {
//      bmsPorMode_t = SLEEP_MODE;  // give comand to afe to go in standby mode and enable charg and discharge mosfet
//    }
//    else
//    {
//       bmsPorMode_t = IDEL_MODE;
//    }
//  }
//  else //  there is fault so keep the state idel
//  {
//   g_sleepModeEnabel = 0;
//   bmsPorMode_t = IDEL_MODE;
//  }
//  
//  if(App_GetBmsState() == AFE_CHARGE_STATE || App_GetBmsState() == AFE_DISCHARGE_STATE || g_can_conneted > 1)
//  {
//    g_can_conneted = 0;
//    g_sleepModeEnabel = 0;
//    bmsPorMode_t = IDEL_MODE;
//  }
//}

uint8_t App_GetBmsState(void)
{
    static uint8_t g_afe_init_count = 0;

    if (g_struct_fill_flag == 1 && ++g_afe_init_count > 5)
    {
        if (oz3717_data.cbsel1 & OZ3717_IN_CHG_STATE)
        {
            bmsStateMode_t = AFE_CHARGE_STATE;
        }
        else if (oz3717_data.cbsel1 & OZ3717_IN_DSG_STATE)
        {
            bmsStateMode_t = AFE_DISCHARGE_STATE;
        }
        else
        {
            bmsStateMode_t = AFE_IDEAL_STATE;
        }
    }
    return bmsStateMode_t;
}




void App_BmsChgDsgFetOff(void)
{
    o2m_register_write(OZ3717_FETCTRL, OZ3717_FETCTRL_CHG_DISABLE | OZ3717_FETCTRL_DSG_DISABLE);
}

void App_BmsChgDsgFetOn(void)
{
    o2m_register_write(OZ3717_FETCTRL, OZ3717_FETCTRL_CHG_ENABLE | OZ3717_FETCTRL_DSG_ENABLE);
}

void App_BmsChgFetOn(void)
{
    o2m_register_write(OZ3717_FETCTRL, OZ3717_FETCTRL_CHG_ENABLE);
}

void App_BmsDsgFetOn(void)
{
    o2m_register_write(OZ3717_FETCTRL, OZ3717_FETCTRL_DSG_ENABLE);
}

void App_BmsChgFetOff(void)
{
    o2m_register_write(OZ3717_FETCTRL, OZ3717_FETCTRL_CHG_DISABLE);
}

void App_BmsDsgFetOff(void)
{
    o2m_register_write(OZ3717_FETCTRL, OZ3717_FETCTRL_DSG_DISABLE);
}


#define GAIN_Q 15
extern int16_t offset;
static int16_t dsg_gain_q15 = (1 << GAIN_Q);
static int16_t chg_gain_q15 = (1 << GAIN_Q);  // charge gain

int16_t bms_get_calibrated_current(int16_t raw_current,current_dir_t dir);

int16_t bms_get_calibrated_current(int16_t raw_current, current_dir_t dir)
{
    int16_t temp;
    int16_t gain_q15;

    /* 1. Remove offset */
    temp = (int16_t)raw_current - offset;
  
    if(dsg_calibration_flag)
    {
      dsg_gain_q15 = (int16_t)(((int32_t)(measured_dsg_current * 10) << GAIN_Q) /(temp * 10));
      dsg_calibration_flag = 0;
      
      eepromInterface_writeReverse(0x406F, offset, 2);   // 0x406B + 4
      eepromInterface_writeReverse(0x4071, dsg_gain_q15, 2);   // 0x406F + 2
      eepromInterface_updateEEPROM(0, true, NULL);
 
    }
   
    if (chg_calibration_flag)
    {
      
      chg_gain_q15 = (int16_t)(((int32_t)(measured_chg_current * 10) << GAIN_Q) /(temp * 10));
      chg_calibration_flag = 0;
     
      eepromInterface_writeReverse(0x406F, offset, 2);   // 0x406B + 4
      eepromInterface_writeReverse(0x4073, chg_gain_q15, 2);   // 0x406B + 4
      eepromInterface_updateEEPROM(0, true, NULL);
    }
    

    /* 2. Select gain */
    if (dir == CUR_CHARGE)
    {
        gain_q15 = chg_gain_q15;
    }
    else if (dir == CUR_DISCHARGE)
    {
        gain_q15 = dsg_gain_q15;
        gain_q15 = gain_q15 * (-1); 
    }
    else
    {
        return 0;  // idle
    }
    
    /* 3. Apply gain (Q15) */
    temp = (int16_t)(((int32_t)temp * gain_q15) >> GAIN_Q) ;
 
    return (int16_t)(temp);
}

void App_CellBalancing(void)
{
	if(oz3717_data.cbsel1 & (1 << 13))
	{
		if ((oz3717_data.max_cell_data >= l_cell_balance_start) &&
				((oz3717_data.max_cell_data-oz3717_data.min_cell_data)>= l_cell_balance_delta))
		{
			oz3717_cb_set(oz3717_data.which_cell_max);
			balancing_ckeck = 1;
		}
		else
		{
      balancing_ckeck = 0;
			oz3717_cb_set(0);
		}
	}
	else if(oz3717_data.cbsel1 & (1 << 12))
	{
		//DISCHARGE
		oz3717_cb_set(0);
		balancing_ckeck = 0;

	}else
	{
		//IDLE
		if(oz3717_data.max_cell_data >= 1000 && oz3717_data.max_cell_data < 5000 && oz3717_data.min_cell_data >= 1000 && oz3717_data.min_cell_data < 5000)
		{
			if ((oz3717_data.max_cell_data >= l_cell_balance_start) &&((oz3717_data.max_cell_data-oz3717_data.min_cell_data)>= l_cell_balance_delta) && (oz3717_data.min_cell_data >= l_cell_balance_start))
			{
				oz3717_cb_set(oz3717_data.which_cell_max);
				balancing_ckeck = 1;

			}
			else
			{
				oz3717_cb_set(0);
				balancing_ckeck = 0;
			}
		}
	}
   
  volt_scaled = (uint16_t)(g_packVolt);
  current_scaled = ((int16_t)(oz3717_data.system_current / 10.0f)) - offset;
  current_dir_t cur_dir = get_current_direction(current_scaled);
  current_scaled  = bms_get_calibrated_current(current_scaled,cur_dir); 
}



 BmsParameters_t writeCfg;
 
extern void ais_readAllLogs_intoBuffer(void);
 
void App_SendSoCMeterData(void)
{
   char jsonBuffer[200];   // buffer for JSON string
    // Example data to send
  
   g_soc_status = 0;
  
    if(fault_flags <= 0)
    {
      if(current_scaled > 100)
      {
         g_soc_status |= SOC_METER_CHARGE;
         g_soc_status &= ~SOC_METER_DISCHARGE;
         g_soc_status &= ~SOC_METER_STANDBY;
        
         g_soc_status &= ~SOC_METER_OT;
         g_soc_status &= ~SOC_METER_OC;
         g_soc_status &= ~SOC_METER_CHG_OC;
         g_soc_status &= ~SOC_METER_DISCHG_OC;
        
      }
      else if(current_scaled < -100)
      {
         g_soc_status &= ~SOC_METER_CHARGE;
         g_soc_status |= SOC_METER_DISCHARGE;
         g_soc_status &= ~SOC_METER_STANDBY;
         
         g_soc_status &= ~SOC_METER_OT;
         g_soc_status &= ~SOC_METER_OC;
         g_soc_status &= ~SOC_METER_CHG_OC;
         g_soc_status &= ~SOC_METER_DISCHG_OC;
      }
      else //if(current_scaled > 0 && current_scaled < 900)
      {
         g_soc_status &= ~SOC_METER_CHARGE;         
         g_soc_status &= ~SOC_METER_DISCHARGE;
         g_soc_status |= SOC_METER_STANDBY;
         g_soc_status &= ~SOC_METER_OT;
         g_soc_status &= ~SOC_METER_OC;
         g_soc_status &= ~SOC_METER_CHG_OC;
         g_soc_status &= ~SOC_METER_DISCHG_OC;
      }
    }
    else // in error
    {
      g_soc_status &= ~SOC_METER_STANDBY;
      g_soc_status &= ~SOC_METER_CHARGE;
      g_soc_status &= ~SOC_METER_DISCHARGE;
      g_soc_status |= SOC_METER_ERROR;
      
      if((fault_flags & FAULT_EXT_OVER_TEMP_IN_CHARGE) && (fault_flags & FAULT_EXT_OVER_TEMP_IN_DISCHARGE) || (fault_flags & FAULT_THERMALRUNWAY))
      {
        g_soc_status |= SOC_METER_OT;
      }
      else if(fault_flags & FAULT_EXT_OVER_TEMP_IN_CHARGE)
      {
        g_soc_status |= SOC_METER_OT;
        g_soc_status |= SOC_METER_CHG_OC;
      }
      
      else if(fault_flags & FAULT_EXT_OVER_TEMP_IN_DISCHARGE)
      {
        g_soc_status |= SOC_METER_OT;
        g_soc_status |= SOC_METER_DISCHG_OC;
      }
      
      if(fault_flags & FAULT_UNDER_TEMP)
      {
       g_soc_status |= SOC_METER_OT;
      }
      
      if(fault_flags & FAULT_OVERCHARGE)
      { 
        g_soc_status |= SOC_METER_OC;
        g_soc_status |= SOC_METER_CHG_OC;
      }

      if(fault_flags & FAULT_OVERDISCHARGE)
      { 
        g_soc_status |= SOC_METER_OC;
        g_soc_status |= SOC_METER_DISCHG_OC;
      }      
    }
  
    // ---- Format as JSON ----
    snprintf(jsonBuffer, sizeof(jsonBuffer),
             "{\"V\":%d,\"I\":%d,\"S\":%d,\"T\":%d,\"F\":%d}",
             volt_scaled/100, current_scaled*10, (uint8_t)(b.soc * 100.0),oz3717_data.max_temperature_data/10,g_soc_status);

    // ---- Send JSON string ----
    App_UartTransmitPoll(M0P_UART0, (uint8_t*)jsonBuffer, strlen(jsonBuffer));

    // Optionally send newline so receiver can split frames
    char newline[2] = "\n";
    App_UartTransmitPoll(M0P_UART0, (uint8_t*)newline, strlen(newline));

    // Small delay
    vTaskDelay(pdMS_TO_TICKS(100));
 
}



void App_BleSendJsonData(void)
{
	char jsonBuffer[200];   // buffer for JSON string
	int len = 0;

	// Start JSON with key "C" (cell voltages)
	len = snprintf(jsonBuffer, sizeof(jsonBuffer), "{\"C\":[");

	// Append 16 cell voltages
	for (int i = 0; i < 16; i++) {
		len += snprintf(jsonBuffer + len, sizeof(jsonBuffer) - len,
				"%u%s", oz3717_data.cell_volt[i], (i < 15) ? "," : "");
	}

	// Add comma and start temperatures
	len += snprintf(jsonBuffer + len, sizeof(jsonBuffer) - len, "],\"T\":[");

	// Copy onboard temperature into slot 4
	oz3717_data.external_temperature[4] = oz3717_data.onBoard_tempertaure_data;

	// Append 5 temperatures
	for (int i = 0; i < 5; i++) {
		len += snprintf(jsonBuffer + len, sizeof(jsonBuffer) - len,
				"%d%s", oz3717_data.external_temperature[i], (i < 4) ? "," : "");
	}

	// Add pack voltage and current
   uint8_t l_soc = (uint8_t)(b.soc * 100.0);
   uint8_t l_soh = (uint8_t)(b.soh * 100.0);
	 uint8_t l_cc  = (uint8_t)cycle_count_3;
	//last est time

	len += snprintf(jsonBuffer + len, sizeof(jsonBuffer) - len,
			"],\"P\":%u,\"I\":%d,\"F\":%d,\"M\":%d,\"S\":%d,\"H\":%d,\"c\":%d}", volt_scaled, current_scaled,fault_flags,oz3717_data.fetCtrl,l_soc,l_soh,l_cc);

	// Append newline
	len += snprintf(jsonBuffer + len, sizeof(jsonBuffer) - len, "\n");

	// Send JSON over UART     
  App_UartTransmitPoll(M0P_UART2, (uint8_t*)jsonBuffer, len);
 
}


void App_Bleoriginsettings(void)
{
    char jsonBuffer[220];
    int len = 0;

    /* Basic info (static) */
    const char *n  = "SmartBMS_V1";
    const char *dm = "BMS-58V";
    const char *v  = "1.0.3";
    const char *bm = "OZ3717";
    const char *id = "DON132";
    const char *nc = "105Ah";
  
  
  
    const int SIZE = 150;   // EXACT size of all parameters
    uint8_t buf[SIZE];

    // Read EXACT bytes written
    for (int i = 0; i < SIZE; i++)
    {
        if (eepromInterface_readByte(0x4000 + i, &buf[i]) != 0)
            return ;
    }
    
    
        // -------- more 16-bit fields --------
    bmsCfg.cellOpenVoltage = (uint16_t)((buf[80] << 8) | buf[81]);
    bmsCfg.balanceDriftVoltage = (uint16_t)((buf[82] << 8) | buf[83]);
    
     bmsCfg.LCell_UV_set = (uint16_t)((buf[66] << 8) | buf[67]); 
    bmsCfg.LCell_UV_rel = (uint16_t)((buf[68] << 8) | buf[69]); 
   bmsCfg.LCell_UV_delay = (uint16_t)((buf[70] << 8) | buf[71]);  

   l_cell_balance_start = bmsCfg.cellOpenVoltage;
   l_cell_balance_delta= bmsCfg.balanceDriftVoltage;
    
   cell_diff_volt =  bmsCfg.LCell_UV_set;
   cell_diff_volt_rel = bmsCfg.LCell_UV_delay;
      
    /* Battery info */
//      uint16_t nc  = (uint16_t)bmsCfg.nominalCapacity/1000 ;  // mAh
//      uint16_t cc  = (uint16_t) bmsCfg.cycleCapacity/1000;  // mAh
//      uint16_t fcc = 150;  // mAh
//      uint8_t  cn  = 16;

    /* Cell balancing settings */
      uint16_t bVs = bmsCfg.cellOpenVoltage;  // Balance start voltage (mV)
      uint16_t bVd = bmsCfg.balanceDriftVoltage;    // Balance delta voltage (mV)
      uint16_t Cd = bmsCfg.LCell_UV_set;  // Balance start voltage (mV)
      uint16_t Cr =  bmsCfg.LCell_UV_delay;    // Balance delta voltage (mV)

    /* Build compact JSON */
len = snprintf(jsonBuffer, sizeof(jsonBuffer),
    "{"
    "\"N\":\"%s\","
    "\"DM\":\"%s\","
    "\"V\":\"%s\","
    "\"BM\":\"%s\","
    "\"ID\":\"%s\","
    "\"NC\":%s,"
    "\"bVs\":%u,"
    "\"bVd\":%u,"
    "\"Cd\":%u,"
    "\"Cr\":%u"
    "}\n",
    n, dm, v, bm, id,
    nc,
    bVs, bVd, Cd, Cr
);


    /* Send over UART (BLE module) */
    App_UartTransmitPoll(M0P_UART2, (uint8_t *)jsonBuffer, len);

}


void App_BleProtectionSettings(void)
{
    char jsonBuffer[220];
    int len = 0;

  
      const int SIZE = 150;   // EXACT size of all parameters
    uint8_t buf[SIZE];

    // Read EXACT bytes written
    for (int i = 0; i < SIZE; i++)
    {
        if (eepromInterface_readByte(0x4000 + i, &buf[i]) != 0)
            return ;
    }
 
    // -------- First 16-bit groups -------
 bmsCfg.Cell_OV_set  = (uint16_t)((buf[0] << 8) | buf[1]);
 bmsCfg.Cell_OV_rel = (uint16_t)((buf[2] << 8) | buf[3]);
 bmsCfg.Cell_OV_delay = (uint16_t)((buf[4] << 8) | buf[5]);
   
 bmsCfg.Cell_UV_set = (uint16_t)((buf[6] << 8) | buf[7]);
 bmsCfg.Cell_UV_rel = (uint16_t)((buf[8] << 8) | buf[9]);
 bmsCfg.Cell_UV_delay = (uint16_t)((buf[10] << 8) | buf[11]);
 
 bmsCfg.Pack_OV_set= (uint16_t)((buf[12] << 8) | buf[13]);
 bmsCfg.Pack_OV_rel= (uint16_t)((buf[14] << 8) | buf[15]);
 bmsCfg.Pack_OV_delay= (uint16_t)((buf[16] << 8) | buf[17]);
 
 bmsCfg.Pack_UV_set= (uint16_t)((buf[18] << 8) | buf[19]);
 bmsCfg.Pack_UV_rel= (uint16_t)((buf[20] << 8) | buf[21]);
 bmsCfg.Pack_UV_delay= (uint16_t)((buf[22] << 8) | buf[23]);
    
    
   l_cell_over_volt = bmsCfg.Cell_OV_set;
   cellovervoltagerecovery = bmsCfg.Cell_OV_rel;
   fault_table[F_CELL_OV].recovery_delay_ms = (bmsCfg.Cell_OV_delay *1000)+5000;  // load runtime value

   l_cell_under_volt = bmsCfg.Cell_UV_set;
   cellundervoltagerecovery = bmsCfg.Cell_UV_rel;
   fault_table[F_CELL_UV].recovery_delay_ms = (bmsCfg.Cell_UV_delay *1000)+5000;  // load runtime value
   

   l_pack_overVolt =  bmsCfg.Pack_OV_set;
   packOverVoltRecovery = bmsCfg.Pack_OV_rel;
   fault_table[F_PACK_OV].recovery_delay_ms   = (bmsCfg.Pack_OV_delay*1000) + 9000;

   l_pack_underVolt = bmsCfg.Pack_UV_set;
   packUnderVoltRecovery = bmsCfg.Pack_UV_rel;
   fault_table[F_PACK_UV].recovery_delay_ms  = (bmsCfg.Pack_UV_delay *1000) + 9000;
    /* Protection parameters (short names) */
      uint16_t c_ov_set   =  bmsCfg.Cell_OV_set;  // Cell OV set (mV)
      uint16_t c_ov_rel   =  bmsCfg.Cell_OV_rel;  // Cell OV release (mV)
      uint16_t c_ov_del   =  bmsCfg.Cell_OV_delay;     // Cell OV delay (s)

      uint16_t c_uv_set   =  bmsCfg.Cell_UV_set;  // Cell UV set (mV)
      uint16_t c_uv_rel   =  bmsCfg.Cell_UV_rel;  // Cell UV release (mV)
      uint16_t c_uv_del   =  bmsCfg.Cell_UV_delay;     // Cell UV delay (s)

      uint16_t p_ov_set   = bmsCfg.Pack_OV_set; // Pack OV set (mV)
      uint16_t p_ov_rel   = bmsCfg.Pack_OV_rel; // Pack OV release (mV)
      uint16_t p_ov_del   = bmsCfg.Pack_OV_delay;     // Pack OV delay (s)

      uint16_t p_uv_set   =  bmsCfg.Pack_UV_set; // Pack UV set (mV)
      uint16_t p_uv_rel   = bmsCfg.Pack_UV_rel; // Pack UV release (mV)
      uint16_t p_uv_del   = bmsCfg.Pack_UV_delay;     // Pack UV delay (s)

    /* Build compact JSON */
    len = snprintf(jsonBuffer, sizeof(jsonBuffer),
        "{"
        "\"cOVs\":%u,\"cOVr\":%u,\"cOVd\":%u,"
        "\"cUVs\":%u,\"cUVr\":%u,\"cUVd\":%u,"
        "\"pOVs\":%u,\"pOVr\":%u,\"pOVd\":%u,"
        "\"pUVs\":%u,\"pUVr\":%u,\"pUVd\":%u"
        "}\n",
        c_ov_set, c_ov_rel, c_ov_del,
        c_uv_set, c_uv_rel, c_uv_del,
        p_ov_set, p_ov_rel, p_ov_del,
        p_uv_set, p_uv_rel, p_uv_del
    );

    /* Send over UART (BLE module) */
    App_UartTransmitPoll(M0P_UART2, (uint8_t *)jsonBuffer, len);

}


//void App_BleProtectionSettings(void)
//{
//    char jsonBuffer[260];
//    int len = 0;

//    /* Voltage protection parameters */
//    const uint16_t cOVs = 4200, cOVr = 4100, cOVd = 5;
//    const uint16_t cUVs = 3000, cUVr = 3100, cUVd = 5;

//    const uint16_t pOVs = 58000, pOVr = 57000, pOVd = 5;
//    const uint16_t pUVs = 42000, pUVr = 43000, pUVd = 5;

//    /* Balancing parameters */
//    const uint16_t bVs  = 3400;   // Balance start voltage (mV)
//    const uint16_t bVd  = 30;     // Balance delta (mV)

//    /* Build compact JSON */
//    len = snprintf(jsonBuffer, sizeof(jsonBuffer),
//        "{"
//        "\"cOVs\":%u,\"cOVr\":%u,\"cOVd\":%u,"
//        "\"cUVs\":%u,\"cUVr\":%u,\"cUVd\":%u,"
//        "\"pOVs\":%u,\"pOVr\":%u,\"pOVd\":%u,"
//        "\"pUVs\":%u,\"pUVr\":%u,\"pUVd\":%u,"
//        "\"bVs\":%u,\"bVd\":%u"
//        "}\n",
//        cOVs, cOVr, cOVd,
//        cUVs, cUVr, cUVd,
//        pOVs, pOVr, pOVd,
//        pUVs, pUVr, pUVd,
//        bVs,  bVd
//    );

//    /* Send over UART (BLE module) */
//    App_UartTransmitPoll(M0P_UART2, (uint8_t *)jsonBuffer, len);
//    vTaskDelay(pdMS_TO_TICKS(100));
//}



void App_BleProtectioncount(void) 
{
    char jsonBuffer[200]; // smaller buffer
    int len = 0;

    /* Protection counts (short keys) */
    const uint16_t counts[] = {
        3, // SC  - Short-circuit
        7, // CH_OC - Charge overcurrent
        5, // DS_OC - Discharge overcurrent
        6, // C_OV - Cell overvoltage
        4, // C_UV - Cell undervoltage
        2, // CH_OT - Charge over temp
        1, // CH_UT - Charge under temp
        2, // DS_OT - Discharge over temp
        0, // DS_UT - Discharge under temp
        2, // P_OV - Pack overvoltage
        1, // P_UV - Pack undervoltage
        0  // SYS_RST - System restart
    };

    const char *keys[] = {
        "SC","CH_OC","DS_OC",
        "C_OV","C_UV",
        "CH_OT","CH_UT",
        "DS_OT","DS_UT",
        "P_OV","P_UV",
        "SYS_RST"
    };

    /* Build JSON */
    len = snprintf(jsonBuffer, sizeof(jsonBuffer), "{");
    for(int i=0; i<12; i++) {
        len += snprintf(jsonBuffer+len, sizeof(jsonBuffer)-len,
                        "\"%s\":%u%s", keys[i], counts[i], (i<11)?",":"");
    }
    len += snprintf(jsonBuffer+len, sizeof(jsonBuffer)-len, "}\n");

    /* Send over UART (BLE module) */
    App_UartTransmitPoll(M0P_UART2, (uint8_t *)jsonBuffer, len);
}


void App_BleCurrentProtectionSettings(void)
{
    char buf2[200];
    int len = 0;
  
     const int SIZE = 150;   // EXACT size of all parameters
    uint8_t buf[SIZE];

    // Read EXACT bytes written
    for (int i = 0; i < SIZE; i++)
    {
        if (eepromInterface_readByte(0x4000 + i, &buf[i]) != 0)
            return ;
    }
   
    bmsCfg.Chg_OC_set= (uint16_t)((buf[42] << 8) | buf[43]);
    bmsCfg.Chg_OC_rel= (uint16_t)((buf[44] << 8) | buf[45]);
    bmsCfg.Chg_OC_delay= (uint16_t)((buf[46] << 8) | buf[47]);
 
    bmsCfg.Dsg_OC2_set= (uint16_t)((buf[48] << 8) | buf[49]); 
    bmsCfg.Dsg_OC2_rel= (uint16_t)((buf[50] << 8) | buf[51]); 
    bmsCfg.Dsg_OC2_delay= (uint16_t)((buf[52] << 8) | buf[53]);

    bmsCfg.SC_set= (uint16_t)((buf[54] << 8) | buf[55]); 
    bmsCfg.SC_rel= (uint16_t)((buf[56] << 8) | buf[57]); 
    bmsCfg.SC_delay= (uint16_t)((buf[58] << 8) | buf[59]); 
    
    bmsCfg.Dsg_OC_set = (uint16_t)((buf[99] << 8) | buf[100]);
    bmsCfg.Dsg_OC_rel  = (uint16_t)((buf[101] << 8) | buf[102]);
    bmsCfg.Dsg_OC_delay  = (uint16_t)((buf[103] << 8) | buf[104]);

   l_disChgCurrent = (bmsCfg.Dsg_OC_set *1000); // 65 A
   fault_table[F_DSG_OC].recovery_delay_ms  =  bmsCfg.Dsg_OC_rel+5000;   
   fault_table[F_DSG_OC].detect_debounce_ms = (bmsCfg.Dsg_OC_delay*1000);
    
   l_ChgCurrent = (bmsCfg.Chg_OC_set * 1000);    // 40 A
   fault_table[F_CHG_OC].recovery_delay_ms  = bmsCfg.Chg_OC_rel+5000;  
   fault_table[F_CHG_OC].detect_debounce_ms = (bmsCfg.Chg_OC_delay*1000);
   
   doc2_p =  bmsCfg.Dsg_OC2_set;
   doc2_delay = bmsCfg.Dsg_OC2_delay;
   
   sc_p =  bmsCfg.SC_set;
   fault_table[F_SHORTCIRCUIT].recovery_delay_ms = (bmsCfg.SC_rel *1000)+5000;
   sc_delay = bmsCfg.SC_delay; 
   
   
   if(doc2_p >= 50 || sc_p >= 2)
   {
     App_advance_Doc2_setting();
   }  
   
    /* Current protection parameters (ms for delays) */
      uint16_t CCP = bmsCfg.Chg_OC_set, CCR = bmsCfg.Chg_OC_rel/1000, CCD = bmsCfg.Chg_OC_delay;   // limits mA
      uint16_t DCP =  bmsCfg.Dsg_OC_set, DCR = bmsCfg.Dsg_OC_rel/1000, DCD = bmsCfg.Dsg_OC_delay;   // charge delays ms
      uint16_t D2P = bmsCfg.Dsg_OC2_set,  D2D = bmsCfg.Dsg_OC2_delay;              // discharge delays ms
      uint16_t SCP =  bmsCfg.SC_set, SCR =  bmsCfg.SC_rel, SCD =  bmsCfg.SC_delay;     // short-circuit params

    /* Build compact JSON */
    len = snprintf(buf2, sizeof(buf2),
        "{"
        "\"CCP\":%u,\"CCR\":%u,\"CCD\":%u,"
        "\"DCP\":%u,\"DCR\":%u,\"DCD\":%u,"
        "\"D2P\":%u,\"D2D\":%u,"
        "\"SCP\":%u,\"SCR\":%u,\"SCD\":%u"
        "}\n",
        CCP, CCR, CCD,
        DCP, DCR, DCD,
        D2P, D2D,
        SCP, SCR, SCD
    );

    /* Send over UART (BLE module) */
    App_UartTransmitPoll(M0P_UART2, (uint8_t *)buf2, len);
}



void App_BleTemperatureProtectionSettings(void)
{
    char buf1[200];
    int len = 0;
  
     const int SIZE = 150;   // EXACT size of all parameters
    uint8_t buf[SIZE];

    // Read EXACT bytes written
    for (int i = 0; i < SIZE; i++)
    {
        if (eepromInterface_readByte(0x4000 + i, &buf[i]) != 0)
            return ;
    }

 
 bmsCfg.Chg_UT_set= (uint16_t)((buf[24] << 8) | buf[25]);
 bmsCfg.Chg_UT_rel= (uint16_t)((buf[26] << 8) | buf[27]);
 bmsCfg.Chg_UT_delay= (uint16_t)((buf[28] << 8) | buf[29]);

 bmsCfg.Dsg_OT_set= (uint16_t)((buf[30] << 8) | buf[31]);
 bmsCfg.Dsg_OT_rel= (uint16_t)((buf[32] << 8) | buf[33]);
 bmsCfg.Dsg_OT_delay= (uint16_t)((buf[34] << 8) | buf[35]);


 bmsCfg.Chg_OT_set= (uint16_t)((buf[36] << 8) | buf[37]);
 bmsCfg.Chg_OT_rel= (uint16_t)((buf[38] << 8) | buf[39]);
 bmsCfg.Chg_OT_delay= (uint16_t)((buf[40] << 8) | buf[41]);
    
    bmsCfg.Dsg_UT_set= (uint16_t)((buf[105] << 8) | buf[106]);
    bmsCfg.Dsg_UT_rel= (uint16_t)((buf[107] << 8) | buf[108]);
    bmsCfg.Dsg_UT_delay= (uint16_t)((buf[109] << 8) | buf[110]);
    
   l_max_chg_temp =  (bmsCfg.Chg_OT_set*100);
   l_max_chg_temp_recovery =  (bmsCfg.Chg_OT_rel *100);
   fault_table[F_CHG_OT].recovery_delay_ms  = (bmsCfg.Chg_OT_delay *1000)+5000;
    
   l_min_chg_temp =(bmsCfg.Chg_UT_set*100);
   l_min_chg_temp_recovery =(bmsCfg.Chg_UT_rel*100);
   fault_table[F_CHG_UT].recovery_delay_ms  = (bmsCfg.Chg_UT_delay *1000)+5000;
  
   l_max_dis_temp = (bmsCfg.Dsg_OT_set *100);
   l_max_dis_temp_recovery = (bmsCfg.Dsg_OT_rel*100); 
   fault_table[F_DSG_OT].recovery_delay_ms  = (bmsCfg.Dsg_OT_delay *1000)+5000;
   
   l_min_dis_temp =(bmsCfg.Dsg_UT_set*100);
   l_min_dis_temp_recovery =(bmsCfg.Dsg_UT_rel*100);
   fault_table[F_DSG_UT].recovery_delay_ms  = (bmsCfg.Dsg_UT_delay *1000)+5000;
   

      uint16_t  CUTS =  bmsCfg.Chg_UT_set,  CUTR =  bmsCfg.Chg_UT_rel;     // Charge Under Temp set / release
      uint16_t CUTD =  bmsCfg.Chg_UT_delay;              // Charge Under Temp delay (ms)

      uint16_t COTS = bmsCfg.Chg_OT_set,  COTR = bmsCfg.Chg_OT_rel;    // Charge Over Temp set / release
      uint16_t COTD = bmsCfg.Chg_OT_delay;              // Charge Over Temp delay (ms)

      uint16_t  DUTS = bmsCfg.Dsg_UT_set, DUTR = bmsCfg.Dsg_UT_rel;    // Discharge Under Temp set / release
      uint16_t DUTD = bmsCfg.Dsg_UT_delay;              // Discharge Under Temp delay (ms)

      uint16_t DOTS = bmsCfg.Dsg_OT_set,  DOTR = bmsCfg.Dsg_OT_rel;    // Discharge Over Temp set / release
      uint16_t DOTD = bmsCfg.Dsg_OT_delay;              // Discharge Over Temp delay (ms)

    /* Build compact JSON */
    len = snprintf(buf1, sizeof(buf1),
        "{"
        "\"CUTS\":%d,\"CUTR\":%d,\"CUTD\":%u,"
        "\"COTS\":%u,\"COTR\":%u,\"COTD\":%u,"
        "\"DUTS\":%d,\"DUTR\":%d,\"DUTD\":%u,"
        "\"DOTS\":%u,\"DOTR\":%u,\"DOTD\":%u"
        "}\n",
        CUTS, CUTR, CUTD,
        COTS, COTR, COTD,
        DUTS, DUTR, DUTD,
        DOTS, DOTR, DOTD
    );

    /* Send over UART (BLE module) */
    App_UartTransmitPoll(M0P_UART2, (uint8_t *)buf1, len);
}







/******************************************SOC-SOH-CC-EST-TIME****************************************************************************/

// Initialize BMS
void bms_init(SimpleBMS *b, double nominal_capacity_ah,
              double initial_soc, double initial_soh,
              double v_full_threshold, double v_empty_threshold)
{
    b->nominal_capacity_ah = nominal_capacity_ah;
    b->soc = initial_soc;//MIN(MAX(initial_soc, 0.0), 1.0);
    b->soh = MIN(MAX(initial_soh, 0.0), 1.0);
    b->coulomb_acc_ah = b->soc * b->nominal_capacity_ah;
    b->last_avg_current_a = 0.0;
    b->current_ma_window_s = 30.0; //30
    b->in_discharge_cycle = false;
    b->cycle_discharge_ah = 0.0;
    b->soh_ema_alpha = 0.05;
    b->cycle_count = 0;
    b->charged_ah_accum = 0.0;
    b->v_full_threshold = v_full_threshold;
    b->v_empty_threshold = v_empty_threshold;
    b->voltage_idle_time_s = 60.0;
    b->idle_time_accum_s = 0.0;
    b->current_snap_threshold_a = 0.1;
}

// Update moving average current
static void update_avg_current(SimpleBMS *b, double current_a, double dt_s)
{
    if (b->current_ma_window_s <= 0.0) {
        b->last_avg_current_a = current_a;
        return;
    }
    double alpha = dt_s / (b->current_ma_window_s + dt_s);
    b->last_avg_current_a = (1.0 - alpha) * b->last_avg_current_a + alpha * current_a;
}

// Update BMS: call every tick
void bms_update(SimpleBMS *b, double current_a, double dt_s, double pack_voltage)
{
    if (dt_s <= 0.0) return;

    // Update smoothed current
    update_avg_current(b, current_a, dt_s);

    // Integrate coulomb counting (Ah)
    double delta_ah = (current_a * dt_s) / 3600.0;
    b->coulomb_acc_ah += delta_ah;

    // Update SOC
    double raw_soc = b->coulomb_acc_ah / b->nominal_capacity_ah;
    b->soc = MIN(MAX(raw_soc, 0.0), 1.0);

    // --- Equivalent Full Cycle (EFC) tracking ---
    if(current_a > 0.05) {
        // Charging
        b->charged_ah_accum += (current_a * dt_s) / 3600.0;
    } else if(current_a < -0.05) {
        // Discharging
        b->cycle_discharge_ah += (-current_a * dt_s) / 3600.0;
    }

    // Check if full cycle completed
    double efc_ah = b->charged_ah_accum + b->cycle_discharge_ah;
    if(efc_ah >= b->nominal_capacity_ah) {
        b->cycle_count++;
        b->charged_ah_accum = 0.0;
        b->cycle_discharge_ah = 0.0;
    }

    // Voltage-based SOC correction
    if (fabs(b->last_avg_current_a) < b->current_snap_threshold_a) {
        b->idle_time_accum_s += dt_s;
    } else {
        b->idle_time_accum_s = 0.0;
    }

    if (b->idle_time_accum_s >= b->voltage_idle_time_s) {
        if (pack_voltage >= b->v_full_threshold) {
            b->soc = 1.0;
            b->coulomb_acc_ah = b->nominal_capacity_ah;
            b->idle_time_accum_s = 0.0;
        } else if (pack_voltage <= b->v_empty_threshold) {
            b->soc = 0.0;
            b->coulomb_acc_ah = 0.0;
            b->idle_time_accum_s = 0.0;
        }
    }

    b->soc =  b->soc; 
    b->soh = MIN(MAX(b->soh, 0.0), 2.0);
}

// Get SOC %
double bms_get_soc_percent(const SimpleBMS *b) { return b->soc * 100.0; }

// Get SOH %
double bms_get_soh_percent(const SimpleBMS *b) { return b->soh * 100.0; }

// Estimate time remaining (seconds)
double bms_estimate_time_s(const SimpleBMS *b, double current_a)
{
    if (fabs(current_a) < 0.01) return INFINITY; // near zero ? idle

    double usable_capacity_ah = b->nominal_capacity_ah * MIN(MAX(b->soh, 0.0), 1.0);

    if (current_a > 0.0) { // charging
        double to_full_ah = usable_capacity_ah * (1.0 - b->soc);
        return to_full_ah / current_a * 3600.0; // seconds
    } else { // discharging
        double remaining_ah = usable_capacity_ah * b->soc;
        return remaining_ah / fabs(current_a) * 3600.0;
    }
}


typedef struct {
    uint16_t voltage_mV;  // millivolts
    double soc;           // normalized SOC (0.0 to 1.0)
} VoltageSOCMap;

static const VoltageSOCMap lfp_soc_table[] = {
    {2800, 0.00},
    {2900, 0.05},
    {3000, 0.10},
    {3100, 0.20},
    {3200, 0.40},
    {3250, 0.50},
    {3300, 0.60},
    {3350, 0.70},
    {3400, 0.80},
    {3450, 0.90},
    {3500, 0.95},
    {3600, 0.98},
    {3700, 1.00}
};

double Get_SOC_From_Voltage(uint16_t voltage_mV)
{
    // Below minimum
    if (voltage_mV <= lfp_soc_table[0].voltage_mV)
        return lfp_soc_table[0].soc;

    // Above maximum
    if (voltage_mV >= lfp_soc_table[LFP_TABLE_SIZE - 1].voltage_mV)
        return lfp_soc_table[LFP_TABLE_SIZE - 1].soc;

    // Find the voltage range and interpolate
    for (uint8_t i = 0; i < LFP_TABLE_SIZE - 1; i++)
    {
        uint16_t v1 = lfp_soc_table[i].voltage_mV;
        uint16_t v2 = lfp_soc_table[i + 1].voltage_mV;
        double s1 = lfp_soc_table[i].soc;
        double s2 = lfp_soc_table[i + 1].soc;

        if (voltage_mV >= v1 && voltage_mV <= v2)
        {
            // Linear interpolation (double precision)
            return s1 + ((double)(voltage_mV - v1) * (s2 - s1)) / (double)(v2 - v1);
        }
    }
    return 0.0; // fallback
}


uint32_t app_getTick(void)
{
  return xTaskGetTickCount() * portTICK_PERIOD_MS;
}
 

 
 /* ---------- Runtime snapshot (exported) ---------- */
 bms_faults_snapshot_t g_bms_faults_snapshot;
//extern bms_faults_snapshot_t g_bms_faults_snapshot;
/* Inline helpers */
static inline void set_bit(fault_mask_t *m, int id) { *m |= (1u << id); }
static inline void clear_bit(fault_mask_t *m, int id) { *m &= ~(1u << id); }
static inline bool test_bit(fault_mask_t m, int id) { return ((m >> id) & 1u) != 0; }

/* Forward */
static void apply_system_actions(void);
static void Bms_UpdateFaultFlags(void);
void bms_faults_init(void);
void bms_faults_periodic(void);
static void bms_faults_force_clear_latched(fault_mask_t mask);
void bms_faults_get_masks(fault_mask_t *active, fault_mask_t *latched);
static  char* bms_fault_name(fault_id_t id);

static bool hw_persist_read_latched(fault_mask_t *out_mask);
static bool hw_persist_write_latched(fault_mask_t mask);
static void hw_log(  char *fmt, ...);

static uint32_t hw_millis(void);

static void hw_disable_charging(void);
static void hw_disable_discharging(void);
static void hw_enable_charging(void);
static void hw_enable_discharging(void);



uint8_t hw_cell_overvoltage_condition(void);
uint8_t hw_cell_undervoltage_condition(void);
uint8_t hw_pack_overvoltage_condition(void);
uint8_t hw_pack_undervoltage_condition(void);
uint8_t hw_charge_overtemp_condition(void);
uint8_t hw_charge_undertemp_condition(void);
uint8_t hw_discharge_overtemp_condition(void);
uint8_t hw_discharge_undertemp_condition(void);
uint8_t hw_charge_overcurrent_condition(void);
uint8_t hw_discharge_overcurrent_condition(void);
uint8_t hw_shortcircuit_condition(void);
uint8_t hw_afe_error_condition(void);
uint8_t hw_soft_lock_condition(void);
uint8_t hw_hg_mos_damage_condition(void);
uint8_t hw_sg_mos_damage_condition(void);
uint8_t hw_mos_overtemp_condition(void);



uint8_t hw_hg_mos_damage_condition(void)
{
	static uint8_t hg_latched = 0;

	if (((oz3717_data.max_cell_data - oz3717_data.min_cell_data) >= cell_diff_volt) && (oz3717_data.min_cell_data <= 5000 && oz3717_data.max_cell_data <= 5000)) // cell diff
	{
		if (!hg_latched)
		{
			fCounts[12]++;
			hg_latched = 1;
		}
		return 1;
	}
	else if(((oz3717_data.max_cell_data - oz3717_data.min_cell_data) <= cell_diff_volt_rel)  && (oz3717_data.min_cell_data <= 5000 && oz3717_data.max_cell_data <= 5000))
	{
		hg_latched = 0;
		return 0;
	}
  
  return 2;
}


uint8_t hw_cell_undervoltage_condition(void)
{
    static uint8_t uv_latched = 0;

    // -------- Undervoltage detection --------
    if (oz3717_data.min_cell_data <= l_cell_under_volt)
    {
        if (uv_latched == 0)
        {
            fCounts[0]++;      // increment ONLY ONCE
            uv_latched = 1;   // latch UV event
        }
        return 1;
    }

    // -------- Recovery condition --------
    if (oz3717_data.min_cell_data >= cellundervoltagerecovery &&
        oz3717_data.min_cell_data <= 5000)
    {
        uv_latched = 0;       // clear latch on recovery
        return 0;
    }

    return 2;
}
 
uint8_t hw_cell_overvoltage_condition(void)
{
	static uint8_t ov_latched = 0;

	if ((oz3717_data.max_cell_data >= l_cell_over_volt && oz3717_data.max_cell_data < 5100) ||
	    ((oz3717_data.status1 & OZ3717_OVP_FLAG) == 1))
	{
		if (!ov_latched)
		{
			fCounts[1]++;
			ov_latched = 1;
		}
		return 1;
	}

	if (oz3717_data.max_cell_data <= cellovervoltagerecovery)
	{
		ov_latched = 0;
		return 0;
	}
	return 2;
}
 
uint8_t hw_pack_overvoltage_condition(void)
{
	static uint8_t pov_latched = 0;

	if (oz3717_data.vbat >= l_pack_overVolt)
	{
		if (!pov_latched)
		{
			fCounts[2]++;
			pov_latched = 1;
		}
		return 1;
	}
	else if (oz3717_data.vbat < packOverVoltRecovery)
	{
		pov_latched = 0;
		return 0;
	}
	return 3;
}

 
uint8_t hw_pack_undervoltage_condition(void)
{
	static uint8_t puv_latched = 0;

	if (oz3717_data.vbat <= l_pack_underVolt)
	{
		if (!puv_latched)
		{
			fCounts[3]++;
			puv_latched = 1;
		}
		return 1;
	}
	else if (oz3717_data.vbat >= packUnderVoltRecovery)
	{
		puv_latched = 0;
		return 0;
	}
	return 3;
}

 
uint8_t hw_charge_overtemp_condition(void)
{
	static uint8_t chg_ot_latched = 0;

	if (oz3717_data.max_temperature_data >= l_max_chg_temp)
	{
		if (!chg_ot_latched)
		{
			fCounts[4]++;
			chg_ot_latched = 1;
		}
		return 1;
	}
	else if (oz3717_data.max_temperature_data <= l_max_chg_temp_recovery)
	{
		chg_ot_latched = 0;
		return 0;
	}
	return 3;
}
 

uint8_t hw_discharge_overtemp_condition(void)
{
	static uint8_t dis_ot_latched = 0;

	if (oz3717_data.max_temperature_data >= l_max_dis_temp)
	{
		if (!dis_ot_latched)
		{
			fCounts[5]++;
			dis_ot_latched = 1;
		}
		return 1;
	}
	else if (oz3717_data.max_temperature_data <= l_max_dis_temp_recovery)
	{
		dis_ot_latched = 0;
		return 0;
	}
	return 3;
}

 

uint8_t hw_mos_overtemp_condition(void)
{
	static uint8_t mos_ot_latched = 0;

	if (oz3717_data.onBoard_tempertaure_data >= 8500)
	{
		if (!mos_ot_latched)
		{
			fCounts[6]++;
			mos_ot_latched = 1;
		}
		return 1;
	}
	else if (oz3717_data.onBoard_tempertaure_data < 6500)
	{
		mos_ot_latched = 0;
		return 0;
	}
	return 3;
}

 
uint8_t hw_charge_undertemp_condition(void)
{
	static uint8_t chg_ut_latched = 0;

	if (oz3717_data.min_temperature_data <= l_min_chg_temp)
	{
		if (!chg_ut_latched)
		{
			fCounts[7]++;
			chg_ut_latched = 1;
		}
		return 1;
	}
	else if (oz3717_data.min_temperature_data >= l_min_chg_temp_recovery)
	{
		chg_ut_latched = 0;
		return 0;
	}
	return 3;
}

 
uint8_t hw_discharge_undertemp_condition(void)
{
	static uint8_t dis_ut_latched = 0;

	if (oz3717_data.min_temperature_data <= l_min_dis_temp)
	{
		if (!dis_ut_latched)
		{
			fCounts[8]++;
			dis_ut_latched = 1;
		}
		return 1;
	}
	else if (oz3717_data.min_temperature_data >= l_min_dis_temp_recovery)
	{
		dis_ut_latched = 0;
		return 0;
	}
	return 3;
}

uint8_t hw_soft_lock_condition(void)
{
	static uint8_t softlock_latched = 0;

	if (oz3717_data.external_temperature[0] >= 4000 &&
	    oz3717_data.external_temperature[1] >= 4000 &&
	    oz3717_data.external_temperature[2] >= 4000 &&
	    oz3717_data.external_temperature[3] >= 4000)
	{
		if ((oz3717_data.max_temperature_data -
		     oz3717_data.min_temperature_data) >= 1500)
		{
			if (!softlock_latched)
			{
				fCounts[9]++;
				softlock_latched = 1;
			}
			App_BmsAlertOn();
			return 1;
		}
	}

	if (oz3717_data.external_temperature[0] <= 5000 &&
	    oz3717_data.external_temperature[1] <= 5000 &&
	    oz3717_data.external_temperature[2] <= 5000 &&
	    oz3717_data.external_temperature[3] <= 5000)
	{
		softlock_latched = 0;
		App_BmsAlertOff();
		return 0;
	}
	return 3;
}

 
uint8_t hw_charge_overcurrent_condition(void)
{
	static uint8_t coc_latched = 0;

	if ((App_GetBmsState() == AFE_CHARGE_STATE) &&
	    (oz3717_data.system_current >= l_ChgCurrent))
	{
		if (!coc_latched)
		{
			fCounts[10]++;
			coc_latched = 1;
		}
		return 1;
	}
	else
	{
		coc_latched = 0;
		return 0;
	}
}



extern volatile uint8_t g_ErrorIrqTrigger; 

uint8_t hw_discharge_overcurrent_condition(void)
{
	static uint8_t doc_latched = 0;
	static int32_t i_value;

	i_value = (int32_t)oz3717_data.system_current;
	if (App_GetBmsState() == AFE_DISCHARGE_STATE)
	{
		i_value *= -1;
	}

	if (App_GetBmsState() == AFE_DISCHARGE_STATE &&
	    i_value >= l_disChgCurrent)
	{
		if (!doc_latched)
		{
			fCounts[11]++;
			doc_latched = 1;
		}
		return 1;
	}
	else
	{
		doc_latched = 0;
		return 0;
	}
}

 
 
uint8_t hw_afe_error_condition(void)
{
	static uint8_t afe_latched = 0;

	if ((oz3717_data.min_temperature_data == -27315) ||
	    (oz3717_data.min_cell_data <= 300))
	{
		if (!afe_latched)
		{
			fCounts[13]++;
			afe_latched = 1;
		}
		return 1;
	}
	else
	{
		afe_latched = 0;
		return 0;
	}
}


uint8_t hw_shortcircuit_condition(void)
{
	static uint8_t sc_latched = 0;
	static uint16_t fault_recover;
  static uint8_t sc_event_count = 0;

	if (oz3717_data.status1 & OZ3717_SCP_FLAG)
	{
		if (!sc_latched)
		{
			fCounts[14]++;
			sc_latched = 1;
		}
    sc_event_count++;
        /* Return 1 for first 10 SCP events */
    if (sc_event_count >= 10)
     {            /* Clear SCP flag immediately */
       fault_recover = OZ3717_SCP_FLAG;
       o2m_register_write(OZ3717_STATUS, fault_recover);
       g_ErrorIrqTrigger = 0; 
       sc_event_count = 0;          
     }
		return 1;
	}
	else
	{
		sc_latched = 0;
		return 0;
	}
}

//uint8_t hw_shortcircuit_condition(void)
//{
//    static uint8_t sc_event_count = 0;
//    static uint8_t sc_latched = 0;
//    static uint16_t fault_recover;

//    if (oz3717_data.status1 & OZ3717_SCP_FLAG)
//    {
//        /* Count only once per SCP event */
//        if (!sc_latched)
//        {
//            sc_latched = 1;

//            if (sc_event_count < 10)   // limit count to 10
//            {
//                sc_event_count++;
//                fCounts[14]++;
//            }
//        }

//        /* Return 1 for first 10 SCP events */
//        if (sc_event_count >= 10)
//        {
//                  /* Clear SCP flag immediately */
//          fault_recover = OZ3717_SCP_FLAG;
//          o2m_register_write(OZ3717_STATUS, fault_recover);
//          g_ErrorIrqTrigger = 0;    
//        }
//        
//        return 1;
//    }
//    else
//    {
//        /* Reset latch when SCP clears */
//        sc_latched = 0;
//    }

//    return 0;
//}


uint8_t hw_sg_mos_damage_condition(void)
{
	static uint8_t sg_latched = 0;
	static uint16_t fault_recover;

	if (oz3717_data.status1 & OZ3717_DOC1P_FLAG || oz3717_data.status1 & OZ3717_DOC2P_FLAG)
	{
		if (!sg_latched)
		{
			fCounts[15]++;
			sg_latched = 1;
		}
		fault_recover = OZ3717_DOC1P_FLAG|OZ3717_DOC2P_FLAG;
		o2m_register_write(OZ3717_STATUS, fault_recover);
		g_ErrorIrqTrigger = 0;
		return 1;
	}
	else
	{
		sg_latched = 0;
		return 0;
	}
}

  
bool chg_mos;
bool dsg_mos;
 
 

/* ---------- Fault metadata (tune values here) ---------- */
fault_meta_t fault_table[F_FAULT_COUNT] = {
    [F_CELL_OV]         = {500,  24000,  8,  false, "Cell_OV"}, //time ok l_cov_delay  // also add afe
    [F_CELL_UV]         = {100,  15000,  7,  false, "Cell_UV"}, //timer ok    // also add afe
    [F_PACK_OV]         = {100,  15000,  6,  false, "Pack_OV"}, //
    [F_PACK_UV]         = {500,  15000,  5,  false, "Pack_UV"}, //
    [F_CHG_OT]          = {100,  20000, 6,  false, "Chg_OT"},   // timer ok
    [F_CHG_UT]          = {100,  20000, 6,  false, "Chg_UT"},
    [F_DSG_OT]          = {100,  20000, 6,  false, "Dsg_OT"},
    [F_DSG_UT]          = {100,  20000, 6,  false, "Dsg_UT"},
    [F_CHG_OC]          = {200,   24000,  9,  false, "Chg_OC"}, //time ok
    [F_DSG_OC]          = {200,   24000,  10, false, "Dsg_OC"},
    [F_SHORTCIRCUIT]    = {1,   60000, 15, false,  "SC"},
    [F_AFE_ERROR]       = {100,   24000,     12, false,  "AFE_Error"},  //hw fault 
    [F_SOFT_LOCK]       = {500,  24000,     3,  false,  "Soft_Lock"},  // thermalrunway
    [F_HG_MOS_DAMAGE]   = {100,   24000,     14, false,  "HgMosDam"}, //cell diff
    [F_SG_MOS_DAMAGE]   = {10,   24000,     14, false,  "SgMosDam"},
    [F_MOS_OT]          = {100,  24000,     13, false,  "MOS_OT"},
};

/* Condition function table */
static uint8_t (*condition_checks[F_FAULT_COUNT])(void) = {
    [F_CELL_OV]      = hw_cell_overvoltage_condition,
    [F_CELL_UV]      = hw_cell_undervoltage_condition,
    [F_PACK_OV]      = hw_pack_overvoltage_condition,
    [F_PACK_UV]      = hw_pack_undervoltage_condition,
    [F_CHG_OT]       = hw_charge_overtemp_condition,
    [F_CHG_UT]       = hw_charge_undertemp_condition,
    [F_DSG_OT]       = hw_discharge_overtemp_condition,
    [F_DSG_UT]       = hw_discharge_undertemp_condition,
    [F_CHG_OC]       = hw_charge_overcurrent_condition,
    [F_DSG_OC]       = hw_discharge_overcurrent_condition,
    [F_SHORTCIRCUIT] = hw_shortcircuit_condition,
    [F_AFE_ERROR]    = hw_afe_error_condition,
    [F_SOFT_LOCK]    = hw_soft_lock_condition,
    [F_HG_MOS_DAMAGE]= hw_hg_mos_damage_condition,
    [F_SG_MOS_DAMAGE]= hw_sg_mos_damage_condition,
    [F_MOS_OT]       = hw_mos_overtemp_condition,
};

/* ---------- MOS action mapping (user-provided mapping) ---------- */
typedef enum { MOS_KEEP = 0, MOS_OFF, MOS_ON } mos_action_t;
typedef struct { mos_action_t chg; mos_action_t dsg; } fault_action_t;

static   fault_action_t fault_action_map[F_FAULT_COUNT] = {
    [F_CELL_OV]      = {MOS_OFF, MOS_ON},
    [F_CELL_UV]      = {MOS_ON,  MOS_OFF},
    [F_PACK_OV]      = {MOS_OFF, MOS_ON},
    [F_PACK_UV]      = {MOS_ON,  MOS_OFF},
    [F_CHG_OT]       = {MOS_OFF, MOS_ON},
    [F_CHG_UT]       = {MOS_OFF, MOS_ON},//{MOS_OFF, MOS_ON},
    [F_DSG_OT]       = {MOS_ON,  MOS_OFF},
    [F_DSG_UT]       = {MOS_ON,  MOS_OFF},//{MOS_ON,  MOS_OFF},
    [F_CHG_OC]       = {MOS_OFF, MOS_ON},  
    [F_DSG_OC]       = {MOS_ON,  MOS_OFF},
    [F_SHORTCIRCUIT] = {MOS_OFF, MOS_OFF},
    [F_AFE_ERROR]    = {MOS_OFF, MOS_OFF},
    [F_SOFT_LOCK]    = {MOS_OFF, MOS_OFF},
    [F_HG_MOS_DAMAGE]= {MOS_ON, MOS_OFF}, //in cell diff  {MOS_OFF, MOS_OFF},
    [F_SG_MOS_DAMAGE]= {MOS_ON,  MOS_OFF}, // doc2
    [F_MOS_OT]       = {MOS_OFF, MOS_OFF}
};
 
/* ---------- Aggregation & MOS actions ---------- */
static void apply_system_actions(void) {
    /* default allow both MOS (ON) */
    mos_action_t chg_action = MOS_ON;
    mos_action_t dsg_action = MOS_ON;

    for (int i=0;i<F_FAULT_COUNT;i++) {
        if (!test_bit(g_bms_faults_snapshot.active_mask, i)) continue;
          fault_action_t *act = &fault_action_map[i];
        /* CHG */
        if (act->chg == MOS_OFF) chg_action = MOS_OFF;
        else if (act->chg == MOS_ON && chg_action != MOS_OFF) chg_action = MOS_ON;
        /* DSG */
        if (act->dsg == MOS_OFF) dsg_action = MOS_OFF;
        else if (act->dsg == MOS_ON && dsg_action != MOS_OFF) dsg_action = MOS_ON;
    }

    /* Execute actions (idempotent) */
        // Apply the FET actions using your App_Bms* functions
    Bms_UpdateFaultFlags();
    if(chg_action == MOS_OFF && dsg_action == MOS_OFF)
    {
       chg_mos = 0; 
       dsg_mos = 0;  
      App_BmsChgDsgFetOff();
      vTaskDelay(pdMS_TO_TICKS(100));
    }
    else if(chg_action == MOS_OFF && dsg_action == MOS_ON) 
    {
       chg_mos = 0; 
       dsg_mos = 1;
      App_BmsDsgFetOn();
      vTaskDelay(pdMS_TO_TICKS(100));
    }
    else if(chg_action == MOS_ON && dsg_action == MOS_OFF) 
    {
       chg_mos = 1; 
       dsg_mos = 0;
      App_BmsChgFetOn();
      vTaskDelay(pdMS_TO_TICKS(100));
    }
    else if(chg_action == MOS_ON && dsg_action == MOS_ON)
    {    
       chg_mos = 1; 
       dsg_mos = 1;      
      App_BmsChgDsgFetOn();
      vTaskDelay(pdMS_TO_TICKS(100));
    } 
}


 
 

/* ---------- Public API implementation ---------- */

void bms_faults_init(void) {
    memset(&g_bms_faults_snapshot, 0, sizeof(g_bms_faults_snapshot));
    fault_mask_t latched = 0;
    if (hw_persist_read_latched(&latched)) 
    {
        g_bms_faults_snapshot.latched_mask = latched;
        g_bms_faults_snapshot.detect_start_ms[0] = 0; /* ensure clean */
        g_bms_faults_snapshot.recover_start_ms[0] = 0;
        /* treat latched faults as active on boot so MOS decisions reflect saved latch */
        g_bms_faults_snapshot.active_mask |= latched;
    }
  
uint8_t buf[2];   // temporary buffer for each int16

uint32_t addr = 0x406F;  // starting EEPROM address    
// Read val1
eepromInterface_readByte(addr, &buf[0]);
eepromInterface_readByte(addr + 1, &buf[1]);
offset = (buf[0] << 8) | buf[1];   // combine bytes

// Read val2
eepromInterface_readByte(addr + 2, &buf[0]);
eepromInterface_readByte(addr + 3, &buf[1]);
dsg_gain_q15 = (buf[0] << 8) | buf[1];

// Read val3
eepromInterface_readByte(addr + 4, &buf[0]);
eepromInterface_readByte(addr + 5, &buf[1]);
chg_gain_q15 = (buf[0] << 8) | buf[1];

    
for (uint8_t i = 0; i < 16; i++)
{
    eepromInterface_readByte(0x4075+ i, &fCounts[i]);
}

}

/* Helper: log wrapper (safe) */
static void local_log(  char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    hw_log(fmt, ap); /* note: hw_log must handle va_list or we could implement vprintf wrapper */
    va_end(ap);
}

/* Periodic processing: call in main loop at >= 10ms */
void bms_faults_periodic(void) {
    uint32_t now = hw_millis();

    for (int i=0;i<F_FAULT_COUNT;i++) {
        uint8_t cond = false;
        if (condition_checks[i]) cond = condition_checks[i]();

        uint32_t *detect_start = &g_bms_faults_snapshot.detect_start_ms[i];
        uint32_t *recover_start = &g_bms_faults_snapshot.recover_start_ms[i];
           fault_meta_t *m = &fault_table[i];

        if (cond == 1) {
            if (*detect_start == 0) *detect_start = now;
            else if ((now - *detect_start) >= m->detect_debounce_ms) {
                if (!test_bit(g_bms_faults_snapshot.active_mask, i)) {
                    set_bit(&g_bms_faults_snapshot.active_mask, i);
                          //hw_log("Fault SET: %s\n", m->name);
                   fault_tigger_flag = 1;
                    if (m->sticky) {
                        set_bit(&g_bms_faults_snapshot.latched_mask, i);
                        hw_persist_write_latched(g_bms_faults_snapshot.latched_mask);
                    }
                }
            }
            *recover_start = 0; /* reset recovery timer while condition persists */
        } else if(cond == 0){
            /* not present */
            *detect_start = 0;
            if (test_bit(g_bms_faults_snapshot.active_mask, i)) {
                if (*recover_start == 0) *recover_start = now;
                else {
                    uint32_t elapsed = now - *recover_start;
                    if (m->sticky) {
                        /* do not auto-clear sticky; wait manual clear */
                    } else if (m->recovery_delay_ms == 0 || elapsed >= m->recovery_delay_ms) {
                        clear_bit(&g_bms_faults_snapshot.active_mask, i);
                        *recover_start = 0;
                           //hw_log("Fault CLEARED (auto): %s\n", m->name);
                           for (uint8_t i = 0; i < 16; i++){eepromInterface_writeReverse(0x4075 + i,(int32_t)fCounts[i],1);}
                            eepromInterface_updateEEPROM(0, true, NULL);
                         fault_tigger_flag = 0;
                        /* ensure latched cleared as well */
                        clear_bit(&g_bms_faults_snapshot.latched_mask, i);
                        hw_persist_write_latched(g_bms_faults_snapshot.latched_mask);
                    }
                }
            } else {
                *recover_start = 0;
            }
        }
    }
    apply_system_actions();
}

bool pack_uv_flag;
bool cell_ov_flag;
bool cell_uv_flag;
bool pack_ov_flag;
bool pack_uv_flag;
bool chg_ot_flag;
bool chg_ut_flag;
bool dsg_ot_flag;
bool dsg_ut_flag;
bool chg_oc_flag;
bool dsg_oc_flag;
bool sc_flag;
bool afe_error_flag;
bool soft_lock_flag;
bool hg_mos_damage_flag;
bool mos_ot_flag;
bool sg_mos_damage_flag;   // reserved

static void Bms_UpdateFaultFlags(void)
{
    uint32_t active, latched;
    bms_faults_get_masks(&active, &latched);
 
      cell_ov_flag = (active & (1UL << F_CELL_OV)) ? true : false;
    fault_flags = (cell_ov_flag) ? (fault_flags | FAULT_OVERVOLTAGE) : (fault_flags & ~FAULT_OVERVOLTAGE);

      cell_uv_flag       = (active & (1UL << F_CELL_UV)) ? true : false;
    fault_flags = (cell_uv_flag) ? (fault_flags | FAULT_UNDERVOLTAGE) : (fault_flags & ~FAULT_UNDERVOLTAGE);
  
      pack_ov_flag       = (active & (1UL << F_PACK_OV)) ? true : false;
    fault_flags = (pack_ov_flag) ? (fault_flags | FAULT_OVERPACKVOLTAGE) : (fault_flags & ~FAULT_OVERPACKVOLTAGE);
  
      pack_uv_flag       = (active & (1UL << F_PACK_UV)) ? true : false;
    fault_flags = (pack_uv_flag) ? (fault_flags | FAULT_UNDERPACKVOLTAGE) : (fault_flags & ~FAULT_UNDERPACKVOLTAGE);
  
      chg_ot_flag        = (active & (1UL << F_CHG_OT)) ? true : false;
    fault_flags = (chg_ot_flag) ? (fault_flags | FAULT_EXT_OVER_TEMP_IN_CHARGE) : (fault_flags & ~FAULT_EXT_OVER_TEMP_IN_CHARGE);
  
      chg_ut_flag        = (active & (1UL << F_CHG_UT)) ? true : false;
    fault_flags = (chg_ut_flag) ? (fault_flags | FAULT_TMP_DSG) : (fault_flags & ~FAULT_TMP_DSG);
    
      dsg_ot_flag        = (active & (1UL << F_DSG_OT)) ? true : false;
    fault_flags = (dsg_ot_flag) ? (fault_flags | FAULT_EXT_OVER_TEMP_IN_DISCHARGE) : (fault_flags & ~FAULT_EXT_OVER_TEMP_IN_DISCHARGE);
     
      dsg_ut_flag        = (active & (1UL << F_DSG_UT)) ? true : false;
    fault_flags = (dsg_ut_flag) ? (fault_flags | FAULT_UNDER_TEMP) : (fault_flags & ~FAULT_UNDER_TEMP);
    
     chg_oc_flag        = (active & (1UL << F_CHG_OC)) ? true : false;
    fault_flags = (chg_oc_flag) ? (fault_flags | FAULT_OVERCHARGE) : (fault_flags & ~FAULT_OVERCHARGE);
    
      dsg_oc_flag        = (active & (1UL << F_DSG_OC)) ? true : false;
      fault_flags = (dsg_oc_flag) ? (fault_flags | FAULT_OVERDISCHARGE) : (fault_flags & ~FAULT_OVERDISCHARGE);
 
  
      sc_flag            = (active & (1UL << F_SHORTCIRCUIT)) ? true : false;
    fault_flags = (sc_flag) ? (fault_flags | FAULT_SHORT_CKT) : (fault_flags & ~FAULT_SHORT_CKT);
    
    afe_error_flag     = (active & (1UL << F_AFE_ERROR)) ? true : false;
    fault_flags = (afe_error_flag) ? (fault_flags | FAULT_HARDWARE) : (fault_flags & ~FAULT_HARDWARE);
    
    soft_lock_flag     = (active & (1UL << F_SOFT_LOCK)) ? true : false;
    fault_flags = (soft_lock_flag) ? (fault_flags | FAULT_THERMALRUNWAY) : (fault_flags & ~FAULT_THERMALRUNWAY);
    
     hg_mos_damage_flag = (active & (1UL << F_HG_MOS_DAMAGE)) ? true : false;
    fault_flags = (hg_mos_damage_flag) ? (fault_flags | FAULT_CELLDIFF) : (fault_flags & ~FAULT_CELLDIFF);
        
    
      mos_ot_flag        = (active & (1UL << F_MOS_OT)) ? true : false;
    fault_flags = (mos_ot_flag) ? (fault_flags | FAULT_ONBOARD_OVER_TEMP) : (fault_flags & ~FAULT_ONBOARD_OVER_TEMP);
    
     sg_mos_damage_flag = (active & (1UL << F_SG_MOS_DAMAGE)) ? true : false; // reserved
}
/* Manual clear latched faults: will only clear latched bits for which the condition is not present */
static void bms_faults_force_clear_latched(fault_mask_t mask) {
    for (int i=0;i<F_FAULT_COUNT;i++) {
        if (!test_bit(mask, i)) continue;
        if (!fault_table[i].sticky) continue;
        if (!condition_checks[i]()) {
            clear_bit(&g_bms_faults_snapshot.latched_mask, i);
            clear_bit(&g_bms_faults_snapshot.active_mask, i);
            hw_log("Latched fault manually cleared: %s\n", fault_table[i].name);
        } else {
            hw_log("Cannot clear %s: condition still present\n", fault_table[i].name);
        }
    }
    hw_persist_write_latched(g_bms_faults_snapshot.latched_mask);
}

void bms_faults_get_masks(fault_mask_t *active, fault_mask_t *latched) {
    if (active) *active = g_bms_faults_snapshot.active_mask;
    if (latched) *latched = g_bms_faults_snapshot.latched_mask;
}

static   char* bms_fault_name(fault_id_t id) {
    if (id < 0 || id >= F_FAULT_COUNT) return "UNKNOWN";
    return fault_table[id].name;
}

/* Persistence stubs - override to store latched_mask in flash/EEPROM if desired */
static bool hw_persist_read_latched(fault_mask_t *out_mask) {
    if (out_mask) *out_mask = 0;
    return false;
}
static bool hw_persist_write_latched(fault_mask_t mask) {
    (void)mask;
    return false;
}

static void hw_log(  char *fmt, ...) {
    va_list ap; va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
}

static void hw_disable_charging(void)
{
  App_BmsChgFetOff();
}

static void hw_disable_discharging(void)
{
  App_BmsDsgFetOff();
}
static void hw_enable_charging(void)
{
  App_BmsChgFetOn();
}
static void hw_enable_discharging(void)
{
   App_BmsDsgFetOn();
}

uint32_t hw_millis(void)
{
 return xTaskGetTickCount() * portTICK_PERIOD_MS;
}




 


/**********************************************Static Definitions*************************************/

/** 
 * @brief Calculates the checksum for the given data packet. 
 * @param ptr Pointer to the data packet. 
 * @param len Length of the data packet. 
 * @return The calculated checksum. 
 */
void Rs485_data_send(void)
{
    rs485_send_basicInfo_Struct sendObj;

    sendObj.startByte    = RS485_START_BYTE;
    sendObj.command_code = BASIC_INFO_CMD;
    sendObj.status       = RS485_STATUS_OK;

    sendObj.data_length = (uint8_t)(sizeof(rs485_basicInfo_Struct));

    // Fill default data
    rs485_basicInfo_Struct filldata = {
              //fault_flags --chg_oc_flag dsg_oc_flag
    //  .protectionStatus = {chg_oc_flag,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}, 
         .protectionStatus = {chg_oc_flag ,dsg_oc_flag ,sc_flag,afe_error_flag,soft_lock_flag,hg_mos_damage_flag,sg_mos_damage_flag,mos_ot_flag, cell_ov_flag,cell_uv_flag,pack_ov_flag,pack_uv_flag,chg_ot_flag,chg_ut_flag,dsg_ot_flag,dsg_ut_flag}, //dsg_ut_flag

        .packVoltage_10mV        = volt_scaled,
        .packCurrent_10mA        = current_scaled,
        .remainingCapacity_10mAh = (uint16_t)b.coulomb_acc_ah ,
        .nominalCapacity_10mAh   = 105,
        .cycles                  = cycle_count_3,
        .productionDate          = 0x2024,

      // oz3717_data.cbsel2
        .balanceStatus_0 = {balancing_ckeck},
        .balanceStatus_1 = {balancing_ckeck},

        .softwareVersion = (uint8_t)(b.soh * 100.0),
        .remainingSOC    = (uint8_t)(b.soc * 100.0),
 
        .fetStatus       = {chg_mos, dsg_mos, 0},  

        .cellsInSeries   = 16,
        .ntcNumber       = 5,
        .ntcReading      = {oz3717_data.external_temperature[0], oz3717_data.external_temperature[1], oz3717_data.external_temperature[2], oz3717_data.external_temperature[3],oz3717_data.onBoard_tempertaure_data}
    };
    
     CC_soc =  (uint8_t)filldata.remainingSOC;
   
    // Write to active data object
    rs485_basicInfo_obj = filldata;

    // Copy into send buffer
    memcpy(&sendObj.data_buff[0], &rs485_basicInfo_obj, sizeof(rs485_basicInfo_Struct));

    // Calculate checksum
    uint16_t tmpChecksum = rs485_getChecksum1(&sendObj.status, (2 + sendObj.data_length));
    revMemcpy1(&sendObj.checksum, &tmpChecksum, sizeof(sendObj.checksum));

    sendObj.endByte = RS485_END_BYTE;

    App_UartTransmitPoll(M0P_UART3, (uint8_t *)(&sendObj), sizeof(rs485_send_basicInfo_Struct));
}


void Rs485_data_send2(void)
{
  rs485_send_cellVoltages_Struct sendObj;

    sendObj.startByte = RS485_START_BYTE;
    sendObj.command_code = CELL_VOLTAGES_CMD;
    sendObj.status = RS485_STATUS_OK;

    sendObj.data_length = (uint8_t)(sizeof(rs485_cellVoltages_Struct));

  
  rs485_cellVoltages_Struct rs485_cellInfo_obj = {
    .rs485_cellVoltages = {
        oz3717_data.cell_volt[0], oz3717_data.cell_volt[1], oz3717_data.cell_volt[2], oz3717_data.cell_volt[3],  oz3717_data.cell_volt[4], oz3717_data.cell_volt[5], oz3717_data.cell_volt[6],  oz3717_data.cell_volt[7],
         oz3717_data.cell_volt[8], oz3717_data.cell_volt[9], oz3717_data.cell_volt[10], oz3717_data.cell_volt[11], oz3717_data.cell_volt[12], oz3717_data.cell_volt[13], oz3717_data.cell_volt[14],  oz3717_data.cell_volt[15]
    },
    
      .fault_count = {
        fCounts[0],  fCounts[1],  fCounts[2],  fCounts[3],
        fCounts[4],  fCounts[5],  fCounts[6],  fCounts[7],
        fCounts[8],  fCounts[9],  fCounts[10], fCounts[11],
        fCounts[12], fCounts[13], fCounts[14], fCounts[15]
    }
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
        rs485_getChecksum1(checksumDataBuff,
                          sizeof(checksumDataBuff));

    revMemcpy1(&sendObj.checksum,
              &tmpChecksum,
              sizeof(sendObj.checksum));

    sendObj.endByte = RS485_END_BYTE;

    // --------------------------------------------
    // Transmit entire object
    // --------------------------------------------
    App_UartTransmitPoll(M0P_UART3,
                         (uint8_t *)&sendObj,
                         sizeof(rs485_send_cellVoltages_Struct));
}




void Rs485_data_send3(void)
{
    rs485_send_mis_Struct sendObj;

    sendObj.startByte    = RS485_START_BYTE;
    sendObj.command_code = HW_VERSION;       // <--- BASIC_INFO_CMD was wrong
    sendObj.status       = RS485_STATUS_OK;

    sendObj.data_length  = (uint8_t)sizeof(rs485_misinfo_Struct);

    // -----------------------------------------
    // Fill data (only if needed)
    // -----------------------------------------
          
    rs485_misinfo_Struct filldata = 
    {
        .afe_fault = fault_flags,
        .afe_fet_status  = oz3717_data.fetCtrl,
        .soh = (uint8_t)b.soh,
    };

    // Copy into TX buffer
    memcpy(sendObj.data_buff,
           &filldata,                   // <--- fixed (was rs485_basicInfo_obj)
           sizeof(rs485_misinfo_Struct));

    // -----------------------------------------
    // Prepare checksum source = status + length + data
    // -----------------------------------------
    uint8_t checksumBuff[2 + sizeof(rs485_misinfo_Struct)];
    checksumBuff[0] = sendObj.status;
    checksumBuff[1] = sendObj.data_length;

    memcpy(&checksumBuff[2],
           sendObj.data_buff,
           sizeof(rs485_misinfo_Struct));

    // -----------------------------------------
    // Compute checksum
    // -----------------------------------------
    uint16_t tmpChecksum =
        rs485_getChecksum1(checksumBuff,
                           sizeof(checksumBuff));

    revMemcpy1(&sendObj.checksum,
               &tmpChecksum,
               sizeof(sendObj.checksum));

    sendObj.endByte = RS485_END_BYTE;

    // -----------------------------------------
    // Transmit full packet
    // -----------------------------------------
    App_UartTransmitPoll(M0P_UART3,
                         (uint8_t *)&sendObj,
                         sizeof(rs485_send_mis_Struct));   // <--- FIXED!
}


void App_advanced_protection_config(void)
{
	uint8_t ret = 0;
	uint16_t ustmp = 0;
}
 



/* ================== GLOBAL FLAGS ================== */
uint8_t empty_flag = 0;
uint8_t full_flag  = 0;

uint32_t empty_count = 0;
uint32_t full_count  = 0;
//uint32_t cycle_count = 0;
#define SOC_EMPTY_FORCE_TH   5     // %
#define SOC_FULL_FORCE_TH    95    // %

 
 

/* ================== SOC PROCESS FUNCTION ================== */

void bms_soc_process(SimpleBMS *b,
                     int16_t current_scaled,
                     uint16_t volt_scaled,
                     uint8_t cell_uv_flag,
                     uint8_t cell_ov_flag)
{
    uint8_t soc_check;

    /* =====================================================
       1?? ALWAYS UPDATE COULOMB MODEL FIRST
       ===================================================== */
    bms_update(b,
               (double)(current_scaled / 10.0),
               1.0,
               (double)(volt_scaled / 1000.0));

    soc_check = (uint8_t)(b->soc * 100.0);

    /* =====================================================
       2?? FAULT OVERRIDE (SOC-QUALIFIED)
       ===================================================== */

    /* --- UV ? force 0% ONLY near empty --- */
    if (cell_uv_flag &&
        empty_flag == 0 &&
        soc_check <= SOC_EMPTY_FORCE_TH)
    {
        b->soc = 0.0;
        b->coulomb_acc_ah = 0.0;
        b->last_avg_current_a = 0.0;

        empty_flag = 1;
        empty_count++;
    }

    /* --- OV ? force 100% ONLY near full --- */
    if (cell_ov_flag &&
        full_flag == 0 &&
        soc_check >= SOC_FULL_FORCE_TH)
    {
        b->soc = 1.0;
        b->coulomb_acc_ah = b->nominal_capacity_ah;
        b->last_avg_current_a = 0.0;

        full_flag = 1;
        full_count++;
        cycle_count_3++;
    }

    /* =====================================================
       3?? CURRENT DIRECTION
       ===================================================== */
    current_dir_t cur_dir = get_current_direction(current_scaled);
    uint8_t charging    = (cur_dir == CUR_CHARGE);
    uint8_t discharging = (cur_dir == CUR_DISCHARGE);

    /* =====================================================
       4?? UNLATCH ON DIRECTION CHANGE
       ===================================================== */
    if (charging && empty_flag)
        empty_flag = 0;

    if (discharging && full_flag)
        full_flag = 0;

    /* =====================================================
       5?? SOFT SOC CLAMPS (NO FAULT)
       ===================================================== */
    soc_check = (uint8_t)(b->soc * 100.0);

    if (discharging && !cell_uv_flag && !empty_flag)
    {
        if (soc_check <= 1)
        {
            b->soc = 0.01;
            b->coulomb_acc_ah = 0.01 * b->nominal_capacity_ah;
        }
    }

    if (charging && !cell_ov_flag && !full_flag)
    {
        if (soc_check >= 99)
        {
            b->soc = 0.99;
            b->coulomb_acc_ah = 0.99 * b->nominal_capacity_ah;
        }
    }

    /* =====================================================
       6?? CYCLE COUNT
       ===================================================== */
    static uint32_t last_empty_count = 0;
    static uint32_t last_full_count  = 0;

    if ((empty_count != last_empty_count) &&
        (full_count  != last_full_count))
    {
//        cycle_count++;
    }

    last_empty_count = empty_count;
    last_full_count  = full_count;
}




uint16_t sleep_flag = 0;
extern volatile uint8_t flag_init;

void Sleep_function(void)
{
    static boolean_t bFlag = TRUE;
    static uint8_t charging_latched = 1;
    static uint8_t addcounter = 0;

    current_dir_t cur_dir = get_current_direction(current_scaled);
    uint8_t charging = (cur_dir == CUR_CHARGE);

    /* --------------------------------
       1. LATCH CHARGING EVENT
       -------------------------------- */
    if (charging)
    {
        charging_latched = 1;
        addcounter = 0;               // reset sleep counter
    }

    /* --------------------------------
       2. FORCE SLEEP CONDITION
       --------------------------------
       - Not charging
       - Min cell < 2000 mV
       - Init done
       - Condition persists > 30 calls
       -------------------------------- */
    if (!charging &&
        flag_init == 1 &&
        oz3717_data.min_cell_data < 2000)
    {
        if (++addcounter > 30)
        {
            sleep_flag = 1;
            App_BmsLedOff(); 
            App_BmsBleOff();
            charging_latched = 0;
            addcounter = 0;
            return;
        }
    }
    else
    {
        addcounter = 0;   // condition broken ? reset counter
    }

    /* --------------------------------
       3. STAY AWAKE ONCE CHARGING SEEN
       -------------------------------- */
    if (charging_latched)
    {
        sleep_flag = 0;
        App_BmsBleOn();
        if (g_main_init_done != 2)
        {
            if (bFlag)
            {
                bFlag = FALSE;
                App_BmsLedOn();
            }
            else
            {
                bFlag = TRUE;
                App_BmsLedOff();
            }
        }
    }
}

 