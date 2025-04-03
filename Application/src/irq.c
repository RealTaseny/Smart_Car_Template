//
// Created by Taseny on 24-11-11.
//

#include "irq.h"

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    #ifdef USE_DBG_UART
    if (huart == &huart1)
    {
        packet_revieved = 0;
        if (rx_buffer_index < UART_BUFFER_SIZE - 1)
        {
            if (!packet_start && rx_data == EOT)
            {
                packet_revieved = 1;
                ymodem_receive_packet[0] = rx_data;
            }

            if (!packet_start && (rx_data == SOH || rx_data == STX))
            {
                packet_start = 1;
            }

            if (packet_start)
            {
                rx_buffer[rx_buffer_index] = rx_data;
                rx_buffer_index++;
                if (!packet_revieved)
                {
                    if (rx_buffer[0] == SOH && rx_buffer_index == PACKET_SIZE_128 + PACKET_OVERHEAD)
                    {
                        packet_start = 0;
                        packet_revieved = 1;
                        memcpy(ymodem_receive_packet, rx_buffer, PACKET_SIZE_128 + PACKET_OVERHEAD);
                        rx_buffer_index = 0;
                        memset(rx_buffer, 0, sizeof(rx_buffer));
                    }

                    if (rx_buffer[0] == STX && rx_buffer_index == PACKET_SIZE_1024 + PACKET_OVERHEAD)
                    {
                        packet_start = 0;
                        packet_revieved = 1;
                        memcpy(ymodem_receive_packet, rx_buffer, PACKET_SIZE_1024 + PACKET_OVERHEAD);
                        rx_buffer_index = 0;
                        memset(rx_buffer, 0, sizeof(rx_buffer));
                    }
                }
            }
        }

        else
        {
            printf("\r\nERROR:Get the Max UART rx buffer!\r\n");
            rx_buffer_index = 0;
            memset(rx_buffer, 0, sizeof(rx_buffer));
        }
        HAL_UART_Receive_IT(&huart1, &rx_data, 1);
    }
    #endif

    #ifdef USE_WIRELESS_UART
    if (huart == &huart2)
    {
        if (rx_buffer_index < UART_BUFFER_SIZE - 1)
        {
            if (rx_buffer[rx_buffer_index - 1] == '!' && rx_buffer[rx_buffer_index] == '#')
            {
                /*字符串处理 帧尾\r\n*/
                CMD_structure.recieved_complete_flag = 1;
                memcpy(CMD_structure.uart_rx_data_packet, rx_buffer, sizeof(rx_buffer));
                CMD_structure.frame_packet_size = rx_buffer_index + 1;
                rx_buffer_index = 0;
                memset(rx_buffer, 0, sizeof(rx_buffer));    //Clear the rx_buffer
            }
            else
            {
                rx_buffer_index++;
            }
        }
        else
        {
            printf("\r\nERROR:Get the Max UART rx buffer!\r\n");
            rx_buffer_index = 0;
            memset(rx_buffer, 0, sizeof(rx_buffer));
        }
    }
    HAL_UART_Receive_IT(&huart2, &rx_buffer[rx_buffer_index], 1);
    #endif

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    UNUSED(HAL_GPIO_EXTI_Callback);
}
