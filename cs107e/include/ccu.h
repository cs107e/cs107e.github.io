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
 * Type: `ccu_module_id_t`, `ccu_bgr_id_t`, `ccu_pll_id_t`
 *
 * Type used to identify a ccu clock register by id (offset).
 * We list only the ids of those ccu registers that we are using.
 */

typedef enum { // ids for Module Clock registers
    CCU_PSI_CLK_REG         = 0x0510,
    CCU_APB0_CLK_REG        = 0x0520,
    CCU_APB1_CLK_REG        = 0x0524,
    CCU_MBUS_CLK_REG        = 0x0540,
    CCU_DE_CLK_REG          = 0x0600,  // Display engine
    CCU_DRAM_CLK_REG        = 0x0800,
    CCU_MBUS_MCG_REG        = 0x0804, // mbus master clock gating
    CCU_HDMI_24M_CLK_REG    = 0x0B04,
    CCU_TCONTV_CLK_REG      = 0x0B80,
    CCU_SPI0_CLK_REG        = 0x0940,
    CCU_SPI1_CLK_REG        = 0x0944,
    CCU_I2S0_CLK_REG        = 0x0A10,
    CCU_I2S1_CLK_REG        = 0x0A14,
    CCU_I2S2_CLK_REG        = 0x0A18,
    CCU_LEDC_CLK_REG        = 0x0BF0,
    CCU_SMHC0_CLK_REG       = 0x0830,
    CCU_SMHC1_CLK_REG       = 0x0834,
    CCU_SMHC2_CLK_REG       = 0x0838,
} ccu_module_id_t;

typedef enum {  // ids for Bus Gating Reset registers
    CCU_DE_BGR_REG          = 0x060C,  // Display engine
    CCU_DMA_BGR_REG         = 0x070C,
    CCU_HSTIMER_BGR_REG     = 0x073C,
    CCU_PWM_BGR_REG         = 0x07AC,
    CCU_DRAM_BGR_REG        = 0x080C,
    CCU_UART_BGR_REG        = 0x090C,
    CCU_TWI_BGR_REG         = 0x091C,  // TWI == I2C
    CCU_SPI_BGR_REG         = 0x096C,
    CCU_I2S_BGR_REG         = 0x0A20,
    CCU_DPSS_TOP_BGR_REG    = 0x0ABC,  // TCON TOP
    CCU_HDMI_BGR_REG        = 0x0B1C,
    CCU_TCONTV_BGR_REG      = 0x0B9C,
    CCU_LEDC_BGR_REG        = 0x0BFC,
    CCU_SMHC_BGR_REG        = 0x084C,
} ccu_bgr_id_t;

typedef enum { // ids for PLL registers
    CCU_PLL_CPU_CTRL_REG    = 0x0000,
    CCU_PLL_DDR_CTRL_REG    = 0x0010,
    CCU_PLL_PERI_CTRL_REG   = 0x0020,
    CCU_PLL_VIDEO0_CTRL_REG = 0x0040,
    CCU_PLL_VIDEO1_CTRL_REG = 0x0048,
    CCU_PLL_VE_CTRL_REG     = 0x0058,
    CCU_PLL_AUDIO0_CTRL_REG = 0x0078,
    CCU_PLL_AUDIO1_CTRL_REG = 0x0080,
} ccu_pll_id_t;

/*
 * Type: `ccu_parent_id_t`
 *
 * Enum type used to identify a clock's parent clock.
 */
typedef enum {
    PARENT_NONE = 0,
    PARENT_HOSC = 1,
    PARENT_32K,
    PARENT_DDR,
    PARENT_DDR_DIV4,
    PARENT_PERI,
    PARENT_PERI_2X,
    PARENT_VIDEO0,
    PARENT_VIDEO0_4X,
    PARENT_VIDEO1,
    PARENT_VIDEO1_4X,
    PARENT_AUDIO0,
    PARENT_AUDIO1,
    PARENT_AUDIO1_DIV5,
    PARENT_AHB0,
    PARENT_APB0,
    PARENT_APB1,
    PARENT_PSI,
    PARENT_MBUS,
} ccu_parent_id_t;

/*
 * `ccu_config_pll_rate`: Update settings and enable pll
 *
 * Update settings of pll to match requested rate.
 * Will disable PLL output, apply new factors,
 * reacquire lock, wait for stable, then reenable output
 *
 * @param id       id of pll from pll enum above
 * @param rate     desired rate in hz
 * @return         pll rate of new configuration
 */
long ccu_config_pll_rate(ccu_pll_id_t id, long rate);

/*
 * `ccu_config_module_clock_rate`: Update settings and enable module clock
 *
 * Update settings to use parent src and and rate, enable clock.
 *
 * @param id        id of module clock from module_clk enum above
 * @param src       id of parent clock from parent_id enum above
 * @param rate      desired rate in hz
 * @return          rate of module clock
 */
long ccu_config_module_clock_rate(ccu_module_id_t id, ccu_parent_id_t parent, long rate);

/*
 * `ccu_ungate_bus_clock`: Ungate bus clock
 *
 * Reset/enable bus clock. (Alternate version allows control of which gating/reset bits to use,
 * otherwise uses default bit 0 and bit 16)
 *
 * @param id    id of bgr from bgr enum above
 * @return      rate of bus clock
 */
long ccu_ungate_bus_clock(ccu_bgr_id_t reg_id);
long ccu_ungate_bus_clock_bits(ccu_bgr_id_t reg_id, uint32_t gating_bits, uint32_t reset_bits);

// oddball for mbus clock
void ccu_ungate_mbus_bits(uint32_t gating_bits);

// for debugging purposes
void ccu_debug_show_clocks(const char *label);

#endif
