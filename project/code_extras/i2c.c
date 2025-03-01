/*
    Module to support i2c communication via TWI hardware peripheral

    Author: Julie Zelenski
    Thu May 16 17:04:14 PDT 2024
 */
#include "assert.h"
#include "ccu.h"
#include "gpio.h"
#include "i2c.h"
#include "malloc.h"
#include <stddef.h>
#include "strings.h"
#include "timer.h"

/*
 * Use D1-H TWI engine peripheral as hardware controller for I2C communication
 */

// p. 859 D1 user manual
// structs defined to match layout of hardware registers
typedef union {
    struct {
        uint32_t addr;
        uint32_t xaddr;
        uint32_t data;
        struct {
            uint32_t clk_mode   : 1;
            uint32_t            : 1;
            uint32_t ack        : 1;
            uint32_t int_flag   : 1;
            uint32_t m_stp      : 1;
            uint32_t m_sta      : 1;
            uint32_t bus_en     : 1;
            uint32_t int_en     : 1;
            uint32_t            : 24;
        } cntr;
        uint32_t stat;
        struct {
            uint32_t clk_N      : 3;
            uint32_t clk_M      : 4;
            uint32_t clk_duty   : 1;
            uint32_t            : 24;
        } ccr;
        uint32_t srst;
        uint32_t efr;
        uint32_t lcr;
    } regs;
    uint32_t padding[0x100];
} twi_t;

#define TWI_BASE ((twi_t *)0x02502000)
_Static_assert(&(TWI_BASE[0].regs.lcr)   ==  (uint32_t *)0x02502020, "TWI0 lcr reg must be at address 0x02502020");
_Static_assert(&(TWI_BASE[1].regs.efr)   ==  (uint32_t *)0x0250241c, "TWI1 efr reg must be at address 0x0250241c");

static struct {
    volatile twi_t * const twi_base, *twi;
    const gpio_id_t sda, scl;
} module = {
    .twi_base = &TWI_BASE[0],   // twi0
    .sda = GPIO_PG13,
    .scl = GPIO_PG12,
    .twi = NULL,
};

#define SENTINEL 0x7e

struct i2c_device {
    uint8_t addr;
};

i2c_device_t * i2c_new(uint8_t addr) {
    i2c_device_t *dev = malloc(sizeof(*dev));
    dev->addr = addr;
    if (!i2c_block_write(dev, 0, 0)) {
        free(dev);
        return NULL;
    }
    return dev;
}

bool i2c_write_reg(i2c_device_t *dev, uint8_t reg, uint8_t val) {
    assert(dev);
    uint8_t buf[2] = {reg, val};
    return i2c_block_write(dev, buf, sizeof(buf));
}

bool i2c_write_reg_n(i2c_device_t *dev, uint8_t reg, uint8_t *bytes, int n) {
    assert(dev);
    uint8_t buf[n+1];
    buf[0] = reg;
    memcpy(buf+1, bytes, n);
    return i2c_block_write(dev, buf, sizeof(buf));
}

uint8_t i2c_read_reg(i2c_device_t *dev, uint8_t reg) {
    assert(dev);
    uint8_t buf[1];
    if (!i2c_block_write(dev, &reg, 1) || !i2c_block_read(dev, buf, 1)) {
        return SENTINEL; // return distinctive pattern to help debug read failure
    }
    return buf[0];
}

bool i2c_read_reg_n(i2c_device_t *dev, uint8_t reg, uint8_t *bytes, int n) {
    assert(dev);
    memset(bytes, SENTINEL, n); // init distinctive pattern to help debug read failure
    if (!i2c_block_write(dev, &reg, 1)) return false;
    return i2c_block_read(dev, bytes, n);
}

enum { WRITE_BIT = 0, READ_BIT = 1};

typedef enum  {
    BUS_ERROR = 0,
    START_TRANSMIT = 0x08,
    REPEATED_START_TRANSMIT= 0x10,
    ADDR_W_ACK = 0x18,
    ADDR_W_NAK = 0x20,
    DATA_TRANSMIT_ACK = 0x28,
    DATA_TRANSMIT_NAK = 0x30,
    LOST_ARBITRATION = 0x38,
    ADDR_R_ACK = 0x40,
    ADDR_R_NAK = 0x48,
    DATA_RECEIVE_ACK = 0x50,
    DATA_RECEIVE_NAK = 0x58,
    IDLE = 0xf8,
} i2c_stat_t;

void i2c_init(void) {
    // this driver supports only TWI0
    module.twi = &module.twi_base[0];
    //  (gating bit 16)
    ccu_ungate_bus_clock(CCU_TWI_BGR_REG);
    gpio_set_function(module.sda, GPIO_FN_ALT3); // TWI0
    gpio_set_function(module.scl, GPIO_FN_ALT3);
    // clock divisor values from p.876 of user manual
    // set for 100Khz
    module.twi->regs.ccr.clk_duty = 1;
    module.twi->regs.ccr.clk_M = 11;
    module.twi->regs.ccr.clk_N = 1;
    module.twi->regs.cntr.bus_en = 1;
    module.twi->regs.efr = 0;
    // efr disables special-case handling for unusual devices
    // see https://lore.kernel.org/linux-kernel/CAF8uH3u9L1cVyAZiY=981bDewYgVYM=27kcV0GwqHFURg21FgA@mail.gmail.com/T/
}

static i2c_stat_t wait_completion(void) {
    // Note: int_flag is R/W1C Read/Write 1 to Clear. Write 0 has no effect!
    module.twi->regs.cntr.int_flag = 1;
    unsigned int wait_count = 1000*1000;
    while (module.twi->regs.cntr.int_flag == 0 && --wait_count);
    if (wait_count == 0) error("TIMEOUT wait_completion in i2c driver\n");
    return module.twi->regs.stat;
}

static bool do_start(i2c_stat_t expected_status) {
    if (module.twi == NULL) error("i2c_init() has not been called!\n");
    module.twi->regs.cntr.m_sta = 1;
    i2c_stat_t status = wait_completion();
    return status == expected_status;
}

static void do_stop(void) {
    module.twi->regs.cntr.m_stp = 1;
    while (module.twi->regs.cntr.m_stp == 1) ; // no interrupt after stop, wait for stop bit to reset
    timer_delay_us(30); // add delay enough for min bus free time (required by adafruit seesaw for one)
}

static bool do_transmit(uint8_t byte, i2c_stat_t expected_status) {
    module.twi->regs.data = byte;
    i2c_stat_t status = wait_completion();
    return status == expected_status;
}

static bool do_receive(uint8_t *p, bool is_last) {
    i2c_stat_t expected_status = is_last? DATA_RECEIVE_NAK : DATA_RECEIVE_ACK;
    int response = is_last? 0: 1; // respond NAK for last, ACK otherwise
    module.twi->regs.cntr.ack = response;
    i2c_stat_t status = wait_completion();
    *p = module.twi->regs.data;
    return status == expected_status;
}

bool i2c_block_read(i2c_device_t *dev, uint8_t *bytes, int n) {
    assert(dev);
    memset(bytes, SENTINEL, n); // init distinctive pattern to help debug read failure
    bool success = do_start(START_TRANSMIT);
    success = success && do_transmit((dev->addr << 1) | READ_BIT, ADDR_R_ACK);
    for (int i = 0; success && i < n; i++) {
        bool is_last = (i == n - 1);
        success = do_receive(&bytes[i], is_last);
    }
    do_stop();
    return success;
}

bool i2c_block_write(i2c_device_t *dev, uint8_t *bytes, int n) {
    assert(dev);
    bool success = do_start(START_TRANSMIT);
    success = success && do_transmit((dev->addr << 1) | WRITE_BIT, ADDR_W_ACK);
    for (int i = 0; success && i < n; i++) {
        success = do_transmit(bytes[i], DATA_TRANSMIT_ACK);
    }
    do_stop();
    return success;
}
