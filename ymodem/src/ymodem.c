//
// Created by Taseny on 24-12-27.
//

#include "ymodem.h"

extern FIL file;

uint8_t packet_start = 0;
uint8_t packet_revieved = 0;
uint8_t ymodem_packet_state = YMODEM_INVALID;
uint8_t ymodem_receive_packet[PACKET_SIZE_1024 + PACKET_OVERHEAD];

static void send_byte(const uint8_t c) {
    HAL_UART_Transmit(&huart1, &c, 1, HAL_MAX_DELAY);
}

static int receive_packet(uint32_t timeout) {
    uint32_t time_start = HAL_GetTick();
    while (!packet_revieved)
    {
        if (HAL_GetTick() - time_start > timeout) {
            return -1; // 超时
        }
    }
    packet_revieved = 0;
    return 0;
}

/**
  * @brief  Update CRC16 for input byte
  * @param  crc_in input value
  * @param  input byte
  * @retval None
  */
static uint16_t UpdateCRC16(uint16_t crc_in, uint8_t byte)
{
    uint32_t crc = crc_in;
    uint32_t in = byte | 0x100;

    do
    {
        crc <<= 1;
        in <<= 1;
        if(in & 0x100)
            ++crc;
        if(crc & 0x10000)
            crc ^= 0x1021;
    }

    while(!(in & 0x10000));

    return crc & 0xffffu;
}

/**
 * @brief  Calculate CRC16 checksum
 */
static uint16_t calculate_crc16(const uint8_t *pdata, uint16_t size) {
    uint32_t crc = 0;
    const uint8_t* dataEnd = pdata+size;

    while(pdata < dataEnd)
        crc = UpdateCRC16(crc, *pdata++);

    crc = UpdateCRC16(crc, 0);
    crc = UpdateCRC16(crc, 0);

    return crc&0xffffu;
}

YmodemStatus ymodem_receive(YmodemFileInfo *file_info)
{
    char file_path[128];
    char received_size[128];

    uint8_t head_flag = 0;
    uint8_t file_open_flag = 0;
    uint8_t eot_flag = 0;
    uint8_t packet_number = 0;
    uint8_t retries = 0;

    uint16_t received_crc;
    uint16_t calculated_crc;
    uint16_t packet_size = 0;

    UINT bytes_written_total = 0;
    UINT bytes_written = 0;

    send_byte(CRC16);
    // 发送初始 'C' 表示准备接收
    while (1)
    {
       if (receive_packet(WAIT_MSENCOND) != 0)
       {
           if (++retries > MAX_ERRORS)
           {
               send_byte(CAN);
               if (file_open_flag)
               {
                   f_close(&file);
                   f_unlink(file_path);
               }
               return YMODEM_TIMEOUT;
           }
           continue;
       }

       switch (ymodem_receive_packet[0])
       {
       case SOH:
           packet_size = PACKET_SIZE_128;
           if (eot_flag)
           {
               if (ymodem_receive_packet[1] == 0x00 && ymodem_receive_packet[2] == 0xFF && 0x0000 == (ymodem_receive_packet[packet_size + 3] << 8 | ymodem_receive_packet[packet_size + 4]))
               {
                   f_close(&file);
                   return YMODEM_OK;
               }
           }

           received_crc = ymodem_receive_packet[packet_size + 3] << 8 | ymodem_receive_packet[packet_size + 4];
           calculated_crc = calculate_crc16(&ymodem_receive_packet[3], packet_size);

           if (received_crc != calculated_crc)
           {
               if (++retries > MAX_ERRORS)
               {
                   send_byte(CAN);
                   if (file_open_flag)
                   {
                       f_close(&file);
                       f_unlink(file_path);
                   }
                   return YMODEM_ERROR;
               }
               send_byte(NAK);
               continue;
           }
           break;

       case STX:
           packet_size = PACKET_SIZE_1024;

           received_crc = (ymodem_receive_packet[packet_size + 3] << 8) | (ymodem_receive_packet[packet_size + 4]);
           calculated_crc = calculate_crc16(&ymodem_receive_packet[3], packet_size);

           if (received_crc != calculated_crc)
           {
               if (++retries > MAX_ERRORS)
               {
                   send_byte(CAN);
                   if (file_open_flag)
                   {
                       f_close(&file);
                       f_unlink(file_path);
                   }
                   return YMODEM_ERROR;
               }
               send_byte(NAK);
               continue;
           }
           break;

       case EOT:
            if (eot_flag == 0)
            {
                eot_flag = 1;
                send_byte(NAK);
                continue;
            }
            send_byte(ACK);
            send_byte(CRC16);
           break;
       }

        if (ymodem_receive_packet[0] == SOH || ymodem_receive_packet[0] == STX)
        {
            if (!head_flag && ymodem_receive_packet[1] == 0x00 && ymodem_receive_packet[2] == 0xFF)
            {
                packet_number++;
                send_byte(ACK);
                send_byte(CRC16); // 准备接收数据
                head_flag = 1;
                for (uint8_t i = 3, j = 0; i < 128;j++)
                {
                    if (j < i && ymodem_receive_packet[i] == 0x00)
                    {
                        j = i + 1;
                        strncpy(file_info->file_name, (char *)&ymodem_receive_packet[3], i - 3);
                    }
                    else if (j < i)
                    {
                        i++;
                    }

                    if (j > i && ymodem_receive_packet[j] == 0x00)
                    {
                        strncpy(received_size, (char *)&ymodem_receive_packet[i+1], j - 4 - strlen(file_info->file_name));
                        file_info->file_size = atoi(received_size);
                        break;
                    }
                }

                sprintf(file_path,"%s%s","1:",file_info->file_name);

                if (f_open(&file, file_path, FA_CREATE_ALWAYS | FA_WRITE)!= FR_OK) {
                    send_byte(CAN);
                    return YMODEM_ERROR;
                }
                f_lseek(&file, 0);
                file_open_flag = 1;
                continue;
            }

            if (head_flag && ymodem_receive_packet[1] == (packet_number & 0xFF) && ymodem_receive_packet[2] == (~packet_number & 0xFF))
            {
                packet_number++;
                send_byte(ACK);
                if (file_info->file_size - bytes_written_total >= packet_size)
                {
                    if (f_write(&file, &ymodem_receive_packet[3], packet_size, &bytes_written)!= FR_OK) {
                        send_byte(CAN);
                        return YMODEM_ERROR;
                    }
                }

                else
                {
                    if (f_write(&file, &ymodem_receive_packet[3], (file_info->file_size - bytes_written_total), &bytes_written)!= FR_OK)
                    {
                        send_byte(CAN);
                        return YMODEM_ERROR;
                    }
                }
                bytes_written_total += bytes_written;
                if (bytes_written_total % 4096 == 0)
                { // 例如每接收10个数据包时
                    f_sync(&file);
                }
            }
        }
    }
}
