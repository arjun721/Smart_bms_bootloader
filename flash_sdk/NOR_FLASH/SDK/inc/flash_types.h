#include "stdint.h"
#include "stdbool.h"

#ifndef INC_FLASH_TYPES_H_
#define INC_FLASH_TYPES_H_

/*------------------ Select Flash ------------------*/
#define NOR_FLASH   32    // Using W25Q32JV (32 Mbit / 4 MB)

/*------------------ Flash Size Configuration ------------------*/
#if NOR_FLASH == 128        // 128 Mbit (16 MB)
#define BLOCK_COUNT         256
#define SECTORS             4096
#define PAGES_PER_SECTOR    16

#elif NOR_FLASH == 32       // 32 Mbit (4 MB)
#define BLOCK_COUNT         128
#define SECTORS             2048
#define PAGES_PER_SECTOR    16

#elif NOR_FLASH == 16       // 16 Mbit (2 MB)
#define BLOCK_COUNT         64
#define SECTORS             512
#define PAGES_PER_SECTOR    16

#else
#error "NOR Flash not Selected"
#endif

/*------------------ Derived Constants ------------------*/
#define PAGE_SIZE           256
#define SECTOR_SIZE         4096
#define PAGES               (PAGES_PER_SECTOR * SECTORS)

#define FLASH_START_ADDRESS 0

#if NOR_FLASH == 128
#define FLASH_LAST_ADDRESS  0xFFFFFF      // 16 MB
#elif NOR_FLASH == 32
#define FLASH_LAST_ADDRESS  0x3FFFFF      // 4 MB (W25Q32)
#elif NOR_FLASH == 16
#define FLASH_LAST_ADDRESS  0x1FFFFF      // 2 MB
#else
#error "NOR Flash Last Address not defined"
#endif

/*------------------ Address Calculation Macros ------------------*/
#define PAGE_NUMBER(address)        ((uint32_t)(address / PAGE_SIZE))
#define PAGE_ADDR_FROM_NUM(pNum)    ((uint32_t)(pNum * PAGE_SIZE))
#define PAGE_ADDRESS(address)       ((PAGE_NUMBER(address)) * PAGE_SIZE)

#define SECTOR_NUMBER(address)      ((uint32_t)(address / SECTOR_SIZE))
#define SECTOR_ADDR(secNum)         ((uint32_t)(secNum * SECTOR_SIZE))

#if NOR_FLASH == 128
#define BLOCK_NUMBER(address)       ((uint32_t)(address / 65536))   // 64KB block
#else   // W25Q32 or W25Q16
#define BLOCK_NUMBER(address)       ((uint32_t)(address / 32768))   // 32KB block
#endif

/*------------------ Flash Command Opcodes ------------------*/
#define WriteEnable     0x06
#define WriteDisable    0x04

#define ReadData        0x03
#define PageProgram     0x02

#define PageErase       0x81
#define SectorErase     0x20
#define BlockErase      0xD8    // Valid for W25Q16/W25Q32/W25Q128
#define ChipErase       0xC7    // 0x60 or 0xC7 both work

#define DEVICE_ID       0x90    // Manufacturer ID
#define UID             0x4B
#define READ_ID         0x9F

#define STAT_REG_1      0x05
#define STAT_REG_2      0x35

/*------------------ Flash Status and Enums ------------------*/
typedef enum {
    FLASH_STATUS_ERROR             = -1,
    FLASH_STATUS_OK                = 0,
    FLASH_STATUS_WRITE_ALLOWED     = 1,
    FLASH_STATUS_WRITE_BLOCKED     = 2,
    FLASH_BUSY_IN_OPERATION        = 3,
} flashStatus_t;

typedef enum {
    FLASH_PAGE_ERASE   = PageErase,
    FLASH_SECTOR_ERASE = SectorErase,
    FLASH_BLOCK_ERASE  = BlockErase,
    FLASH_CHIP_ERASE   = ChipErase,
} flash_eraseCmds;

/*------------------ Status Register Bit Definitions ------------------*/
typedef struct {
    /* Byte 0 */
    uint8_t WIP     : 1;    // Write in progress
    uint8_t WEL     : 1;    // Write enable latch
    uint8_t BP0     : 1;    // Block protect bits
    uint8_t BP1     : 1;
    uint8_t BP2     : 1;
    uint8_t BP3     : 1;
    uint8_t BP4     : 1;
    uint8_t SRP0    : 1;    // Status register protect

    /* Byte 1 */
    uint8_t SRP1    : 1;
    uint8_t QE      : 1;    // Quad enable
    uint8_t SUS2    : 1;
    uint8_t LB1     : 1;
    uint8_t LB2     : 1;
    uint8_t LB3     : 1;
    uint8_t CMP     : 1;
    uint8_t SUS1    : 1;
} flash_statusReg_Struct;

#endif /* INC_FLASH_TYPES_H_ */
