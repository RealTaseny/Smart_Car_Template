//
// Created by Taseny on 24-12-27.
//

#ifndef YMODEM_H
#define YMODEM_H

#include "usart.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ff.h"

#define SOH  0x01  // 128-byte data packet
#define STX  0x02  // 1024-byte data packet
#define EOT  0x04  // End of Transmission
#define ACK  0x06  // Acknowledge
#define NAK  0x15  // Negative Acknowledge
#define CAN  0x18  // Cancel
#define CRC16 0x43 // 'C' for CRC16 checksum

#define PACKET_SIZE_128 128
#define PACKET_SIZE_1024 1024
#define PACKET_OVERHEAD 5 // 3-byte header + 2-byte CRC
#define MAX_ERRORS 5
#define WAIT_MSENCOND 1000

typedef enum {
    YMODEM_OK = 0,
    YMODEM_ERROR,
    YMODEM_TIMEOUT,
    YMODEM_FILE_END,
    YMODEM_ABORT
} YmodemStatus;

enum {
    YMODEM_INVALID,
    PACKET_SIZE_128_START,
    PACKET_SIZE_1024_START,
    PACKET_EOT
};

typedef struct {
    char file_name[128];
    UINT file_size;
} YmodemFileInfo;

YmodemStatus ymodem_receive(YmodemFileInfo *file_info);

extern uint8_t packet_start;
extern uint8_t packet_revieved;
extern uint8_t ymodem_packet_state;
extern uint8_t ymodem_receive_packet[PACKET_SIZE_1024 + PACKET_OVERHEAD];

#endif //YMODEM_H
