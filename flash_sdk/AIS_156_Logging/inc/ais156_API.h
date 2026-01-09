
 #ifndef INC_AIS156_API_H_
 #define INC_AIS156_API_H_
 
 #include "ais156_logging_interface.h"
 #include "ais156_sendLogs.h"
 
 
 #define BMS_STATE_IDLE   0
 
 #define AIS156_INIT_SECTORS    5
 
 /**
  * @brief Initializes the logging system.
  * 
  * @details This function initializes the logging system by setting up the filesystem in the flash memory and \n
  * 			allocating it to the filesystem structure, with the indicated log entry version number and size.
  *
  * @returns 0 on success, -1 on failure.
  */
 int ais156_loggingInit(void);
 
 /**
  * Scans for existing ring buffer file system
  * and formats the FLASH if no file system found.
  *
  * @returns 0 if the ring buffer is formatted, otherwise 1.
  */
 int ais156_scanFormat(void);
 
 /**
  * Formats the flash storage.
  *
  * @returns 0 on success, -1 on failure.
  */
 int ais156_formatFlash(void);
 
 /**
  * Appends an object to the log.
  *
  * @param object The object to append.
  *
  * @returns 0 on success, -1 on failure.
  */
 int ais156_Append(const log_entry *object);
 
 /**
  * Reads an entry from the logged data.
  *
  * @param object The log entry to read.
  *
  * @returns 0 on success, -1 on failure.
  */
 int ais156_Read(log_entry* object);
 
 /**
  * Reads a specific log entry from the flash.
  *
  * @param object The log entry to be read.
  * @param logNum The log number to be read.
  *
  * @returns 0 on success, -1 on failure.
  */
 int ais156_ReadSpecific(log_entry* object, uint32_t logNum);
 
 /**
  * Advances the read cursor to the specified log sector.
  *
  * @param logNum The log sector to advance to.
  *
  * @returns 0 on success, -1 on failure.
  */
 int ais156_advanceReadCursor(uint32_t logNum);
 
 /**
  * Discards the records in the ring buffer.
  *
  * @returns None
  */
 int ais165_discardRecords(void);
 
 /**
  * Rewinds the file pointer to the beginning of the file.
  *
  * @returns 0 on success, -1 on failure.
  */
 int ais156_Rewind(void);
 
 /**
  * Returns the number of records in the file by counting total logs, runs in O(n).
  *
  * @returns The number of records in the file.
  */
 int ais156_getRecordCountExact(void);
 
 /**
  * Returns the number of records in the file by calculating the total logs, runs in O(1).
  *
  * @returns The number of records in the file.
  */
 int ais156_getRecordCountEstimate(void);
 
 /**
  * Reads the next entry from the log file and rewinds it.
  *
  * @param object The log entry to read.
  *
  * @returns 0 on success, -1 on failure.
  */
 int ais156_readAndRewind(log_entry* object);
 
 /**
  * Reads an object from the ring buffer and discards it.
  *
  * @param object The object to read and discard.
  *
  * @returns 0 on success, -1 on failure.
  */
 int ais156_readAndDiscard(log_entry* object);
 
 /**
  * Sends a log entry over CAN.
  *
  * @param object The log entry to send.
  * @param logNumber The log number to send the entry to.
  *
  * @returns 0 on success, -1 on failure.
  */
 int ais156_sendLogsOverCAN(log_entry* object, uint32_t logNumber);
 
 /**
  * Sends the meta data over CAN.
  *
  * @param totalLogs The total number of logs to be sent.
  *
  * @returns 0 on success, -1 on failure.
  */
 int ais156_metaData(uint32_t totalLogs, uint32_t loggingInterval_s);
 
 /**
  * Sends the end of the transmission message over CAN.
  *
  * @param logsSent The number of logs sent to the server.
  *
  * @returns 0 on success, -1 on failure.
  */
 int ais156_logTransmissionEnd(uint32_t logsSent);
 
 /**
  * @brief Formats a single sector and marks  it as free
  * 
  * @return int - 0,1 for format successfull or not respectively 
  */
 int ais156_formatSector(void);
 
 /**
  * @brief Returns the logging init flag of ais156 sdk
  * 
  * @return int - 0,1 for initialized or not respectively
  */
 int ais156_getLoggingInitFlag(void);
 
 /**
  * @brief Returns the memory init flag of ais156 sdk
  * 
  * @return int - 0,1 for initialized or not respectively
  */
 int ais156_getMemoryInitFlag(void);
 
 /**
  * @brief CAN messages handler for AIS Data Logging feature
  * 
  * @param Identifier CAN ID received
  * @param data CAN Data received
  */
 void ais156_DataGetHandle(uint32_t Identifier, uint8_t *data);
 
 /**
  * @brief Snapshot feature handler which performs the following functions:
  *          - Send total size written in Snapshot memory region in flash
  *          - Send Data in while loop
  *          - Send Specific Data
  *          - Erase Snapshot memory in flash
  */
 uint8_t ais156_DataGetRoutine(uint32_t initialTime);
 
  void ais_verify_startup(void);
  void ais_getrecode(void);
   void sendAllLogs_RS485(void);
 
 #endif /* INC_AIS156_API_H_ */
 