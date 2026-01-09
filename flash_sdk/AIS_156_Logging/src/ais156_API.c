 #include "ringfs.h"
 #include "ais156_sendLogs.h"
 #include "ais156_API.h"
 #include "ais156_logging_interface.h"
 #include "mcal_rtc_interface.h"
 #include "nor_flash.h"
 #include <string.h>
 #include "uart.h"
 #include "rtc.h"
 
 
 
 #define MAX_LOG_BUFFER 100

log_entry logBuffer[MAX_LOG_BUFFER];
uint32_t logCount = 0;
bool logsReady = false;


 #define FLASH_PARTITION_OFFSET EEPROM_PARTITION
 
 volatile ais156_Flags_Struct g_ais156_Flags;
 ais156_Log_Handle_Struct g_ais156Log_Handle;
 static uint8_t loggingInitialized = 0;
 extern volatile uint8_t sendLogsOverCAN;
 extern volatile uint8_t sendSpecificLog;
 
 /**
  * @brief Structure for ringfs partition
  * @details Struct ringfs_flash_partition flash contains details related to the flash partition used for RingFS. \n
  * 			It includes the sector size, sector offset, sector count and the flash operations functions such as sector_erase, program and read.
  */
 static struct ringfs_flash_partition flash = {
	 .sector_size = SECTOR_SIZE,
	 .sector_offset = FLASH_PARTITION_OFFSET,
	 .sector_count = AIS_LOGGING_SECTORS - FLASH_PARTITION_OFFSET,
 
	 .sector_erase = op_sector_erase,
	 .program = op_program,
	 .read = op_read,
 };
 
 
 static struct ringfs fs;
 static struct ringfs_initFlags ais156_initFlags;
 
 /**
  * @brief Formats initial AIS156_INIT_SECTORS sectors in code initialization
  * 
  * @return 0,-1 for init successfull and fail respectively 
  */
 static int ais156_FormatInitialSectors(void);
 
 /**
  * @brief Formats initial AIS156_INIT_SECTORS sectors in code initialization
  * 
  * @return 0,-1 for init successfull and fail respectively 
  */
 static int ais156_FormatInitialSectors(void)
 {
	 int8_t retVal = 0;
	 for(uint32_t secCnt = 0;secCnt < AIS156_INIT_SECTORS; secCnt++)
	 {
		 retVal = ringfs_formatSector(&fs,  0);//mcu_wwdgRefresh
		 __asm("nop");
		 if((retVal == -1))
		 {
			 return -1;
		 }
		 else if(retVal == 2)
		 {
			 ais156_initFlags.u8Memory_Initialized = 1;
			 return 0;
		 }
	 }
	 fs.read.sector = 0;
	 fs.read.slot = 0;
	 fs.write.sector = 0;
	 fs.write.slot = 0;
	 fs.cursor.sector = 0;
	 fs.cursor.slot = 0;
	 return 0;
 }
 
 /**
  * @brief Initializes the logging system.
  * 
  * @details This function initializes the logging system by setting up the filesystem in the flash memory and \n
  * 			allocating it to the filesystem structure, with the indicated log entry version number and size.
  *
  * @returns 0 on success, -1 on failure.
  */
 int ais156_loggingInit(void){
	 int retVal = -1;
	 retVal = ringfs_init(&fs, &flash, LOG_ENTRY_VERSION, sizeof(log_entry));
	 if(retVal == 0){
		 ais156_initFlags.u8Logging_Initialized = 1;
	 }
	 return retVal;
 }
 
 /**
  * Scans for existing ring buffer file system
  * and formats the FLASH if no file system found.
  *
  * @returns 0 if the ring buffer is formatted, otherwise 1.
  */
 int ais156_scanFormat(void){
	 if(ais156_initFlags.u8Logging_Initialized != 1){
		 return -1;
	 }
	 if (ringfs_scan(&fs) == 0) {
		 __asm("nop");
		 ais156_initFlags.u8Memory_Initialized = 1;
		 return 0;
	 }
	 else{
		 ais156_initFlags.u8Memory_Initialized = 0;
		  return ringfs_format(&fs,  0); //mcu_wwdgRefresh
		 return ais156_FormatInitialSectors();
	 }
 }
 
 /**
  * Formats the flash storage.
  *
  * @returns 0 on success, -1 on failure.
  */
 int ais156_formatFlash(void){
	 if(ais156_initFlags.u8Logging_Initialized != 1){
		 return -1;
	 }
	 return ringfs_format(&fs,  0);// mcu_wwdgRefresh
 }
 
 /**
  * Appends an object to the log.
  *
  * @param object The object to append.
  *
  * @returns 0 on success, -1 on failure.
  */
 int ais156_Append(const log_entry *object){
	 if(ais156_initFlags.u8Logging_Initialized != 1){
		 return -1;
	 }
	 int retVal = 0;
	 if(ais156_initFlags.u8Memory_Initialized == 1)
	 // {
	 // 	if(ringfs_checkSectorAvailable(&fs,fs.eraseCursor,fs.object_size))
	 // 	{	
	 // 		retVal = ringfs_append(&fs, object);
	 // 	}
	 // 	else
	 // 	{
	 // 		retVal = 1;
	 // 	}
	 // }
	 // else
	 {
		 retVal = ringfs_append(&fs, object);
	 }
	 return retVal;
 }
 
 /**
  * Reads an entry from the logged data.
  *
  * @param object The log entry to read.
  *
  * @returns 0 on success, -1 on failure.
  */
 int ais156_Read(log_entry* object){
	 if(ais156_initFlags.u8Logging_Initialized != 1){
		 return -1;
	 }
	 return ringfs_fetch(&fs, object);
 }
 
 /**
  * Reads a specific log entry from the flash.
  *
  * @param object The log entry to be read.
  * @param logNum The log number to be read.
  *
  * @returns 0 on success, -1 on failure.
  */
 int ais156_ReadSpecific(log_entry* object, uint32_t logNum){
 
	 if(ais156_initFlags.u8Logging_Initialized != 1){
		 return -1;
	 }
 
	 if(logNum > (fs.slots_per_sector * (fs.flash->sector_count-1)) || logNum < 1){
		 return -1;
	 }
	 int retVal = -1;
	 retVal = ringfs_advance_slot_specific(&fs, logNum-1); 
	 retVal |= ringfs_fetch(&fs, object);
	 retVal |= ringfs_rewind(&fs);
	 return retVal;
 }
 
 /**
  * Advances the read cursor to the specified log sector.
  *
  * @param logNum The log sector to advance to.
  *
  * @returns 0 on success, -1 on failure.
  */
 int ais156_advanceReadCursor(uint32_t logNum){
 
	 if(ais156_initFlags.u8Logging_Initialized != 1){
		 return -1;
	 }
 
	 if(logNum > (fs.slots_per_sector * (fs.flash->sector_count-1)) || logNum < 1){
		 return -1;
	 }
	 int retVal = -1;
	 retVal = ringfs_advance_slot_specific(&fs, logNum-1);
	 return retVal;
 }
 
 /**
  * Discards the records in the ring buffer.
  *
  * @returns None
  */
 int ais165_discardRecords(void){
 
	 if(ais156_initFlags.u8Logging_Initialized != 1){
		 return -1;
	 }
 
	 return ringfs_discard(&fs);
 }
 
 /**
  * Rewinds the file pointer to the beginning of the file.
  *
  * @returns 0 on success, -1 on failure.
  */
 int ais156_Rewind(void){
 
	 if(ais156_initFlags.u8Logging_Initialized != 1){
		 return -1;
	 }
 
	 return ringfs_rewind(&fs);
 }
 
 /**
  * Returns the number of records in the file by counting total logs, runs in O(n).
  *
  * @returns The number of records in the file.
  */
 int ais156_getRecordCountExact(void){
 
	 if(ais156_initFlags.u8Logging_Initialized != 1){
		 return -1;
	 }
 
	 return ringfs_count_exact(&fs);
 }
 
 /**
  * Returns the number of records in the file by calculating the total logs, runs in O(1).
  *
  * @returns The number of records in the file.
  */
 int ais156_getRecordCountEstimate(void){
 
	 if(ais156_initFlags.u8Logging_Initialized != 1){
		 return -1;
	 }
 
	 return ringfs_count_estimate(&fs);
 }
 
 /**
  * Reads the next entry from the log file and rewinds it.
  *
  * @param object The log entry to read.
  *
  * @returns 0 on success, -1 on failure.
  */
 int ais156_readAndRewind(log_entry* object){
 
	 if(ais156_initFlags.u8Logging_Initialized != 1){
		 return -1;
	 }
 
	 int retVal = -1;
	 retVal = ringfs_fetch(&fs, object);
	 retVal |= ringfs_rewind(&fs);
	 return retVal;
 }
 
 /**
  * Reads an object from the ring buffer and discards it.
  *
  * @param object The object to read and discard.
  *
  * @returns 0 on success, -1 on failure.
  */
 int ais156_readAndDiscard(log_entry* object){
 
	 if(ais156_initFlags.u8Logging_Initialized != 1){
		 return -1;
	 }
 
	 int retVal = -1;
	 retVal = ringfs_fetch(&fs, object);
	 retVal |= ringfs_discard(&fs);
	 return retVal;
 }
 
 /**
  * Sends a log entry over CAN.
  *
  * @param object The log entry to send.
  * @param logNumber The log number to send the entry to.
  *
  * @returns 0 on success, -1 on failure.
  */
// int ais156_sendLogsOverCAN(log_entry* object, uint32_t logNumber){
// 
//	 if(ais156_initFlags.u8Logging_Initialized != 1){
//		 return -1;
//	 }
// 
//	 return (int)ais156_breakAndSend(object, logNumber);
// }
// 
// /**
//  * Sends the meta data over CAN.
//  *
//  * @param totalLogs The total number of logs to be sent.
//  *
//  * @returns 0 on success, -1 on failure.
//  */
// int ais156_metaData(uint32_t totalLogs, uint32_t loggingInterval_s){
// 
//	 if(ais156_initFlags.u8Logging_Initialized != 1){
//		 return -1;
//	 }
// 
//	 return ais156_sendInfoPacket(totalLogs, loggingInterval_s);
// }
// 
// /**
//  * Sends the end of the transmission message over CAN.
//  *
//  * @param logsSent The number of logs sent to the server.
//  *
//  * @returns 0 on success, -1 on failure.
//  */
// int ais156_logTransmissionEnd(uint32_t logsSent){
// 
//	 if(ais156_initFlags.u8Logging_Initialized != 1){
//		 return -1;
//	 }
// 
//	 return ais156_sendEndProcessMsg(logsSent);
// }
// 
// /****************************************NOR DATA EXTRACTION FUNCTIONS****************************************** */
// /**
//  * @brief CAN messages handler for AIS Data Logging feature
//  * 
//  * @param Identifier CAN ID received
//  * @param data CAN Data received
//  */
// void ais156_DataGetHandle(uint32_t Identifier, uint8_t *data){
//	 switch(Identifier){
//		 case LOG_COUNT_REQ:
//			 if(ais156_getBmsStatus() == BMS_STATE_IDLE){										
//				 g_ais156Log_Handle.ais156_retreiveLogCount = 1;
//			 }
//			 break;
//		 case ALL_LOGS_REQ:
//			 if(ais156_getBmsStatus() == BMS_STATE_IDLE){										
//				 __asm("nop");
//				 memcpy(&(g_ais156Log_Handle.ais156_logsToRetrieve), &data[0], sizeof(g_ais156Log_Handle.ais156_logsToRetrieve));
//				 memcpy(&g_ais156Log_Handle.ais156_startingLog, &data[4], sizeof(g_ais156Log_Handle.ais156_startingLog));
//				 if(g_ais156Log_Handle.ais156_startingLog > g_ais156Log_Handle.ais156_logsToRetrieve){
//					 //TODO: Send NACK
//					 break;
//				 }
//				 sendLogsOverCAN = 1;
//				 g_ais156Log_Handle.ais156_retreiveLogCount = 1;
//				 g_ais156_Flags.ais156_moveCursor = g_ais156Log_Handle.ais156_startingLog > 1 ? 1 : 0;
//				 g_ais156Log_Handle.ais156_startingLog = g_ais156Log_Handle.ais156_startingLog == 0 ? 1 :g_ais156Log_Handle.ais156_startingLog;
//				 g_ais156Log_Handle.ais156_logsToRetrieve = g_ais156Log_Handle.ais156_logsToRetrieve - (g_ais156Log_Handle.ais156_startingLog - 1);
//				 g_ais156Log_Handle.ais156_logsRead = 0;
//				 g_ais156Log_Handle.ais156_logsSent = 0;
//				 g_ais156_Flags.ais156_g_exitSendRoutine = 0;
//			 }
//			 break;
//		 case SPEC_LOG_REQ:
//			 __asm("nop");
//			 if(sendLogsOverCAN == 0){
//				 memcpy(&g_ais156Log_Handle.ais156_specificLogNumToRetrieve, data, sizeof(g_ais156Log_Handle.ais156_specificLogNumToRetrieve));
//				 sendSpecificLog = 1;
//				 g_ais156Log_Handle.ais156_retreiveLogCount = 1;
//			 }
//			 else{
//				 //TODO: Send NACK
//			 }
//			 break;
//		 case LOG_TX_RESP:
//			 if(data[7] == LOG_TX_STOP_PACKET)
//			 {
//				 g_ais156_Flags.ais156_g_exitSendRoutine = 1;
//			 }
//		 #if LOG_ENTRY_VERSION == 4
//			 else if(data[7] == LOG_TX_ACK_PACKET)
//			 {
//				 g_ais156_Flags.ais156_logAckRecvd = 1;
//			 }
//		 #endif
//			 break;
//		 default:
// 
//			 break;
//	 }
// }
// 
// 
// /**
//  * @brief Snapshot feature handler which performs the following functions:
//  *          - Send total size written in Snapshot memory region in flash
//  *          - Send Data in while loop
//  *          - Send Specific Data
//  *          - Erase Snapshot memory in flash
//  */
// uint8_t ais156_DataGetRoutine(uint32_t initialTime)
// {
//	 uint8_t ais156_loggingEnabled = 0;
//	 /*_____Retrieve_number_of_logs_and_send_over_CAN_____*/
//	 if(g_ais156Log_Handle.ais156_retreiveLogCount == 1){
// 
//		 /*Reset the flag*/
//		 g_ais156Log_Handle.ais156_retreiveLogCount = 0;
// 
//		 /*Get the total number of logs*/
//		 int32_t tmpLogs ;
//		 tmpLogs = ais156_getRecordCountEstimate();
//		 if(tmpLogs >= 0){
//			 g_ais156Log_Handle.ais156_totalLogs = (uint32_t)tmpLogs;
//		 }
// 
//		 /*Send the total number of logs over CAN*/
//		 if(sendLogsOverCAN != 1 && sendSpecificLog != 1){
//			 ais156_metaData(g_ais156Log_Handle.ais156_totalLogs , 0); //eparams.loggingInterval_s = 0
//		 }
//	 }
// 
//	 __asm("nop");
// 
//	 if(sendSpecificLog == 1)
//	 {
//		 ais156_loggingEnabled = 0;
// 
//		 /*Exit condition flag*/
//		 uint8_t exit = 0;
// 
//		 /*Initialize temp structure object*/
//		 log_entry specificLog = {0};
// 
//		 /*If logs requested are greater than available logs exit and send NACK*/
//		 if(g_ais156Log_Handle.ais156_specificLogNumToRetrieve > g_ais156Log_Handle.ais156_totalLogs || g_ais156Log_Handle.ais156_specificLogNumToRetrieve == 0){
//			 exit = 1;
//			 //TODO:Send NACK
//			 __asm("nop");
//		 }
// 
//		 /*If battery is not idle exit and send NACK*/
//		 if(ais156_getBmsStatus() != BMS_STATE_IDLE){												
//			 //TODO: Send NACK
//			 exit = 1;
//			 __asm("nop");
//		 }
// 
//		 /*If exit flag is zero, read log and send over CAN*/
//		 if(exit == 0){
//			 /*Reset the log structure*/
//			 memset(&specificLog, 0, sizeof(specificLog));
// 
//			 /*Read the next log entry*/
//			 ais156_ReadSpecific(&specificLog, g_ais156Log_Handle.ais156_specificLogNumToRetrieve);
// 
//			 /*Send the read log over CAN*/
//			 ais156_sendLogsOverCAN(&specificLog, g_ais156Log_Handle.ais156_specificLogNumToRetrieve);
//		 }
// 
//		 ais156_sendEndProcessMsg(g_ais156Log_Handle.ais156_logsSent);
//		 sendSpecificLog = 0;
//		 ais156_loggingEnabled = 1;
//	 }
// 
// 
//	 /**********************If sendLogsOverCAN is set**********************/
//	 if(sendLogsOverCAN == 1){
// 
//		 /*Initialize temp structure object*/
//		 log_entry logToSend = {0};
//		 uint32_t ais156_checkAck = 0;;// GET_TICK();
//		 /*While sent logs are less than requested logs*/
//		 while(g_ais156Log_Handle.ais156_logsSent <= g_ais156Log_Handle.ais156_logsToRetrieve){
//			 #if LOG_ENTRY_VERSION == 4
//			// 0;//mcu_wwdgRefresh();
//			 #endif
//			 /*Exit condition flag*/
//			 uint8_t exit = 0;
// 
//			 exit = g_ais156_Flags.ais156_g_exitSendRoutine;
// 
//			 if(g_ais156_Flags.ais156_moveCursor == 1){
//				 int retVal = -1;
//				 retVal = ais156_advanceReadCursor(g_ais156Log_Handle.ais156_startingLog);
//				 if(retVal == -1){
//					 exit = 1;
//				 }
//				 g_ais156_Flags.ais156_moveCursor = 0;
//			 }
// 
// 
//			 /*If logs requested are greater than available logs exit and send NACK*/
//			 if(g_ais156Log_Handle.ais156_logsToRetrieve > g_ais156Log_Handle.ais156_totalLogs || g_ais156Log_Handle.ais156_logsToRetrieve == 0){
//				 exit = 1;
//				 //TODO:Send NACK
//				 __asm("nop");
//			 }
// 
//			 /*If battery is not idle exit and send NACK*/
//			 if(ais156_getBmsStatus() != BMS_STATE_IDLE){											
//				 //TODO: Send NACK
//				 exit = 1;
//				 __asm("nop");
//			 }
// 
//			 /*If exit flag is zero, read log and send over CAN*/
//			 if(exit == 0){
//				 /*Reset the log structure*/
//				 memset(&logToSend, 0, sizeof(logToSend));
// 
//				 /*Read the next log entry*/
//				 ais156_Read(&logToSend);
// 
//				 /*Increment the ais156_logsRead flag*/
//				 g_ais156Log_Handle.ais156_logsRead++;
// 
//				 /*Send the read log over CAN*/
//				 if(ais156_sendLogsOverCAN(&logToSend, g_ais156Log_Handle.ais156_logsRead) == 0){
//					 /*If log successfully sent over CAN increment the logsSent flag*/
//					 g_ais156Log_Handle.ais156_logsSent++;
//					 g_ais156Log_Handle.ais156_logsFailed = 0;
//				 }
//				 else{
//					 //TODO: Bring cursor 1 step back and decrement ais156_logsRead flag
//					 g_ais156Log_Handle.ais156_logsFailed++;
//					 if(g_ais156Log_Handle.ais156_logsFailed >= (g_ais156Log_Handle.ais156_logsToRetrieve > 100 ? 10 : 1)){
//						 g_ais156Log_Handle.ais156_logsSent = g_ais156Log_Handle.ais156_logsToRetrieve;
//						 __asm("nop");
//					 }
//				 }
//			 }
// 
//			 if( ( 0 - ais156_checkAck) >= LOG_ACK_CHECK_TIME_MS) //GET_TICK() 
//			 {
//				 if(!g_ais156_Flags.ais156_logAckRecvd)
//				 {
//					 exit = 1;
//					 ais156_checkAck = 0;
//					 
//				 }
//				 else
//				 {
//					 ais156_checkAck =  0;  //GET_TICK();
//					 g_ais156_Flags.ais156_logAckRecvd = 0;
//				 }
//			 }
//			 else
//			 {
//				 //Do Nothing
//				 __asm("nop");
//			 }
//			 __asm("nop");
//			 /*If all requested logs have been sent OR an exit condition is true*/
//			 if(g_ais156Log_Handle.ais156_logsSent >= g_ais156Log_Handle.ais156_logsToRetrieve || exit == 1){
//				 /*Send ending process message over CAN*/
//				 ais156_sendEndProcessMsg(g_ais156Log_Handle.ais156_logsSent);
// 
//				 /*Reset all flags and counters*/
//				 g_ais156Log_Handle.ais156_logsSent = 0;
//				 g_ais156Log_Handle.ais156_logsToRetrieve = 0;
//				 sendLogsOverCAN = 0;
//				 g_ais156Log_Handle.ais156_logsRead = 0;
//				 g_ais156_Flags.ais156_logAckRecvd = 0;
// 
//				 /*Reset the cursor to beginning*/
//				 ais156_Rewind();
//				 ais156_loggingEnabled = 1;
// 
//				 /*Exit the while loop*/
//				 break;
//			 }
//		 #if LOG_ENTRY_VERSION == 3
//			 /*If the timer interrupt callback is greater than 80ms break the loop and continue in next callback*/
//			 if(GET_TICK() - initialTime > 90){
//				 __asm("nop");
//				 break;
//			 }
//		 #endif
//		 }
//	 }
//	 else{
//		 /*Just to make sure*/
//		 /*Reset all flags and counters*/
//		 g_ais156Log_Handle.ais156_logsSent = 0;
//		 g_ais156Log_Handle.ais156_logsToRetrieve = 0;
//		 sendLogsOverCAN = 0;
//		 g_ais156Log_Handle.ais156_logsRead = 0;
// 
//		 /*Reset the cursor to beginning*/
//		 ais156_Rewind();
//		 ais156_loggingEnabled = 1;
//	 }
//	 return ais156_loggingEnabled;
// }
// 
// /**
//  * @brief Formats a single sector and marks  it as free
//  * 
//  * @return int - 0,1 for format successfull or not respectively 
//  */
// int ais156_formatSector(void)
// {
//	 if(ais156_initFlags.u8Logging_Initialized != 1){
//		 return -1;
//	 }
//	 if(ais156_initFlags.u8Memory_Initialized != 1)
//	 {
//		 int8_t retVal = ringfs_formatSector(&fs,  0);// mcu_wwdgRefresh =0
//		 if((retVal == 0))
//		 {
//			 return 0;
//		 }
//		 else if(retVal == 2)
//		 {
//			 ais156_initFlags.u8Memory_Initialized = 1;
//			 return 0;
//		 }
//	 }
//	 return 1;
// }
// 
 /**
  * @brief Returns the logging init flag of ais156 sdk
  * 
  * @return int - 0,1 for initialized or not respectively
  */
 int ais156_getLoggingInitFlag(void)
 {
	 return ais156_initFlags.u8Logging_Initialized;
 }
 
 /**
  * @brief Returns the memory init flag of ais156 sdk
  * 
  * @return int - 0,1 for initialized or not respectively
  */
 int ais156_getMemoryInitFlag(void)
 {
	 return ais156_initFlags.u8Memory_Initialized;
 }
 
extern uint8_t ais_init_ok;
extern uint8_t ais_format_ok ;
extern uint8_t ais_append_ok ;
extern uint8_t ais_read_ok  ;
extern uint8_t ais_record_count_ok ;
 
 void ais_verify_startup(void)
{
    ais_init_ok = (ais156_loggingInit() == 0);
    ais_format_ok = (ais156_scanFormat() >= 0);

    // Prepare test log
    log_entry sample;
    memset(&sample, 0, sizeof(sample));
 
    ais_append_ok = (ais156_Append(&sample) == 0);

    log_entry readBack;
    if (ais156_Read(&readBack) == 0)

    ais_record_count_ok = (ais156_getRecordCountEstimate() > 0);
    
}

static void data_load(void);
 
extern volatile uint16_t fault_flags;
extern uint16_t volt_scaled;
extern int16_t current_scaled;
extern struct oz3717_chip oz3717_data;

extern uint16_t maxCellVoltage;
extern uint8_t maxVoltage_cellno;
extern uint16_t minCellVoltage;
extern uint8_t minVoltage_cellno;
extern int16_t maxTemp;
extern int16_t minTemp;
extern uint16_t mosFet,CC_soc;
extern struct oz3717_chip oz3717_data; 
uint32_t time1, time2 = 0;
uint8_t second, minute, hour, week, day, month, year;
uint32_t counter = 0;

void ais_eventLogging(void)
{
  //  if (!ais_init_ok) return;
        
       
        log_entry log;
        memset(&log, 0, sizeof(log));
        counter++;
        stc_rtc_time_t readtime;
        Rtc_ReadDateTime(&readtime);
  
       // 28-03-2000 22:57
        second = BCD2Dec(readtime.u8Second);
        minute = BCD2Dec(readtime.u8Minute);
        hour   = BCD2Dec(readtime.u8Hour);
        day    = BCD2Dec(readtime.u8Day);
        week   = BCD2Dec(readtime.u8DayOfWeek);
        month  = BCD2Dec(readtime.u8Month);
        year   = BCD2Dec(readtime.u8Year);
      
        time1 = ((uint32_t)minute << 24)| ((uint32_t)hour << 16) | ((uint32_t)day   << 8)|  (uint32_t)week;
        time2 = ((uint32_t)month << 24)| ((uint32_t)year << 16);
 
        log.tick  =  counter; 
        log.time =  time1;
        log.time2 = time2;
        log.faults = fault_flags; 
        log.packVolt = volt_scaled;
        log.packCurrent = current_scaled;
        log.RmCap = (uint8_t)CC_soc;
        log.FcCap = 105;
        log.maxCellVoltage = maxCellVoltage;
        log.maxVoltage_cellno = maxVoltage_cellno;
        log.minVoltage_cellno =  minVoltage_cellno;
        log.minCellVoltage = minCellVoltage;
        log.minTemp =  minTemp;
        log.maxTemp =  maxTemp;   
        log.mosFet = mosFet;
        ais156_Append(&log);
}



void ais_readAllLogs_intoBuffer(void)
{
    log_entry log;
    int status;

    logCount = 0;
    logsReady = false;

    // Move pointer to start
    if (ais156_Rewind() < 0)
        return;

    while (1)
    {
        status = ais156_Read(&log);

        if (status < 0)   // no more logs
            break;

        if (logCount < MAX_LOG_BUFFER)
        {
            logBuffer[logCount++] = log;   // store log entry
        }
        else
        {
            break;
        }
    }

    logsReady = true;    // tell main loop logs are ready
}

void ais_eraseAllLogs(void)
{
    // This erases the entire RingFS area and reinitializes it
    ringfs_format(&fs,0);

    // After erase, MUST re-scan & re-init
    ais156_scanFormat();
}


int istimate;
void ais_getrecode(void)
{
 istimate = ais156_getRecordCountEstimate();
}


extern void App_UartTransmitPoll(M0P_UART_TypeDef* UARTx, uint8_t* data, uint16_t length);
extern void revMemcpy(void * destination, void * source, uint32_t length);
extern uint16_t rs485_getChecksum(uint8_t *ptr, uint8_t len);
   
 

typedef struct __attribute__((packed)) {
    uint32_t tick;
    uint32_t time;
    uint32_t time2;
    uint16_t faults;
    uint16_t packVolt;
    int16_t packCurrent;
    uint8_t RmCap;
    uint8_t FcCap;
    uint16_t maxCellVoltage;
    uint8_t maxVoltage_cellno;
    uint16_t minCellVoltage;
    uint8_t minVoltage_cellno;
    int16_t maxTemp;
    int16_t minTemp;
    uint8_t mosFet;
} rs485_log;

 
 typedef struct{
	uint8_t startByte;
	uint8_t command_code;
	uint8_t status;
	uint8_t data_length;
	uint8_t data_buff[sizeof(rs485_log)];
	uint16_t checksum;
	uint8_t endByte;
}rs485_send_log;
 
extern uint16_t all_log;
 void sendAllLogs_RS485(void)
{
    log_entry log;
    rs485_send_log sendObj;
   int total = ais156_getRecordCountExact();   // total records
   int start = total;
   int end   = (total > all_log) ? (total - all_log + 1) : 1;


    for (int i = start; i >= end; i--)
    {
        memset(&log, 0, sizeof(log));
        ais156_ReadSpecific(&log, i);
 
    sendObj.startByte = 0x12;
    sendObj.command_code = 0x34;
    sendObj.status = 1;

    sendObj.data_length = (uint8_t)(sizeof(rs485_log));

  
rs485_log rs485_cellInfo_obj = {
    .tick               =  log.tick,
    .time               =  log.time, //0x160A1303
    .time2              =  log.time2,//0x0B190000
    .faults             =  log.faults,
    .packVolt           =  log.packVolt,
    .packCurrent        =  log.packCurrent,
    .RmCap              =  log.RmCap,
    .FcCap              =  log.FcCap,
    .maxCellVoltage     =  log.maxCellVoltage,
    .maxVoltage_cellno  =  log.maxVoltage_cellno,
    .minCellVoltage     =  log.minCellVoltage,
    .minVoltage_cellno  =  log.minVoltage_cellno,
    .maxTemp            =  log.maxTemp,
    .minTemp            =  log.minTemp,
    .mosFet             =  log.mosFet,
};

    // Copy all 16 cell voltages
    memcpy(sendObj.data_buff,
           &rs485_cellInfo_obj,
           sizeof(rs485_log));

    // --------------------------------------------
    // Prepare checksum buffer = status + length + data
    // --------------------------------------------
    uint8_t checksumDataBuff[2 + sizeof(rs485_log)];

    checksumDataBuff[0] = sendObj.status;
    checksumDataBuff[1] = sendObj.data_length;

    memcpy(&checksumDataBuff[2],
           sendObj.data_buff,
           sizeof(rs485_log));

    // --------------------------------------------
    // Calculate checksum
    // --------------------------------------------
    uint16_t tmpChecksum =
        rs485_getChecksum(checksumDataBuff,
                          sizeof(checksumDataBuff));

    revMemcpy(&sendObj.checksum,
              &tmpChecksum,
              sizeof(sendObj.checksum));

    sendObj.endByte = 0x1;

    // --------------------------------------------
    // Transmit entire object
    // --------------------------------------------
    App_UartTransmitPoll(M0P_UART3,
                         (uint8_t *)&sendObj,
                         sizeof(rs485_send_log));
   // delay1ms(100);
   
   for (volatile int i = 0; i < 10; i++) {
    __NOP();   // or asm("nop");
}
}
    //sendEndPacket_RS485(total);
}
