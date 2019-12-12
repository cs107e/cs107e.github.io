/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include <sys/stat.h>
#include <sys/time.h>

#include "emmc.h"
#ifdef HAVE_USPI
#include "uspi.h"
#endif

#include "diskio.h"		/* FatFs lower layer API */

/* Definitions of physical drive number for each drive */
#define MMC		0	/* Example: Map MMC/SD card to drive number 1 */
#define USB		1	/* Example: Map USB drive to drive number 2 */


static struct emmc_block_dev *emmc_dev;

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
    BYTE pdrv       /* Physical drive number to identify the drive */
)
{
    switch (pdrv) {
        case MMC :
            if (emmc_dev == NULL)
            {
                return STA_NOINIT;
            }
            return 0;

#ifdef HAVE_USPI
        default :
        {
            int nDeviceIndex = pdrv - USB;
            if (nDeviceIndex < 0 || nDeviceIndex >= USPiMassStorageDeviceAvailable())
            {
                return STA_NODISK;
            }
            return 0;
        }
#endif
    }
    return STA_NODISK;
}



/*-----------------------------------------------------------------------*/
/* Initialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
    BYTE pdrv               /* Physical drive number to identify the drive */
)
{
    switch (pdrv) {
        case MMC :
            if (emmc_dev == NULL)
            {
                if (sd_card_init((struct block_device **)&emmc_dev) != 0)
                {
                    return STA_NOINIT;
                }
            }
            return 0;

#ifdef HAVE_USPI
        default :
        {
            int nDeviceIndex = pdrv - USB;
            if (nDeviceIndex < 0 || nDeviceIndex >= USPiMassStorageDeviceAvailable())
            {
                return STA_NODISK;
            }
            return 0;
        }
#endif
    }
    return STA_NODISK;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
    BYTE pdrv,      /* Physical drive number to identify the drive */
    BYTE *buff,     /* Data buffer to store read data */
    DWORD sector,   /* Sector address in LBA */
    UINT count      /* Number of sectors to read */
)
{
    switch (pdrv) {
        case MMC :
        {
            size_t buf_size = count * emmc_dev->bd.block_size;
            if (sd_read(buff, buf_size, sector) < buf_size)
            {
                return RES_ERROR;
            }

            return RES_OK;
        }

#ifdef HAVE_USPI
        default :
        {
            int nDeviceIndex = pdrv - USB;
            if (nDeviceIndex < 0 || nDeviceIndex >= USPiMassStorageDeviceAvailable())
            {
                return RES_PARERR;
            }

            unsigned buf_size = count * USPI_BLOCK_SIZE;
            if (USPiMassStorageDeviceRead(sector * USPI_BLOCK_SIZE, buff, buf_size, nDeviceIndex) < buf_size)
            {
                return RES_ERROR;
            }

            return RES_OK;
        }
#endif
    }

    return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write (
    BYTE pdrv,          /* Physical drive number to identify the drive */
    const BYTE *buff,   /* Data to be written */
    DWORD sector,       /* Sector address in LBA */
    UINT count          /* Number of sectors to write */
)
{
    switch (pdrv) {
        case MMC :
        {
            size_t buf_size = count * emmc_dev->bd.block_size;
            if (sd_write((uint8_t *)buff, buf_size, sector) < buf_size)
            {
                return RES_ERROR;
            }

            return RES_OK;
        }

#ifdef HAVE_USPI
        default :
        {
            int nDeviceIndex = pdrv - USB;
            if (nDeviceIndex < 0 || nDeviceIndex >= USPiMassStorageDeviceAvailable())
            {
                return RES_PARERR;
            }

            unsigned buf_size = count * USPI_BLOCK_SIZE;
            if (USPiMassStorageDeviceWrite(sector * USPI_BLOCK_SIZE, buff, buf_size, nDeviceIndex) < buf_size)
            {
                return RES_ERROR;
            }

            return RES_OK;
        }
#endif
    }

    return RES_PARERR;
}


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
    BYTE pdrv,      /* Physical drive number (0..) */
    BYTE cmd,       /* Control code */
    void *buff      /* Buffer to send/receive control data */
)
{
    switch (pdrv) {
        case MMC :
            if (cmd == CTRL_SYNC)
            {
                return RES_OK;
            }
            if (cmd == GET_SECTOR_COUNT)
            {
                *(DWORD *)buff = emmc_dev->bd.num_blocks;
                return RES_OK;
            }
            if (cmd == GET_SECTOR_SIZE)
            {
                emmc_dev->bd.block_size = 512; // added by Chris
                *(DWORD *)buff = emmc_dev->bd.block_size;
                return RES_OK;
            }
            if (cmd == GET_BLOCK_SIZE)
            {
                *(DWORD *)buff = emmc_dev->bd.block_size;
                return RES_OK;
            }
            return RES_PARERR;

#ifdef HAVE_USPI
        default :
        {
            int nDeviceIndex = pdrv - USB;
            if (nDeviceIndex < 0 || nDeviceIndex >= USPiMassStorageDeviceAvailable())
            {
                return RES_PARERR;
            }
            if (cmd == CTRL_SYNC)
            {
                return RES_OK;
            }
            if (cmd == GET_SECTOR_COUNT)
            {
                *(DWORD *)buff = USPiMassStorageDeviceGetCapacity(nDeviceIndex);
                return RES_OK;
            }
            if (cmd == GET_SECTOR_SIZE)
            {
                *(DWORD *)buff = USPI_BLOCK_SIZE;
                return RES_OK;
            }
            if (cmd == GET_BLOCK_SIZE)
            {
                *(DWORD *)buff = USPI_BLOCK_SIZE;
                return RES_OK;
            }
            return RES_PARERR;
        }
#endif
    }

    return RES_PARERR;
}

#if !_FS_READONLY && !_FS_NORTC
DWORD get_fattime (void)
{
    time_t now = time(NULL);
    struct tm *ltm = localtime(&now);

    return   ((DWORD)(ltm->tm_year - 80) << 25)
           | ((DWORD)(ltm->tm_mon + 1) << 21)
           | ((DWORD)ltm->tm_mday << 16)
           | ((DWORD)ltm->tm_hour << 11)
           | ((DWORD)ltm->tm_min << 5)
           | ((DWORD)ltm->tm_sec >> 1);
}
#endif
