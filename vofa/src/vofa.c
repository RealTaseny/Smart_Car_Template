//
// Created by Taseny on 24-11-12.
//

#include "vofa.h"
#include "Fomatting.h"
#include "pid.h"

JustFloat_Frame_structure JF_structure;
VOFA_command_structure CMD_structure;

static void uart_send_bytes(uint8_t *ptr_bytes, uint8_t psize)
{
	while (psize){

		/*移植此处替换为串口发送单个字节函数接口*/

#ifdef USE_DBG_UART
		HAL_UART_Transmit(&huart1, ptr_bytes, 1, HAL_MAX_DELAY);
#endif

#ifdef USE_WIRELESS_UART
		HAL_UART_Transmit(&huart2, ptr_bytes, 1, HAL_MAX_DELAY);
#endif

		/*移植此处替换为串口发送单个字节函数接口*/

		ptr_bytes++;
		psize--;
	}
}

void vofa_init(VOFA_command_structure *ptr_CMD_structure, JustFloat_Frame_structure *ptr_JF_structure){
	static const uint8_t justfloat_frame_tail[4] = {
		0x00,	//first byte
		0x00,	//second byte
		0x80,	//third byte
		0x7f	//fourth byte
	};

	ptr_CMD_structure->command_id = INVALID;															// 初始化指令ID
	ptr_CMD_structure->command_type = INVALID;															// 初始化指令类型
	ptr_CMD_structure->command_object = INVALID;														// 初始化指令对象
	ptr_CMD_structure->float_data = 0;
	ptr_CMD_structure->frame_valid_flag = 0;															// 初始化帧合法标志位
	ptr_CMD_structure->recieved_complete_flag = 0;
	ptr_CMD_structure->frame_packet_size = 0;

	memset(ptr_CMD_structure->uart_rx_data_packet, 0, sizeof(ptr_CMD_structure->uart_rx_data_packet));
	memset(ptr_CMD_structure->valid_bytes, 0, sizeof(ptr_CMD_structure->valid_bytes));

	/**************帧尾数组***********/
	memcpy(ptr_JF_structure->frametail, justfloat_frame_tail, sizeof(justfloat_frame_tail));
}

void vofa_rawdata_send(uint8_t *ptr_byte_data){
	uart_send_bytes(ptr_byte_data, sizeof(ptr_byte_data));
}

void vofa_firewater_send(const float float_data, const uint8_t is_last_data){
	if (is_last_data) printf("%.6f\r\n", float_data);
	else printf("%.6f,", float_data);
}

void vofa_justfloat_send(JustFloat_Frame_structure *ptr_structure, const uint8_t channel_size)
{
	uint8_t uint8_t_array[4];
	for (uint8_t i = 0; i < channel_size; i++)
	{
		float2uint8_t_array(uint8_t_array, &ptr_structure->float_data[i]);
		uart_send_bytes(uint8_t_array, sizeof(uint8_t_array));
	}
	uart_send_bytes(ptr_structure->frametail, sizeof(ptr_structure->frametail));			//send the frame tail data
}

/*
 * '@'+'B/S/T'+'1/2/3'+'P/I/D'+'='+data+'!'+'#'
 */
void vofa_command_transfer(VOFA_command_structure *ptr_CMD_structure)
{
	uint8_t *Ptr_to_Rx_Packet = ptr_CMD_structure->uart_rx_data_packet;											//将指针指向接受数据包
	/*做指令分析判断，清空接受缓存数据包中错误指令*/

	if (*Ptr_to_Rx_Packet == '@' && *(Ptr_to_Rx_Packet + 4) == '=' && *(Ptr_to_Rx_Packet + ptr_CMD_structure->frame_packet_size - 2) == '!' && *(Ptr_to_Rx_Packet + ptr_CMD_structure->frame_packet_size - 1) == '#')
	{
		ptr_CMD_structure->frame_valid_flag = 1;

	}
	else
	{
		ptr_CMD_structure->frame_valid_flag = 0;
		memset(Ptr_to_Rx_Packet, 0, ptr_CMD_structure->frame_packet_size);
		return;
	}
	/*判别操作对象*/

	switch (*(Ptr_to_Rx_Packet + 1)) {
		case 'B':
			ptr_CMD_structure->command_type = BALANCE;
			break;
		case 'S':
			ptr_CMD_structure->command_type = SPEED;
			break;
		case 'T':
			ptr_CMD_structure->command_type = TURN;
			break;
		default:
			ptr_CMD_structure->command_type = INVALID;
			break;
	}

	/*判别操作码*/
	switch (*(Ptr_to_Rx_Packet + 2)) {
	case '1':
		ptr_CMD_structure->command_id = DIRECT;
		break;
	case '2':
		ptr_CMD_structure->command_id = INCRESE;
		break;
	case '3':
		ptr_CMD_structure->command_id = DECRESE;
		break;
	default:
		ptr_CMD_structure->command_id = INVALID;
		break;
	}

	/*判别操作变量*/
	switch (*(Ptr_to_Rx_Packet + 3)) {
	case 'P':
		ptr_CMD_structure->command_object = KP;
		break;
	case 'I':
		ptr_CMD_structure->command_object = KI;
		break;
	case 'D':
		ptr_CMD_structure->command_object = KD;
		break;
	default:
		ptr_CMD_structure->command_object = INVALID;
		break;
	}

	if (ptr_CMD_structure->command_type == INVALID || ptr_CMD_structure->command_id == INVALID || ptr_CMD_structure->command_object == INVALID)
	{
		ptr_CMD_structure->frame_valid_flag = 0;
		memset(Ptr_to_Rx_Packet, 0, ptr_CMD_structure->frame_packet_size);
		return;
	}

	memcpy(ptr_CMD_structure->valid_bytes, (Ptr_to_Rx_Packet + 5), 4);						//捕获有效浮点数据(四个字节)
	uint8_t_array2float(ptr_CMD_structure->valid_bytes, &ptr_CMD_structure->float_data);					//将暂存在数组中的浮点字节数据转换回浮点数据

	Ptr_to_Rx_Packet = NULL;												//清空指针
	memset(ptr_CMD_structure->valid_bytes, 0, 4);											//清空暂存浮点字节数据的数组
	memset(ptr_CMD_structure->uart_rx_data_packet, 0, ptr_CMD_structure->frame_packet_size);							//清空串口接收缓存的数据包
}

void vofa_command_execute(VOFA_command_structure *ptr_CMD_structure)
{
	if (!ptr_CMD_structure->frame_valid_flag) return;
	switch (ptr_CMD_structure->command_type)
	{
		case BALANCE:
			switch (ptr_CMD_structure->command_object)
			{
				case KP:
					switch (ptr_CMD_structure->command_id)
					{
						case DIRECT:
							balance_pid_structure.Kp = ptr_CMD_structure->float_data;
						break;

						case INCRESE:
							balance_pid_structure.Kp += ptr_CMD_structure->float_data;
						break;

						case DECRESE:
							balance_pid_structure.Kp -= ptr_CMD_structure->float_data;
						break;

						default:
							return;
					}
				break;

				case KD:
					switch (ptr_CMD_structure->command_id)
					{
						case DIRECT:
							balance_pid_structure.Kd = ptr_CMD_structure->float_data;
						break;

						case INCRESE:
							balance_pid_structure.Kd += ptr_CMD_structure->float_data;
						break;

						case DECRESE:
							balance_pid_structure.Kd -= ptr_CMD_structure->float_data;
						break;

						default:
							return;
					}
				break;

				default:
					return;
			}
			balance_pid_init(&balance_pid_structure, balance_pid_structure.Kp, balance_pid_structure.Kd);
			ptr_CMD_structure->frame_valid_flag = 0;
			break;

		case SPEED:
			switch (ptr_CMD_structure->command_object)
			{
				case KP:
					switch (ptr_CMD_structure->command_id)
					{
						case DIRECT:
							velocity_pid_structure.Kp = ptr_CMD_structure->float_data;
						break;

						case INCRESE:
							velocity_pid_structure.Kp += ptr_CMD_structure->float_data;
						break;

						case DECRESE:
							velocity_pid_structure.Kp -= ptr_CMD_structure->float_data;
						break;

						default:
							return;
					}
				break;

				case KI:
					switch (ptr_CMD_structure->command_id)
					{
						case DIRECT:
							velocity_pid_structure.Ki = ptr_CMD_structure->float_data;
						break;

						case INCRESE:
							velocity_pid_structure.Ki += ptr_CMD_structure->float_data;
						break;

						case DECRESE:
							velocity_pid_structure.Ki -= ptr_CMD_structure->float_data;
						break;

						default:
							return;
					}
				break;

				default:
					return;
			}
			velocity_pid_init(&velocity_pid_structure, velocity_pid_structure.Kp, velocity_pid_structure.Ki);
			ptr_CMD_structure->frame_valid_flag = 0;
			break;

		case TURN:
			switch (ptr_CMD_structure->command_object)
			{
				case KP:
					switch (ptr_CMD_structure->command_id)
					{
						case DIRECT:
							turn_pid_structure.Kp = ptr_CMD_structure->float_data;
						break;

						case INCRESE:
							turn_pid_structure.Kp += ptr_CMD_structure->float_data;
						break;

						case DECRESE:
							turn_pid_structure.Kp -= ptr_CMD_structure->float_data;
						break;

						default:
							return;
					}
				break;

				case KD:
					switch (ptr_CMD_structure->command_id)
					{
						case DIRECT:
							turn_pid_structure.Kd = ptr_CMD_structure->float_data;
						break;

						case INCRESE:
							turn_pid_structure.Kd += ptr_CMD_structure->float_data;
						break;

						case DECRESE:
							turn_pid_structure.Kd -= ptr_CMD_structure->float_data;
						break;

						default:
							return;
					}
				break;

				default:
					return;
			}
			turn_pid_init(&turn_pid_structure, turn_pid_structure.Kp, turn_pid_structure.Kd);
			ptr_CMD_structure->frame_valid_flag = 0;
			break;

		default:
			break;
	}
}
