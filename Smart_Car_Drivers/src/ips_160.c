#include "ips_160.h"

static uint16_t									ips160_pencolor     = IPS160_DEFAULT_PENCOLOR;
static uint16_t									ips160_bgcolor      = IPS160_DEFAULT_BGCOLOR;
static ips160_dir_enum							ips160_display_dir  = IPS160_DEFAULT_DISPLAY_DIR;
static uint16_t									ips160_x_max        = 162;
static uint16_t									ips160_y_max        = 132;

volatile uint8_t spi2_tx_complete = 0;

static void ips160_write_command (const uint8_t command)
{
	uint8_t data[1] = {command};
	IPS160_DC(0);
	HAL_SPI_Transmit_DMA(IPS160_SPI_PORT, data, sizeof(data));
	//while (!spi2_tx_complete);
	//spi2_tx_complete = 0;
	IPS160_DC(1);
}

static void ips160_write_8bit_data (const uint8_t dat)
{
	uint8_t data[1] = {dat};
	HAL_SPI_Transmit_DMA(IPS160_SPI_PORT, data, sizeof(data));
	//while (!spi2_tx_complete);
	//spi2_tx_complete = 0;
}

void ips160_write_16bit_data(const uint16_t dat) {
    uint8_t data[2];
    data[0] = (dat >> 8) & 0xFF;
    data[1] = dat & 0xFF;
    HAL_SPI_Transmit_DMA(IPS160_SPI_PORT, data, sizeof(data));
	//while (!spi2_tx_complete);
	//spi2_tx_complete = 0;
}

static void ips160_set_region (uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	switch(ips160_display_dir)
	{
		case IPS160_PORTAIT:
			ips160_write_command(0x2a);
			ips160_write_16bit_data(x1+X_OFFSET);
			ips160_write_16bit_data(x2+X_OFFSET);
			ips160_write_command(0x2b);
			ips160_write_16bit_data(y1+Y_OFFSET);
			ips160_write_16bit_data(y2+Y_OFFSET);
			ips160_write_command(0x2c);
			break;
		case IPS160_PORTAIT_180:
			ips160_write_command(0x2a);
			ips160_write_16bit_data(x1+X_OFFSET);
			ips160_write_16bit_data(x2+X_OFFSET);
			ips160_write_command(0x2b);
			ips160_write_16bit_data(y1+Y_OFFSET);
			ips160_write_16bit_data(y2+Y_OFFSET);
			ips160_write_command(0x2c);
			break;
		case IPS160_CROSSWISE:      
			ips160_write_command(0x2a);
			ips160_write_16bit_data(x1+Y_OFFSET);
			ips160_write_16bit_data(x2+Y_OFFSET);
			ips160_write_command(0x2b);
			ips160_write_16bit_data(y1+X_OFFSET);
			ips160_write_16bit_data(y2+X_OFFSET);
			ips160_write_command(0x2c);
			break;
		case IPS160_CROSSWISE_180:  
			ips160_write_command(0x2a);
			ips160_write_16bit_data(x1+Y_OFFSET);
			ips160_write_16bit_data(x2+Y_OFFSET);
			ips160_write_command(0x2b);
			ips160_write_16bit_data(y1+X_OFFSET);
			ips160_write_16bit_data(y2+X_OFFSET);
			ips160_write_command(0x2c);
			break;
	}
}

void ips160_clear(void) {
    ips160_set_region(0, 0, ips160_x_max - 1, ips160_y_max - 1);
    uint16_t color = ips160_bgcolor;
    uint8_t data[ips160_x_max * 2];
    

    for (uint16_t i = 0; i < ips160_x_max; i++) {
        data[i * 2] = (color >> 8) & 0xFF;
        data[i * 2 + 1] = color & 0xFF;
    }


    for (uint16_t j = 0; j < ips160_x_max; j++) {
        HAL_SPI_Transmit_DMA(IPS160_SPI_PORT, data, sizeof(data));
    	while (!spi2_tx_complete);
    	spi2_tx_complete = 0;
    }
}

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

void ips160_set_color (const uint16_t pen, const uint16_t bgcolor)
{
    ips160_pencolor = pen;
    ips160_bgcolor = bgcolor;
}

void ips160_draw_point (uint16_t x, uint16_t y, const uint16_t color)
{
    ips160_set_region(x, y, x, y);
    ips160_write_16bit_data(color);
}

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

static char get_font_model(const char *ch, const uint8_t **font)
{
	if((ch[0] > 0xA0) && (ch[1] > 0xA0))//中文
	{
		*font = &ascii_font_6x8[0][0] + 18 * ((ch[0] - 161) * 94 + ch[1] - 161);//获取此字模的地址
		return 12;
	}

	if(*ch >= 0x20)//英文
	{
		*font = &ascii_font_6x8[(ch[0] - 0x20)][0];
		return 6;
	}
	*font = &ascii_font_6x8[0][0];//无法匹配，显示空白
	return 0;
}

void ips160_show_char(uint8_t x, uint8_t y, uint8_t width, const uint8_t *font, uint16_t bgcolor, uint16_t pencolor)
{
	uint8_t i ,j;
	uint8_t mask;
	uint8_t height;

	if (width == 6) height = 8;
	else if (width == 12) height = 16;
	else height = 0;

	for (i = 0; i < width; i++)//竖着显示，先判断y坐标。
	{
		ips160_set_region(x + i, y, x + i, y + height);
		mask = *(font + i);
		for(j = 0; height > j; j ++)
		{
			if(mask & 0x01)
			{
				ips160_write_16bit_data(pencolor);
			}
			else
			{
				ips160_write_16bit_data(bgcolor);
			}
			mask >>= 1;
		}
	}
}

void ips160_show_string(uint16_t x, uint16_t y, char *ptrStr, uint16_t bgcolor, uint16_t pencolor)
{
	unsigned char temp_x, temp_width;
	const uint8_t *font;
	char ch[2];
	temp_x = x;
	while(*ptrStr != '\0')
	{
		ch[0] = *ptrStr;
		ptrStr++;
		ch[1] = *ptrStr;

		if(ch[0] == '\n')
		{
			x = temp_x;
			y += 12;
		}
		else if(ch[0] < 128)
		{
			temp_width = get_font_model(ch, &font);
			if(x < 128) ips160_show_char(x, y, temp_width, font, bgcolor, pencolor);
			x += temp_width;//下一个字符的横坐标
		}
		else if((ch[0] > 160) && (ch[1] > 160))	//中文
		{
			ch[1] = *ptrStr;
			temp_width = get_font_model(ch, &font);
			if(x < 128) ips160_show_char(x, y, temp_width, font, bgcolor, pencolor);
			x += temp_width;//下一个字符的横坐标
			ptrStr++;
		}
	}
}

void ips160_show_int (uint16_t x, uint16_t y, const int32_t dat, uint8_t num)
{
    int32_t dat_temp = dat;
    int32_t offset = 1;
    char data_buffer[12];

    memset(data_buffer, 0, 12);
    memset(data_buffer, ' ', num+1);

    if(10 > num)
    {
        for(; 0 < num; num --)
        {
            offset *= 10;
        }
        dat_temp %= offset;
    }
    func_int_to_str(data_buffer, dat_temp);
    ips160_show_string(x, y, data_buffer, IPS160_DEFAULT_BGCOLOR, IPS160_DEFAULT_PENCOLOR);
}

void ips160_show_uint (uint16_t x, uint16_t y, const uint32_t dat, uint8_t num)
{
    uint32_t dat_temp = dat;
    int32_t offset = 1;
    char data_buffer[12];
    memset(data_buffer, 0, 12);
    memset(data_buffer, ' ', num);

    if(10 > num)
    {
        for(; 0 < num; num --)
        {
            offset *= 10;
        }
        dat_temp %= offset;
    }
    func_uint_to_str(data_buffer, dat_temp);
    ips160_show_string(x, y, data_buffer, IPS160_DEFAULT_BGCOLOR, IPS160_DEFAULT_PENCOLOR);
}

void ips160_show_float (uint16_t x, uint16_t y, const double dat, uint8_t num, uint8_t pointnum)
{
    double dat_temp = dat;
    double offset = 1.0;
    char data_buffer[17];
    memset(data_buffer, 0, 17);
    memset(data_buffer, ' ', num+pointnum+2);

    for(; 0 < num; num --)
    {
        offset *= 10;
    }
    dat_temp = dat_temp - ((int)dat_temp / (int)offset) * offset;
    func_double_to_str(data_buffer, dat_temp, pointnum);
    ips160_show_string(x, y, data_buffer, IPS160_DEFAULT_BGCOLOR, IPS160_DEFAULT_PENCOLOR);
}

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
	ips160_clear();
}

