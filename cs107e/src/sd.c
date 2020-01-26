/* 
 * File: sd.c
 * ----------
 *
 * Authored by Omar
 *
 * Julie: not sure whether/how much this module has been tested
 *
 */

#include "timer.h"
#include "sd.h"
#include <stddef.h>


#define EMMC_BASE 0x20300000

/* The controller on the Pi ('EMMC') is a standard SD host controller,
   I think. See
   https://www.sdcard.org/developers/overview/host_controller/simple_spec/
   for specification. */

/* SD host controller spec PDF page 31 (Table 2-1, SD Host Controller
   register map) and
   http://cs107e.github.io/readings/BCM2835-ARM-Peripherals.pdf#page=66 */
typedef struct {
    unsigned arg2;
    unsigned blksizecnt;
    unsigned arg1;
    unsigned cmdtm;

    unsigned resp0;
    unsigned resp1;
    unsigned resp2;
    unsigned resp3;

    unsigned data;
    unsigned status;

    /* (Wakeup Control, Block Gap Control, Power Control, Host Control) */
    unsigned control0;
    /* (Software Reset, Timeout Control, Clock Control[2 bytes]) */
    unsigned control1;

    unsigned interrupt;
    unsigned irpt_mask;
    unsigned irpt_en;

    /* Auto CMD12 Error Status */
    unsigned control2;

    unsigned capabilities;
    unsigned capabilities_reserved;
    unsigned max_capabilities;
    unsigned max_capabilities_reserved;

    unsigned force_irpt;

    unsigned padding2[7];

    unsigned boot_timeout;
    unsigned dbg_sel;

    unsigned padding3[2];

    unsigned exrdfifo_cg;
    unsigned exrdfifo_en;

    unsigned tune_step;
    unsigned tune_steps_std;
    unsigned tune_steps_ddr;

    unsigned padding4[23];

    unsigned spi_int_spt;

    unsigned padding5[2];

    unsigned slotisr_ver;
} emmc_t;

/* See https://www.sdcard.org/downloads/pls/pdf/partA2_300.pdf PDF page 44. */
#define STATUS_CMD_INHIBIT (1 << 0)
#define STATUS_CARD_INSERTED (1 << 16)

#define CONTROL1_CLK_INTLEN (1 << 1)

#define INTERRUPT_READ_READY (1 << 5)
#define INTERRUPT_WRITE_READY (1 << 4)

volatile emmc_t *emmc = (volatile emmc_t *) EMMC_BASE;

/* See SD physical layer spec for details on commands:
   https://www.sdcard.org/downloads/pls/pdf/index.php?p=part1_410-1.jpg&f=part1_410.pdf&e=EN_SS1 */
typedef enum {
    CMD_GO_IDLE_STATE = 0,
    CMD_ALL_SEND_CID = 2,
    CMD_SEND_RELATIVE_ADDR = 3,
    CMD_SELECT_CARD = 7,
    CMD_SEND_IF_COND = 8,
    CMD_READ_SINGLE_BLOCK = 17,
    CMD_WRITE_BLOCK = 24,
    CMD_APP_CMD = 55
} cmd_t;

typedef enum {
    SD_SEND_OP_COND = 41
} app_cmd_t;

#define DATA_NONE 0

static void sd_send_cmd(cmd_t cmd, unsigned arg, unsigned *response, unsigned data) {
    while (emmc->status & STATUS_CMD_INHIBIT);

    emmc->arg1 = arg;
    emmc->cmdtm =
            /* CMD_INDEX */
            ((cmd & 0x3F) << 24) |
            /* CMD_RSPNS_TYPE */
            (response ? (2 << 16) : 0) |
            /* TM_DAT_DIR */
            (1 << 4) |
            /* CMD_ISDATA */
            (data ? (1 << 21) : 0);

    timer_delay_us(2000); // Still not totally sure why we need this.

    while (!(emmc->status & STATUS_CMD_INHIBIT) &&
           !(emmc->interrupt & 0x1));

    emmc->interrupt = 1;

    if (response)
        *response = emmc->resp0;
}

static void sd_send_app_cmd(app_cmd_t cmd, unsigned arg, unsigned *response) {
    do {
        sd_send_cmd(CMD_APP_CMD, 0, response, DATA_NONE);
        *response = 0;
        sd_send_cmd(cmd, arg, response, DATA_NONE);
    } while (!((*response) & (1 << 31)));
}

bool sd_init(void) {
    timer_init();

    int sdversion = (emmc->slotisr_ver >> 16) & 0xFF;

    if (!(emmc->status & STATUS_CARD_INSERTED)) { // No SD card inserted
        return false;
    }

    emmc->control1 =
        // Timeout Control, DATA_TOUNIT = Disable data timeout.
        (0b1111 << 16) |
        // Clock Control
        (0b0000111100100111 << 0);
    // Host Control 2 and Auto CMD Error Status = No interrupts, slow
    // SDR12 speed, 3.3V signaling. Simplest possible config.
    emmc->control2 = 0x0;

    // Block Size, Transfer Block Size = 0x200 (512 bytes)
    // Block Count, Blocks Count For Current Transfer = 1
    emmc->blksizecnt = (1 << 16) | SD_SECTOR_SIZE;

    // Enable all the interrupt flags. Why?
    emmc->irpt_mask = 0xFFFFFFFF;

    while (!(emmc->control1 & CONTROL1_CLK_INTLEN));

    unsigned response;

    // See SD physical layer spec, section 4.2 (Card Identification
    // Mode).

    sd_send_cmd(CMD_GO_IDLE_STATE, 0, NULL, 0);
    if (sdversion >= 2) {
        // I don't think this works on non-SDHC cards, anyway...

        // "It is mandatory to issue CMD8 [CMD_SEND_IF_COND] prior to
        // the first ACMD41."
        sd_send_cmd(CMD_SEND_IF_COND, 0x1AA, &response, 1 << 16);
    }

    // "SD_SEND_OP_COND (ACMD41) is used to start initialization and
    // to check if the card has completed initialization."
    // I think we use this to trigger SDHC support?
    sd_send_app_cmd(SD_SEND_OP_COND, 0x50FF0000, &response);

    // "The host then issues the command ALL_SEND_CID (CMD2), to each
    // card to get its unique card identification (CID) number. Card
    // that is unidentified (i.e. which is in Ready State) sends its
    // CID number as the response (on the CMD line)."
    sd_send_cmd(CMD_ALL_SEND_CID, 0, NULL, DATA_NONE);

    // "After the CID was sent by the card it goes into Identification
    // State. Thereafter, the host issues CMD3 (SEND_RELATIVE_ADDR)
    // asks the card to publish a new relative card address (RCA),
    // which is shorter than CID and which is used to address the card
    // in the future data transfer mode."
    sd_send_cmd(CMD_SEND_RELATIVE_ADDR, 0, &response, DATA_NONE);

    // "CMD7 is used to select one card and put it into the Transfer State."
    unsigned rca = response >> 16;
    sd_send_cmd(CMD_SELECT_CARD, rca << 16, NULL, DATA_NONE);

    emmc->irpt_en = 0;
    emmc->irpt_mask = 0xFFFFFFFF;
    return true;
}

void sd_sector_read(void *buf, unsigned addr) {
    sd_send_cmd(CMD_READ_SINGLE_BLOCK, addr, NULL, 1);

    unsigned *buf32 = (unsigned *) buf;
    for (int i = 0; i < SD_SECTOR_SIZE / sizeof(unsigned); i++) {
        while (!(emmc->interrupt & INTERRUPT_READ_READY));

        buf32[i] = emmc->data;
    }
}

void sd_sector_write(const void *buf, unsigned addr) {
    unsigned response;
    /* We don't read response, but the command needs to have it. */
    sd_send_cmd(CMD_WRITE_BLOCK, addr, &response, 1);

    const unsigned *buf32 = (const unsigned *) buf;
    for (int i = 0; i < SD_SECTOR_SIZE / sizeof(unsigned); i++) {
        while (!(emmc->interrupt & INTERRUPT_WRITE_READY));

        emmc->data = buf32[i];
    }
}
