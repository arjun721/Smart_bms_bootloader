/*
 * Copyright © 2014 Kosma Moczek <kosma@cloudyourcar.com>
 * This program is free software. It comes without any warranty, to the extent
 * permitted by applicable law. You can redistribute it and/or modify it under
 * the terms of the Do What The Fuck You Want To Public License, Version 2, as
 * published by Sam Hocevar. See the COPYING file for more details.
 */

#ifndef RINGFS_H
#define RINGFS_H

/**
 * @defgroup ringfs_api RingFS API
 * @{
 */

#include <stdint.h>
#include <stdio.h>
//#include <unistd.h>

/**
 * Flash memory+parition descriptor.
 */
struct ringfs_flash_partition
{
    int sector_size;            /**< Sector size, in bytes. */
    int sector_offset;          /**< Partition offset, in sectors. */
    int sector_count;           /**< Partition size, in sectors. */

    /**
     * Erase a sector.
     * @param address Any address inside the sector.
     * @returns Zero on success, -1 on failure.
     */
    int (*sector_erase)(struct ringfs_flash_partition *flash, int address);
    /**
     * Program flash memory bits by toggling them from 1 to 0.
     * @param address Start address, in bytes.
     * @param data Data to program.
     * @param size Size of data.
     * @returns size on success, -1 on failure.
     */
    signed int (*program)(struct ringfs_flash_partition *flash, int address, const void *data, size_t size);
    /**
     * Read flash memory.
     * @param address Start address, in bytes.
     * @param data Buffer to store read data.
     * @param size Size of data.
     * @returns size on success, -1 on failure.
     */
    signed int (*read)(struct ringfs_flash_partition *flash, int address, void *data, size_t size);
};

/** @private */
struct ringfs_loc {
    int sector;
    int slot;
};

/**
 * RingFS instance. Should be initialized with ringfs_init() befure use.
 * Structure fields should not be accessed directly.
 * */
struct ringfs {
    /* Constant values, set once at ringfs_init(). */
    struct ringfs_flash_partition *flash;
    uint32_t version;
    int eraseCursor;
    int object_size;
    /* Cached values. */
    int slots_per_sector;

    /* Read/write pointers. Modified as needed. */
    struct ringfs_loc read;
    struct ringfs_loc write;
    struct ringfs_loc cursor;
};

struct ringfs_initFlags{                    //TODO: Either use bit fields or use one flag with enums
    uint8_t u8Logging_Initialized;
    uint8_t u8Memory_Initialized;
};

/**
 * Initialize a RingFS instance. Must be called before the instance can be used
 * with the other ringfs_* functions.
 *
 * @param fs RingFS instance to be initialized.
 * @param flash Flash memory interface. Must be implemented externally.
 * @param version Object version. Should be incremented whenever the object's
 *                semantics or size change in a backwards-incompatible way.
 * @param object_size Size of one stored object, in bytes.
 * @returns Zero on success, -1 on failure.
 */
int ringfs_init(struct ringfs *fs, struct ringfs_flash_partition *flash, uint32_t version, int object_size);

/**
 * Format the flash memory.
 *
 * @param fs Initialized RingFS instance.
 * @returns Zero on success, -1 on failure.
 */
int ringfs_format(struct ringfs *fs, int8_t (*wdg_refresh)());

/**
 * Scan the flash memory for a valid filesystem.
 *
 * @param fs Initialized RingFS instance.
 * @returns Zero on success, -1 on failure.
 */
int ringfs_scan(struct ringfs *fs);

/**
 * Calculate maximum RingFS capacity.
 *
 * @param fs Initialized RingFS instance.
 * @returns Maximum capacity on success, -1 on failure.
 */
int ringfs_capacity(struct ringfs *fs);

/**
 * Calculate approximate object count.
 * Runs in O(1).
 *
 * @param fs Initialized RingFS instance.
 * @returns Estimated object count on success, -1 on failure.
 */
int ringfs_count_estimate(struct ringfs *fs);

/**
 * Calculate exact object count.
 * Runs in O(n).
 *
 * @param fs Initialized RingFS instance.
 * @returns Exact object count on success, -1 on failure.
 */
int ringfs_count_exact(struct ringfs *fs);

/**
 * Append an object at the end of the ring. Deletes oldest objects as needed.
 *
 * @param fs Initialized RingFS instance.
 * @param object Object to be stored.
 * @returns Zero on success, -1 on failure.
 */
int ringfs_append(struct ringfs *fs, const void *object);

/**
 * Fetch next object from the ring, oldest-first. Advances read cursor.
 *
 * @param fs Initialized RingFS instance.
 * @param object Buffer to store retrieved object.
 * @returns Zero on success, -1 on failure.
 */
int ringfs_fetch(struct ringfs *fs, void *object);

/**
 * Discard all fetched objects up to the read cursor.
 *
 * @param fs Initialized RingFS instance.
 * @returns Zero on success, -1 on failure.
 */
int ringfs_discard(struct ringfs *fs);

int ringfs_item_discard(struct ringfs *fs);

/**
 * Rewind the read cursor back to the oldest object.
 *
 * @param fs Initialized RingFS instance.
 * @returns Zero on success, -1 on failure.
 */
int ringfs_rewind(struct ringfs *fs);


/**
 * Set the read cursor to the specific object.
 *
 * @param fs Initialized RingFS instance.
 * @returns recordNumber on success, -1 on failure.
 */
int ringfs_advance_slot_specific(struct ringfs *fs, uint32_t moveToLogNum);


int ringfs_getTotalSlotsandLatesValidSlot(struct ringfs *fs, uint32_t * latestValidSlot);

/**
* @brief Formatting sector and marking it as free
* 
* @param fs file structure to be used
* @param wdg_refresh watchdog function
* @return int 0,1 for successfull formatting or failure respectively
*/
int ringfs_formatSector(struct ringfs *fs, int8_t (*wdg_refresh)());

/**
* @brief To be called when flash memory is under formatting and is being initialised in 100ms callback.
*        We check the availability of sector before writing to the memory in th above case.
* @param fs file structure to be checked
* @param AvailableSector Number of sectors initialised
* @param objSize Size of object to be appended , used for comparison purpose
* @return int 0,1 for space available or not respectively
*/
int ringfs_checkSectorAvailable(struct ringfs *fs,int AvailableSector,int objSize);
/**
 * 
/**
 * Dump filesystem metadata. For debugging purposes.
 * @param stream File stream to write to.
 * @param fs Initialized RingFS instance.
 */

/*
void ringfs_dump(FILE *stream, struct ringfs *fs);
*/

/**
 * @}
 */

#endif

/* vim: set ts=4 sw=4 et: */
