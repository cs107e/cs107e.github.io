/*
 * Functions to manage D1 clocks
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 * Sat Sep  7 11:08:33 PDT 2024
 */

#include "ccu.h"
#include "assert.h"
#include <stddef.h>
#include "printf.h"
#include "timer.h"

/*
 * Models the D-1 clock tree as diagrammed on p.39 of D-1 manual
 * (with some simplifications to only consider clocks we are using)
 * The implementation borrows design elements from linux clock tree
 * (e.g. clock parent)
 */
struct debug_info {
    long (*fn)(uint32_t);
    uint32_t reg_id;
    const char *name;
    ccu_parent_id_t parents[4];
    uint32_t ncount, mcount;
};
static struct debug_info *info_for_id(uint32_t id);
static long debug_rate_pll(ccu_pll_id_t id);
static long debug_rate_clk(ccu_module_id_t id);
static long debug_rate_bgr(ccu_bgr_id_t id);
static long debug_rate_parent(ccu_parent_id_t id);
static int get_parent_src_index(ccu_module_id_t id, ccu_parent_id_t parent);
static void validate_pll(ccu_pll_id_t id);
static void validate_module_clk(ccu_module_id_t id);
static void validate_bgr(ccu_bgr_id_t id);

typedef union {
    struct {
        uint32_t factor_m0  : 1;
        uint32_t factor_m1  : 1;
        uint32_t            : 6;
        uint32_t factor_n   : 8;
        uint32_t factor_p   : 6;
        uint32_t            : 5;
        uint32_t output_ena : 1;
        uint32_t locked     : 1;
        uint32_t lock_ena   : 1;
        uint32_t ldo_ena    : 1;
        uint32_t ena        : 1;
    };
    uint32_t bits;
} pll_reg_t;

typedef union {
    struct {
        uint32_t factor_m   : 5;
        uint32_t            : 3;
        uint32_t factor_n   : 2;
        uint32_t            : 14;
        uint32_t src        : 3;
        uint32_t            : 4;
        uint32_t ena        : 1;
    };
    uint32_t bits;
} module_clk_reg_t;

typedef struct {
    union {
        uint8_t raw[0xD10];
        uint32_t regs[1];
    } ;
} ccu_t;

#define CCU_BASE ((ccu_t *)0x02001000)
_Static_assert(&CCU_BASE->regs[0]               == (uint32_t *)0x02001000, "CCU pll cpu reg must be at address 0x02001000");
_Static_assert(&CCU_BASE->raw[CCU_APB0_CLK_REG] ==  (uint8_t *)0x02001520, "CCU apb0 reg must be at address 0x02001520");

static volatile ccu_t *const module = CCU_BASE;

static volatile uint32_t *reg_for_id(uint32_t raw_offset) {
    assert(raw_offset % 4 == 0);                // reject if not on 4-byte boundary
    assert(raw_offset < sizeof(module->raw));   // reject if not in range
    return (uint32_t *)(module->raw + raw_offset);
}

// Converting PLL rate into appropriate set of dividers on the fly
// is messy. Rather, take from manual the recommended dividers for
// commonly used PLL rates and list in this table for easy access.
// if you need an additional rate config, simply add to this table
static const struct pll_config_t {
    ccu_pll_id_t pll_id;
    uint32_t rate;
    struct { uint8_t P; uint8_t N; uint8_t M1; uint8_t M0; };
} pll_table[] = {
    {CCU_PLL_VIDEO0_CTRL_REG,  297000000, {        .N= 99,  .M1= 2 }},          // recommended dividers 99,2 p.43 D-1 manual
    {CCU_PLL_VIDEO0_CTRL_REG,  120000000, {        .N= 20,  .M1= 1 }},
    {CCU_PLL_AUDIO0_CTRL_REG,   22545454, {.P= 33, .N= 124, .M1= 1, .M0= 1 }},  // base rate for audio 22.1Khz
    {CCU_PLL_AUDIO1_CTRL_REG, 3072000000, {        .N= 128, .M1= 1 }},          // base rate for audio 24Khz
    {0}
};

static const struct pll_config_t *get_pll_config_for_rate(ccu_pll_id_t id, long rate) {
    for (const struct pll_config_t *cfg = pll_table; cfg->rate != 0; cfg++) {
        if (cfg->pll_id == id && cfg->rate == rate) return cfg;
    }
    return NULL;
}

#define ASSERT_IN_RANGE(val, lo, hi) assert(val >= lo && val <= hi)
#define BITS_N_M1(n, m1)             ((pll_reg_t){ .factor_n=n, .factor_m1=m1 }).bits
#define BITS_P_N_M1_M0(p, n, m1, m0) ((pll_reg_t){ .factor_p=p, .factor_n=n, .factor_m1=m1, .factor_m0=m0 }).bits;

static void get_pll_bits(ccu_pll_id_t id, long rate, uint32_t *factor_mask, uint32_t *new_factors) {
    uint32_t out_mhz;
    const struct pll_config_t *cfg = get_pll_config_for_rate(id, rate);
    if (!cfg) error("No matching pll config found in rate table.");

    switch(cfg->pll_id) {
    case CCU_PLL_VIDEO0_CTRL_REG: // N, M1
    case CCU_PLL_VIDEO1_CTRL_REG:
        ASSERT_IN_RANGE(cfg->N, 13, 255);
        ASSERT_IN_RANGE(cfg->M1, 1, 2);
        *factor_mask = BITS_N_M1(-1,-1);
        *new_factors = BITS_N_M1(cfg->N-1,cfg->M1-1);
        return;
    case CCU_PLL_AUDIO0_CTRL_REG:   // P, N, M1, M0
        ASSERT_IN_RANGE(cfg->P, 1, 64);
        ASSERT_IN_RANGE(cfg->N, 13, 255);
        ASSERT_IN_RANGE(cfg->M1, 1, 2);
        ASSERT_IN_RANGE(cfg->M0, 1, 2);
        out_mhz = (cfg->N/cfg->M0/cfg->M1)*24;
        ASSERT_IN_RANGE(out_mhz, 180, 3000); // valid output freq range is 180M-3G
        *factor_mask = BITS_P_N_M1_M0(-1,-1,-1,-1);
        *new_factors = BITS_P_N_M1_M0(cfg->P-1,cfg->N-1,cfg->M1-1,cfg->M0-1);
        return;
    case CCU_PLL_AUDIO1_CTRL_REG: // N, M1
        ASSERT_IN_RANGE(cfg->N, 13, 255);
        ASSERT_IN_RANGE(cfg->M1, 1, 2);
        out_mhz = (cfg->N/cfg->M1)*24;
        ASSERT_IN_RANGE(out_mhz, 180, 3500); // valid output freq range is 180M-3.5G
        *factor_mask = BITS_N_M1(-1,-1);
        *new_factors = BITS_N_M1(cfg->N-1,cfg->M1-1);
        return;

    case CCU_PLL_PERI_CTRL_REG:
    case CCU_PLL_CPU_CTRL_REG:
    case CCU_PLL_DDR_CTRL_REG:
    case CCU_PLL_VE_CTRL_REG:
        error("Attempt to change PLL that should not be modified.");
    default:
        error("Invalid PLL id");
    }
}

// Procedure to update PLL from p46 of D-1 user manual
// NOTE: code explicitly sets and clears bits using bitwise ops!
// This ensures it acts exactly as required in spec
// (not using bitfields, gcc-generated code can vary)
static void update_pll_bits(volatile uint32_t *reg, uint32_t factor_mask, uint32_t factor_bits) {
    const uint32_t PLL_ENA =  (1 << 31);
    const uint32_t LOCK_ENA = (1 << 29);
    const uint32_t LOCKED =   (1 << 28);
    const uint32_t OUT_ENA =  (1 << 27);

    *reg |= PLL_ENA;            // enable PLL
    *reg &= ~OUT_ENA;           // disable output while changing
    *reg = (*reg & ~factor_mask) | factor_bits;   // clear previous factors & apply new
    *reg |= LOCK_ENA;           // lock enable
    while (! (*reg & LOCKED))   // wait until lock
       ;
    timer_delay_us(50);         // short delay to stabilize
    *reg |= OUT_ENA;            // re-enable output
}

long ccu_config_pll_rate(ccu_pll_id_t id, long rate) {
    validate_pll(id);
    uint32_t factor_mask, new_factors;
    get_pll_bits(id, rate, &factor_mask, &new_factors);
    update_pll_bits(reg_for_id(id), factor_mask, new_factors);
    long set_rate = debug_rate_pll(id);
    assert(rate == set_rate);
    return set_rate;
}

static uint32_t get_module_clk_bits(ccu_module_id_t id, ccu_parent_id_t parent, long rate) {
    int src = get_parent_src_index(id, parent);
    if (src == -1) error("Parent id is not valid for module clock")
    long parent_rate = debug_rate_parent(parent);
    module_clk_reg_t new_settings = { .src= src, .factor_n= 0, .factor_m= 0 };

    if (parent_rate == rate) { // no dividers needed, src parent at desired rate
        return new_settings.bits;
    }
    struct debug_info *info = info_for_id(id);
    int n_exp_max = (1 << info->ncount) - 1;
    int m_max = (1 << info->mcount);
    int divisor_max = m_max * (1 << n_exp_max);
    assert(parent_rate >= rate);
    assert(parent_rate % rate == 0);
    ASSERT_IN_RANGE(parent_rate/rate, 1, divisor_max);
    int divider_needed = parent_rate/rate;
    for (int exp = n_exp_max; exp >= 0; exp--) {
        int power_of_two = 1 << exp;
        if (divider_needed % power_of_two == 0 && divider_needed/power_of_two <= m_max) {
            new_settings.factor_n = exp;
            new_settings.factor_m = divider_needed/power_of_two - 1;
            //printf("Choosing factors n %d m %d to reach rate %ld\n", new_settings.factor_n,new_settings.factor_m, rate);
            return new_settings.bits;
        }
    }
    error("No compatible factors between parent and module rate.");
}

/* From p47 D-1 user manual:
 * Configure the clock source and frequency division factor first,
 * and then release the clock gating (that is, set enable to 1)
 */
static void update_clock_bits(volatile uint32_t *reg, uint32_t bits) {
    const uint32_t ENA = (1 << 31);
    *reg &= ~ENA;   // disable clock during change
    *reg = bits;
    *reg |= ENA;    // re-enable
}

long ccu_config_module_clock_rate(ccu_module_id_t id, ccu_parent_id_t parent, long rate) {
    validate_module_clk(id);
    uint32_t new_bits = get_module_clk_bits(id, parent, rate);
    update_clock_bits(reg_for_id(id), new_bits);
    long set_rate = debug_rate_clk(id);
    assert(rate == set_rate);
    return set_rate;
}

/*
 *  From p47 D-1 user manual:
 *  For the Bus Gating Reset register of a module, the reset bit is de-asserted first,
 *  and then the clock gating bit is enabled to avoid potential problems
 *  caused by the asynchronous release of the reset signal.
 */
long ccu_ungate_bus_clock_bits(ccu_bgr_id_t id, uint32_t gating_bits, uint32_t reset_bits) {
    validate_bgr(id);
    volatile uint32_t *reg = reg_for_id(id);
    *reg |= reset_bits;      // de-assert reset
    *reg |= gating_bits;     // enable
    return debug_rate_bgr(id);
}

// most bus clocks use standard bits for reset/gate
// general function above allow other use cases
long ccu_ungate_bus_clock(ccu_bgr_id_t id) {
    const uint32_t standard_gating_bits = 1 << 0;
    const uint32_t standard_reset_bits  = 1 << 16;
    return ccu_ungate_bus_clock_bits(id, standard_gating_bits, standard_reset_bits);
}

/****  DEBUG INFO from here down ***/



#define STRINGIFY(x) #x
#define INFO_PLL(x) debug_rate_pll, x, STRINGIFY(x)
#define INFO_CLK(x) debug_rate_clk, x, STRINGIFY(x)
#define INFO_BGR(x) debug_rate_bgr, x, STRINGIFY(x)

#define NOT_IN_MODEL PARENT_NONE

static struct debug_info info_table[] = {
    { .name= "PLL" },
    { INFO_PLL(CCU_PLL_CPU_CTRL_REG)    },
    { INFO_PLL(CCU_PLL_DDR_CTRL_REG)    },
    { INFO_PLL(CCU_PLL_PERI_CTRL_REG)   },
    { INFO_PLL(CCU_PLL_VIDEO0_CTRL_REG) },
    { INFO_PLL(CCU_PLL_VIDEO1_CTRL_REG) },
    { INFO_PLL(CCU_PLL_VE_CTRL_REG)     },
    { INFO_PLL(CCU_PLL_AUDIO0_CTRL_REG) },
    { INFO_PLL(CCU_PLL_AUDIO1_CTRL_REG) },
    { .name= "Module Clock" },       // parent not listed defaults to NOT_IN_MODEL
    { INFO_CLK(CCU_PSI_CLK_REG),      {PARENT_HOSC, PARENT_32K, NOT_IN_MODEL, PARENT_PERI}, .ncount=2,.mcount=2 },
    { INFO_CLK(CCU_APB0_CLK_REG),     {PARENT_HOSC, PARENT_32K, PARENT_PSI, PARENT_PERI}, .ncount=2,.mcount=5 },
    { INFO_CLK(CCU_APB1_CLK_REG),     {PARENT_HOSC, PARENT_32K, PARENT_PSI, PARENT_PERI}, .ncount=2,.mcount=5  },
    { INFO_CLK(CCU_DRAM_CLK_REG),     {PARENT_DDR, NOT_IN_MODEL, PARENT_PERI_2X}, .ncount=2,.mcount=2 },
    { INFO_CLK(CCU_DE_CLK_REG),       {PARENT_PERI_2X, PARENT_VIDEO0_4X, PARENT_VIDEO1_4X}, .ncount=0,.mcount=5 },
    { INFO_CLK(CCU_TCONTV_CLK_REG),   {PARENT_VIDEO0, PARENT_VIDEO0_4X}, .ncount=2,.mcount=4 },
    { INFO_CLK(CCU_HDMI_24M_CLK_REG), {PARENT_HOSC}, .ncount=0,.mcount=0  },
    { INFO_CLK(CCU_SPI0_CLK_REG),     {PARENT_HOSC, PARENT_PERI, PARENT_PERI_2X}, .ncount=2,.mcount=4 },
    { INFO_CLK(CCU_SPI1_CLK_REG),     {PARENT_HOSC, PARENT_PERI, PARENT_PERI_2X}, .ncount=2,.mcount=4 },
    { INFO_CLK(CCU_I2S2_CLK_REG),     {PARENT_AUDIO0, NOT_IN_MODEL, NOT_IN_MODEL, PARENT_AUDIO1_DIV5}, .ncount=0,.mcount=5},
    { INFO_CLK(CCU_LEDC_CLK_REG),     {PARENT_HOSC, PARENT_PERI, NOT_IN_MODEL, NOT_IN_MODEL}, .ncount=2,.mcount=4},
    { .name= "Bus Clock" },
    { INFO_BGR(CCU_DE_BGR_REG),       {PARENT_AHB0} },
    { INFO_BGR(CCU_DPSS_TOP_BGR_REG), {PARENT_AHB0} },
    { INFO_BGR(CCU_HDMI_BGR_REG),     {PARENT_AHB0} },
    { INFO_BGR(CCU_TCONTV_BGR_REG),   {PARENT_AHB0} },
    { INFO_BGR(CCU_DMA_BGR_REG),      {PARENT_AHB0} },
    { INFO_BGR(CCU_HSTIMER_BGR_REG),  {PARENT_AHB0} },
    { INFO_BGR(CCU_PWM_BGR_REG),      {PARENT_APB0} },
    { INFO_BGR(CCU_UART_BGR_REG),     {PARENT_APB1} },
    { INFO_BGR(CCU_I2S_BGR_REG),      {PARENT_APB0} },
    { INFO_BGR(CCU_TWI_BGR_REG),      {PARENT_APB1} },
    { INFO_BGR(CCU_SPI_BGR_REG),      {PARENT_APB1} },
    { INFO_BGR(CCU_LEDC_BGR_REG),     {PARENT_APB1} },
    {0},
  };

static int get_parent_src_index(ccu_module_id_t id, ccu_parent_id_t parent) {
    for (struct debug_info *info = info_table; info->name; info++) {
        if (info->reg_id == id) {
            for (int i = 0; i < sizeof(info->parents)/sizeof(*info->parents); i++) {
                if (info->parents[i] == parent) return i;
            }
        }
    }
    return -1;
}
static struct debug_info *info_for_id(uint32_t id) {
    for (struct debug_info *info = info_table; info->name; info++) {
        if (!info->fn) continue;
        if (info->reg_id == id) {
            return info;
        }
    }
    return NULL;
}

void ccu_debug_show_clocks(const char *label) {
    printf("\n++++++++ CCU clock debug (%s) ++++++++\n", label);
    for (struct debug_info *i = info_table; i->name; i++) {
        if (!i->fn) {
            printf("\n        Rate  %s\n", i->name);
            continue;
        }
        long rate = i->fn(i->reg_id);
        if (rate != 0) printf("%12ld  %s\t raw=[%08x]\n", rate, i->name, *reg_for_id(i->reg_id));
    }
}

static long debug_rate_parent(ccu_parent_id_t id) {
    int mult = 1, div = 1;
    switch (id) {
        case NOT_IN_MODEL:      return -1;
        case PARENT_HOSC:       return 24*1000*1000;
        case PARENT_32K:        return 32768;
        case PARENT_DDR:        return debug_rate_pll(CCU_PLL_DDR_CTRL_REG);
        case PARENT_PERI_2X:           mult = 2; // *** fallthrough
        case PARENT_PERI:       return mult*debug_rate_pll(CCU_PLL_PERI_CTRL_REG);
        case PARENT_VIDEO0_4X:         mult = 4; // *** fallthrough
        case PARENT_VIDEO0:     return mult*debug_rate_pll(CCU_PLL_VIDEO0_CTRL_REG);
        case PARENT_VIDEO1_4X:         mult = 4; // *** fallthrough
        case PARENT_VIDEO1:     return mult*debug_rate_pll(CCU_PLL_VIDEO1_CTRL_REG);
        case PARENT_AUDIO0:     return debug_rate_pll(CCU_PLL_AUDIO0_CTRL_REG);
        case PARENT_AUDIO1_DIV5:       div = 5; // *** fallthrough
        case PARENT_AUDIO1:     return debug_rate_pll(CCU_PLL_AUDIO1_CTRL_REG) / div;
        case PARENT_APB0:       return debug_rate_clk(CCU_APB0_CLK_REG);
        case PARENT_APB1:       return debug_rate_clk(CCU_APB1_CLK_REG);
        case PARENT_AHB0:       return debug_rate_clk(CCU_PSI_CLK_REG);
        case PARENT_PSI:        return debug_rate_clk(CCU_PSI_CLK_REG);
    }
    return -1;
}

static long debug_rate_pll(ccu_pll_id_t id) {
    pll_reg_t pll;
    pll.bits = *reg_for_id(id);
    if (!pll.ena || !pll.output_ena) return 0;
    int p = pll.factor_p+1, n = pll.factor_n+1, m1 = pll.factor_m1+1, m0 = pll.factor_m0+1;
    long parent_rate = debug_rate_parent(PARENT_HOSC);

    switch (id) {
        case CCU_PLL_PERI_CTRL_REG:
        case CCU_PLL_VIDEO0_CTRL_REG:
        case CCU_PLL_VIDEO1_CTRL_REG:
            return parent_rate*n/m1/4;
        case CCU_PLL_AUDIO0_CTRL_REG:
            return parent_rate*n/m1/m0/p/4;
        default:
            return parent_rate*n/m1/m0;
    }
}

static long debug_rate_clk(ccu_module_id_t id) {
    module_clk_reg_t clk;
    clk.bits = *reg_for_id(id);
    if (id > CCU_APB1_CLK_REG && !clk.ena) return 0; // cheezy (ena bits not applicable for psi/apb?)
    int n = 1 << clk.factor_n;
    int m = clk.factor_m + 1;
    struct debug_info *i = info_for_id(id);
    ccu_parent_id_t parent = i->parents[clk.src];
    return debug_rate_parent(parent)/n/m;
}

static long debug_rate_bgr(ccu_bgr_id_t id) {
    uint32_t val = *reg_for_id(id);
    struct debug_info *i = info_for_id(id);
    return (val & 0xff) ? debug_rate_parent(i->parents[0]) : 0;
}

static void validate_pll(ccu_pll_id_t id) {
    struct debug_info *info = info_for_id(id);
    if (!info || info->fn != debug_rate_pll) error("PLL id is not valid");
}

static void validate_module_clk(ccu_module_id_t id) {
    struct debug_info *info = info_for_id(id);
    if (!info || info->fn != debug_rate_clk) error("Module clock id is not valid");
}

static void validate_bgr(ccu_bgr_id_t id) {
    struct debug_info *info = info_for_id(id);
    if (!info || info->fn != debug_rate_bgr) error("Bus clock id is not valid");
}

