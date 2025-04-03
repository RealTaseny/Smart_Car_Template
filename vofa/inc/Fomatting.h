//
// Created by Taseny on 24-11-12.
//

#ifndef FOMATTING_H_
#define FOMATTING_H_

#include <stdint.h>

#define USE_LITTLE_ENDIAN       //定义使用大小端存储  USE_LITTLE_ENDIAN  USE_BIG_ENDIAN

void uint8_t_array2float(const uint8_t *uint8_t_array, float *pfloat_data);
void float2uint8_t_array(uint8_t *uint8_t_array, const float *pfloat_data);

#endif
