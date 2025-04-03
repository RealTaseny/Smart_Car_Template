//
// Created by Taseny on 24-11-12.
//

#include <string.h>
#include "../Inc/Fomatting.h"

/****************************数据在内存中以大小端方式存储的问题************************************/
/************************************************************************************************
*				               使用小端(Little-endian)存储的架构：			  	   				  *
* x86、6502、Z80、eZ80、MCS-48、DEC Alpha、Altera Nios、Atmel AVR、SuperH、VAX、PDP-11等等;	     **
*************************************************************************************************
*								使用大端(Big-endian)存储的架构：			                      *
*	Motorola 6800 and 68k、8051、Xilinx Microblaze、IBM POWER、system/360、System/370 等等; 	  *
**************************************************************************************************
*								  支持配置大小端存储的架构：		     						   *
*				      ARM、PowerPC、Alpha、SPARC V9、MIPS、PA-RISC 和 IA-64 等等。   	          *
***************************************************************************************************/

/*
*Name:字节数组转浮点函数
*Function:将VOFA Justfloat协议下浮点数在字节型数组中存储的方式转换到float数
*Formal Parameter:指向存储这需要转换为浮点数的字节数组的指针
*Return Parameter:转换后的浮点数
*Author:王彦皓
*Created on:2023/10/26
*/
void uint8_t_array2float(const uint8_t *uint8_t_array, float *pfloat_data)
{
	uint8_t temp_data[4];
#ifdef USE_BIG_ENDIAN
		temp_data[3] = uint8_t_array[0];
		temp_data[2] = uint8_t_array[1];
		temp_data[1] = uint8_t_array[2];
		temp_data[0] = uint8_t_array[3];
#endif
#ifdef USE_LITTLE_ENDIAN
		temp_data[0] = uint8_t_array[0];
		temp_data[1] = uint8_t_array[1];
		temp_data[2] = uint8_t_array[2];
		temp_data[3] = uint8_t_array[3];
#endif
	memcpy(pfloat_data, temp_data, 4);
}

/*
*Name:浮点数转字节数组
*Function:将送入的浮点数转换为四个字节的字节数组
*Formal Parameter:用来存储转换结果的字节数组，待转换的浮点数
*Return Parameter:无
*Author:王彦皓
*Created on:2023/10/26
*/
void float2uint8_t_array(uint8_t *uint8_t_array, const float *pfloat_data)
{
	uint8_t float_array[4];
	*(float *)float_array = *pfloat_data;
#ifdef USE_BIG_ENDIAN
		uint8_t_array[3] = float_array[0];
		uint8_t_array[2] = float_array[1];
		uint8_t_array[1] = float_array[2];
		uint8_t_array[0] = float_array[3];
#endif
#ifdef USE_LITTLE_ENDIAN
		uint8_t_array[0] = float_array[0];
		uint8_t_array[1] = float_array[1];
		uint8_t_array[2] = float_array[2];
		uint8_t_array[3] = float_array[3];
#endif
}
