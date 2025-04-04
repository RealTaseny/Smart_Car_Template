//
// Created by Taseny on 24-12-26.
//

#ifndef W25QXX_FLASH_H
#define W25QXX_FLASH_H

#include "spi.h"

#define W25QXX_FLASH_SPI_Handle (&hspi1)

#define W25Q80 	0XEF13
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17

#define W25QXX_FLASH_CS(x) ((x) == (0) ? (HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET)) : (HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET)))

#define W25QXX_WriteEnable		    (0x06)
#define W25QXX_WriteDisable		    (0x04)
#define W25QXX_ReadStatusReg		(0x05)
#define W25QXX_WriteStatusReg		(0x01)
#define W25QXX_ReadData			    (0x03)
#define W25QXX_FastReadData		    (0x0B)
#define W25QXX_FastReadDual		    (0x3B)
#define W25QXX_PageProgram		    (0x02)
#define W25QXX_BlockErase			(0xD8)
#define W25QXX_SectorErase		    (0x20)
#define W25QXX_ChipErase			(0xC7)
#define W25QXX_PowerDown			(0xB9)
#define W25QXX_ReleasePowerDown	    (0xAB)
#define W25QXX_DeviceID			    (0xAB)
#define W25QXX_ManufactDeviceID	    (0x90)
#define W25QXX_JedecDeviceID		(0x9F)

int w25qxx_flash_init(void);
void w25qxx_flash_read_UID(uint8_t *UID);
uint16_t  w25qxx_flash_read_ID(void);  	    		//读取FLASH ID
uint8_t	 w25qxx_flash_read_SR(void);        		//读取状态寄存器
void w25qxx_flash_write_SR(uint8_t sr);  			//写状态寄存器
void w25qxx_flash_write_enable(void);  		//写使能
void w25qxx_flash_write_disable(void);		//写保护
void w25qxx_flash_write_no_check(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);
void w25qxx_flash_read(uint8_t* pBuffer,uint32_t ReadAddr,uint16_t NumByteToRead);   //读取flash
void w25qxx_flash_write(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite);//写入flash
void w25qxx_flash_erase_all(void);    	  	//整片擦除
void w25qxx_flash_erase_sector(uint32_t Dst_Addr);	//扇区擦除
void w25qxx_flash_wait_busy(void);           	//等待空闲
void w25qxx_flash_power_down(void);        	//进入掉电模式
void w25qxx_flash_wakeup(void);				//唤醒
uint32_t w25qxx_flash_read_capacity(void);

extern uint16_t w25qxx_Flash_Type;
extern uint32_t w25qxx_Flash_Size;
extern uint8_t  w25qxx_Flash_UID[8];

#endif //W25QXX_FLASH_H