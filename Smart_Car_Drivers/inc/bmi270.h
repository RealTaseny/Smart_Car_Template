#ifndef BMI270_H__
#define BMI270_H__

#include "gpio.h"
#include "spi.h"

#define BMI270_SPI_Handle (&hspi1)

#define GYRO_CS_PORT GPIOB
#define GYRO_CS_PIN GPIO_PIN_9

#define GYRO_CS(x) ((x) == (0) ? (HAL_GPIO_WritePin(GYRO_CS_PORT, GYRO_CS_PIN, GPIO_PIN_RESET)) : (HAL_GPIO_WritePin(GYRO_CS_PORT, GYRO_CS_PIN, GPIO_PIN_SET)))

#define G (9.7913f)

typedef enum
{
    BMI270_ACC_SAMPLE_SGN_2G ,                                                // 加速度计量程 ±2G  (ACC = Accelerometer 加速度计) (SGN = signum 带符号数 表示正负范围) (G = g 重力加速度 g≈9.80 m/s^2)
    BMI270_ACC_SAMPLE_SGN_4G ,                                                // 加速度计量程 ±4G  (ACC = Accelerometer 加速度计) (SGN = signum 带符号数 表示正负范围) (G = g 重力加速度 g≈9.80 m/s^2)
    BMI270_ACC_SAMPLE_SGN_8G ,                                                // 加速度计量程 ±8G  (ACC = Accelerometer 加速度计) (SGN = signum 带符号数 表示正负范围) (G = g 重力加速度 g≈9.80 m/s^2)
    BMI270_ACC_SAMPLE_SGN_16G,                                                // 加速度计量程 ±16G (ACC = Accelerometer 加速度计) (SGN = signum 带符号数 表示正负范围) (G = g 重力加速度 g≈9.80 m/s^2)
}bmi270_acc_sample_config;

typedef enum
{
    BMI270_GYRO_SAMPLE_SGN_125DPS ,                                           // 陀螺仪量程 ±125DPS  (GYRO = Gyroscope 陀螺仪) (SGN = signum 带符号数 表示正负范围) (DPS = Degree Per Second 角速度单位 °/S)
    BMI270_GYRO_SAMPLE_SGN_250DPS ,                                           // 陀螺仪量程 ±250DPS  (GYRO = Gyroscope 陀螺仪) (SGN = signum 带符号数 表示正负范围) (DPS = Degree Per Second 角速度单位 °/S)
    BMI270_GYRO_SAMPLE_SGN_500DPS ,                                           // 陀螺仪量程 ±500DPS  (GYRO = Gyroscope 陀螺仪) (SGN = signum 带符号数 表示正负范围) (DPS = Degree Per Second 角速度单位 °/S)
    BMI270_GYRO_SAMPLE_SGN_1000DPS,                                           // 陀螺仪量程 ±1000DPS (GYRO = Gyroscope 陀螺仪) (SGN = signum 带符号数 表示正负范围) (DPS = Degree Per Second 角速度单位 °/S)
    BMI270_GYRO_SAMPLE_SGN_2000DPS,                                           // 陀螺仪量程 ±2000DPS (GYRO = Gyroscope 陀螺仪) (SGN = signum 带符号数 表示正负范围) (DPS = Degree Per Second 角速度单位 °/S)
}bmi270_gyro_sample_config;

#define BMI270_ACC_SAMPLE_DEFAULT   ( BMI270_ACC_SAMPLE_SGN_8G )            // 在这设置默认的 加速度计 初始化量程
#define BMI270_GYRO_SAMPLE_DEFAULT  ( BMI270_GYRO_SAMPLE_SGN_2000DPS )      // 在这设置默认的 陀螺仪   初始化量程
#define BMI270_MAX_TIMEOUT_COUNT        (0x00FF)                                  // IMU660 超时计数
#define BMI270_SPI_W                (0x00)
#define BMI270_SPI_R                (0x80)
//================================================定义 BMI270 基本配置================================================

//================================================定义 BMI270 内部地址================================================
#define BMI270_CHIP_ID              (0x00)
#define BMI270_INT_STATUS_0         (0x1C)
#define BMI270_INT_STATUS_1         (0x1D)
#define BMI270_INT1_IO_CTRL         (0x53)
#define BMI270_INT2_IO_CTRL         (0x54)
#define BMI270_INT_LATCH            (0x55)
#define BMI270_INT1_MAP             (0x56)
#define BMI270_INT2_MAP             (0x57)
#define BMI270_DATA_MAP             (0x58)
#define BMI270_INIT_ADDR_0			(0X5B)
#define BMI270_INIT_ADDR_1			(0X5C)
#define BMI270_PWR_CONF             (0x7C)
#define BMI270_PWR_CTRL             (0x7D)
#define BMI270_INIT_CTRL            (0x59)
#define BMI270_INIT_DATA            (0x5E)
#define BMI270_INT_STA              (0x21)
#define BMI270_ACC_ADDRESS          (0x0C)
#define BMI270_GYRO_ADDRESS         (0x12)
#define BMI270_ACC_CONF             (0x40)
#define BMI270_ACC_RANGE            (0x41)
#define BMI270_GYR_CONF             (0x42)
#define BMI270_GYR_RANGE            (0x43)
#define BMI270_ACC_SAMPLE           (0x02)                                    // 加速度计量程
//                                以下为25℃的转换比例 BMI270的加速度温漂系数为 0.004%/K  常值零偏的温度敏感系数为 ±0.25mg/K
// 设置为:0x00 加速度计量程为:±2g         获取到的加速度计数据 除以 16384   可以转化为带物理单位的数据 单位：g(m/s^2)
// 设置为:0x01 加速度计量程为:±4g         获取到的加速度计数据 除以 8192    可以转化为带物理单位的数据 单位：g(m/s^2)
// 设置为:0x02 加速度计量程为:±8g         获取到的加速度计数据 除以 4096    可以转化为带物理单位的数据 单位：g(m/s^2)
// 设置为:0x03 加速度计量程为:±16g        获取到的加速度计数据 除以 2048    可以转化为带物理单位的数据 单位：g(m/s^2)

#define BMI270_GYR_SAMPLE           (0x00)                                    // 陀螺仪量程
//                                以下为25℃的转换比例 BMI270的陀螺仪温漂系数为 0.02%/K  常值零偏的温度敏感系数为 ±0.015dps/K
// 设置为:0x00 陀螺仪量程为:±2000dps      获取到的陀螺仪数据除以16.384          可以转化为带物理单位的数据，单位为：°/s
// 设置为:0x01 陀螺仪量程为:±1000dps      获取到的陀螺仪数据除以32.768          可以转化为带物理单位的数据，单位为：°/s
// 设置为:0x02 陀螺仪量程为:±500 dps      获取到的陀螺仪数据除以65.536          可以转化为带物理单位的数据，单位为：°/s
// 设置为:0x03 陀螺仪量程为:±250 dps      获取到的陀螺仪数据除以131.072         可以转化为带物理单位的数据，单位为：°/s
// 设置为:0x04 陀螺仪量程为:±250 dps      获取到的陀螺仪数据除以262.144         可以转化为带物理单位的数据，单位为：°/s
//================================================定义 BMI270 内部地址================================================


//================================================声明 BMI270 全局变量================================================
extern int16_t bmi270_gyro_x, bmi270_gyro_y, bmi270_gyro_z;                 // 三轴陀螺仪数据      GYRO (陀螺仪)
extern int16_t bmi270_acc_x, bmi270_acc_y, bmi270_acc_z;                    // 三轴加速度计数据     ACC  (accelerometer 加速度计)
extern float bmi270_transition_factor[2];                                     // 转换实际值的比例
//================================================声明 BMI270 全局变量================================================


//================================================声明 BMI270 基础函数================================================
void  bmi270_get_acc              	(void);                                     // 获取 BMI270 加速度计数据
void  bmi270_get_gyro             	(void);                                     // 获取 BMI270 陀螺仪数据
void bmi270_get_data                (void);
uint8_t bmi270_init                 (void);                                     // 初始化 BMI270
//================================================声明 BMI270 基础函数================================================


//================================================声明 BMI270 拓展函数================================================
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     将 BMI270 加速度计数据转换为实际物理数据
// 参数说明     acc_value       任意轴的加速度计数据
// 返回参数     void
// 使用示例     float data = bmi270_acc_transition(bmi270_acc_x);           // 单位为 g(m/s^2)
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
#define bmi270_acc_transition(acc_value)      ((float)(acc_value) / bmi270_transition_factor[0])

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     将 BMI270 陀螺仪数据转换为实际物理数据
// 参数说明     gyro_value      任意轴的陀螺仪数据
// 返回参数     void
// 使用示例     float data = bmi270_gyro_transition(bmi270_gyro_x);         // 单位为 °/s
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
#define bmi270_gyro_transition(gyro_value)    ((float)(gyro_value) / bmi270_transition_factor[1])
//================================================声明 BMI270 拓展函数================================================

#endif