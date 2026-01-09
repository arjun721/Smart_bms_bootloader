
#include "main.h"


extern void ais_eventLogging(void);
extern void ais_readAllLogs_intoBuffer(void);
extern void ais_eraseAllLogs(void);
extern void ais_getrecode(void);
extern void sendAllLogs_RS485(void);
extern void ais_verify_startup(void);

static uint8_t IncTimer;
static uint16_t g_data_log_every_one_min;
volatile uint32_t sys_time_sec = 0;
extern uint8_t g_struct_fill_flag,can_flag,g_main_init_done;
extern struct oz3717_chip oz3717_data;

volatile Bms_ModeType_t bmsPorLedMode_t;
volatile uint8_t g_sleepModeEnabel;
volatile uint8_t g_ErrorIrqTrigger;

extern SimpleBMS b;
extern uint16_t volt_scaled;
extern int16_t current_scaled;
extern volatile uint16_t fault_flags;
extern uint16_t sleep_flag;
extern uint8_t fault_tigger_flag;
extern uint8_t anyProtectFlag;
extern volatile uint8_t enable_flag;
uint32_t time_remaining_s;

uint8_t ais_init_ok = 0;
uint8_t ais_format_ok = 0;
uint8_t ais_append_ok = 0;
uint8_t ais_read_ok = 0;
uint8_t ais_record_count_ok = 0;

uint16_t maxCellVoltage;
uint8_t maxVoltage_cellno;
uint16_t minCellVoltage;
uint8_t minVoltage_cellno;
int16_t maxTemp;
int16_t minTemp;
uint16_t mosFet;
uint16_t recovery_count;
uint16_t CC_soc;
//        static uint8_t empty_flag = 0;
//        static uint8_t full_flag = 0;
//        uint8_t cycle_count=1, empty_count =1, full_count=1;
//        uint8_t soc_check;

extern volatile uint16_t rxCnt;
extern volatile uint8_t cfgReady;
 
extern volatile uint8_t rs485_frame_ready,rs485_frame_ready1,rs485_frame_ready3,rs485_frame_ready4,rs485_frame_ready5;
extern current_dir_t get_current_direction(int16_t current_scaled);
extern void static_calibration(void);
static void App_rs485_comm(void);
static void App_BleDataProcess(void);
static void soc_process_task(void);
/* ================== CURRENT DIRECTION ================== */

 
 

/**
 * 1) AFE trigger interrupt on iotp_ie, cocp_ie, doc1p_ie, doc2p_ie, scp_ie;
 * 2) 2nd level interrupt - loadon_ie, loadoff_ie, chgrin_ie, ovp_ie;
 * 3) 3rd level interrupt - cb_timeout_ie, spiwdt_ie, trigger_scan_ie;
 * 4) 4th level interrupt - indsg_ie, uv_ie;
 * */

void PORTB_IRQHandler(void)
{
	if(TRUE == Gpio_GetIrqStatus(BMS_AFE_INT_IRQ_PORT, BMS_AFE_INT_IRO_PIN))
	{
     App_BmsChgDsgFetOff();
     g_ErrorIrqTrigger = 1;
		Gpio_ClearIrq(BMS_AFE_INT_IRQ_PORT, BMS_AFE_INT_IRO_PIN);
	}
}

extern bool cell_ov_flag;
extern bool cell_uv_flag;
volatile uint8_t flag_init = 0;
volatile uint32_t timer_cnt = 0;

void TIM3_IRQHandler(void)
{
	if (TRUE == Tim3_GetIntFlag(Tim3UevIrq))
	{
		static uint8_t ledBlinkCounter = 0;  // Counts 100ms intervals
    static uint8_t fault_flag_count_time = 25;

		IncTimer = 1;

		ledBlinkCounter++;

		//if the bms is in ideal mode or unknow mode go then only blink led.
		if(bmsPorLedMode_t == IDEL_MODE)
		{
			if (ledBlinkCounter >= 8) // 10 x 100ms = 1000ms = 1 second
			{
				ledBlinkCounter = 0;

				g_sleepModeEnabel++;
				g_data_log_every_one_min++;
        timer_cnt++; 
        sys_time_sec++;
        
        if(fault_tigger_flag == 1)
        {
          fault_flag_count_time = 2;
        }
        else
        {
          fault_flag_count_time = 10;
        }

				if(g_data_log_every_one_min >= fault_flag_count_time) //25
				{
					if(g_main_init_done == 1 && rs485_frame_ready5 == 0 && sleep_flag == 0)
					{
					 	ais_eventLogging();   // event based + time based
						g_data_log_every_one_min = 0;
					}
				}
        
          soc_process_task();
        
          if (timer_cnt >= 25)   // 20 sec = 20,000 ms
           {
              flag_init = 1;
              timer_cnt = 0;        // optional: reset if one-shot
           }
          Sleep_function();
        
			}
		}
		Tim3_ClearIntFlag(Tim3UevIrq);
	}
}


void Bms_AfeTask(void *pvParameters);
void Bms_MonitorTask(void *pvParameters);
void Bms_PeriodicTask(void *pvParameters);




/*
 *This task only for afe called every 1 sec
 *fill the structer in every one sec.
*/
extern bool dsg_oc_flag;
extern bool dsg_mos,chg_mos;

void Bms_AfeTask(void *pvParameters)
{
	vTaskDelay(pdMS_TO_TICKS(3000));

	while (1)
	{
		if(IncTimer == 1)
		{
			IncTimer = 0;
			App_BmsMonitor();
      bms_faults_periodic();
			App_CellBalancing();
      App_rs485_comm();
      App_BleDataProcess();
		}
		vTaskDelay(pdMS_TO_TICKS(50));
	}
}

void Bms_PeriodicTask(void *pvParameters)
{
    while (1)
    {
      App_GetTempPoll();
      if(sleep_flag == 0)
      {
        App_SendSoCMeterData();
      }
      vTaskDelay(pdMS_TO_TICKS(100));
    }
}

 

void Bms_MonitorTask(void *pvParameters)
{
    while (1)
    {
      if (enable_flag == 9)
      {
         App_BleSendJsonData();
         enable_flag  = 0;
      }
     
      vTaskDelay(pdMS_TO_TICKS(100));
    }
}
 
int32_t main(void)
{
    App_BmsInit();
    delay1ms(100);
    App_AfeInit();
    App_BmsAlertOn();
    App_BmsPorOn();
  
    delay1ms(100);
    App_BmsBleOn();
    App_BmsAlertOff();
    App_BmsChgDsgFetOn();
    App_BmsRs485RxMode();
    eepromInterface_eepromInit();

    delay1ms(100);
    App_BmsMonitor();
    App_BmsMonitor();
     
    bms_init(&b, 73,  0.99, 100, 56.0, 48.0); // soc,soh,tick,max,min
    bms_faults_init();
    RTC_Cfg();
    delay1ms(100);
    

    g_main_init_done = 2;
    ais_verify_startup();
    App_LoadNorData();
    delay1ms(100);
    App_BmsRs485RxMode();
    g_main_init_done = 1;
    
    xTaskCreate(Bms_AfeTask, "Bms_AfeTask", 512, NULL, 1, NULL);
    xTaskCreate(Bms_MonitorTask, "Bms_MonitorTask", 128, NULL, 1, NULL);
    xTaskCreate(Bms_PeriodicTask, "Bms_PeriodicTask", 128, NULL, 1, NULL);

    // Start the scheduler
    vTaskStartScheduler();

  while(1)
  {

  }
}



// This function will be called if a task overflows its stack
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    // You can log the task name or flash an LED for debugging
    (void)xTask;
    (void)pcTaskName;

    // You can add a breakpoint here or handle it safely
    while (1)
    {

    }
}


void App_rs485_comm(void)
{
      if(oz3717_data.max_cell_data < 5000)
      {
        maxCellVoltage = oz3717_data.max_cell_data;
      }
      if(oz3717_data.min_cell_data < 5000)
      {
        minCellVoltage = oz3717_data.min_cell_data;
      }
			maxVoltage_cellno = oz3717_data.which_cell_max;
			minVoltage_cellno = oz3717_data.which_cell_min;
			maxTemp = oz3717_data.max_temperature_data;
			minTemp = oz3717_data.min_temperature_data;
			mosFet = oz3717_data.fetCtrl;

			if(sleep_flag == 1)
			{
      
			}
			else
			{
				if(rs485_frame_ready ==1)
				{
					App_BmsRs485TxMode();
					vTaskDelay(pdMS_TO_TICKS(50));
					Rs485_data_send2();
					vTaskDelay(pdMS_TO_TICKS(50));
					App_BmsRs485RxMode();
          rs485_frame_ready = 0;
				}

				if(rs485_frame_ready1 ==1)
				{
					App_BmsRs485TxMode();
					vTaskDelay(pdMS_TO_TICKS(70));
					Rs485_data_send();
					vTaskDelay(pdMS_TO_TICKS(70));
					App_BmsRs485RxMode();
          rs485_frame_ready1 = 0;
				}

				if(rs485_frame_ready3 == 1)
				{
					App_BmsRs485TxMode();
					vTaskDelay(pdMS_TO_TICKS(50));

					decode_rs485_config();

					rs485_frame_ready3 = 0;
					vTaskDelay(pdMS_TO_TICKS(50));
					App_BmsRs485RxMode();

				}

				if(rs485_frame_ready4 == 1)
				{
					App_BmsRs485TxMode();
					vTaskDelay(pdMS_TO_TICKS(50));
					Load_All_Thresholds_From_EEPROM();
					rs485_frame_ready4 = 0;
					vTaskDelay(pdMS_TO_TICKS(50));
					App_BmsRs485RxMode();

				}
				if(rs485_frame_ready5 == 1)
				{
					App_BmsRs485TxMode();
					vTaskDelay(pdMS_TO_TICKS(50));
					sendAllLogs_RS485();
					rs485_frame_ready5 = 0;
					vTaskDelay(pdMS_TO_TICKS(50));
					App_BmsRs485RxMode();
				}
			 }
}

void App_BleDataProcess(void)
{
      if(anyProtectFlag == 1)
      {
        App_getBleDta_config();
        anyProtectFlag = 0;
      }
      
      if(enable_flag == 1)
      {
         App_BleProtectionSettings();
         enable_flag = 0;
      }
      
       if(enable_flag == 2)
      {
        App_BleTemperatureProtectionSettings();
        enable_flag = 0;
      }
      
       if(enable_flag == 8)
      {
         App_BleCurrentProtectionSettings();
        enable_flag = 0;
      }
      
      if(enable_flag == 4)
      {
         App_Bleoriginsettings();
        enable_flag = 0;
      }
      
      if(enable_flag == 5)
      {
        App_Bleoriginsettings();
        enable_flag = 0;
      }
}

 

void soc_process_task(void)
{
    static uint32_t idle_start_time = 0;
    current_dir_t cur_dir = get_current_direction(current_scaled);
    uint32_t now = sys_time_sec;

    /* If charging or discharging ? reset idle timer */
    if (cur_dir == CUR_CHARGE || cur_dir == CUR_DISCHARGE)
    {
        idle_start_time = 0;

        bms_soc_process(&b,
                        current_scaled,
                        volt_scaled,
                        cell_uv_flag,
                        cell_ov_flag);
        return;
    }

    /* If IDLE ? start debounce timer */
    if (cur_dir == CUR_IDLE)
    {
        if (idle_start_time == 0)
        {
            idle_start_time = now;   // first IDLE detected
        }

        /* Allow SOC update for 10 seconds after IDLE */
        if ((now - idle_start_time) <= 20)
        {
            bms_soc_process(&b,
                            current_scaled,
                            volt_scaled,
                            cell_uv_flag,
                            cell_ov_flag);
        }
        /* else: real IDLE ? stop SOC update */
    }
}
