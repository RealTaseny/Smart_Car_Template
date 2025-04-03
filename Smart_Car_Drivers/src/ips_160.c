#include "ips_160.h"

static uint16_t									ips160_pencolor     = IPS160_DEFAULT_PENCOLOR;          // ������ɫ(����ɫ)
static uint16_t									ips160_bgcolor      = IPS160_DEFAULT_BGCOLOR;           // ������ɫ
static ips160_dir_enum					ips160_display_dir  = IPS160_DEFAULT_DISPLAY_DIR;       // ��ʾ����
static ips160_font_size_enum		ips160_display_font = IPS160_DEFAULT_DISPLAY_FONT;      // ��ʾ��������
static uint16_t									ips160_x_max        = 132;                              // ��ĻX���������ֵ
static uint16_t									ips160_y_max        = 162;                              // ��ĻY���������ֵ

//-------------------------------------------------------------------------------------------------------------------
// �������       	IPS160 д���� �ڲ�����
// ����˵��       	command         ����
// @return      		void
// ��ע��Ϣ        	�ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void ips160_write_command (const uint8_t command)
{
	uint8_t data[1] = {command};
	IPS160_DC(0);
	HAL_SPI_Transmit_DMA(IPS160_SPI_PORT, data, sizeof(data));
	IPS160_DC(1);
}

//-------------------------------------------------------------------------------------------------------------------
// �������    IPS160 ��Һ����д 8bit ���� �ڲ�����
// ����˵��    dat             ����
// @return     void
// ��ע��Ϣ    �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void ips160_write_8bit_data (const uint8_t dat)
{
	uint8_t data[1] = {dat};
	HAL_SPI_Transmit_DMA(IPS160_SPI_PORT, data, sizeof(data));
}

//-------------------------------------------------------------------------------------------------------------------
// �������       IPS160 ��Һ����д 16bit ���� �ڲ�����
// ����˵��       dat             ����
// @return      void
// ��ע��Ϣ        �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
void ips160_write_16bit_data(const uint16_t dat) {
    uint8_t data[2];
    data[0] = (dat >> 8) & 0xFF; // ���ֽ�
    data[1] = dat & 0xFF; // ���ֽ�
    HAL_SPI_Transmit_DMA(IPS160_SPI_PORT, data, sizeof(data));
}
//-------------------------------------------------------------------------------------------------------------------
// �������     ������ʾ����
// ����˵��     x1              ��ʼx������
// ����˵��     y1              ��ʼy������
// ����˵��     x2              ����x������
// ����˵��     y2              ����y������
// ���ز���     void
// ʹ��ʾ��     ips160_set_region(0, 0, ips160_x_max - 1, ips160_y_max - 1);
// ��ע��Ϣ     �ڲ����� �û��������
//-------------------------------------------------------------------------------------------------------------------
static void ips160_set_region (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	switch(ips160_display_dir)
	{
		case IPS160_PORTAIT:
			ips160_write_command(0x2a);//�е�ַ����
			ips160_write_16bit_data(x1+X_OFFSET);
			ips160_write_16bit_data(x2+X_OFFSET);
			ips160_write_command(0x2b);//�е�ַ����
			ips160_write_16bit_data(y1+Y_OFFSET);
			ips160_write_16bit_data(y2+Y_OFFSET);
			ips160_write_command(0x2c);//������д  
			break;
		case IPS160_PORTAIT_180:
			ips160_write_command(0x2a);//�е�ַ����
			ips160_write_16bit_data(x1+X_OFFSET);
			ips160_write_16bit_data(x2+X_OFFSET);
			ips160_write_command(0x2b);//�е�ַ����
			ips160_write_16bit_data(y1+Y_OFFSET);
			ips160_write_16bit_data(y2+Y_OFFSET);
			ips160_write_command(0x2c);//������д 
			break;
		case IPS160_CROSSWISE:      
			ips160_write_command(0x2a);//�е�ַ����
			ips160_write_16bit_data(x1+Y_OFFSET);
			ips160_write_16bit_data(x2+Y_OFFSET);
			ips160_write_command(0x2b);//�е�ַ����
			ips160_write_16bit_data(y1+X_OFFSET);
			ips160_write_16bit_data(y2+X_OFFSET);
			ips160_write_command(0x2c);//������д
			break;
		case IPS160_CROSSWISE_180:  
			ips160_write_command(0x2a);//�е�ַ����
			ips160_write_16bit_data(x1+Y_OFFSET);
			ips160_write_16bit_data(x2+Y_OFFSET);
			ips160_write_command(0x2b);//�е�ַ����
			ips160_write_16bit_data(y1+X_OFFSET);
			ips160_write_16bit_data(y2+X_OFFSET);
			ips160_write_command(0x2c);//������д
			break;
	}
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IPS160 ��������
// ����˵��     void
// ���ز���     void
// ʹ��ʾ��     ips160_clear();
// ��ע��Ϣ     ����Ļ��ճɱ�����ɫ
//-------------------------------------------------------------------------------------------------------------------
void ips160_clear(void) {
    ips160_set_region(0, 0, ips160_x_max - 1, ips160_y_max - 1);
    uint16_t color = ips160_bgcolor;
    uint8_t data[ips160_x_max * 2]; // ÿ������ 16 λ��2 �ֽ�
    
    // �����������
    for (uint16_t i = 0; i < ips160_x_max; i++) {
        data[i * 2] = (color >> 8) & 0xFF; // ���ֽ�
        data[i * 2 + 1] = color & 0xFF;    // ���ֽ�
    }

    // ���������е�����
    for (uint16_t j = 0; j < ips160_y_max; j++) {
        HAL_SPI_Transmit(IPS160_SPI_PORT, data, sizeof(data), HAL_MAX_DELAY);
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IPS160 ��Ļ��亯��
// ����˵��     color           ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �� rgb565_color_enum ö��ֵ��������д��
// ���ز���     void
// ʹ��ʾ��     ips160_full(RGB565_BLACK);
// ��ע��Ϣ     ����Ļ����ָ����ɫ
//-------------------------------------------------------------------------------------------------------------------
void ips160_full (const uint16_t color)
{
    uint16_t i = 0, j = 0;
    ips160_set_region(0, 0, ips160_x_max - 1, ips160_y_max - 1);
    for(i = 0; i < ips160_x_max; i ++)
    {
        for (j = 0; j < ips160_y_max; j ++)
        {
            ips160_write_16bit_data(color);			
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ������ʾ����
// ����˵��     dir             ��ʾ����  ���� zf_device_ips160.h �� ips160_dir_enum ö���嶨��
// ���ز���     void
// ʹ��ʾ��     ips160_set_dir(IPS160_PORTAIT);
// ��ע��Ϣ     �������ֻ���ڳ�ʼ����Ļ֮ǰ���ò���Ч
//-------------------------------------------------------------------------------------------------------------------
void ips160_set_dir (ips160_dir_enum dir)
{
    ips160_display_dir = dir;
    switch(ips160_display_dir)
    {
        case IPS160_PORTAIT:
        case IPS160_PORTAIT_180:
        {
            ips160_x_max = 132;
            ips160_y_max = 162;
        }break;
        case IPS160_CROSSWISE:
        case IPS160_CROSSWISE_180:
        {
            ips160_x_max = 162;
            ips160_y_max = 132;
        }break;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ������ʾ����
// ����˵��     dir             ��ʾ����  ���� zf_device_ips160.h �� ips160_font_size_enum ö���嶨��
// ���ز���     void
// ʹ��ʾ��     ips160_set_font(IPS160_8x16_FONT);
// ��ע��Ϣ     ���������ʱ�������� ���ú���Ч ������ʾ�����µ������С
//-------------------------------------------------------------------------------------------------------------------
void ips160_set_font (ips160_font_size_enum font)
{
    ips160_display_font = font;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     ������ʾ��ɫ
// ����˵��     pen             ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �� rgb565_color_enum ö��ֵ��������д��
// ����˵��     bgcolor         ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �� rgb565_color_enum ö��ֵ��������д��
// ���ز���     void
// ʹ��ʾ��     ips160_set_color(RGB565_RED, RGB565_GRAY);
// ��ע��Ϣ     ������ɫ�ͱ�����ɫҲ������ʱ�������� ���ú���Ч
//-------------------------------------------------------------------------------------------------------------------
void ips160_set_color (const uint16_t pen, const uint16_t bgcolor)
{
    ips160_pencolor = pen;
    ips160_bgcolor = bgcolor;
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IPS160 ����
// ����˵��     x               ����x�������� [0, ips160_x_max-1]
// ����˵��     y               ����y�������� [0, ips160_y_max-1]
// ����˵��     color           ��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �� rgb565_color_enum ö��ֵ��������д��
// ���ز���     void
// ʹ��ʾ��     ips160_draw_point(0, 0, RGB565_RED);            //����0,0��һ����ɫ�ĵ�
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void ips160_draw_point (uint16_t x, uint16_t y, const uint16_t color)
{
    ips160_set_region(x, y, x, y);
    ips160_write_16bit_data(color);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IPS160 ����
// ����˵��     x_start     ����x�������� [0, ips160_x_max-1]
// ����˵��     y_start     ����y�������� [0, ips160_y_max-1]
// ����˵��     x_end       ����x������յ� [0, ips160_x_max-1]
// ����˵��     y_end       ����y������յ� [0, ips160_y_max-1]
// ����˵��     color				��ɫ��ʽ RGB565 ���߿���ʹ�� zf_common_font.h �� rgb565_color_enum ö��ֵ��������д��
// ���ز���     void
// ʹ��ʾ��     ips160_draw_line(0, 0, 10, 10, RGB565_RED);     // ���� 0,0 �� 10,10 ��һ����ɫ����
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void ips160_draw_line (uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, const uint16_t color)
{
    int16_t x_dir = (x_start < x_end ? 1 : -1);
    int16_t y_dir = (y_start < y_end ? 1 : -1);
    float temp_rate = 0;
    float temp_b = 0;

    do
    {
        if(x_start != x_end)
        {
            temp_rate = (float)(y_start - y_end) / (float)(x_start - x_end);
            temp_b = (float)y_start - (float)x_start * temp_rate;
        }
        else
        {
            while(y_start != y_end)
            {
                ips160_draw_point(x_start, y_start, color);
                y_start += y_dir;
            }
            break;
        }
        if(func_abs(y_start - y_end) > func_abs(x_start - x_end))
        {
            while(y_start != y_end)
            {
                ips160_draw_point(x_start, y_start, color);
                y_start += y_dir;
                x_start = (int16_t)(((float)y_start - temp_b) / temp_rate);
            }
        }
        else
        {
            while(x_start != x_end)
            {
                ips160_draw_point(x_start, y_start, color);
                x_start += x_dir;
                y_start = (int16_t)((float)x_start * temp_rate + temp_b);
            }
        }
    }while(0);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IPS160 ��ʾ�ַ�
// ����˵��     x               ����x�������� ������Χ [0, ips160_x_max-1]
// ����˵��     y               ����y�������� ������Χ [0, ips160_y_max-1]
// ����˵��     dat             ��Ҫ��ʾ���ַ�
// ���ز���     void
// ʹ��ʾ��     ips160_show_char(0, 0, 'x');                     // ����0,0дһ���ַ�x
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void ips160_show_char (uint16_t x, uint16_t y, const char dat)
{
    uint8_t i = 0, j = 0;

    switch(ips160_display_font)
    {
        case IPS160_6X8_FONT:
        {
            for(i = 0; 6 > i; i ++)
            {
                ips160_set_region(x + i, y, x + i, y + 8);
                // �� 32 ��Ϊ��ȡģ�Ǵӿո�ʼȡ�� �ո��� ascii ������� 32
                uint8_t temp_top = ascii_font_6x8[dat - 32][i];
                for(j = 0; 8 > j; j ++)
                {
                    if(temp_top & 0x01)
                    {
                        ips160_write_16bit_data(ips160_pencolor);
                    }
                    else
                    {
                        ips160_write_16bit_data(ips160_bgcolor);
                    }
                    temp_top >>= 1;
                }
            }
        }break;
        case IPS160_8X16_FONT:
        {
            for(i = 0; 8 > i; i ++)
            {
                ips160_set_region(x + i, y, x + i, y + 15);
                // �� 32 ��Ϊ��ȡģ�Ǵӿո�ʼȡ�� �ո��� ascii ������� 32
                uint8_t temp_top = ascii_font_8x16[dat - 32][i];
                uint8_t temp_bottom = ascii_font_8x16[dat - 32][i + 8];
                for(j = 0; 8 > j; j ++)
                {
                    if(temp_top & 0x01)
                    {
                        ips160_write_16bit_data(ips160_pencolor);
                    }
                    else
                    {
                        ips160_write_16bit_data(ips160_bgcolor);
                    }
                    temp_top >>= 1;
                }
                for(j = 0; 8 > j; j ++)
                {
                    if(temp_bottom & 0x01)
                    {
                        ips160_write_16bit_data(ips160_pencolor);
                    }
                    else
                    {
                        ips160_write_16bit_data(ips160_bgcolor);
                    }
                    temp_bottom >>= 1;
                }
            }
        }break;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IPS160 ��ʾ�ַ���
// ����˵��     x               ����x�������� ������Χ [0, ips160_x_max-1]
// ����˵��     y               ����y�������� ������Χ [0, ips160_y_max-1]
// ����˵��     dat             ��Ҫ��ʾ���ַ���
// ���ز���     void
// ʹ��ʾ��     ips160_show_string(0, 0, "seekfree");
// ��ע��Ϣ
//-------------------------------------------------------------------------------------------------------------------
void ips160_show_string (uint16_t x, uint16_t y, const char dat[])
{
    uint16_t j = 0;
    while('\0' != dat[j])
    {
        switch(ips160_display_font)
        {
            case IPS160_6X8_FONT:   ips160_show_char(x + 6 * j, y, dat[j]); break;
            case IPS160_8X16_FONT:  ips160_show_char(x + 8 * j, y, dat[j]); break;
        }
        j ++;
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IPS160 ��ʾ32λ�з��� (ȥ������������Ч��0)
// ����˵��     x               ����x�������� ������Χ [0, ips160_x_max-1]
// ����˵��     y               ����y�������� ������Χ [0, ips160_y_max-1]
// ����˵��     dat             ��Ҫ��ʾ�ı��� �������� int32_t
// ����˵��     num             ��Ҫ��ʾ��λ�� ���10λ  ������������
// ���ز���     void
// ʹ��ʾ��     ips160_show_int(0, 0, x, 3);                    // x ����Ϊ int32_t int16_t int8 ����
// ��ע��Ϣ     ��������ʾһ�� ��-����
//-------------------------------------------------------------------------------------------------------------------
void ips160_show_int (uint16_t x, uint16_t y, const int32_t dat, uint8_t num)
{
    int32_t dat_temp = dat;
    int32_t offset = 1;
    char data_buffer[12];

    memset(data_buffer, 0, 12);
    memset(data_buffer, ' ', num+1);

    // ��������������ʾ 123 ��ʾ 2 λ��Ӧ����ʾ 23
    if(10 > num)
    {
        for(; 0 < num; num --)
        {
            offset *= 10;
        }
        dat_temp %= offset;
    }
    func_int_to_str(data_buffer, dat_temp);
    ips160_show_string(x, y, (const char *)&data_buffer);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IPS160 ��ʾ32λ�޷��� (ȥ������������Ч��0)
// ����˵��     x               ����x�������� ������Χ [0, ips114_x_max-1]
// ����˵��     y               ����y�������� ������Χ [0, ips114_y_max-1]
// ����˵��     dat             ��Ҫ��ʾ�ı��� �������� uint32_t
// ����˵��     num             ��Ҫ��ʾ��λ�� ���10λ  ������������
// ���ز���     void
// ʹ��ʾ��     ips160_show_uint(0, 0, x, 3);                   // x ����Ϊ uint32_t uint16_t uint8_t ����
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void ips160_show_uint (uint16_t x, uint16_t y, const uint32_t dat, uint8_t num)
{
    uint32_t dat_temp = dat;
    int32_t offset = 1;
    char data_buffer[12];
    memset(data_buffer, 0, 12);
    memset(data_buffer, ' ', num);

    // ��������������ʾ 123 ��ʾ 2 λ��Ӧ����ʾ 23
    if(10 > num)
    {
        for(; 0 < num; num --)
        {
            offset *= 10;
        }
        dat_temp %= offset;
    }
    func_uint_to_str(data_buffer, dat_temp);
    ips160_show_string(x, y, (const char *)&data_buffer);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IPS160 ��ʾ������(ȥ������������Ч��0)
// ����˵��     x               ����x�������� ������Χ [0, ips160_x_max-1]
// ����˵��     y               ����y�������� ������Χ [0, ips160_y_max-1]
// ����˵��     dat             ��Ҫ��ʾ�ı��� �������� double
// ����˵��     num             ����λ��ʾ����   ���8λ  
// ����˵��     pointnum        С��λ��ʾ����   ���6λ
// ���ز���     void
// ʹ��ʾ��     ips160_show_float(0, 0, x, 2, 3);               // ��ʾ������   ������ʾ2λ   С����ʾ��λ
// ��ע��Ϣ     �ر�ע�⵱����С��������ʾ��ֵ����д���ֵ��һ����ʱ��
//              ���������ڸ��������ȶ�ʧ���⵼�µģ��Ⲣ������ʾ���������⣬
//              �й���������飬�����аٶ�ѧϰ   ���������ȶ�ʧ���⡣
//              ��������ʾһ�� ��-����
//-------------------------------------------------------------------------------------------------------------------
void ips160_show_float (uint16_t x, uint16_t y, const double dat, uint8_t num, uint8_t pointnum)
{
    double dat_temp = dat;
    double offset = 1.0;
    char data_buffer[17];
                          memset(data_buffer, 0, 17);
    memset(data_buffer, ' ', num+pointnum+2);

    // ��������������ʾ 123 ��ʾ 2 λ��Ӧ����ʾ 23
    for(; 0 < num; num --)
    {
        offset *= 10;
    }
    dat_temp = dat_temp - ((int)dat_temp / (int)offset) * offset;
    func_double_to_str(data_buffer, dat_temp, pointnum);
    ips160_show_string(x, y, data_buffer);
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IPS160 ��ʾ RGB565 ��ɫͼ��
// ����˵��     x               ����x�������� ������Χ [0, ips160_x_max-1]
// ����˵��     y               ����y�������� ������Χ [0, ips160_y_max-1]
// ����˵��     *image          ͼ������ָ��
// ����˵��     width           ͼ��ʵ�ʿ���
// ����˵��     height          ͼ��ʵ�ʸ߶�
// ����˵��     dis_width       ͼ����ʾ���� ������Χ [0, ips160_x_max]
// ����˵��     dis_height      ͼ����ʾ�߶� ������Χ [0, ips160_y_max]
// ����˵��     color_mode      ɫ��ģʽ 0-��λ��ǰ 1-��λ��ǰ
// ���ز���     void
// ʹ��ʾ��     ips160_show_rgb565_image(0, 0, scc8660_image[0], SCC8660_W, SCC8660_H, SCC8660_W, SCC8660_H, 1);
// ��ע��Ϣ     ������ʾ��ͫ�� RGB565 ��ͼ��
//              ���Ҫ��ʾ��λ��ǰ������ RGB565 ͼ�� �޸����һ����������
//              ���Ҫ��ʾ��λ��ǰ������ RGB565 ͼ�� �޸����һ����������
//              ���Ҫ��ʾ��λ��ǰ������ RGB565 ͼ�� �޸����һ����������
//-------------------------------------------------------------------------------------------------------------------
void ips160_show_rgb565_image (uint16_t x, uint16_t y, const uint16_t *image, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height, uint8_t color_mode)
{
    uint32_t i = 0, j = 0;
    uint16_t color = 0;
    uint32_t width_index = 0, height_index = 0;

    ips160_set_region(x, y, x + dis_width - 1, y + dis_height - 1);             // ������ʾ����

    for(j = 0; j < dis_height; j ++)
    {
        height_index = j * height / dis_height;
        for(i = 0; i < dis_width; i ++)
        {
            width_index = i * width / dis_width;
            color = *(image + height_index * width + width_index);              // ��ȡ���ص�
            if(color_mode)
            {
                color = ((color & 0xff) << 8) | (color >> 8);
            }
            ips160_write_16bit_data(color);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------
// �������     IPS160 ��ʾ����
// ����˵��     x               ����x�������� ������Χ [0, ips160_x_max-1]
// ����˵��     y               ����y�������� ������Χ [0, ips160_y_max-1]
// ����˵��     *wave           ��������ָ��
// ����˵��     width           ����ʵ�ʿ���
// ����˵��     value_max       ����ʵ�����ֵ
// ����˵��     dis_width       ������ʾ���� ������Χ [0, ips160_x_max]
// ����˵��     dis_value_max   ������ʾ���ֵ ������Χ [0, ips160_y_max]
// ���ز���     void
// ʹ��ʾ��     ips160_show_wave(0, 0, data, 128, 64, 64, 32);
// ��ע��Ϣ     
//-------------------------------------------------------------------------------------------------------------------
void ips160_show_wave (uint16_t x, uint16_t y, const uint16_t *wave, uint16_t width, uint16_t value_max, uint16_t dis_width, uint16_t dis_value_max)
{
    uint32_t i = 0, j = 0;
    uint32_t width_index = 0, value_max_index = 0;

    ips160_set_region(x, y, x + dis_width - 1, y + dis_value_max - 1);          // ������ʾ����
    for(i = 0; i < dis_value_max; i ++)
    {
        for(j = 0; j < dis_width; j ++)
        {
            ips160_write_16bit_data(ips160_bgcolor); 
        }
    }

    for(i = 0; i < dis_width; i ++)
    {
        width_index = i * width / dis_width;
        value_max_index = *(wave + width_index) * (dis_value_max - 1) / value_max;
        ips160_draw_point((uint16_t)(i + x), (uint16_t)((dis_value_max - 1) - value_max_index + y), ips160_pencolor);
    }
}

void ips160_init(void)
{
	ips160_set_dir(ips160_display_dir);
	ips160_set_color(ips160_pencolor, ips160_bgcolor);

	IPS160_RES(0);  
	HAL_Delay(100);
  IPS160_RES(1);      
  HAL_Delay(100);
 
	ips160_write_command(0x11);     //Sleep out
	HAL_Delay(120);                //Delay 120ms
	ips160_write_command(0xB1);     //Normal mode
	ips160_write_8bit_data(0x05);   
	ips160_write_8bit_data(0x3C);   
	ips160_write_8bit_data(0x3C);   
	ips160_write_command(0xB2);     //Idle mode
	ips160_write_8bit_data(0x05);   
	ips160_write_8bit_data(0x3C);   
	ips160_write_8bit_data(0x3C);   
	ips160_write_command(0xB3);     //Partial mode
	ips160_write_8bit_data(0x05);   
	ips160_write_8bit_data(0x3C);   
	ips160_write_8bit_data(0x3C);   
	ips160_write_8bit_data(0x05);   
	ips160_write_8bit_data(0x3C);   
	ips160_write_8bit_data(0x3C);   
	ips160_write_command(0xB4);     //Dot inversion
	ips160_write_8bit_data(0x03);   
	ips160_write_command(0xC0);     //AVDD GVDD
	ips160_write_8bit_data(0xAB);   
	ips160_write_8bit_data(0x0B);   
	ips160_write_8bit_data(0x04);   
	ips160_write_command(0xC1);     //VGH VGL
	ips160_write_8bit_data(0xC5);   //C0
	ips160_write_command(0xC2);     //Normal Mode
	ips160_write_8bit_data(0x0D);   
	ips160_write_8bit_data(0x00);   
	ips160_write_command(0xC3);     //Idle
	ips160_write_8bit_data(0x8D);   
	ips160_write_8bit_data(0x6A);   
	ips160_write_command(0xC4);     //Partial+Full
	ips160_write_8bit_data(0x8D);   
	ips160_write_8bit_data(0xEE);   
	ips160_write_command(0xC5);     //VCOM
	ips160_write_8bit_data(0x0F);   
	ips160_write_command(0xE0);     //positive gamma
	ips160_write_8bit_data(0x07);   
	ips160_write_8bit_data(0x0E);   
	ips160_write_8bit_data(0x08);   
	ips160_write_8bit_data(0x07);   
	ips160_write_8bit_data(0x10);   
	ips160_write_8bit_data(0x07);   
	ips160_write_8bit_data(0x02);   
	ips160_write_8bit_data(0x07);   
	ips160_write_8bit_data(0x09);   
	ips160_write_8bit_data(0x0F);   
	ips160_write_8bit_data(0x25);   
	ips160_write_8bit_data(0x36);   
	ips160_write_8bit_data(0x00);   
	ips160_write_8bit_data(0x08);   
	ips160_write_8bit_data(0x04);   
	ips160_write_8bit_data(0x10);   
	ips160_write_command(0xE1);     //negative gamma
	ips160_write_8bit_data(0x0A);   
	ips160_write_8bit_data(0x0D);   
	ips160_write_8bit_data(0x08);   
	ips160_write_8bit_data(0x07);   
	ips160_write_8bit_data(0x0F);   
	ips160_write_8bit_data(0x07);   
	ips160_write_8bit_data(0x02);   
	ips160_write_8bit_data(0x07);   
	ips160_write_8bit_data(0x09);   
	ips160_write_8bit_data(0x0F);   
	ips160_write_8bit_data(0x25);   
	ips160_write_8bit_data(0x35);   
	ips160_write_8bit_data(0x00);   
	ips160_write_8bit_data(0x09);   
	ips160_write_8bit_data(0x04);   
	ips160_write_8bit_data(0x10);
		 
	ips160_write_command(0xFC);    
	ips160_write_8bit_data(0x80);  
		
	ips160_write_command(0x3A);     
	ips160_write_8bit_data(0x05);   
	ips160_write_command(0x36);
	switch(ips160_display_dir)
	{
		case IPS160_PORTAIT:        ips160_write_8bit_data(0x08);   break;
		case IPS160_PORTAIT_180:    ips160_write_8bit_data(0xC8);   break;
		case IPS160_CROSSWISE:      ips160_write_8bit_data(0x78);   break;
		case IPS160_CROSSWISE_180:  ips160_write_8bit_data(0xA8);   break;
	}
  
	ips160_write_command(0x21);     //Display inversion
	ips160_write_command(0x29);     //Display on
	ips160_clear();                                                             // ��ʼ��Ϊ����
}
