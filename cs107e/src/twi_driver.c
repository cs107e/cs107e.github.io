/*
 * File: twi_driver.c
 *
 * Description: Contains backend HAL for TWI driver. Allows for the initialization
 * of the TWI driver and essential block read/write features to interact with 
 * TWI/I2C based devices. This is the layer under the generalized API in i2c.c
 *
 * Author: Julie Zelenski  <zelenski@cs.stanford.edu>
 * Author: Elias Chikwanda <eliaschi@stanford.edu>
 * Author: Joe Robertazzi  <tazzi@stanford.edu>
 *
 * Last updated: Curis summer 2025
 */

#include "twi_driver.h"
#include "assert.h"
#include "ccu.h"
#include "gpio.h"
#include "malloc.h"
#include "strings.h"
#include "timer.h"
#include "i2c.h"
#include <stddef.h>


/*
 * IMPORTANT: bitfields & hardware registers
 * -----------------------------------------
 * TL;DR  Be sure to compile with gcc flag -fstrict-volatile-bitfields
 *
 * This flag tells gcc to generate 32-bit load/store instructions (i.e. lw/sw)
 * to access volatile bitfields. Without flag, gcc can generate 8 or 16-bit
 * instructions (i.e. sh or lb) that access subword. Subword access appears to
 * interact badly with twi hardware registers. This did not appear to be documented
 * anywhere; I only found out the hard way when observing garbled bits and lost
 * updates. I think it best to assume it is needed for all volatile bitfields
 * (i.e. any bitfield within peripheral registers)
*/

typedef union {
    struct {
        uint32_t padding[0x200 / sizeof(uint32_t)];
        struct { // TWI driver fields
            struct {
                uint32_t en                : 1; // driver enable
                uint32_t soft_reset        : 1;
                uint32_t                   : 6;
                uint32_t timeout_n         : 8;
                const uint32_t sta         : 8; // codes from FSM p.867, list p.875
                const uint32_t read_tran_error : 4;
                uint32_t read_tran_mode    : 1;
                uint32_t restart_mode      : 1;
                uint32_t                   : 1;
                uint32_t start_tran        : 1; // auto cleared
            } ctrl;
            struct {
                uint32_t
                    packet_interval        :16; // time between packets (in cycles?)
                uint32_t packet_cnt        :16;
            } cfg;
            struct {
                uint32_t id_x              : 8;
                uint32_t cmd               : 1; // cmd is read or write
                uint32_t id                : 7;
                uint32_t                   :16;
            } slv;
            struct {
                uint32_t n_data_bytes      :16;
                uint32_t n_addr_bytes      : 8;
                uint32_t                   : 8;
            } fmt;
            struct {
                uint32_t                   : 6;
                const uint32_t sta_sda     : 1;
                const uint32_t sta_scl     : 1;
                uint32_t clk_M             : 4; // output = (F0/(clkM+1)) /10
                uint32_t clk_N             : 3; // sampling 24Mhz/2^clkN
                uint32_t clk_duty          : 1;
                uint32_t clk_count_mode    : 1;
                uint32_t                   :15;
            } bus_ctrl;
            struct {
                uint32_t transmit_complete : 1;
                uint32_t transmit_error    : 1;
                uint32_t txfifo_req        : 1;
                uint32_t rxfifo_req        : 1;
                uint32_t                   :12;
                uint32_t transmit_complete_int_en : 1;
                uint32_t transmit_error_int_en    : 1;
                uint32_t txfifo_req_int_en  : 1;
                uint32_t rxfifo_req_int_en  : 1;
                uint32_t                    :12;
            } int_ctrl;
            uint32_t dma_cfg; // not used
            struct {
                const uint32_t txfifo_cnt   : 6;
                uint32_t txfifo_clear       : 1;
                uint32_t                    : 9;
                const uint32_t rxfifo_cnt   : 6;
                uint32_t rxfifo_clear       : 1;
                uint32_t                    : 9;
            } fcr; // fifo control
            uint32_t padding[56];
            struct {
                uint32_t d8                 : 8;
                uint32_t                    : 24;
            } txfifo;
            struct {
                uint32_t d8                 : 8;
                uint32_t                    : 24;
            } rxfifo;
        } driver;
    } regs;
    uint32_t padding[0x100];
} twi_t;

// NEVER remove: Static asserts to ensure bit field layout is correct
#define TWI_BASE ((twi_t *)0x02502000)
_Static_assert(&(TWI_BASE[0].regs.driver.ctrl)   == (void *)0x02502200, "TWI0 driver ctrl reg must be at address 0x02502200");
_Static_assert(&(TWI_BASE[0].regs.driver.fcr)    == (void *)0x0250221c, "TWI0 driver fifo control reg must be at address 0x0250221c");
_Static_assert(&(TWI_BASE[0].regs.driver.rxfifo) == (void *)0x02502304, "TWI0 driver rx fifo reg must be at address 0x02502304");

/*
 * Type: `twi_timing_t`
 *
 * `twi_timing_t` is a struct type used to refer to the M and N values associated
 * with setting the clock speed of the TWI (I2C) module. 
 */
typedef struct {
    int n;
    int m;
} twi_timing_t;

/*
 * Type: `twi_timing_entry_t`
 *
 * `twi_timing_entry_t` is a struct type used to store the frequency of the TWI (I2C) clock
 * and the timing configuration (in `twi_timing_t`) of the clock.
 */
typedef struct {
    int frequency;
    twi_timing_t timing;
} twi_timing_entry_t;

// TWI module creation
static struct {
    volatile twi_t *const twi_base, *twi;
    const gpio_id_t sda, scl;
} module = {
    .twi_base = &TWI_BASE[0], // TWI0
    .sda = GPIO_PG13,
    .scl = GPIO_PG12,
    .twi = NULL,
};

enum {
    CMD_WRITE = 0,
    CMD_READ = 1,
    TRANSITION_REPEATED_START = 0,
    TRANSITION_STOP_START = 1,
    YES_W_BEFORE_READ = 0,
    NO_W_BEFORE_READ = 1,
};

/*
 * The TWI serial clock line output frequency
 * Fscl = Fin/(2^N * (M + 1) * 10), Fin = 24 MHz (APB clock input)
 */
const twi_timing_entry_t i2c_timing_table[] = {
                // N , M
    {I2C_100KHZ, { 1 , 11 }},
    {I2C_20KHZ,  { 3 , 14 }},
    {I2C_400KHZ, { 1 ,  2 }}
};

// Chooses correct CLK_M and CLK_N values matching the passed in rate
static bool select_timing_settings(int frequency, twi_timing_t *timing) {
    for (int i = 0; i  < sizeof(i2c_timing_table) / sizeof(i2c_timing_table[0]); i++) {
        if (i2c_timing_table[i].frequency == frequency) {
            *timing = i2c_timing_table[i].timing;
            return true;
        }
    }
    return false;
}

#if 0
#include "printf.h"
static void debug_status(const char *label) {
    printf("\t[%02x] %s%s%s%s%s (%s)\n", module.twi->regs.driver.ctrl.sta,
        !module.twi->regs.driver.int_ctrl.txfifo_req && module.twi->regs.driver.fcr.txfifo_cnt? " txfifo_nonempty" : "",
        module.twi->regs.driver.int_ctrl.rxfifo_req && module.twi->regs.driver.fcr.rxfifo_cnt? " rxfifo_nonempty" : "",
        module.twi->regs.driver.int_ctrl.transmit_error ? " TRANSMIT_ERROR" : "",
        module.twi->regs.driver.int_ctrl.transmit_complete ? " transmit_complete" : "",
        module.twi->regs.driver.ctrl.read_tran_error ? " READ_TRAN_ERROR" : "",
        label);
}
#endif

// Initializes the TWI module and runs it at `rate` clock frequency
void twi_init(int frequency) {
    twi_timing_t timing = {i2c_timing_table[0].timing.n, i2c_timing_table[0].timing.m}; // Default 100 KHZ
    if (!select_timing_settings(frequency, &timing)) error("Invalid clock speed.");

    module.twi = &module.twi_base[0];                           // TWI0
    ccu_ungate_bus_clock(CCU_TWI_BGR_REG);
    gpio_set_function(module.sda, GPIO_FN_ALT3);
    gpio_set_function(module.scl, GPIO_FN_ALT3);
    module.twi->regs.driver.ctrl.en = 1;                        // enable TWI driver
    module.twi->regs.driver.bus_ctrl.clk_duty = 1;
    module.twi->regs.driver.bus_ctrl.clk_M = timing.m;
    module.twi->regs.driver.bus_ctrl.clk_N = timing.n;
    module.twi->regs.driver.int_ctrl.transmit_complete_int_en = 1;
    module.twi->regs.driver.int_ctrl.transmit_error_int_en = 1; // JMR - enable error flagging
}

static void reset_on_error(void) {
    // TODO: does anything else need reset, fifos? force release sda/scl?
    module.twi->regs.driver.ctrl.soft_reset = 1; // reset on error
    module.twi->regs.driver.ctrl.soft_reset = 0; // clear
   // debug_status("after reset on error");
}

static void send_bytes(const uint8_t *bytes, int n) {
    #define FIFO_FULL_CNT 32
    for (int i = 0; i < n && !module.twi->regs.driver.int_ctrl.transmit_error; /* adv in loop */) {
        if (module.twi->regs.driver.fcr.txfifo_cnt < FIFO_FULL_CNT) {
            module.twi->regs.driver.txfifo.d8 = bytes[i++];
        }
    }
}

static void receive_bytes(uint8_t *bytes, int n) {
    for (int i = 0; i < n && !module.twi->regs.driver.int_ctrl.transmit_error; /* adv in loop */) {
        if (module.twi->regs.driver.fcr.rxfifo_cnt > 0) {
            bytes[i++] = module.twi->regs.driver.rxfifo.d8;
        }
    }
}

static bool finish_transaction(void) {
    while (1) {
        if (module.twi->regs.driver.int_ctrl.transmit_complete) {
            return true;
        }
        if (module.twi->regs.driver.int_ctrl.transmit_error) {
            reset_on_error();
            return false;
        }
    }
}

static void config_transaction(uint8_t dev_id, int n_write, int n_read) {
    module.twi->regs.driver.slv.id = dev_id;
    module.twi->regs.driver.cfg.packet_cnt = 1;
    if (n_write > 0 && n_read > 0) {    // (write THEN read)
        module.twi->regs.driver.ctrl.restart_mode = TRANSITION_REPEATED_START;
        module.twi->regs.driver.ctrl.read_tran_mode = YES_W_BEFORE_READ;
        module.twi->regs.driver.slv.cmd = CMD_READ;
        module.twi->regs.driver.fmt.n_addr_bytes = n_write;
        module.twi->regs.driver.fmt.n_data_bytes = n_read;
    } else {                            // else
        module.twi->regs.driver.ctrl.restart_mode = TRANSITION_STOP_START;
        module.twi->regs.driver.ctrl.read_tran_mode = NO_W_BEFORE_READ;
        module.twi->regs.driver.fmt.n_addr_bytes = 0;
        if (n_read > 0) {               // (read ONLY)
            module.twi->regs.driver.slv.cmd = CMD_READ;
            module.twi->regs.driver.fmt.n_data_bytes = n_read;
         } else {                       // (write ONLY)
            module.twi->regs.driver.slv.cmd = CMD_WRITE;
            module.twi->regs.driver.fmt.n_data_bytes = n_write;
        }
    }

    // clear previous state before start new transaction
    module.twi->regs.driver.int_ctrl.transmit_complete = 1;    // write 1 to clear
    module.twi->regs.driver.int_ctrl.transmit_error = 1;       // write 1 to clear
    module.twi->regs.driver.ctrl.start_tran = 1;
}

bool twi_do_transaction(uint8_t dev_id, const uint8_t* bytes_w, int num_w, uint8_t* bytes_r, int num_r) {
    config_transaction(dev_id, num_w, num_r);
    if (num_w) send_bytes(bytes_w, num_w);
    if (num_r) receive_bytes(bytes_r, num_r);
    return finish_transaction();
}
