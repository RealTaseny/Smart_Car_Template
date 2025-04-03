//
// Created by Taseny on 24-11-12.
//

#ifndef VOFA_H_
#define VOFA_H_

#include "usart.h"

#define INVALID 0xFF						//定义指令无效码0xFF;
#define FRAME_TAIL_SIZE 4				    //Justfloat协议帧尾大小
#define MAX_CHANNEL_SIZE 16					//JustFloat协议通道数量, 请与枚举变量相同

enum JustFloat_Channels {					//枚举变量通道标号
    CH1,
    CH2,
    CH3,
    CH4,
    CH5,
    CH6,
    CH7,
    CH8,
    CH9,
    CH10,
    CH11,
    CH12,
    CH13,
    CH14,
    CH15
};

enum command_types
{
    BALANCE,
    SPEED,
    TURN
};

enum command_ids
{
    DIRECT,
    INCRESE,
    DECRESE
};

enum command_objects
{
    KP,
    KI,
    KD
};

typedef struct JustFloat_Frame_structure{						//JustFloat协议数据结构体
    float float_data[MAX_CHANNEL_SIZE];								//JustFloat通道数据
    uint8_t frametail[FRAME_TAIL_SIZE];			                //JustFloat帧尾数组
}JustFloat_Frame_structure;

typedef struct VOFA_command_structure{
    uint8_t recieved_complete_flag;
    uint8_t frame_valid_flag;
    uint8_t command_type;
    uint8_t command_id;
    uint8_t command_object;
    uint8_t uart_rx_data_packet[UART_BUFFER_SIZE];			    //串口数据包接收数组
    uint8_t valid_bytes[4];
    uint8_t frame_packet_size;
    float float_data;
}VOFA_command_structure;


void vofa_init(VOFA_command_structure *ptr_CMD_structure, JustFloat_Frame_structure *ptr_JF_structure);
void vofa_rawdata_send(uint8_t *ptr_byte_data);
void vofa_firewater_send(float float_data, uint8_t is_last_data);
void vofa_justfloat_send(JustFloat_Frame_structure *ptr_structure, uint8_t channel_size);
void vofa_command_transfer(VOFA_command_structure *ptr_CMD_structure);
void vofa_command_execute(VOFA_command_structure *ptr_CMD_structure);


extern JustFloat_Frame_structure JF_structure;
extern VOFA_command_structure CMD_structure;


#endif
