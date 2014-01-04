/*	$NetBSD: imx51_ccm.c,v 1.3 2012/09/19 07:28:38 bsh Exp $	*/
/*
 * Copyright (c) 2010, 2011, 2012  Genetec Corporation.  All rights reserved.
 * Written by Hashimoto Kenichi for Genetec Corporation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY GENETEC CORPORATION ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL GENETEC CORPORATION
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Clock Controller Module (CCM)
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: imx51_ccm.c,v 1.3 2012/09/19 07:28:38 bsh Exp $");

#include <sys/types.h>
#include <sys/time.h>
#include <sys/bus.h>
#include <sys/device.h>
#include <sys/param.h>

#include <machine/cpu.h>

#include <arm/imx/imx51_ccmvar.h>
#include <arm/imx/imx51_ccmreg.h>
#include <arm/imx/imx51_dpllreg.h>

#include <arm/imx/imx51var.h>
#include <arm/imx/imx51reg.h>

#include "opt_imx51clk.h"
#include "locators.h"

//#define	IMXCCMDEBUG

#ifndef	IMX51_OSC_FREQ
#define	IMX51_OSC_FREQ	(24 * 1000 * 1000)	/* 24MHz */
#endif

struct imxccm_softc {
	device_t	sc_dev;
	bus_space_tag_t	sc_iot;
	bus_space_handle_t	sc_ioh;

	struct {
		bus_space_handle_t pll_ioh;
		u_int pll_freq;
	} sc_pll[IMX51_N_DPLLS];
};

struct imxccm_softc *ccm_softc;

static uint64_t imx51_get_pll_freq(u_int);

static int imxccm_match(device_t, cfdata_t, void *);
static void imxccm_attach(device_t, device_t, void *);

CFATTACH_DECL_NEW(imxccm, sizeof(struct imxccm_softc),
    imxccm_match, imxccm_attach, NULL, NULL);

static int
imxccm_match(device_t parent, cfdata_t cfdata, void *aux)
{
	struct axi_attach_args *aa = aux;

	if (ccm_softc != NULL)
		return 0;

	if (aa->aa_addr == CCMC_BASE)
		return 1;

	return 0;
}

static void
imxccm_attach(device_t parent, device_t self, void *aux)
{
	struct imxccm_softc * const sc = device_private(self);
	struct axi_attach_args *aa = aux;
	bus_space_tag_t iot = aa->aa_iot;

	ccm_softc = sc;
	sc->sc_dev = self;
	sc->sc_iot = iot;

	if (bus_space_map(iot, aa->aa_addr, CCMC_SIZE, 0, &sc->sc_ioh)) {
		aprint_error(": can't map registers\n");
		return;
	}

	for (u_int i=1; i <= IMX51_N_DPLLS; ++i) {
		if (bus_space_map(iot, DPLL_BASE(i), DPLL_SIZE, 0,
			&sc->sc_pll[i-1].pll_ioh)) {
			aprint_error(": can't map pll registers\n");
			return;
		}
	}

	aprint_normal(": Clock control module\n");
	aprint_naive("\n");

	imx51_get_pll_freq(1);
	imx51_get_pll_freq(2);
	imx51_get_pll_freq(3);


	aprint_verbose_dev(self, "CPU clock=%d, UART clock=%d\n",
	    imx51_get_clock(IMX51CLK_ARM_ROOT),
	    imx51_get_clock(IMX51CLK_UART_CLK_ROOT));
	aprint_verbose_dev(self, 
	    "mainbus clock=%d, ahb clock=%d ipg clock=%d perclk=%d\n",
	    imx51_get_clock(IMX51CLK_MAIN_BUS_CLK),
	    imx51_get_clock(IMX51CLK_AHB_CLK_ROOT),
	    imx51_get_clock(IMX51CLK_IPG_CLK_ROOT),
	    imx51_get_clock(IMX51CLK_PERCLK_ROOT));
}


u_int
imx51_get_clock(enum imx51_clock clk)
{
	bus_space_tag_t iot = ccm_softc->sc_iot;
	bus_space_handle_t ioh = ccm_softc->sc_ioh;

	u_int freq = 0;
	u_int sel;
	uint32_t cacrr;	/* ARM clock root register */
	uint32_t ccsr;
	uint32_t cscdr1;
	uint32_t cscmr1;
	uint32_t cbcdr;
	uint32_t cbcmr;
	uint32_t cdcr;

	switch (clk) {
	case IMX51CLK_PLL1:
	case IMX51CLK_PLL2:
	case IMX51CLK_PLL3:
		return ccm_softc->sc_pll[clk - IMX51CLK_PLL1].pll_freq;
	case IMX51CLK_PLL1SW:
		ccsr = bus_space_read_4(iot, ioh, CCMC_CCSR);
		if ((ccsr & CCSR_PLL1_SW_CLK_SEL) == 0)
			return ccm_softc->sc_pll[1-1].pll_freq;
		/* step clock */
		/* FALLTHROUGH */
	case IMX51CLK_PLL1STEP:
		ccsr = bus_space_read_4(iot, ioh, CCMC_CCSR);
		switch (__SHIFTOUT(ccsr, CCSR_STEP_SEL_MASK)) {
		case 0:
			return imx51_get_clock(IMX51CLK_LP_APM);
		case 1:
			return 0; /* XXX PLL bypass clock */
		case 2:
			return ccm_softc->sc_pll[2-1].pll_freq /
			    (1 + __SHIFTOUT(ccsr, CCSR_PLL2_DIV_PODF_MASK));
		case 3:
			return ccm_softc->sc_pll[3-1].pll_freq /
			    (1 + __SHIFTOUT(ccsr, CCSR_PLL3_DIV_PODF_MASK));
		}
		/*NOTREACHED*/
	case IMX51CLK_PLL2SW:
		ccsr = bus_space_read_4(iot, ioh, CCMC_CCSR);
		if ((ccsr & CCSR_PLL2_SW_CLK_SEL) == 0)
			return imx51_get_clock(IMX51CLK_PLL2);
		return 0; /* XXX PLL2 bypass clk */
	case IMX51CLK_PLL3SW:
		ccsr = bus_space_read_4(iot, ioh, CCMC_CCSR);
		if ((ccsr & CCSR_PLL3_SW_CLK_SEL) == 0)
			return imx51_get_clock(IMX51CLK_PLL3);
		return 0; /* XXX PLL3 bypass clk */

	case IMX51CLK_LP_APM:
		ccsr = bus_space_read_4(iot, ioh, CCMC_CCSR);
		return (ccsr & CCSR_LP_APM) ?
			    imx51_get_clock(IMX51CLK_FPM) : IMX51_OSC_FREQ;

	case IMX51CLK_ARM_ROOT:
		freq = imx51_get_clock(IMX51CLK_PLL1SW);
		cacrr = bus_space_read_4(iot, ioh, CCMC_CACRR);
		return freq / (cacrr + 1);

		/* ... */
	case IMX51CLK_MAIN_BUS_CLK_SRC:
		cbcdr = bus_space_read_4(iot, ioh, CCMC_CBCDR);
		if ((cbcdr & CBCDR_PERIPH_CLK_SEL) == 0)
			freq = imx51_get_clock(IMX51CLK_PLL2SW);
		else {
			cbcmr = bus_space_read_4(iot, ioh,  CCMC_CBCMR);
			switch (__SHIFTOUT(cbcmr, CBCMR_PERIPH_APM_SEL_MASK)) {
			case 0:
				freq = imx51_get_clock(IMX51CLK_PLL1SW);
				break;
			case 1:
				freq = imx51_get_clock(IMX51CLK_PLL3SW);
				break;
			case 2:
				freq = imx51_get_clock(IMX51CLK_LP_APM);
				break;
			case 3:
				/* XXX: error */
				break;
			}
		}
		return freq;
	case IMX51CLK_MAIN_BUS_CLK:
		freq = imx51_get_clock(IMX51CLK_MAIN_BUS_CLK_SRC);
		cdcr = bus_space_read_4(iot, ioh, CCMC_CDCR);
		return freq / __SHIFTOUT(cdcr, CDCR_PERIPH_CLK_DVFS_PODF_MASK);
	case IMX51CLK_AHB_CLK_ROOT:
		freq = imx51_get_clock(IMX51CLK_MAIN_BUS_CLK);
		cbcdr = bus_space_read_4(iot, ioh, CCMC_CBCDR);
		return freq / (1 + __SHIFTOUT(cbcdr, CBCDR_AHB_PODF_MASK));
	case IMX51CLK_IPG_CLK_ROOT:
		freq = imx51_get_clock(IMX51CLK_AHB_CLK_ROOT);
		cbcdr = bus_space_read_4(iot, ioh, CCMC_CBCDR);
		return freq / (1 + __SHIFTOUT(cbcdr, CBCDR_IPG_PODF_MASK));
	case IMX51CLK_PERCLK_ROOT:
		cbcmr = bus_space_read_4(iot, ioh, CCMC_CBCMR);
		if (cbcmr & CBCMR_PERCLK_IPG_SEL)
			return imx51_get_clock(IMX51CLK_IPG_CLK_ROOT);
		if (cbcmr & CBCMR_PERCLK_LP_APM_SEL)
			freq = imx51_get_clock(IMX51CLK_LP_APM);
		else
			freq = imx51_get_clock(IMX51CLK_MAIN_BUS_CLK_SRC);

		cbcdr = bus_space_read_4(iot, ioh, CCMC_CBCDR);

#ifdef IMXCCMDEBUG
		printf("cbcmr=%x cbcdr=%x\n", cbcmr, cbcdr);
#endif

		freq /= 1 + __SHIFTOUT(cbcdr, CBCDR_PERCLK_PRED1_MASK);
		freq /= 1 + __SHIFTOUT(cbcdr, CBCDR_PERCLK_PRED2_MASK);
		freq /= 1 + __SHIFTOUT(cbcdr, CBCDR_PERCLK_PODF_MASK);
		return freq;
	case IMX51CLK_UART_CLK_ROOT:
		cscdr1 = bus_space_read_4(iot, ioh, CCMC_CSCDR1);
		cscmr1 = bus_space_read_4(iot, ioh, CCMC_CSCMR1);

#ifdef IMXCCMDEBUG
		printf("cscdr1=%x cscmr1=%x\n", cscdr1, cscmr1);
#endif

		sel = __SHIFTOUT(cscmr1, CSCMR1_UART_CLK_SEL_MASK);

		switch (sel) {
		case 0:
		case 1:
		case 2:
			freq = imx51_get_clock(IMX51CLK_PLL1SW + sel);
			break;
		case 3:
			freq = imx51_get_clock(IMX51CLK_LP_APM);
			break;
		}

		return freq / (1 + __SHIFTOUT(cscdr1, CSCDR1_UART_CLK_PRED_MASK)) /
		    (1 + __SHIFTOUT(cscdr1, CSCDR1_UART_CLK_PODF_MASK));
	case IMX51CLK_IPU_HSP_CLK_ROOT:
		cbcmr = bus_space_read_4(iot, ioh,  CCMC_CBCMR);
		switch (__SHIFTOUT(cbcmr, CBCMR_IPU_HSP_CLK_SEL_MASK)) {
			case 0:
				freq = imx51_get_clock(IMX51CLK_ARM_AXI_A_CLK);
				break;
			case 1:
				freq = imx51_get_clock(IMX51CLK_ARM_AXI_B_CLK);
				break;
			case 2:
				freq = imx51_get_clock(
					IMX51CLK_EMI_SLOW_CLK_ROOT);
				break;
			case 3:
				freq = imx51_get_clock(IMX51CLK_AHB_CLK_ROOT);
				break;
			}
		return freq;
	default:
		aprint_error_dev(ccm_softc->sc_dev,
		    "clock %d: not supported yet\n", clk);
		return 0;
	}
}


static uint64_t
imx51_get_pll_freq(u_int pll_no)
{
	uint32_t dp_ctrl;
	uint32_t dp_op;
	uint32_t dp_mfd;
	uint32_t dp_mfn;
	uint32_t mfi;
	int32_t mfn;
	uint32_t mfd;
	uint32_t pdf;
	uint32_t ccr;
	uint64_t freq = 0;
	u_int ref = 0;
	bus_space_tag_t iot = ccm_softc->sc_iot;
	bus_space_handle_t ioh = ccm_softc->sc_pll[pll_no-1].pll_ioh;

	KASSERT(1 <= pll_no && pll_no <= IMX51_N_DPLLS);

	dp_ctrl = bus_space_read_4(iot, ioh, DPLL_DP_CTL);

	if (dp_ctrl & DP_CTL_HFSM) {
		dp_op = bus_space_read_4(iot, ioh, DPLL_DP_HFS_OP);
		dp_mfd = bus_space_read_4(iot, ioh, DPLL_DP_HFS_MFD);
		dp_mfn = bus_space_read_4(iot, ioh, DPLL_DP_HFS_MFN);
	} else {
		dp_op = bus_space_read_4(iot, ioh, DPLL_DP_OP);
		dp_mfd = bus_space_read_4(iot, ioh, DPLL_DP_MFD);
		dp_mfn = bus_space_read_4(iot, ioh, DPLL_DP_MFN);
	}

	pdf = dp_op & DP_OP_PDF_MASK;
	mfi = max(5, __SHIFTOUT(dp_op, DP_OP_MFI_MASK));
	mfd = dp_mfd;
	if (dp_mfn & __BIT(26))
		/* 27bit signed value */
		mfn = (int32_t)(__BITS(31,27) | dp_mfn);
	else
		mfn = dp_mfn;

	switch (dp_ctrl &  DP_CTL_REF_CLK_SEL_MASK) {
	case DP_CTL_REF_CLK_SEL_COSC:
		/* Internal Oscillator */
		ref = IMX51_OSC_FREQ;
		break;
	case DP_CTL_REF_CLK_SEL_FPM:
		ccr = bus_space_read_4(iot, ccm_softc->sc_ioh, CCMC_CCR);
		if (ccr & CCR_FPM_MULT)
			ref = IMX51_CKIL_FREQ * 1024;
		else
			ref = IMX51_CKIL_FREQ * 512;
		break;
	default:
		ref = 0;
	}


	if (dp_ctrl & DP_CTL_REF_CLK_DIV)
		ref /= 2;

#if 0
	if (dp_ctrl & DP_CTL_DPDCK0_2_EN)
		ref *= 2;

	ref /= (pdf + 1);
	freq = ref * mfn;
	freq /= (mfd + 1);
	freq = (ref * mfi) + freq;
#endif

	ref *= 4;
	freq = (int64_t)ref * mfi + (int64_t)ref * mfn / (mfd + 1);
	freq /= pdf + 1;

	if (!(dp_ctrl & DP_CTL_DPDCK0_2_EN))
		freq /= 2;


#ifdef IMXCCMDEBUG
	printf("dp_ctl: %08x ", dp_ctrl);
	printf("pdf: %3d ", pdf);
	printf("mfi: %3d ", mfi);
	printf("mfd: %3d ", mfd);
	printf("mfn: %3d ", mfn);
	printf("pll: %lld\n", freq);
#endif

	ccm_softc->sc_pll[pll_no-1].pll_freq = freq;

	return freq;
}

void
imx51_clk_gating(int clk_src, int mode)
{
	bus_space_tag_t iot = ccm_softc->sc_iot;
	bus_space_handle_t ioh = ccm_softc->sc_ioh;
	uint32_t group = CCMR_CCGR_MODULE(clk_src);
	uint32_t field = clk_src % CCMR_CCGR_NSOURCE;
	uint32_t reg;
	uint32_t bit;

	bit = (mode << field * 2);
	reg = bus_space_read_4(iot, ioh, CCMC_CCGR(group));
	reg &= ~(0x03 << field * 2);
	reg |= bit;
	bus_space_write_4(iot, ioh, CCMC_CCGR(group), reg);
}
