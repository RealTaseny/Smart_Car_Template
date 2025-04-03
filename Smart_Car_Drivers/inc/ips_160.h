#ifndef IPS_160_H__
#define IPS_160_H__

#include "stm32f4xx_hal.h"
#include "spi.h"
#include "gpio.h"
#include "zf_common_function.h"
#include "font.h"

#define IPS160_SPI_PORT (&hspi2)

#define IPS160_DC_PIN GPIO_PIN_14 //PB14
#define IPS160_DC_PORT GPIOB
#define IPS160_RES_PIN GPIO_PIN_13 //PB13
#define IPS160_RES_PORT GPIOB

// �������
#define IPS160_RES(x)                   	((x) ? (HAL_GPIO_WritePin(IPS160_RES_PORT, IPS160_RES_PIN, GPIO_PIN_SET))	:	(HAL_GPIO_WritePin(IPS160_RES_PORT, IPS160_RES_PIN, GPIO_PIN_RESET)))
#define IPS160_DC(x)                    	((x) ? (HAL_GPIO_WritePin(IPS160_DC_PORT, IPS160_DC_PIN, GPIO_PIN_SET))	:	(HAL_GPIO_WritePin(IPS160_DC_PORT, IPS160_DC_PIN, GPIO_PIN_RESET)))
//==================================================���� IPS160 �����ṹ��===============================================

#define X_OFFSET 26
#define Y_OFFSET 1

typedef enum
{
    IPS160_PORTAIT                      = 0,                                    // ����ģʽ
    IPS160_PORTAIT_180                  = 1,                                    // ����ģʽ  ��ת180
    IPS160_CROSSWISE                    = 2,                                    // ����ģʽ
    IPS160_CROSSWISE_180                = 3,                                    // ����ģʽ  ��ת180
}ips160_dir_enum;

typedef enum
{
    IPS160_6X8_FONT                     = 0,                                    // 6x8      ����
    IPS160_8X16_FONT                    = 1,                                    // 8x16     ����
}ips160_font_size_enum;
//==================================================���� IPS160 �����ṹ��===============================================

#define IPS160_DEFAULT_DISPLAY_DIR      (IPS160_CROSSWISE_180)                        // Ĭ�ϵ���ʾ����
#define IPS160_DEFAULT_PENCOLOR         (RGB565_RED    )                        // Ĭ�ϵĻ�����ɫ
#define IPS160_DEFAULT_BGCOLOR          (RGB565_WHITE  )                        // Ĭ�ϵı�����ɫ
#define IPS160_DEFAULT_DISPLAY_FONT     (IPS160_6X8_FONT)                      // Ĭ�ϵ�����ģʽ

//==================================================���� IPS160 ��������================================================
void    ips160_clear                    (void);                                                                                // IPS160 ��������
void    ips160_full                     (const uint16_t color);                                                                  // IPS160 ��Ļ��亯��
void    ips160_set_dir                  (ips160_dir_enum dir);                                                                 // IPS160 ������ʾ����
void    ips160_set_font                 (ips160_font_size_enum font);                                                          // IPS160 ������ʾ����
void    ips160_set_color                (const uint16_t pen, const uint16_t bgcolor);                                              // IPS160 ������ʾ��ɫ
void    ips160_draw_point               (uint16_t x, uint16_t y, const uint16_t color);                                              // IPS160 ���㺯��
void    ips160_draw_line                (uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end, const uint16_t color);      // IPS160 ���ߺ���

void    ips160_show_char                (uint16_t x, uint16_t y, const char dat);
void    ips160_show_string              (uint16_t x, uint16_t y, const char dat[]);
void    ips160_show_int                 (uint16_t x, uint16_t y, const int32_t dat, uint8_t num);
void    ips160_show_uint                (uint16_t x, uint16_t y, const uint32_t dat, uint8_t num);
void    ips160_show_float               (uint16_t x, uint16_t y, const double dat, uint8_t num, uint8_t pointnum);

void    ips160_show_binary_image        (uint16_t x, uint16_t y, const uint8_t *image, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height);                      // IPS160 ��ʾ��ֵͼ�� ����ÿ�˸������һ���ֽ�����
void    ips160_show_gray_image          (uint16_t x, uint16_t y, const uint8_t *image, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height, uint8_t threshold);     // IPS160 ��ʾ 8bit �Ҷ�ͼ�� ����ֵ����ֵ
void    ips160_show_rgb565_image        (uint16_t x, uint16_t y, const uint16_t *image, uint16_t width, uint16_t height, uint16_t dis_width, uint16_t dis_height, uint8_t color_mode);   // IPS160 ��ʾ RGB565 ��ɫͼ��

void    ips160_show_wave                (uint16_t x, uint16_t y, const uint16_t *wave, uint16_t width, uint16_t value_max, uint16_t dis_width, uint16_t dis_value_max);                // IPS160 ��ʾ����
void    ips160_show_chinese             (uint16_t x, uint16_t y, uint8_t size, const uint8_t *chinese_buffer, uint8_t number, const uint16_t color);                                 // IPS160 ������ʾ

void    ips160_init											(void);                                                         // 0.96�� IPSҺ����ʼ��
//==================================================���� IPS160 ��������================================================

#endif

