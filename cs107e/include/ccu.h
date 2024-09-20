#ifndef CCU_H__
#define CCU_H__

/*
 * Functions for interacting with the D1 clocks.
 *
 * Author: Julie Zelenski <zelenski@cs.stanford.edu>
 */
#include <stdbool.h>
#include <stdint.h>

/*
 * Type: `module_clk_id_t`, `bgr_id_t`, `pll_id_t`
 *
 * Type used to identify a ccu register by id (offset).
 * We list only the ids of those ccu registers that we are using.
 */

typedef enum { // ids for Module Clock registers
    CCU_PSI_CLK_REG         = 0x0510,
    CCU_APB0_CLK_REG        = 0x0520,
    CCU_APB1_CLK_REG        = 0x0524,
    CCU_DE_CLK_REG          = 0x0600,  // Display engine
    CCU_DRAM_CLK_REG        = 0x0800,
    CCU_HDMI_24M_CLK_REG    = 0x0B04,
    CCU_TCONTV_CLK_REG      = 0x0B80,
    CCU_I2S0_CLK_REG        = 0x0A10,
    CCU_I2S1_CLK_REG        = 0x0A14,
    CCU_I2S2_CLK_REG        = 0x0A18,
    CCU_SPI0_CLK_REG        = 0x0940,
    CCU_SPI1_CLK_REG        = 0x0944,
} module_clk_id_t;

typedef enum {  // ids for Bus Gating Reset registers
    CCU_DE_BGR_REG          = 0x060C,  // Display engine
    CCU_DPSS_TOP_BGR_REG    = 0x0ABC,  // TCON TOP
    CCU_HDMI_BGR_REG        = 0x0B1C,
    CCU_TCONTV_BGR_REG      = 0x0B9C,
    CCU_DMA_BGR_REG         = 0x070C,
    CCU_HSTIMER_BGR_REG     = 0x073C,
    CCU_PWM_BGR_REG         = 0x07AC,
    CCU_DRAM_BGR_REG        = 0x080C,
    CCU_UART_BGR_REG        = 0x090C,
    CCU_I2S_BGR_REG         = 0x0A20,
    CCU_TWI_BGR_REG         = 0x091C,  // TWI == I2C
    CCU_SPI_BGR_REG         = 0x096C,
} bgr_id_t;

typedef enum { // ids for PLL registers
    CCU_PLL_CPU_CTRL_REG    = 0x0000,
    CCU_PLL_DDR_CTRL_REG    = 0x0010,
    CCU_PLL_PERI_CTRL_REG   = 0x0020,
    CCU_PLL_VIDEO0_CTRL_REG = 0x0040,
    CCU_PLL_VIDEO1_CTRL_REG = 0x0048,
    CCU_PLL_VE_CTRL_REG     = 0x0058,
    CCU_PLL_AUDIO0_CTRL_REG = 0x0078,
    CCU_PLL_AUDIO1_CTRL_REG = 0x0080,
} pll_id_t;


/*
 * `ccu_config_pll_dividers`: Update settings and enable pll
 *
 * Update settings of pll to match requested dividers.
 * Will disable output on PLL, apply new factors,
 * reacquire lock, wait for stable, then reenable output
 * (special case for PLL AUDIO0 to config additional p divider)
 *
 * @param id       id of pll from pll enum above
 * @param n,m,p    dividers (refer to D-1 manual for info on dividers)
 * @return         pll rate of new configuration
 */
long ccu_config_pll_dividers(pll_id_t id, uint32_t n, uint32_t m);
long ccu_config_pll_audio0(uint32_t n, uint32_t p, uint32_t m1, uint32_t m0);

/*
 * `ccu_config_module_clock`: Update settings and enable module clock
 *
 * Update settings to use parent src and given factors, enable clock.
 * Pass 0 for unused src and/or factors.
 *
 * @param id        id of module clock from clk enum above
 * @param src       parent clock (refer to manual for appropriate settings for this clock)
 * @param factor_   factors (refer to manual for appropriate settings for this clock)
 * @return          rate of module clock
 */
long ccu_config_module_clock(module_clk_id_t id, uint32_t src, uint32_t factor_n, uint32_t factor_m);

/*
 * `ccu_ungate_bus_clock`: Ungate bus clock
 *
 * Enable bus clock.
 *
 * @param id    id of bgr from bgr enum above
 * @return      rate of bus clock
 */
long ccu_ungate_bus_clock(bgr_id_t reg_id);
long ccu_ungate_bus_clock_bits(bgr_id_t reg_id, uint32_t gating_bits, uint32_t reset_bits);

// for debugging purposes
void ccu_debug_show_clocks(const char *label);

#endif
