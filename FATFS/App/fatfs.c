/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file   fatfs.c
  * @brief  Code for fatfs applications
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
#include <stdio.h>
#include <string.h>
/* USER CODE END Header */
#include "fatfs.h"

uint8_t retUSER;    /* Return value for USER */
char USERPath[4];   /* USER logical drive path */
FATFS USERFatFS;    /* File system object for USER logical drive */
FIL USERFile;       /* File object for USER */

/* USER CODE BEGIN Variables */
BYTE work[_MAX_SS];

FRESULT scan_files (char* path)
{
  FRESULT res;
  DIR dir;
  UINT i;
  static FILINFO fno;
  static uint8_t layerDeeph=0;
  uint8_t j;


  res = f_opendir(&dir, path);
  if (res == FR_OK)
  {
    layerDeeph+=4;
    for (;;)
    {
      res = f_readdir(&dir, &fno);
      if (res != FR_OK || fno.fname[0] == 0)
        break;
      if (fno.fattrib & AM_DIR)
      {
        i = strlen(path);
        sprintf(&path[i], "/%s", fno.fname);
        for(j=0;j<layerDeeph;j++)
          printf(" ");
        printf("%s%s%s%s%s%s\r\n", "\033[","33;","22m",fno.fname,"  <DIR>","\033[0m");
        res = scan_files(path);
        if (res != FR_OK) break;
        path[i] = 0;
      }
      else
      {
        for(j=0;j<layerDeeph;j++)
          printf(" ");
        printf("%s%s%s%s%s\r\n","\033[","32;","22m",fno.fname,"\033[0m");
      }
    }
    f_closedir(&dir);
    layerDeeph-=4;
  }

  return res;
}
/* USER CODE END Variables */

void fatfs_init(void)
{
  /*## FatFS: Link the USER driver ###########################*/
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath, 0);
  retUSER = FATFS_LinkDriver(&USER_Driver, USERPath, 1);

  /* USER CODE BEGIN Init */
  retUSER = f_mount(&USERFatFS,  "FLASH:",  1);   //挂载文件系统
  if (retUSER == FR_NO_FILESYSTEM)
  {
    printf("FR_NO_Filesystem Error, make filesystem...\r\n");
    retUSER = f_mkfs("FLASH:", FM_FAT, _MAX_SS, work, sizeof(work));
    if (retUSER == FR_OK) printf("Make filesystem done, trying to remount SPI Flash...\r\n");
    else printf("Make filesystem error, return code:%d, \r\n", retUSER);
    retUSER = f_mount(&USERFatFS, "FLASH:", 1);
    if (retUSER == FR_OK) printf("Successfully mount filesystem\r\n");
    else printf("Mount filesystem error, return code:%d, \r\n", retUSER);
  }
  //scan_files("FLASH:");
  /* USER CODE END Init */
}

/* USER CODE BEGIN Application */

/* USER CODE END Application */
