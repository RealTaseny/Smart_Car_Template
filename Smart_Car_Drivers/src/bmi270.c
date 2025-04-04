#include "bmi270.h"
#include "bmi270_config_headfile.h"
#include <string.h>

int16_t bmi270_gyro_x = 0, bmi270_gyro_y = 0, bmi270_gyro_z = 0;            // 三轴陀螺仪数据      GYRO (陀螺仪)
int16_t bmi270_acc_x = 0, bmi270_acc_y = 0, bmi270_acc_z = 0;               // 三轴加速度计数据     ACC  (accelerometer 加速度计)
float bmi270_transition_factor[2] = {4096, 16.4};                           // 转换实际值的比例

volatile uint8_t spi1_tx_complete = 0;
volatile uint8_t spi1_txrx_complete = 0;

static void bmi270_write_register(uint8_t reg_addr, uint8_t data)
{
	uint8_t data_array[2] = {reg_addr | BMI270_SPI_W, data};
	GYRO_CS(0);
    HAL_SPI_Transmit(BMI270_SPI_Handle, data_array, 2, HAL_MAX_DELAY);
	GYRO_CS(1);
}

static void bmi270_write_registers(uint8_t reg_addr, const uint8_t *data, uint32_t length)
{
	uint8_t data_array[length + 1];
	data_array[0] = reg_addr | BMI270_SPI_W;
	memcpy(&data_array[1], data, length);
	GYRO_CS(0);
    HAL_SPI_Transmit(BMI270_SPI_Handle, data_array, length + 1, HAL_MAX_DELAY);
	GYRO_CS(1);
}

static uint8_t bmi270_read_register(uint8_t reg_addr)
{
    uint8_t data = reg_addr | BMI270_SPI_R;
	uint8_t recieved_data[2];

	GYRO_CS(0);
    HAL_SPI_Transmit(BMI270_SPI_Handle, &data, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(BMI270_SPI_Handle, recieved_data, 2, HAL_MAX_DELAY);
	GYRO_CS(1);

	return recieved_data[1];
}

static void bmi270_read_registers(uint8_t reg_addr, uint8_t *pdata, uint32_t len)
{
    uint8_t data = (reg_addr | BMI270_SPI_R);
    uint8_t raw_data[8];

    GYRO_CS(0);
    HAL_SPI_Transmit(BMI270_SPI_Handle, &data, 1, HAL_MAX_DELAY);
    HAL_SPI_Receive(BMI270_SPI_Handle, raw_data, len + 1, HAL_MAX_DELAY);
    GYRO_CS(1);

    memcpy(pdata, raw_data + 1, len);
}

static uint8_t bmi270_self_check (void)
{
    uint8_t verify_code = 0, return_state = 0;
    uint16_t timeout_count = 0;
    do
    {
        if(BMI270_MAX_TIMEOUT_COUNT < timeout_count ++)
        {
            return_state = 1;
            break;
        }
        verify_code = bmi270_read_register(BMI270_CHIP_ID);
        HAL_Delay(1);
    }while(0x24 != verify_code);
    return return_state;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 BMI270 加速度计数据
// 参数说明     void
// 返回参数     void
// 使用示例     bmi270_get_acc();                                             // 执行该函数后，直接查看对应的变量即可
// 备注信息     使用 SPI 的采集时间为69us
//            使用 IIC 的采集时间为126us        采集加速度计的时间与采集陀螺仪的时间一致的原因是都只是读取寄存器数据
//-------------------------------------------------------------------------------------------------------------------
void bmi270_get_acc (void)
{
    uint8_t dat[6];

    bmi270_read_registers(BMI270_ACC_ADDRESS, dat, 6);
    bmi270_acc_x = (int16_t)((uint16_t)dat[1]<<8 | dat[0]);
    bmi270_acc_y = (int16_t)((uint16_t)dat[3]<<8 | dat[2]);
    bmi270_acc_z = (int16_t)((uint16_t)dat[5]<<8 | dat[4]);
}
//-------------------------------------------------------------------------------------------------------------------
// 函数简介     获取 BMI270 陀螺仪数据
// 参数说明     void
// 返回参数     void
// 使用示例     bmi270_get_gyro();                                            // 执行该函数后，直接查看对应的变量即可
// 备注信息     使用 SPI 的采集时间为69us
//            使用 IIC 的采集时间为126us
//-------------------------------------------------------------------------------------------------------------------
void bmi270_get_gyro (void)
{
    uint8_t dat[6];

    bmi270_read_registers(BMI270_GYRO_ADDRESS, dat, 6);

    bmi270_gyro_x = (int16_t)((uint16_t)dat[1]<<8 | dat[0]);
    bmi270_gyro_y = (int16_t)((uint16_t)dat[3]<<8 | dat[2]);
    bmi270_gyro_z = (int16_t)((uint16_t)dat[5]<<8 | dat[4]);
}

void bmi270_get_data(void)
{
    static uint8_t int_status = 0;
    int_status = bmi270_read_register(BMI270_INT_STATUS_1);
    if (int_status & 0x80)
    {
        bmi270_get_acc();
    }
    if (int_status & 0x40)
    {
        bmi270_get_gyro();
    }
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     初始化 BMI270
// 参数说明     void
// 返回参数     uint8_t           1-初始化失败 0-初始化成功
// 使用示例     bmi270_init();
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
uint8_t bmi270_init (void)
{
    uint8_t return_state = 0;
    HAL_Delay(20);                                                    // 等待设备上电成功
    do{
        if(bmi270_self_check())                                           // BMI270自检
        {
            return_state = 1;
            break;
        }
				bmi270_write_register(BMI270_PWR_CONF, 0x00);                       // 关闭高级省电模式
				HAL_Delay(1);
				bmi270_write_register(BMI270_INIT_CTRL, 0x00);                      // 开始对模块进行初始化配置
				for (int i = 0; i < 256; i++)
				{
					bmi270_write_register(BMI270_INIT_ADDR_0, 0x00);
					bmi270_write_register(BMI270_INIT_ADDR_1, i);
					bmi270_write_registers(BMI270_INIT_DATA, &bmi270_config_file[i * 32], 32);
				}
				bmi270_write_register(BMI270_INIT_CTRL, 0x01);                      // 初始化配置结束
				HAL_Delay(20);
        if(1 != bmi270_read_register(BMI270_INT_STA))                       // 检查是否配置完成
        {
            return_state = 1;
            break;
        }
        bmi270_write_register(BMI270_DATA_MAP, 0x04);                       // remap中断向量
        bmi270_write_register(BMI270_PWR_CTRL, 0x0E);                       // 开启性能模式  使能陀螺仪、加速度、温度传感器
        bmi270_write_register(BMI270_ACC_CONF, 0xE9);                       // 加速度采集配置 性能模式 正常采集 200Hz  采样频率
        bmi270_write_register(BMI270_GYR_CONF, 0xE9);                       // 陀螺仪采集配置 性能模式 正常采集 200Hz 采样频率

        // BMI270_ACC_SAMPLE寄存器          以下为25℃的转换比例 BMI270的加速度温漂系数为 0.004%/K  常值零偏的温度敏感系数为 ±0.25mg/K
        // 设置为:0x00 加速度计量程为:±2g          获取到的加速度计数据 除以16384      可以转化为带物理单位的数据，单位：g(m/s^2)
        // 设置为:0x01 加速度计量程为:±4g          获取到的加速度计数据 除以8192       可以转化为带物理单位的数据，单位：g(m/s^2)
        // 设置为:0x02 加速度计量程为:±8g          获取到的加速度计数据 除以4096       可以转化为带物理单位的数据，单位：g(m/s^2)
        // 设置为:0x03 加速度计量程为:±16g         获取到的加速度计数据 除以2048       可以转化为带物理单位的数据，单位：g(m/s^2)
        switch(BMI270_ACC_SAMPLE_DEFAULT)
        {
            case BMI270_ACC_SAMPLE_SGN_2G:
            {
                bmi270_write_register(BMI270_ACC_RANGE, 0x00);
                bmi270_transition_factor[0] = 16384;
            }break;
            case BMI270_ACC_SAMPLE_SGN_4G:
            {
                bmi270_write_register(BMI270_ACC_RANGE, 0x01);
                bmi270_transition_factor[0] = 8192;
            }break;
            case BMI270_ACC_SAMPLE_SGN_8G:
            {
                bmi270_write_register(BMI270_ACC_RANGE, 0x02);
                bmi270_transition_factor[0] = 4096;
            }break;
            case BMI270_ACC_SAMPLE_SGN_16G:
            {
                bmi270_write_register(BMI270_ACC_RANGE, 0x03);
                bmi270_transition_factor[0] = 2048;
            }break;
            default:
            {
                return_state = 1;
            }break;
        }
        if(1 == return_state)
        {
            break;
        }

        // BMI270_GYR_SAMPLE寄存器         以下为25℃的转换比例 BMI270的陀螺仪温漂系数为 0.02%/K  常值零偏的温度敏感系数为 ±0.015dps/K
        // 设置为:0x00 陀螺仪量程为:±2000dps      获取到的陀螺仪数据除以16.384          可以转化为带物理单位的数据，单位为：°/s
        // 设置为:0x01 陀螺仪量程为:±1000dps      获取到的陀螺仪数据除以32.768          可以转化为带物理单位的数据，单位为：°/s
        // 设置为:0x02 陀螺仪量程为:±500 dps      获取到的陀螺仪数据除以65.536          可以转化为带物理单位的数据，单位为：°/s
        // 设置为:0x03 陀螺仪量程为:±250 dps      获取到的陀螺仪数据除以131.072         可以转化为带物理单位的数据，单位为：°/s
        // 设置为:0x04 陀螺仪量程为:±250 dps      获取到的陀螺仪数据除以262.144         可以转化为带物理单位的数据，单位为：°/s
        switch(BMI270_GYRO_SAMPLE_DEFAULT)
        {
            case BMI270_GYRO_SAMPLE_SGN_125DPS:
            {
                bmi270_write_register(BMI270_GYR_RANGE, 0x04);
                bmi270_transition_factor[1] = 262.144;
            }break;
            case BMI270_GYRO_SAMPLE_SGN_250DPS:
            {
                bmi270_write_register(BMI270_GYR_RANGE, 0x03);
                bmi270_transition_factor[1] = 131.072;
            }break;
            case BMI270_GYRO_SAMPLE_SGN_500DPS:
            {
                bmi270_write_register(BMI270_GYR_RANGE, 0x02);
                bmi270_transition_factor[1] = 65.536;
            }break;
            case BMI270_GYRO_SAMPLE_SGN_1000DPS:
            {
                bmi270_write_register(BMI270_GYR_RANGE, 0x01);
                bmi270_transition_factor[1] = 32.768;
            }break;
            case BMI270_GYRO_SAMPLE_SGN_2000DPS:
            {
                bmi270_write_register(BMI270_GYR_RANGE, 0x00);
                bmi270_transition_factor[1] = 16.384;
            }break;
            default:
            {
                return_state = 1;
            }break;
        }
        if(1 == return_state)
        {
            break;
        }
    }while(0);
    return return_state;
}