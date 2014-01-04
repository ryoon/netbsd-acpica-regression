/* $NetBSD: omap2_reg.h,v 1.23 2013/06/20 05:27:31 matt Exp $ */

/*
 * Copyright (c) 2007 Microsoft
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Microsoft
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTERS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _ARM_OMAP_OMAP2_REG_H_
#define _ARM_OMAP_OMAP2_REG_H_

#include "opt_omap.h"

/*
 * Header for misc. omap2/3/4 registers
 */

/*
 * L4 Interconnect WAKEUP address space
 */
#define OMAP2430_L4_CORE_BASE		0x48000000
#define OMAP2430_L4_CORE_SIZE		(16 << 20)	/* 16 MB */

#define OMAP2430_L4_WAKEUP_BASE		0x49000000
#define OMAP2430_L4_WAKEUP_SIZE		(8 << 20)	/* 8 MB */

#define OMAP3430_L4_CORE_BASE		0x48000000
#define OMAP3430_L4_CORE_SIZE		0x01000000	/* 16 MB */

#define OMAP3530_L4_CORE_BASE		0x48000000
#define OMAP3530_L4_CORE_SIZE		0x01000000	/* 16 MB */

/* OMAP3 processors */

#define OMAP3430_L4_WAKEUP_BASE		0x48300000
#define OMAP3430_L4_WAKEUP_SIZE		0x00040000	/* 256KB */

#define OMAP3430_L4_PERIPHERAL_BASE	0x49000000
#define OMAP3430_L4_PERIPHERAL_SIZE	0x00100000	/* 1MB */

#define OMAP3430_L4_EMULATION_BASE	0x54000000
#define OMAP3430_L4_EMULATION_SIZE	0x00800000	/* 8MB */

#define OMAP3530_L4_WAKEUP_BASE		0x48300000
#define OMAP3530_L4_WAKEUP_SIZE		0x00040000	/* 256KB */

#define OMAP3530_L4_PERIPHERAL_BASE	0x49000000
#define OMAP3530_L4_PERIPHERAL_SIZE	0x00100000	/* 1MB */

#define OMAP3530_L4_EMULATION_BASE	0x54000000
#define OMAP3530_L4_EMULATION_SIZE	0x00800000	/* 8MB */

/* OMAP4 processors */

#define OMAP4430_L4_CORE_BASE		0x4A000000
#define OMAP4430_L4_CORE_SIZE		0x01000000	/* 16MB - CFG */

#define OMAP4430_L4_WAKEUP_BASE		0x4A300000
#define OMAP4430_L4_WAKEUP_SIZE		0x00040000	/* 256KB */

#define OMAP4430_L4_PERIPHERAL_BASE	0x48000000
#define OMAP4430_L4_PERIPHERAL_SIZE	0x01000000	/* 16MB */

#define OMAP4430_L4_ABE_BASE		0x49000000	/* Actually L3 */
#define OMAP4430_L4_ABE_SIZE		0x01000000	/* 16MB */

#define OMAP4430_EMIF1_BASE		0x4C000000	/* MemCtrl 0 */
#define OMAP4430_EMIF1_SIZE		0x00100000	/* 4KB padded to 1M */

#define OMAP4430_EMIF2_BASE		0x4D000000	/* MemCtrl 1 */
#define OMAP4430_EMIF2_SIZE		0x00100000	/* 4KB padded to 1M */

/* OMAP5 processors */

#define OMAP5430_L4_CORE_BASE		0x4A000000
#define OMAP5430_L4_CORE_SIZE		0x01000000	/* 16MB - CFG */

#define OMAP5430_L4_WAKEUP_BASE		0x4AE00000
#define OMAP5430_L4_WAKEUP_SIZE		0x00200000	/* 2M */

#define OMAP5430_L4_PERIPHERAL_BASE	0x48000000
#define OMAP5430_L4_PERIPHERAL_SIZE	0x01000000	/* 16MB */

#define OMAP5430_L4_ABE_BASE		0x49000000	/* Actually L3 */
#define OMAP5430_L4_ABE_SIZE		0x01000000	/* 16MB */

#define OMAP5430_EMIF1_BASE		0x4C000000	/* MemCtrl 0 */
#define OMAP5430_EMIF1_SIZE		0x00100000	/* 4KB padded to 1M */

#define OMAP5430_EMIF2_BASE		0x4D000000	/* MemCtrl 1 */
#define OMAP5430_EMIF2_SIZE		0x00100000	/* 4KB padded to 1M */

/* TI Sitara AM335x (OMAP like) */

#define TI_AM335X_L4_WAKEUP_BASE	0x44C00000
#define TI_AM335X_L4_WAKEUP_SIZE	0x00400000	/* 4MB */

#define TI_AM335X_L4_PERIPHERAL_BASE	0x48000000
#define TI_AM335X_L4_PERIPHERAL_SIZE	0x01000000	/* 16MB */

#define TI_AM335X_L4_FAST_BASE		0x4A000000
#define TI_AM335X_L4_FAST_SIZE		0x01000000	/* 16MB */

#define TI_AM335X_EMIF1_BASE		0x4C000000
#define TI_AM335X_EMIF1_SIZE		0x00100000	/* 4KB pad to 1MB */

/* TI Sitara DM37xx (OMAP like) */

#define TI_DM37XX_L4_CORE_BASE		0x48000000
#define TI_DM37XX_L4_CORE_SIZE		0x01000000	/* 16MB */

#define TI_DM37XX_L4_WAKEUP_BASE	0x48300000
#define TI_DM37XX_L4_WAKEUP_SIZE	0x00010000	/* 64KB */

#define TI_DM37XX_L4_PERIPHERAL_BASE	0x49000000
#define TI_DM37XX_L4_PERIPHERAL_SIZE	0x01000000	/* 16MB */

#define TI_DM37XX_L4_EMULATION_BASE	0x54000000
#define TI_DM37XX_L4_EMULATION_SIZE	0x00800000	/* 8MB */

/*
 * Clock Management registers base, offsets, and size
 */
#ifdef OMAP_2430
#define OMAP2_CM_BASE			0x49006000
#endif
#ifdef OMAP_2420
#define OMAP2_CM_BASE			0x48008000
#endif
#ifdef OMAP_3430
#define OMAP2_CM_BASE			(OMAP3430_L4_CORE_BASE + 0x04000)
#endif
#ifdef OMAP_3530
#define OMAP2_CM_BASE			(OMAP3530_L4_CORE_BASE + 0x04000)
#endif
#ifdef OMAP_4430
#define OMAP2_CM_BASE			(OMAP4430_L4_CORE_BASE + 0x04000)
#endif
#ifdef OMAP_5430
#define OMAP2_CM_BASE			(OMAP5430_L4_CORE_BASE + 0x04000)
#endif
#ifdef TI_AM335X
#define OMAP2_CM_BASE			(TI_AM335X_L4_WAKEUP_BASE + 0x200000)
#endif
#ifdef TI_DM37XX
#define OMAP2_CM_BASE			0x48004000
#endif

#define	OMAP2_CM_CLKSEL_MPU	0x140
#define	OMAP2_CM_FCLKEN1_CORE	0x200
#define	OMAP2_CM_FCLKEN2_CORE	0x204
#define	OMAP2_CM_ICLKEN1_CORE	0x210
#define	OMAP2_CM_ICLKEN2_CORE	0x214
#define	OMAP2_CM_CLKSEL2_CORE	0x244
#define OMAP3_CM_IDLEST1_CORE	0xa20
#define	OMAP2_CM_SIZE		(0x1000)

/*
 * bit defines for OMAP2_CM_CLKSEL_MPU
 */
#define	OMAP2_CM_CLKSEL_MPU_FULLSPEED	1
#define	OMAP2_CM_CLKSEL_MPU_HALFSPEED	2

/*
 * bit defines for OMAP2_CM_FCLKEN2_CORE
 */
#define OMAP2_CM_FCLKEN1_CORE_EN_DSS1	__BIT(0)
#define OMAP2_CM_FCLKEN1_CORE_EN_DSS2	__BIT(1)
#define OMAP2_CM_FCLKEN1_CORE_EN_TV		__BIT(2)
#define OMAP2_CM_FCLKEN1_CORE_RESa	__BIT(3)
#define OMAP2_CM_FCLKEN1_CORE_EN_GPT2	__BIT(4)
#define OMAP2_CM_FCLKEN1_CORE_EN_GPT3	__BIT(5)
#define OMAP2_CM_FCLKEN1_CORE_EN_GPT4	__BIT(6)
#define OMAP2_CM_FCLKEN1_CORE_EN_GPT5	__BIT(7)
#define OMAP2_CM_FCLKEN1_CORE_EN_GPT6	__BIT(8)
#define OMAP2_CM_FCLKEN1_CORE_EN_GPT7	__BIT(9)
#define OMAP2_CM_FCLKEN1_CORE_EN_GPT8	__BIT(10)
#define OMAP2_CM_FCLKEN1_CORE_EN_GPT9	__BIT(11)
#define OMAP2_CM_FCLKEN1_CORE_EN_GPT10	__BIT(12)
#define OMAP2_CM_FCLKEN1_CORE_EN_GPT11	__BIT(13)
#define OMAP2_CM_FCLKEN1_CORE_EN_GPT12	__BIT(14)
#define OMAP2_CM_FCLKEN1_CORE_EN_MCBSP1	__BIT(15)
#define OMAP2_CM_FCLKEN1_CORE_EN_MCBSP2	__BIT(16)
#define OMAP2_CM_FCLKEN1_CORE_EN_MCSPI1	__BIT(17)
#define OMAP2_CM_FCLKEN1_CORE_EN_MCSPI2	__BIT(18)
#define OMAP2_CM_FCLKEN1_CORE_RESb	__BITS(20,19)
#define OMAP2_CM_FCLKEN1_CORE_EN_UART1	__BIT(21)
#define OMAP2_CM_FCLKEN1_CORE_EN_UART2	__BIT(22)
#define OMAP2_CM_FCLKEN1_CORE_EN_HDQ		__BIT(23)
#define OMAP2_CM_FCLKEN1_CORE_RESc	__BIT(24)
#define OMAP2_CM_FCLKEN1_CORE_EN_FAC		__BIT(25)
#define OMAP2_CM_FCLKEN1_CORE_RESd	__BIT(26)
#define OMAP2_CM_FCLKEN1_CORE_EN_MSPRO	__BIT(27)
#define OMAP2_CM_FCLKEN1_CORE_RESe	__BIT(28)
#define OMAP2_CM_FCLKEN1_CORE_EN_WDT4	__BIT(29)
#define OMAP2_CM_FCLKEN1_CORE_RESf	__BIT(30)
#define OMAP2_CM_FCLKEN1_CORE_EN_CAM		__BIT(31)
#define OMAP2_CM_FCLKEN1_CORE_RESV \
		(OMAP2_CM_FCLKEN1_CORE_RESa \
		|OMAP2_CM_FCLKEN1_CORE_RESb \
		|OMAP2_CM_FCLKEN1_CORE_RESc \
		|OMAP2_CM_FCLKEN1_CORE_RESd \
		|OMAP2_CM_FCLKEN1_CORE_RESe \
		|OMAP2_CM_FCLKEN1_CORE_RESf)


/*
 * bit defines for OMAP2_CM_FCLKEN2_CORE
 */
#define OMAP2_CM_FCLKEN2_CORE_EN_USB		__BIT(0)
#define OMAP2_CM_FCLKEN2_CORE_EN_SSI		__BIT(1)
#define OMAP2_CM_FCLKEN2_CORE_EN_UART3	__BIT(2)
#define OMAP2_CM_FCLKEN2_CORE_EN_MCBSP3	__BIT(3)
#define OMAP2_CM_FCLKEN2_CORE_EN_MCBSP4	__BIT(4)
#define OMAP2_CM_FCLKEN2_CORE_EN_MCBSP5	__BIT(5)
#define OMAP2_CM_FCLKEN2_CORE_RESa		__BIT(6)
#define OMAP2_CM_FCLKEN2_CORE_EN_MMCHS1	__BIT(7)
#define OMAP2_CM_FCLKEN2_CORE_EN_MMCHS2	__BIT(8)
#define OMAP2_CM_FCLKEN2_CORE_EN_NCSPI3	__BIT(9)
#define OMAP2_CM_FCLKEN2_CORE_EN_GPIO5	__BIT(10)
#define OMAP2_CM_FCLKEN2_CORE_RESb		__BITS(15,11)
#define OMAP2_CM_FCLKEN2_CORE_EN_MMCHSDB1	__BIT(16)
#define OMAP2_CM_FCLKEN2_CORE_EN_MMCHSDB2	__BIT(17)
#define OMAP2_CM_FCLKEN2_CORE_RESc		__BIT(18)
#define OMAP2_CM_FCLKEN2_CORE_I2CHS1		__BIT(19)
#define OMAP2_CM_FCLKEN2_CORE_I2CHS2		__BIT(20)
#define OMAP2_CM_FCLKEN2_CORE_RESd		__BITS(31,21)
#define OMAP2_CM_FCLKEN2_CORE_RESV \
		(OMAP2_CM_FCLKEN2_CORE_RESa  \
		|OMAP2_CM_FCLKEN2_CORE_RESb  \
		|OMAP2_CM_FCLKEN2_CORE_RESc  \
		|OMAP2_CM_FCLKEN2_CORE_RESd)


/*
 * bit defines for OMAP2_CM_ICLKEN1_CORE
 */
#define OMAP2_CM_ICLKEN1_CORE_EN_DSS		__BIT(0)
#define OMAP2_CM_ICLKEN1_CORE_RESa	__BITS(3,1)
#define OMAP2_CM_ICLKEN1_CORE_EN_GPT2	__BIT(4)
#define OMAP2_CM_ICLKEN1_CORE_EN_GPT3	__BIT(5)
#define OMAP2_CM_ICLKEN1_CORE_EN_GPT4	__BIT(6)
#define OMAP2_CM_ICLKEN1_CORE_EN_GPT5	__BIT(7)
#define OMAP2_CM_ICLKEN1_CORE_EN_GPT6	__BIT(8)
#define OMAP2_CM_ICLKEN1_CORE_EN_GPT7	__BIT(9)
#define OMAP2_CM_ICLKEN1_CORE_EN_GPT8	__BIT(10)
#define OMAP2_CM_ICLKEN1_CORE_EN_GPT9	__BIT(11)
#define OMAP2_CM_ICLKEN1_CORE_EN_GPT10	__BIT(12)
#define OMAP2_CM_ICLKEN1_CORE_EN_GPT11	__BIT(13)
#define OMAP2_CM_ICLKEN1_CORE_EN_GPT12	__BIT(14)
#define OMAP2_CM_ICLKEN1_CORE_EN_MCBSP1	__BIT(15)
#define OMAP2_CM_ICLKEN1_CORE_EN_MCBSP2	__BIT(16)
#define OMAP2_CM_ICLKEN1_CORE_EN_MCSPI1	__BIT(17)
#define OMAP2_CM_ICLKEN1_CORE_EN_MCSPI2	__BIT(18)
#define OMAP2_CM_ICLKEN1_CORE_EN_I2C1	__BIT(19)
#define OMAP2_CM_ICLKEN1_CORE_EN_I2C2	__BIT(20)
#define OMAP2_CM_ICLKEN1_CORE_EN_UART1	__BIT(21)
#define OMAP2_CM_ICLKEN1_CORE_EN_UART2	__BIT(22)
#define OMAP2_CM_ICLKEN1_CORE_EN_HDQ		__BIT(23)
#define OMAP2_CM_ICLKEN1_CORE_RESb	__BIT(24)
#define OMAP2_CM_ICLKEN1_CORE_EN_FAC		__BIT(25)
#define OMAP2_CM_ICLKEN1_CORE_RESc	__BIT(26)
#define OMAP2_CM_ICLKEN1_CORE_EN_MSPR0	__BIT(27)
#define OMAP2_CM_ICLKEN1_CORE_RESd	__BIT(28)
#define OMAP2_CM_ICLKEN1_CORE_EN_WDT4	__BIT(29)
#define OMAP2_CM_ICLKEN1_CORE_EN_MAILBOXES	__BIT(30)
#define OMAP2_CM_ICLKEN1_CORE_EN_CAM		__BIT(31)
#define OMAP2_CM_ICLKEN1_CORE_RESV \
		(OMAP2_CM_ICLKEN1_CORE_RESa \
		|OMAP2_CM_ICLKEN1_CORE_RESb \
		|OMAP2_CM_ICLKEN1_CORE_RESc \
		|OMAP2_CM_ICLKEN1_CORE_RESd)


/*
 * bit defines for OMAP2_CM_ICLKEN2_CORE
 */
#define OMAP2_CM_ICLKEN2_CORE_EN_USB		__BIT(0)
#define OMAP2_CM_ICLKEN2_CORE_EN_SSI		__BIT(1)
#define OMAP2_CM_ICLKEN2_CORE_EN_UART3	__BIT(2)
#define OMAP2_CM_ICLKEN2_CORE_EN_MCBSP3	__BIT(3)
#define OMAP2_CM_ICLKEN2_CORE_EN_MCBSP4	__BIT(4)
#define OMAP2_CM_ICLKEN2_CORE_EN_MCBSP5	__BIT(5)
#define OMAP2_CM_ICLKEN2_CORE_EN_USBHS	__BIT(6)
#define OMAP2_CM_ICLKEN2_CORE_EN_MMCHS1	__BIT(7)
#define OMAP2_CM_ICLKEN2_CORE_EN_MMCHS2	__BIT(8)
#define OMAP2_CM_ICLKEN2_CORE_EN_NCSPI3	__BIT(9)
#define OMAP2_CM_ICLKEN2_CORE_EN_GPIO5	__BIT(10)
#define OMAP2_CM_ICLKEN2_CORE_EN_MDM_INTC	__BIT(11)
#define OMAP2_CM_ICLKEN2_CORE_RESV		__BIT(31,12)

/*
 * bit defines for OMAP2_CM_CLKSEL2_CORE
 */
#define OMAP2_CM_CLKSEL2_CORE_GPTn(n, v) \
		(((v) & 0x3) << (2 + ((((n) - 2) << 1))))
# define CLKSEL2_CORE_GPT_FUNC_32K_CLK		0x0
# define CLKSEL2_CORE_GPT_SYS_CLK		0x1
# define CLKSEL2_CORE_GPT_ALT_CLK		0x2
# define CLKSEL2_CORE_GPT_ALT_RESV		0x3

#define OMAP2_CM_CLKSEL2_CORE_RESa	__BITS(1,0)
#define OMAP2_CM_CLKSEL2_CORE_RESb	__BITS(31,24)
#define OMAP2_CM_CLKSEL2_CORE_RESV \
		(OMAP2_CM_CLKSEL2_CORE_RESa \
		|OMAP2_CM_CLKSEL2_CORE_RESb)


#define	OMAP3_CM_CLKSEL1_PLL_MPU	0x940
#define	OMAP3_CM_CLKSEL2_PLL_MPU	0x944

#define	OMAP3_CM_CLKSEL1_PLL_MPU_CLK_SRC	__BITS(21,19)
#define	OMAP3_CM_CLKSEL1_PLL_MPU_DPLL_MULT	__BITS(18,8)
#define	OMAP3_CM_CLKSEL1_PLL_MPU_DPLL_DIV	__BITS(6,0)

#define	OMAP3_CM_CLKSEL2_PLL_MPU_DPLL_CLKOUT_DIV	__BITS(4,0)

#define	OMAP4_CM_SYS_CLKSEL		0x110
#define	OMAP4_CM_CLKSEL_DPLL_MPU	0x16c
#define	OMAP4_CM_DIV_M2_DPLL_MPU	0x170

#define	OMAP4_CM_SYS_CLKSEL_CLKIN	__BITS(2,0)
#define OMAP4_CM_CLKSEL_FREQS	{ 0, 12000, 13000, 16800, 19200, 26000, 27000, 38400 }
#define	OMAP4_CM_CLKSEL_MULT	1000

#define	OMAP4_CM_CLKSEL_DPLL_MPU_DCC_EN		__BIT(22)
#define	OMAP4_CM_CLKSEL_DPLL_MPU_DPLL_MULT	__BITS(18,8)
#define	OMAP4_CM_CLKSEL_DPLL_MPU_DPLL_DIV	__BITS(6,0)

#define	OMAP4_CM_DIV_M2_DPLL_MPU_DPLL_CLKOUT_DIV	__BITS(4,0)

#define	TI_AM335X_CM_CLKSEL_DPLL_MPU			0x2c
#define	  TI_AM335X_CM_CLKSEL_DPLL_MPU_DPLL_BYP_CLKSEL	__BIT(23)
#define	  TI_AM335X_CM_CLKSEL_DPLL_MPU_DPLL_MULT	__BITS(18,8)
#define	  TI_AM335X_CM_CLKSEL_DPLL_MPU_DPLL_DIV		__BITS(6,0)

#define	TI_AM335X_CM_DIV_M2_DPLL_MPU				0xa8
#define	  TI_AM335X_CM_DIV_M2_DPLL_MPU_ST_DPLL_CLKOUT		__BIT(9)
#define	  TI_AM335X_CM_DIV_M2_DPLL_MPU_DPLL_CLKOUT_GATE_CTRL	__BIT(8)
#define	  TI_AM335X_CM_DIV_M2_DPLL_MPU_DPLL_CLKOUT_DIVCHACK	__BIT(5)
#define	  TI_AM335X_CM_DIV_M2_DPLL_MPU_DPLL_CLKOUT_DIV		__BITS(4,0)

/*
 * Power Management registers base, offsets, and size
 */
#ifdef OMAP_3430
#define	OMAP2_PRM_BASE			0x48306000
#endif
#ifdef OMAP_3530
#define	OMAP2_PRM_BASE			0x48306000
#endif
#ifdef OMAP_4430
#define	OMAP2_PRM_BASE			(OMAP4430_L4_WAKEUP_BASE + 0x6000)
#endif
#ifdef OMAP_5430
#define	OMAP2_PRM_BASE			(OMAP5430_L4_WAKEUP_BASE + 0x6000)
#endif
#ifdef TI_AM335X
#define	OMAP2_PRM_BASE			0x48306000
#endif
#ifdef TI_DM37XX
#define	OMAP2_PRM_BASE			0x48306000
#endif

#define OMAP2_PRM_SIZE			0x00002000 /* 8k */

/* module offsets */
#define OCP_MOD		0x0800
#define MPU_MOD		0x0900
#define CORE_MOD	0x0a00
#define GFX_MOD		0x0b00
#define WKUP_MOD	0x0c00
#define PLL_MOD		0x0d00

/* module offsets specific to chip */
#define OMAP24XX_GR_MOD		OCP_MOD
#define OMAP24XX_DSP_MOD	0x1000
#define OMAP2430_MDM_MOD	0x1400
#define OMAP3430_IVA2_MOD	0x0000 /* IVA2 before base! */
#define OMAP3430ES2_SGX_MOD	GFX_MOD
#define OMAP3430_CCR_MOD	PLL_MOD
#define OMAP3430_DSS_MOD	0x0e00
#define OMAP3430_CAM_MOD	0x0f00
#define OMAP3430_PER_MOD	0x1000
#define OMAP3430_EMU_MOD	0x1100
#define OMAP3430_GR_MOD		0x1200
#define OMAP3430_NEON_MOD	0x1300
#define OMAP3430ES2_USBHOST_MOD	0x1400

#define OMAP2_RM_RSTCTRL	0x50
#define OMAP2_RM_RSTTIME	0x54
#define OMAP2_RM_RSTST		0x58
#define OMAP2_PM_WKDEP		0xc8
#define OMAP2_PM_PWSTCTRL	0xe0
#define OMAP2_PM_PWSTST		0xe4
#define OMAP2_PM_PREPWSTST	0xe8
#define OMAP2_PRM_IRQSTATUS	0xf8
#define OMAP2_PRM_IRQENABLE	0xfc

#define OMAP_RST_DPLL3		__BIT(2)
#define OMAP_RST_GS		__BIT(1)

#define	OMAP3_PRM_CLKSEL	0x40	// from PLL_MOD
#define	OMAP3_PRM_CLKSEL_CLKIN	__BITS(2,0)
#define	OMAP3_PRM_CLKSEL_CLKIN_12000KHZ		0
#define	OMAP3_PRM_CLKSEL_CLKIN_13000KHZ		1
#define	OMAP3_PRM_CLKSEL_CLKIN_19200KHZ		2
#define	OMAP3_PRM_CLKSEL_CLKIN_26000KHZ		3
#define	OMAP3_PRM_CLKSEL_CLKIN_38400KHZ		4
#define	OMAP3_PRM_CLKSEL_CLKIN_16800KHZ		5
#define OMAP3_PRM_CLKSEL_FREQS	{ 12000, 13000, 19200, 26000, 38400, 16800, 0, 0 }
#define	OMAP3_PRM_CLKSEL_MULT	1000

#define	OMAP4_PRM_RSTCTRL	0x7b00
#define	OMAP5_PRM_RSTCTRL	0x7c00
#define	OMAP4_PRM_RSTCTRL_WARM	0x0001
#define	OMAP4_PRM_RSTCTRL_COLD	0x0002

/*
 * L3 Interconnect Target Agent Common Registers
 */
#define OMAP2_TA_GPMC		0x68002400
#define OMAP2_TA_L4_CORE	0x68006800

/*
 * L3 Interconnect Target Agent Common Register offsets
 */
#define OMAP2_TA_COMPONENT		0x00
#define OMAP2_TA_CORE			0x18
#define OMAP2_TA_AGENT_CONTROL		0x20
#define OMAP2_TA_AGENT_STATUS		0x28
#define OMAP2_TA_ERROR_LOG		0x58
#define OMAP2_TA_ERROR_LOG_ADDR		0x60

/*
 * OMAP2_TA_COMPONENT bits
 */
#define	TA_COMPONENT_REV(r)		((r) & __BITS(15,0))
#define	TA_COMPONENT_CODE(r)		(((r) >> 16) & __BITS(15,0))

/*
 * OMAP2_TA_CORE bits
 */
#define	TA_AGENT_CORE_REV(r)		((r) & __BITS(15,0))
#define	TA_AGENT_CORE_CODE(r)		(((r) >> 16) & __BITS(15,0))

/*
 * OMAP2_TA_AGENT_CONTROL bits
 */
#define TA_AGENT_CONTROL_CORE_RESET		__BIT(0)
#define TA_AGENT_CONTROL_CORE_REJECT		__BIT(4)
#define TA_AGENT_CONTROL_CORE_TIMEOUT_BASE	__BITS(10,8)
#define TA_AGENT_CONTROL_CORE_TIMEOUT_BASE_SHFT	8
#define TA_AGENT_CONTROL_CORE_TIMEOUT_BASE_NONE		0
#define TA_AGENT_CONTROL_CORE_TIMEOUT_BASE_1		1
#define TA_AGENT_CONTROL_CORE_TIMEOUT_BASE_4		2
#define TA_AGENT_CONTROL_CORE_TIMEOUT_BASE_16		3
#define TA_AGENT_CONTROL_CORE_TIMEOUT_BASE_64		4
#define TA_AGENT_CONTROL_CORE_SERROR_REP	__BIT(24)
#define TA_AGENT_CONTROL_CORE_REQ_TIMEOUT_REP	__BIT(25)

/*
 * OMAP2_TA_AGENT_STATUS bits
 */
#define TA_AGENT_STATUS_CORE_RESET	__BIT(0)
#define TA_AGENT_STATUS_RESVa		__BITS(3,1)
#define TA_AGENT_STATUS_REQ_WAITING	__BIT(4)
#define TA_AGENT_STATUS_RESP_ACTIVE	__BIT(5)
#define TA_AGENT_STATUS_BURST		__BIT(6)
#define TA_AGENT_STATUS_READEX		__BIT(7)
#define TA_AGENT_STATUS_REQ_TIMEOUT	__BIT(8)
#define TA_AGENT_STATUS_RESVb		__BITS(11,9)
#define TA_AGENT_STATUS_TIMEBASE	__BITS(15,12)
#define TA_AGENT_STATUS_BURST_CLOSE	__BIT(16)
#define TA_AGENT_STATUS_RESVc		__BITS(23,17)
#define TA_AGENT_STATUS_SERROR		__BIT(24)		/* XXX */
#define TA_AGENT_STATUS_RESVd		__BITS(31,25)

/*
 * OMAP2_TA_ERROR_LOG bits
 */
#define TA_ERROR_LOG_CMD		__BITS(2,0)
#define TA_ERROR_LOG_RESa		__BITS(7,3)
#define TA_ERROR_LOG_INITID		__BITS(15,8)	/* initiator */
#define TA_ERROR_LOG_RESb		__BITS(23,16)
#define TA_ERROR_LOG_CODE		__BITS(27,24)	/* error */
#define TA_ERROR_LOG_RESc		__BITS(30,28)
#define TA_ERROR_LOG_MULTI		__BIT(31)	/* write to clear */

/*
 * L4 Interconnect CORE address space
 */
#define OMAP2430_L4_S3220_2430_WATCHDOGOCP24	0x48027000
#define OMAP2430_L4_S3220_2430_DMTIMER_DMC2	0x4802B000
#define OMAP2430_L4_S3220_2430_AP		0x48040000
#define OMAP2430_L4_S3220_2430_IA		0x48040800
#define OMAP2430_L4_S3220_2430_LA		0x48041000
#define OMAP2430_L4_S3220_2430_MPU_SS		0x4804A000
#define OMAP2430_L4_S3220_2430_DISPLAY_SUBS	0x48051000
#define OMAP2430_L4_S3220_2430_CAMERA_CORE	0x48053000
#define OMAP2430_L4_S3220_2430_SDMA		0x48057000
#define OMAP2430_L4_S3220_2430_SSI		0x4805C000
#define OMAP2430_L4_S3220_2430_USB_OTG_FS	0x4805F000
#define OMAP2430_L4_S3220_2430_XTI		0x48069000
#define OMAP2430_L4_S3220_2430_UART1		0x4806B000
#define OMAP2430_L4_S3220_2430_UART2		0x4806D000
#define OMAP2430_L4_S3220_2430_UART3		0x4806F000
#define OMAP2430_L4_S3220_2430_MSHSI2C1		0x48071000
#define OMAP2430_L4_S3220_2430_MSHSI2C2		0x48073000
#define OMAP2430_L4_S3220_2430_MCBSP1		0x48075000
#define OMAP2430_L4_S3220_2430_MCBSP2		0x48077000
#define OMAP2430_L4_S3220_2430_DMTIMER_DMC3	0x48079000
#define OMAP2430_L4_S3220_2430_DMTIMER_DMC4	0x4807B000
#define OMAP2430_L4_S3220_2430_DMTIMER_DMC5	0x4807D000
#define OMAP2430_L4_S3220_2430_DMTIMER_DMC6	0x4807F000
#define OMAP2430_L4_S3220_2430_DMTIMER_DMC7	0x48081000
#define OMAP2430_L4_S3220_2430_DMTIMER_DMC8	0x48083000
#define OMAP2430_L4_S3220_2430_DMTIMER_DMC9	0x48085000
#define OMAP2430_L4_S3220_2430_DMTIMER_DMC10	0x48087000
#define OMAP2430_L4_S3220_2430_DMTIMER_DMC11	0x48089000
#define OMAP2430_L4_S3220_2430_DMTIMER_DMC12	0x4808B000
#define OMAP2430_L4_S3220_2430_MCBSP3		0x4808D000
#define OMAP2430_L4_S3220_2430_MCBSP4		0x4808F000
#define OMAP2430_L4_S3220_2430_FAC		0x48093000
#define OMAP2430_L4_S3220_2430_MAILBOX1		0x48095000
#define OMAP2430_L4_S3220_2430_MCBSP5		0x48097000
#define OMAP2430_L4_S3220_2430_MCSPI1		0x48099000
#define OMAP2430_L4_S3220_2430_MCSPI2		0x4809B000
#define OMAP2430_L4_S3220_2430_MMCHS1		0x4809D000
#define OMAP2430_L4_S3220_2430_MSPRO		0x4809F000
#define OMAP2430_L4_S3220_2430_RNG		0x480A1000
#define OMAP2430_L4_S3220_2430_DESOCP		0x480A3000
#define OMAP2430_L4_S3220_2430_SHA1MD5OCP	0x480A5000
#define OMAP2430_L4_S3220_2430_AESOCP		0x480A7000
#define OMAP2430_L4_S3220_2430_PKA		0x480AA000
#define OMAP2430_L4_S3220_2430_USBHHCOCP	0x480AE000
#define OMAP2430_L4_S3220_2430_MGATE		0x480B1000
#define OMAP2430_L4_S3220_2430_HDQ1WOCP		0x480B3000
#define OMAP2430_L4_S3220_2430_MMCHS2		0x480B5000
#define OMAP2430_L4_S3220_2430_GPIO		0x480B7000
#define OMAP2430_L4_S3220_2430_MCSPI3		0x480B9000
#define OMAP2430_L4_S3220_2430_MODEM_INTH	0x480C3000

/*
 * L3 Interconnect Sideband Interconnect register base
 */
#define OMAP2_SI_BASE				0x68000400

/*
 * L3 Interconnect Sideband Interconnect register offsets
 */
#define OMAP2_SI_CONTOL				0x0020
#define OMAP2_SI_FLAG_STATUS_0			0x0110	/* APE_app */
#define OMAP2_SI_FLAG_STATUS_1			0x0130	/* APE_dbg */
#define OMAP2_SI_FLAG_STATUS_2			0x0150	/* MODEM_app */
#define OMAP2_SI_FLAG_STATUS_3			0x0170	/* MODEM_dbg */

/*
 * Interrupts
 */
#define	INTC_BASE		0x480FE000
#define	INTC_BASE_3430		0x48200000
#define	INTC_BASE_3530		0x48200000	/* Also TI_AM335X and TI_DM37XX */
#define	INTC_REVISISON		0x0000
#define	INTC_SYSCONFIG		0x0010
#define	INTC_SYSSTATUS		0x0014
#define	INTC_SIR_IRQ		0x0040	/* active IRQ */
#define	INTC_SIR_FIQ		0x0044	/* active FIQ */
#define	INTC_CONTROL		0x0048
#define	INTC_PROTECTION		0x004c
#define	INTC_IDLE		0x0050

#define	INTC_ITR		0x0080	/* unmask intr state */
#define	INTC_MIR		0x0084	/* intr mask */
#define	INTC_MIR_CLEAR		0x0088	/* clr intr mask bits */
#define	INTC_MIR_SET		0x008c	/* set intr mask bits */
#define	INTC_ISR_SET		0x0090	/* r/w soft intr mask */
#define	INTC_ISR_CLEAR		0x0094	/* clr soft intr mask */
#define	INTC_PENDING_IRQ	0x0098	/* masked irq state */
#define	INTC_PENDING_FIQ	0x009c	/* masked fiq state */

#define	INTC_ILR		0x0100	

#define	INTC_SYSCONFIG_SOFTRESET	0x2
#define	INTC_SYSCONFIG_AUTOIDLE		0x1

#define	INTC_SYSSTATUS_RESETDONE	0x1

#define	INTC_CONTROL_GLOBALMASK		0x4	/* All IRQ & FIQ are masked */
#define	INTC_CONTROL_NEWFIQAGR		0x2
#define	INTC_CONTROL_NEWIRQAGR		0x1

#define	INTC_PROTECTION_ENABLED		0x1	/* only diddle if prived */

#define	INTC_IDLE_TURBO			0x2
#define	INTC_IDLE_FUNCIDLE		0x1

#define INTC_ILR_PRIORTY_SHFT		2
#define	INTC_ILR_FIQNIRQ		0x1	/* intr is a FIQ */

/*
 * GPT - General Purpose Timers
 */
#if defined(OMAP_3430) || defined(OMAP_3530) || defined(TI_DM37XX)
#define	GPT1_BASE			0x48318000
#define	GPT2_BASE			0x49032000
#define	GPT3_BASE			0x49034000
#define	GPT4_BASE			0x49036000
#define	GPT5_BASE			0x49038000
#define	GPT6_BASE			0x4903A000
#define	GPT7_BASE			0x4903C000
#define	GPT8_BASE			0x4903E000
#define	GPT9_BASE			0x49040000
#define	GPT10_BASE			0x48086000
#define	GPT11_BASE			0x48088000
#if defined(OMAP_3430)
#define	GPT12_BASE			0x48304000
#endif
#if defined(OMAP_3530)
#define	GPT12_BASE			0x48304000
#endif
#elif defined(TI_AM33XX)
#if 0
#define	GPT0_BASE			0x44e05000
#define	GPT1_BASE			0x44e31000	/* 1ms */
#define	GPT2_BASE			0x48040000
#define	GPT3_BASE			0x48042000
#define	GPT4_BASE			0x48044000
#define	GPT5_BASE			0x48048000
#define	GPT6_BASE			0x4804A000
#define	GPT7_BASE			0x4804C000
#endif
#else
#define	GPT1_BASE			0x48028000
#define	GPT2_BASE			0x4802a000
#define	GPT3_BASE			0x48078000
#define	GPT4_BASE			0x4807a000
#define	GPT5_BASE			0x4807c000
#define	GPT6_BASE			0x4807e000
#define	GPT7_BASE			0x48080000
#define	GPT8_BASE			0x48082000
#define	GPT9_BASE			0x48084000
#define	GPT10_BASE			0x48086000
#define	GPT11_BASE			0x48088000
#define	GPT12_BASE			0x4808a000
#endif

/*
 * GPIO
 */
#define	GPIO1_BASE_2430			0x4900c000
#define	GPIO2_BASE_2430			0x4900e000
#define	GPIO3_BASE_2430			0x49010000
#define	GPIO4_BASE_2430			0x49012000
#define	GPIO5_BASE_2430			0x480b6000

#define	GPIO1_BASE_2420			0x48018000
#define	GPIO2_BASE_2420			0x4801a000
#define	GPIO3_BASE_2420			0x4801c000
#define	GPIO4_BASE_2420			0x4801e000

#define	GPIO1_BASE_3430			0x48310000
#define	GPIO2_BASE_3430			0x49050000
#define	GPIO3_BASE_3430			0x49052000
#define	GPIO4_BASE_3430			0x49054000
#define	GPIO5_BASE_3430			0x49056000
#define	GPIO6_BASE_3430			0x49058000

#define	GPIO1_BASE_3530			0x48310000
#define	GPIO2_BASE_3530			0x49050000
#define	GPIO3_BASE_3530			0x49052000
#define	GPIO4_BASE_3530			0x49054000
#define	GPIO5_BASE_3530			0x49056000
#define	GPIO6_BASE_3530			0x49058000

#define	GPIO1_BASE_4430			0x4a310000
#define	GPIO2_BASE_4430			0x48055000
#define	GPIO3_BASE_4430			0x48057000
#define	GPIO4_BASE_4430			0x48059000
#define	GPIO5_BASE_4430			0x4805b000
#define	GPIO6_BASE_4430			0x4805d000

#define	GPIO1_BASE_5430			0x4ae10000
#define	GPIO2_BASE_5430			0x48055000
#define	GPIO3_BASE_5430			0x48057000
#define	GPIO4_BASE_5430			0x48059000
#define	GPIO5_BASE_5430			0x4805b000
#define	GPIO6_BASE_5430			0x4805d000
#define	GPIO7_BASE_5430			0x48051000
#define	GPIO8_BASE_5430			0x48053000

#define	GPIO0_BASE_TI_AM335X		0x44e07000
#define	GPIO1_BASE_TI_AM335X		0x4804c000
#define	GPIO2_BASE_TI_AM335X		0x481ac000
#define	GPIO3_BASE_TI_AM335X		0x481ae000

#define	GPIO1_BASE_TI_DM37XX		0x48310000
#define	GPIO2_BASE_TI_DM37XX		0x49050000
#define	GPIO3_BASE_TI_DM37XX		0x49052000
#define	GPIO4_BASE_TI_DM37XX		0x49054000
#define	GPIO5_BASE_TI_DM37XX		0x49056000
#define	GPIO6_BASE_TI_DM37XX		0x49058000

#define	GPIO_IRQSTATUS1			0x018
#define	GPIO_IRQENABLE1			0x01c
#define	GPIO_WAKEUPENABLE		0x020
#define	GPIO_IRQSTATUS2			0x028
#define	GPIO_IRQENABLE2			0x02c
#define	GPIO_CTRL			0x030
#define	GPIO_OE				0x034
#define	GPIO_DATAIN			0x038
#define	GPIO_DATAOUT			0x03c
#define	GPIO_LEVELDETECT0		0x040
#define	GPIO_LEVELDETECT1		0x044
#define	GPIO_RISINGDETECT		0x048
#define	GPIO_FALLINGDETECT		0x04c
#define	GPIO_DEBOUNCENABLE		0x050
#define	GPIO_DEBOUNINGTIME		0x054
#define	GPIO_CLEARIRQENABLE1		0x060
#define	GPIO_SETIRQENABLE1		0x064
#define	GPIO_CLEARIRQENABLE2		0x070
#define	GPIO_SETIRQENABLE2		0x074
#define	GPIO_CLEANWKUENA		0x080
#define GPIO_SETWKUENA			0x084
#define GPIO_CLEARDATAOUT		0x090
#define GPIO_SETDATAOUT			0x094

/*
 * I2C
 */
#define I2C1_BASE_3530			0x48070000
#define I2C2_BASE_3530			0x48072000
#define I2C3_BASE_3530			0x48060000

/*
 * USB Host
 */
#define	OHCI1_BASE_2430			0x4805e000

#define	OHCI1_BASE_OMAP3		0x48064400
#define	EHCI1_BASE_OMAP3		0x48064800

#define	OHCI1_BASE_OMAP4		0x4A064800
#define	EHCI1_BASE_OMAP4		0x4A064C00

/*
 * SDRC
 */
#define OMAP3530_SDRC_BASE		0x6d000000
#define OMAP3530_SDRC_SIZE		0x00010000	/* 16KB */

/*
 * DMA
 */
#define OMAP3530_SDMA_BASE		0x48056000
#define OMAP3530_SDMA_SIZE		0x00001000	/* 4KB */

/*
 * PL310 L2CC (44xx)
 */
#define OMAP4_L2CC_BASE			0x48242000
#define OMAP4_L2CC_SIZE			0x00001000	/* 4KB */

#define OMAP4_CONTROL_ID_CODE		0x4a002204

#define AHCI1_BASE_OMAP5		0x4a140000

#define OMAP5_PRM_FRAC_INCREMENTER_NUMERATOR	0x48243210
#define  PRM_FRAC_INCR_NUM_ABE_LP_MODE	__BITS(27,16)
#define  PRM_FRAC_INCR_NUM_SYS_MODE	__BITS(11,0)
#define OMAP5_PRM_FRAC_INCREMENTER_DENUMERATOR_RELOAD	0x48243214
#define  PRM_FRAC_INCR_DENUM_RELOAD	__BIT(16)
#define  PRM_FRAC_INCR_DENUM_DENOMINATOR	__BITS(11,0)
#define OMAP5_GTIMER_FREQ		6144000		/* 6.144Mhz */

#ifdef TI_AM335X
#define TI_AM335X_CTLMOD_BASE		0x44e10000
#define CTLMOD_CONTROL_STATUS		0x40
#define CTLMOD_CONTROL_STATUS_SYSBOOT1	__BITS(23,22)
#endif
#if defined(OMAP4) || defined(TI_AM335X)
#define EMIF_SDRAM_CONFIG		8
#define SDRAM_CONFIG_WIDTH		__BITS(15,14)
#define SDRAM_CONFIG_RSIZE		__BITS(9,7)
#define SDRAM_CONFIG_IBANK		__BITS(6,4)
#define SDRAM_CONFIG_EBANK		__BIT(3)
#define SDRAM_CONFIG_PAGESIZE		__BITS(2,0)
#endif
	
#endif	/* _ARM_OMAP_OMAP2_REG_H_ */
