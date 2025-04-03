#include "bmi270.h"
#include "bmi270_config_headfile.h"
#include <string.h>

int16_t bmi270_gyro_x = 0, bmi270_gyro_y = 0, bmi270_gyro_z = 0;            // ��������������      GYRO (������)
int16_t bmi270_acc_x = 0, bmi270_acc_y = 0, bmi270_acc_z = 0;               // ������ٶȼ�����     ACC  (accelerometer ���ٶȼ�)
float bmi270_transition_factor[2] = {4096, 16.4};                           // ת��ʵ��ֵ�ı���

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
	uint8_t data = (reg_addr | BMI270_SPI_R);
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
// �������     ��ȡ BMI270 ���ٶȼ�����
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     bmi270_get_acc();                                             // ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
// ��ע��Ϣ     ʹ�� SPI �Ĳɼ�ʱ��Ϊ69us
//            ʹ�� IIC �Ĳɼ�ʱ��Ϊ126us        �ɼ����ٶȼƵ�ʱ����ɼ������ǵ�ʱ��һ�µ�ԭ���Ƕ�ֻ�Ƕ�ȡ�Ĵ�������
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
// �������     ��ȡ BMI270 ����������
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     bmi270_get_gyro();                                            // ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
// ��ע��Ϣ     ʹ�� SPI �Ĳɼ�ʱ��Ϊ69us
//            ʹ�� IIC �Ĳɼ�ʱ��Ϊ126us
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
// �������     ��ʼ�� BMI270
// ����˵��     void
// ���ز���     uint8_t           1-��ʼ��ʧ�� 0-��ʼ���ɹ�
// ʹ��ʾ��     bmi270_init();
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
uint8_t bmi270_init (void)
{
    uint8_t return_state = 0;
    HAL_Delay(20);                                                    // �ȴ��豸�ϵ�ɹ�
    do{
        if(bmi270_self_check())                                           // BMI270�Լ�
        {
            return_state = 1;
            break;
        }
				bmi270_write_register(BMI270_PWR_CONF, 0x00);                       // �رո߼�ʡ��ģʽ
				HAL_Delay(1);
				bmi270_write_register(BMI270_INIT_CTRL, 0x00);                      // ��ʼ��ģ����г�ʼ������
				for (int i = 0; i < 256; i++)
				{		
					bmi270_write_register(BMI270_INIT_ADDR_0, 0x00);
					bmi270_write_register(BMI270_INIT_ADDR_1, i);
					bmi270_write_registers(BMI270_INIT_DATA, &bmi270_config_file[i * 32], 32);
				}
				bmi270_write_register(BMI270_INIT_CTRL, 0x01);                      // ��ʼ�����ý���
				HAL_Delay(20);
        if(1 != bmi270_read_register(BMI270_INT_STA))                       // ����Ƿ��������
        {
            return_state = 1;
            break;
        }
        bmi270_write_register(BMI270_DATA_MAP, 0x04);                       // remap�ж�����
        bmi270_write_register(BMI270_PWR_CTRL, 0x0E);                       // ��������ģʽ  ʹ�������ǡ����ٶȡ��¶ȴ�����
        bmi270_write_register(BMI270_ACC_CONF, 0xE9);                       // ���ٶȲɼ����� ����ģʽ �����ɼ� 200Hz  ����Ƶ��
        bmi270_write_register(BMI270_GYR_CONF, 0xE9);                       // �����ǲɼ����� ����ģʽ �����ɼ� 200Hz ����Ƶ��

        // BMI270_ACC_SAMPLE�Ĵ���          ����Ϊ25���ת������ BMI270�ļ��ٶ���Ưϵ��Ϊ 0.004%/K  ��ֵ��ƫ���¶�����ϵ��Ϊ ��0.25mg/K
        // ����Ϊ:0x00 ���ٶȼ�����Ϊ:��2g          ��ȡ���ļ��ٶȼ����� ����16384      ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
        // ����Ϊ:0x01 ���ٶȼ�����Ϊ:��4g          ��ȡ���ļ��ٶȼ����� ����8192       ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
        // ����Ϊ:0x02 ���ٶȼ�����Ϊ:��8g          ��ȡ���ļ��ٶȼ����� ����4096       ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
        // ����Ϊ:0x03 ���ٶȼ�����Ϊ:��16g         ��ȡ���ļ��ٶȼ����� ����2048       ����ת��Ϊ������λ�����ݣ���λ��g(m/s^2)
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

        // BMI270_GYR_SAMPLE�Ĵ���         ����Ϊ25���ת������ BMI270����������Ưϵ��Ϊ 0.02%/K  ��ֵ��ƫ���¶�����ϵ��Ϊ ��0.015dps/K
        // ����Ϊ:0x00 ����������Ϊ:��2000dps      ��ȡ�������������ݳ���16.384          ����ת��Ϊ������λ�����ݣ���λΪ����/s
        // ����Ϊ:0x01 ����������Ϊ:��1000dps      ��ȡ�������������ݳ���32.768          ����ת��Ϊ������λ�����ݣ���λΪ����/s
        // ����Ϊ:0x02 ����������Ϊ:��500 dps      ��ȡ�������������ݳ���65.536          ����ת��Ϊ������λ�����ݣ���λΪ����/s
        // ����Ϊ:0x03 ����������Ϊ:��250 dps      ��ȡ�������������ݳ���131.072         ����ת��Ϊ������λ�����ݣ���λΪ����/s
        // ����Ϊ:0x04 ����������Ϊ:��250 dps      ��ȡ�������������ݳ���262.144         ����ת��Ϊ������λ�����ݣ���λΪ����/s
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
