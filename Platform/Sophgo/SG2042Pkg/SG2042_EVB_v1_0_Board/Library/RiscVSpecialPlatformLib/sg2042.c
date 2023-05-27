/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c)
 *
 * Authors:
 *   Inochi Amaoto <inochiama@outlook.com>
 *   YuQing Cai <caiyuqing_hz@163.com>
 *   ZhenYu Zhang <1204122531@qq.com>
 */

#include <Library/RiscVSpecialPlatformLib.h>
#include <thead_c9xx.h>
#include <sbi/riscv_asm.h>
#include <sbi/riscv_encoding.h>
#include <sbi/sbi_const.h>
#include <sbi/sbi_ecall_interface.h>
#include <sbi/sbi_pmu.h>
#include <sbi/sbi_domain.h>
#include <sbi/sbi_platform.h>
#include <sbi_utils/fdt/fdt_helper.h>
#include <sbi_utils/timer/aclint_mtimer.h>

#define SOPHGO_MANGO_TIMER_BASE		0x70ac000000UL
#define SOPHGO_MANGO_TIMER_OFFSET	0x10000UL

int mango_early_init(bool cold_boot, const struct fdt_match *match)
{
	const struct sbi_platform * plat = sbi_platform_thishart_ptr();

	/*
	 * Sophgo mango board use seperate 16/32 timers while initiating,
	 * merge them as a single domain to avoid wasting.
	 */
	if (cold_boot)
		return sbi_domain_root_add_memrange(SOPHGO_MANGO_TIMER_BASE,
						    SOPHGO_MANGO_TIMER_OFFSET *
						    sbi_platform_hart_count(plat),
						    MTIMER_REGION_ALIGN,
						    (SBI_DOMAIN_MEMREGION_MMIO |
						     SBI_DOMAIN_MEMREGION_M_READABLE |
						     SBI_DOMAIN_MEMREGION_M_WRITABLE));
	return 0;
}

static void thead_c9xx_pmu_ctr_enable_irq(uint32_t ctr_idx)
{
	unsigned long mip_val;

	if (ctr_idx >= SBI_PMU_HW_CTR_MAX)
		return;

	mip_val = csr_read(CSR_MIP);
	/**
	 * Clear out the OF bit so that next interrupt can be enabled.
	 * This should be done only when the corresponding overflow interrupt
	 * bit is cleared. That indicates that software has already handled the
	 * previous interrupts or the hardware yet to set an overflow interrupt.
	 * Otherwise, there will be race conditions where we may clear the bit
	 * the software is yet to handle the interrupt.
	 */
	if (!(mip_val & THEAD_C9XX_MIP_MOIP))
		csr_clear(THEAD_C9XX_CSR_MCOUNTEROF, BIT(ctr_idx));

	/**
	 * SSCOFPMF uses the OF bit for enabling/disabling the interrupt,
	 * while the C9XX has designated enable bits.
	 * So enable per-counter interrupt on C9xx here.
	 */
	csr_set(THEAD_C9XX_CSR_MCOUNTERINTEN, BIT(ctr_idx));
}

static void thead_c9xx_pmu_ctr_disable_irq(uint32_t ctr_idx)
{
	csr_clear(THEAD_C9XX_CSR_MCOUNTERINTEN, BIT(ctr_idx));
}

static int thead_c9xx_pmu_irq_bit(void)
{
	return THEAD_C9XX_MIP_MOIP;
}

const struct sbi_pmu_device thead_c9xx_pmu_device = {
	.hw_counter_enable_irq = thead_c9xx_pmu_ctr_enable_irq,
	.hw_counter_disable_irq = thead_c9xx_pmu_ctr_disable_irq,
	.hw_counter_irq_bit = thead_c9xx_pmu_irq_bit,
};

static int mango_extensions_init(const struct fdt_match *match,
				     struct sbi_hart_features *hfeatures)
{
	sbi_pmu_set_device(&thead_c9xx_pmu_device);

	/* auto-detection doesn't work on t-head c9xx cores */
	hfeatures->mhpm_count = 29;
	hfeatures->mhpm_bits = 64;

	return 0;
}

static const struct fdt_match sophgo_mango_match[] = {
	{ .compatible = "sophgo,mango" },
	{ },
};

const struct platform_override sophgo_mango = {
	.match_table		= sophgo_mango_match,
	.early_init		= mango_early_init,
	.extensions_init	= mango_extensions_init,
};

const struct platform_override *special_platforms[] = {
  &sophgo_mango,
};
INTN NumberOfPlaformsInArray = array_size(special_platforms);