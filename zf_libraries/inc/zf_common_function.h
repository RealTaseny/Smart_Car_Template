/*********************************************************************************************************************
* TC264 Opensourec Library 即（TC264 开源库）是一个基于官方 SDK 接口的第三方开源库
* Copyright (c) 2022 SEEKFREE 逐飞科技
*
* 本文件是 TC264 开源库的一部分
*
* TC264 开源库 是免费软件
* 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
* 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
*
* 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
* 甚至没有隐含的适销性或适合特定用途的保证
* 更多细节请参见 GPL
*
* 您应该在收到本开源库的同时收到一份 GPL 的副本
* 如果没有，请参阅<https://www.gnu.org/licenses/>
*
* 额外注明：
* 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
* 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
* 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
* 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
*
* 文件名称          zf_common_function
* 公司名称          成都逐飞科技有限公司
* 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
* 开发环境          ADS v1.9.20
* 适用平台          TC264D
* 店铺链接          https://seekfree.taobao.com/
*
* 修改记录
* 日期              作者                备注
* 2022-09-15       pudding            first version
********************************************************************************************************************/

#ifndef zf_common_function_h_
#define zf_common_function_h_

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define     func_abs(x)             ((x) >= 0 ? (x): -(x))
#define     func_limit(x, y)        ((x) > (y) ? (y) : ((x) < -(y) ? -(y) : (x)))
#define     func_limit_ab(x, a, b)  ((x) < (a) ? (a) : ((x) > (b) ? (b) : (x)))
uint32_t      func_get_greatest_common_divisor    (uint32_t num1, uint32_t num2);

void        func_soft_delay                     (volatile long t);

int32_t       func_str_to_int                     (char *str);
void        func_int_to_str                     (char *str, int32_t number);
uint32_t      func_str_to_uint                    (char *str);
void        func_uint_to_str                    (char *str, uint32_t number);
float       func_str_to_float                   (char *str);
void        func_float_to_str                   (char *str, float number, uint8_t point_bit);
double      func_str_to_double                  (char *str);
void        func_double_to_str                  (char *str, double number, uint8_t point_bit);
uint32_t      func_str_to_hex                     (char *str);
void        func_hex_to_str                     (char *str, uint32_t number);

uint32_t      zf_sprintf                          (int8_t *buff, const int8_t *format, ...);

#endif
