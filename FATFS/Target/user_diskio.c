/* USER CODE BEGIN Header */
/**
 ******************************************************************************
  * @file    user_diskio.c
  * @brief   This file includes a diskio driver skeleton to be completed by the user.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
 /* USER CODE END Header */

#ifdef USE_OBSOLETE_USER_CODE_SECTION_0
/*
 * Warning: the user section 0 is no more in use (starting from CubeMx version 4.16.0)
 * To be suppressed in the future.
 * Kept to ensure backward compatibility with previous CubeMx versions when
 * migrating projects.
 * User code previously added there should be copied in the new user sections before
 * the section contents can be deleted.
 */
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */
#endif

/* USER CODE BEGIN DECL */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "ff_gen_drv.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#include "w25qxx_flash.h"
#include <stdio.h>
#define SD_CARD 0
#define SPI_FLASH 1

/**
 * 对于SPI Flash而言， Fatfs需要设置正确的扇区大小与Block Size才能正常使用f_mkfs()函数
 */
#define W25Q128_SECTOR_COUNT 4096
#define W25Q128_SECTOR_SIZE 4096
#define W25Q128_BLOCK_SIZE 256
/* Private variables ---------------------------------------------------------*/
/* Disk status */
static volatile DSTATUS Stat = STA_NOINIT;

/* USER CODE END DECL */

/* Private function prototypes -----------------------------------------------*/
DSTATUS USER_initialize (BYTE pdrv);
DSTATUS USER_status (BYTE pdrv);
DRESULT USER_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
#if _USE_WRITE == 1
  DRESULT USER_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);
#endif /* _USE_WRITE == 1 */
#if _USE_IOCTL == 1
  DRESULT USER_ioctl (BYTE pdrv, BYTE cmd, void *buff);
#endif /* _USE_IOCTL == 1 */

Diskio_drvTypeDef  USER_Driver =
{
  USER_initialize,
  USER_status,
  USER_read,
#if  _USE_WRITE
  USER_write,
#endif  /* _USE_WRITE == 1 */
#if  _USE_IOCTL == 1
  USER_ioctl,
#endif /* _USE_IOCTL == 1 */
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes a Drive
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS USER_initialize (
	BYTE pdrv           /* Physical drive nmuber to identify the drive */
)
{
  /* USER CODE BEGIN INIT */
    Stat = STA_NOINIT;
  switch (pdrv) {
  case SD_CARD:

    return STA_NOINIT;

  case SPI_FLASH:
    w25qxx_flash_init();
    Stat = USER_status(SPI_FLASH);
    return Stat;
  }
    return Stat;
  /* USER CODE END INIT */
}

/**
  * @brief  Gets Disk Status
  * @param  pdrv: Physical drive number (0..)
  * @retval DSTATUS: Operation status
  */
DSTATUS USER_status (
	BYTE pdrv       /* Physical drive number to identify the drive */
)
{
  /* USER CODE BEGIN STATUS */
    Stat = STA_NOINIT;
  switch (pdrv) {
  case SD_CARD:

    return STA_NOINIT;

  case SPI_FLASH:
    if ((w25qxx_flash_read_ID() & 0XEF00) == 0XEF00)
      Stat = 0;
    else
      Stat = STA_NOINIT;
    return Stat;
  }
    return Stat;
  /* USER CODE END STATUS */
}

/**
  * @brief  Reads Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data buffer to store read data
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to read (1..128)
  * @retval DRESULT: Operation result
  */
DRESULT USER_read (
	BYTE pdrv,      /* Physical drive nmuber to identify the drive */
	BYTE *buff,     /* Data buffer to store read data */
	DWORD sector,   /* Sector address in LBA */
	UINT count      /* Number of sectors to read */
)
{
  /* USER CODE BEGIN READ */
  DRESULT res;
  switch (pdrv) {
  case SD_CARD:

    return RES_NOTRDY;

  case SPI_FLASH:
    w25qxx_flash_read(buff, sector * 4096, count * 4096);
    res = RES_OK;
    return res;
  }
    return RES_PARERR;
  /* USER CODE END READ */
}

/**
  * @brief  Writes Sector(s)
  * @param  pdrv: Physical drive number (0..)
  * @param  *buff: Data to be written
  * @param  sector: Sector address (LBA)
  * @param  count: Number of sectors to write (1..128)
  * @retval DRESULT: Operation result
  */
#if _USE_WRITE == 1
DRESULT USER_write (
	BYTE pdrv,          /* Physical drive nmuber to identify the drive */
	const BYTE *buff,   /* Data to be written */
	DWORD sector,       /* Sector address in LBA */
	UINT count          /* Number of sectors to write */
)
{
  /* USER CODE BEGIN WRITE */
  /* USER CODE HERE */
  DRESULT res;

  switch (pdrv) {
  case SD_CARD:

    return RES_NOTRDY;

  case SPI_FLASH:
    w25qxx_flash_write((uint8_t *)buff, sector * 4096, count * 4096);
    res = RES_OK;

    return res;
  }

  return RES_ERROR;
  /* USER CODE END WRITE */
}
#endif /* _USE_WRITE == 1 */

/**
  * @brief  I/O control operation
  * @param  pdrv: Physical drive number (0..)
  * @param  cmd: Control code
  * @param  *buff: Buffer to send/receive control data
  * @retval DRESULT: Operation result
  */
#if _USE_IOCTL == 1
DRESULT USER_ioctl (
	BYTE pdrv,      /* Physical drive nmuber (0..) */
	BYTE cmd,       /* Control code */
	void *buff      /* Buffer to send/receive control data */
)
{
  /* USER CODE BEGIN IOCTL */
  DRESULT res;

  switch (pdrv) {
  case SD_CARD:

    return RES_NOTRDY;

  case SPI_FLASH:

    switch (cmd)
    {
  case GET_SECTOR_COUNT:
    *(DWORD *)buff = w25qxx_flash_read_capacity();
      break;

  case GET_SECTOR_SIZE:
    *(WORD *)buff = 4096;
      break;

  case GET_BLOCK_SIZE:
    *(WORD *)buff = 1;
      break;
    }

    res = RES_OK;
    return res;
  }

  return RES_ERROR;
  /* USER CODE END IOCTL */
}
#endif /* _USE_IOCTL == 1 */

