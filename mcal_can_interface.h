#ifndef _MCAL_CAN_INTERFACE_H_
#define _MCAL_CAN_INTERFACE_H_

#include "can.h"
#include "gpio.h"
#include "reset.h"

#define BMS_CAN_TX_PORT      GpioPortB
#define BMS_CAN_TX_PIN       GpioPin9     
#define BMS_CAN_RX_PORT      GpioPortB
#define BMS_CAN_RX_PIN       GpioPin8    

typedef struct {
	uint32_t id;
	uint8_t data[8];
	uint8_t length;
	uint8_t resv[3];
} can_buff_t;

typedef struct {
	uint8_t front;
	uint8_t rear;
	uint8_t size;
	uint8_t element_size;
	can_buff_t * buffer;
} can_queue_t;


typedef struct __attribute__((packed))
{
    uint32_t u32_can_id;
    uint8_t u8_dlc;
    uint8_t u8_data[8];
} can_frame_info_t;

#define QUEUE(name, buff)                           \
        can_queue_t name = {                        \
                .front = 0,                         \
                .rear = 0,                          \
                .size = (sizeof(buff)/sizeof(buff[0])),  \
                .element_size = sizeof(buff[0]),         \
                .buffer = buff,                          \
}

#define FLUSH(name) do {      \
        name.front = 0;       \
        name.rear = 0;        \
        memset(name.buffer, 0x00, name.size);  \
} while(0)

#define ENQUEUE(queue, data) do {      \
        memcpy(&queue.buffer[queue.front++], &data, sizeof(data));     \
        queue.front %= queue.size;                                     \
} while(0)


#define DEQUEUE(queue, data) do {                                 \
        memcpy(&data, &queue.buffer[queue.rear++], sizeof(data)); \
        queue.rear %= queue.size;                                 \
} while(0)

#define IS_FULL(queue) (((queue.front - queue.rear)    == queue.size - 1)    ||    \
        (queue.front - queue.rear    ==    -1))

#define IS_EMPTY(queue) (queue.front == queue.rear)


void 
App_CanGpioInit(void);

void 
App_CanInit(void);

void 
App_CanTransmit(uint32_t can_id, uint8_t *data, uint8_t dlc);

void
App_CanFrameFromQueue(can_frame_info_t *pkt);

void 
App_CanMessageHook(can_frame_info_t *p_can_frame);

uint8_t
App_CanReceive(uint32_t id, uint8_t* data, uint8_t len);

 


#endif