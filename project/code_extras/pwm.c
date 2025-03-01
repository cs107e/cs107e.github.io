/*
    Module to interface with PWM hardware peripheral

    Author: Julie Zelenski
    Mon Nov 18 21:54:01 PST 2024
 */

#include "pwm.h"
#include "ccu.h"
#include "gpio.h"
#include <stdint.h>
#include "printf.h"
#include "timer.h"
#include "assert.h"
#include "gpio_extra.h"
#include <stdarg.h>

/*
 * IMPORTANT: bitfields & hardware registers
 * -----------------------------------------
 * TL;DR  Be sure to compile with gcc flag -fstrict-volatile-bitfields
 *
 * This flag tells gcc to generate 32-bit load/store instructions (i.e. lw/sw)
 * to access volatile bitfields. Without flag, gcc can generate 8 or 16-bit
 * instructions (i.e. sh or lb) that access subword. Subword access appears to
 * interact badly with pwm hardware registers. This did not appear to be documented
 * anywhere; I only found out the hard way when observing garbled bits and lost
 * updates.
 *
 * I don't know if this behavior is specific to pwm or affects all peripheral
 * registers. I think it best to assume it is needed for all volatile bitfields
 * (i.e. any bitfield within peripheral registers)
*/

typedef union {
    struct {
        uint32_t pier;
        uint32_t pisr;
        uint32_t _resA[2];
        uint32_t cier;
        uint32_t cisr;
        uint32_t _resB[2];
        struct {
            uint32_t clk_div    : 4;
            uint32_t            : 3;
            uint32_t clk_src    : 2;
            uint32_t            : 23;
        } pccr[4]; // clk div/src shared by pair (0-1, 2-3, 4-5, 6-7)
        uint32_t _resC[4];
        struct {
            uint32_t clk_gating : 8;
            uint32_t            : 8;
            uint32_t clk_bypass : 8;
            uint32_t            : 8;
        } pcgr;
        uint32_t _resD[7];
        uint32_t pdzcr[4];
        uint32_t _resE[4];
        uint32_t per;   // enable output (bit per-channel)
        uint32_t _resF[3];
        struct {
            uint32_t select     : 16;
            uint32_t enable     : 1;
            uint32_t start      : 1;
            uint32_t            : 14;
        } pgr[4]; // 4 groups
        uint32_t _resG[8];
        uint32_t cer;
        uint32_t _resH[15];
        struct {
            struct {
                uint32_t prescale       : 8;
                uint32_t active_state   : 1;   // polarity (active low vs high)
                uint32_t mode           : 1;   // continuous or pulse
                uint32_t pulse_start    : 1;
                uint32_t period_ready   : 1;
                uint32_t                : 4;
                uint32_t pulse_num      :16;
            } pcr;
            uint32_t ppr;
            struct {
                uint32_t counter_status :16; // cycle up-counter
                uint32_t counter_start  :16;
            } pcntr;
            uint32_t ppcntr;    // pulse counter
            uint32_t ccr;
            uint32_t crlr;
            uint32_t cflr;
            uint32_t _res;
        } channel[8];
    } regs;
} pwm_t;

#define PWM_BASE ((pwm_t *)0x02000C00)
_Static_assert(&(PWM_BASE->regs.cisr)               == (void *)0x02000C14, "pwm cisr reg must be at address 0x02000C14");
_Static_assert(&(PWM_BASE->regs.pgr)                == (void *)0x02000C90, "pwm pgr reg must be at address 0x02000C90");
_Static_assert(&(PWM_BASE->regs.cer)                == (void *)0x02000CC0, "pwm cer reg must be at address 0x02000CC0");
_Static_assert(&(PWM_BASE->regs.channel[3].pcr)     == (void *)0x02000D60, "pwm pcr[3] reg must be at address 0x02000D60");
_Static_assert(&(PWM_BASE->regs.channel[7].ppr)     == (void *)0x02000DE4, "pwm ppr[7] reg must be at address 0x02000DE4");
_Static_assert(&(PWM_BASE->regs.channel[5].ppcntr)  == (void *)0x02000DAC, "pwm ppcntr[5] reg must be at address 0x02000DAC");

static struct {
    volatile pwm_t * const pwm;
    struct {
        int k;
        int n_entire;
    } clk_settings[8]; // store per channel
    bool initialized;
} module = {
    .pwm = PWM_BASE,
    .initialized = false,
};

enum { SRC_HOSC = 0, SRC_APB0 = 1 };
enum { MODE_CYCLE_CONTINUOUS = 0, MODE_PULSE = 1 };
enum { ACTIVE_LOW = 0, ACTIVE_HIGH = 1 };
enum { PERIOD_READY = 0, PERIOD_BUSY = 1 };

static const int HOSC_FREQ = 24000000;

#define ceil(x, y) ((x) + (y) - 1)/(y)

void pwm_init(void) {
    if (!module.initialized) {
        ccu_ungate_bus_clock(CCU_PWM_BGR_REG);
        for (int i = 0; i < 4; i++) {   // base clock managed per-pair
            // use HOSC clock undivided as source for all pairs (channels)
            // use per-channel prescaler/period/duty settings to config
            module.pwm->regs.pccr[i].clk_src = SRC_HOSC;
            module.pwm->regs.pccr[i].clk_div = 0;
        }
        module.initialized = true;
    }
}

static struct {
    pwm_channel_id_t ch;
    gpio_id_t pin;
    uint8_t fn;
} gpio_pwn[] = {
    // this list only includes PWM outputs avail on pin header
    {PWM3, GPIO_PB0,  GPIO_FN_ALT2},
    {PWM4, GPIO_PB1,  GPIO_FN_ALT2},
    {PWM0, GPIO_PB5,  GPIO_FN_ALT5},
    {PWM1, GPIO_PB6,  GPIO_FN_ALT5},
    {PWM5, GPIO_PB8,  GPIO_FN_ALT3},
    {PWM6, GPIO_PB9,  GPIO_FN_ALT3},
    {PWM7, GPIO_PB10, GPIO_FN_ALT3},
    {PWM2, GPIO_PB11, GPIO_FN_ALT3},
    {PWM0, GPIO_PB12, GPIO_FN_ALT3},
    {PWM1, GPIO_PD17, GPIO_FN_ALT5},
    {PWM2, GPIO_PD18, GPIO_FN_ALT5}, // act led
    {PWM5, GPIO_PD21, GPIO_FN_ALT5},
    {PWM7, GPIO_PD22, GPIO_FN_ALT5},
    {PWM7, GPIO_PE16, GPIO_FN_ALT4},
    {PWM0, GPIO_PG12, GPIO_FN_ALT6},
    {PWM2, GPIO_PG13, GPIO_FN_ALT5},
    {-1} // sentinel to mark end
};

static bool set_pin_fn_to_pwm(pwm_channel_id_t ch, gpio_id_t pin) {
    for (int i = 0; gpio_pwn[i].ch != -1; i++) {
        if (gpio_pwn[i].ch == ch && gpio_pwn[i].pin == pin) {
            gpio_set_function(pin, gpio_pwn[i].fn);
            return true;
        }
    }
    return false;
}

/*
 * IMPORTANT: update of PPR (period) register
 * ------------------------------------------
 * TL;DR: must use single 32-bit write to ppr that sets both active/entire in one go
 *
 * I originally defined ppr as bitfield with upper/lower 16-bit.
 * The gcc-generated code was using lh/sh instructions which
 * did not place nice with hardware register. (see note above)
 * Apply flag fstrict-volatile-bitfields forces gcc to generate
 * 32-bit lw/sw. This corrected most of the problems, but not all.
 * Write of active and entire field in two separate actions
 * (i.e. two read-modify-write) was still problematic.
 * The observed behavior was erratic (timing-sensitive?), sometimes second
 * update just silently dropped. (seemed most likely to happen on the
 * very first set of ppr after init module)
 * Change code to write to ppr in single operation that sets
 * both fields in one go seems to behave perfectly, no glitch
*/
static void set_period(pwm_channel_id_t ch, int n_active, int n_entire) {
    assert(n_entire >= 1 && n_entire < 65535);
    assert(n_active >= 0 && n_active <= n_entire);
    module.pwm->regs.channel[ch].ppr = ((n_entire - 1) << 16) | n_active;   // see note above
    while (module.pwm->regs.channel[ch].pcr.period_ready == PERIOD_BUSY)
             ; // wait for settings to take effect
}

static void config_clock_settings(pwm_channel_id_t ch, int desired_freq) {
    int Q = HOSC_FREQ/desired_freq;
    // Q = k * n_entire
    // choose factors s.t. n_entire < 65536, minimize k (also k must be <= 256)
    int k = ceil(Q, 65535);
    int n_entire = HOSC_FREQ/(k*desired_freq);
    assert(n_entire >= 0 && n_entire <= 65535);
    assert(k >= 0 && k <= 256);
    module.clk_settings[ch].k = k;
    module.clk_settings[ch].n_entire = n_entire;
}

void pwm_config_channel(pwm_channel_id_t ch, gpio_id_t pin, int freq, bool invert) {
    if (!module.initialized) error("pwm_init() has not been called!\n");
    config_clock_settings(ch, freq);
    if (!set_pin_fn_to_pwm(ch, pin)) {
        printf("Did not find pwm functionality for pin %s and PWM%d\n", gpio_get_name_for_id(pin), ch);
        assert(0);
    }
    module.pwm->regs.pcgr.clk_gating |= (1 << ch); // open clock gate for channel
    module.pwm->regs.pcgr.clk_bypass &= ~(1 << ch); // clock bypass off
    int k = module.clk_settings[ch].k;
    module.pwm->regs.channel[ch].pcr.prescale = k - 1; // apply prescaler
    set_period(ch, 0, module.clk_settings[ch].n_entire); // config 0% duty to start
    module.pwm->regs.channel[ch].pcr.mode = MODE_CYCLE_CONTINUOUS;
    module.pwm->regs.channel[ch].pcr.active_state = invert? ACTIVE_LOW : ACTIVE_HIGH;
    module.pwm->regs.channel[ch].pcntr.counter_start = 0;
    module.pwm->regs.per |= (1 << ch);     // enable pwm output
}

void pwm_set_duty(pwm_channel_id_t ch, int percentile) {
    bool pwm_ch_enabled = (module.pwm->regs.per & (1 << ch)); // confirm ch is config/enabled
    assert(pwm_ch_enabled);
    assert(percentile >= 0 && percentile <= 100);
    int n_steps = module.clk_settings[ch].n_entire;
    int scaled_active = (percentile*n_steps)/100;
    set_period(ch, scaled_active, n_steps);
}

void pwm_set_freq(pwm_channel_id_t ch, int freq) {
    bool pwm_ch_enabled = (module.pwm->regs.per & (1 << ch));  // confirm ch is config/enabled
    assert(pwm_ch_enabled);
    int percentile = 50;
    if (freq == 0) {
        pwm_set_duty(ch, 0); // turn off
    } else {
        config_clock_settings(ch, freq);
        int k = module.clk_settings[ch].k;
        module.pwm->regs.channel[ch].pcr.prescale = k - 1; // apply prescaler
        int n_entire = module.clk_settings[ch].n_entire;
        int n_active = (n_entire*percentile)/100;
        module.pwm->regs.channel[ch].pcr.mode = MODE_CYCLE_CONTINUOUS;
        set_period(ch, n_active, n_entire);
    }
}
