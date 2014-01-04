/*	$NetBSD: mvsoc.c,v 1.16 2013/12/23 04:12:09 kiyohara Exp $	*/
/*
 * Copyright (c) 2007, 2008 KIYOHARA Takashi
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
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/cdefs.h>
__KERNEL_RCSID(0, "$NetBSD: mvsoc.c,v 1.16 2013/12/23 04:12:09 kiyohara Exp $");

#include "opt_cputypes.h"
#include "opt_mvsoc.h"

#include <sys/param.h>
#include <sys/bus.h>
#include <sys/device.h>
#include <sys/errno.h>

#include <dev/pci/pcidevs.h>
#include <dev/pci/pcireg.h>
#include <dev/marvell/marvellreg.h>
#include <dev/marvell/marvellvar.h>

#include <arm/marvell/mvsocreg.h>
#include <arm/marvell/mvsocvar.h>
#include <arm/marvell/orionreg.h>
#include <arm/marvell/kirkwoodreg.h>
#include <arm/marvell/mv78xx0reg.h>
#include <arm/marvell/armadaxpreg.h>

#include <uvm/uvm.h>

#include "locators.h"

#ifdef MVSOC_CONSOLE_EARLY
#include <dev/ic/ns16550reg.h>
#include <dev/ic/comreg.h>
#include <dev/cons.h>
#endif

static int mvsoc_match(device_t, struct cfdata *, void *);
static void mvsoc_attach(device_t, device_t, void *);

static int mvsoc_print(void *, const char *);
static int mvsoc_search(device_t, cfdata_t, const int *, void *);

uint32_t mvPclk, mvSysclk, mvTclk = 0;
int nwindow = 0, nremap = 0;
static vaddr_t regbase = 0xffffffff, dsc_base, pex_base;
vaddr_t mlmb_base;

void (*mvsoc_intr_init)(void);
int (*mvsoc_clkgating)(struct marvell_attach_args *);


#ifdef MVSOC_CONSOLE_EARLY
static vaddr_t com_base;

static inline uint32_t
uart_read(bus_size_t o)
{
	return *(volatile uint32_t *)(com_base + (o << 2));
}

static inline void
uart_write(bus_size_t o, uint32_t v)
{
	*(volatile uint32_t *)(com_base + (o << 2)) = v;
}

static int
mvsoc_cngetc(dev_t dv)
{
        if ((uart_read(com_lsr) & LSR_RXRDY) == 0)
		return -1;

	return uart_read(com_data) & 0xff;
}

static void
mvsoc_cnputc(dev_t dv, int c)
{
	int timo = 150000;

        while ((uart_read(com_lsr) & LSR_TXRDY) == 0 && --timo > 0)
		;

	uart_write(com_data, c);

	timo = 150000;
        while ((uart_read(com_lsr) & LSR_TSRE) == 0 && --timo > 0)
		;
}

static struct consdev mvsoc_earlycons = {
	.cn_putc = mvsoc_cnputc,
	.cn_getc = mvsoc_cngetc,
	.cn_pollc = nullcnpollc,
};
#endif


/* attributes */
static struct {
	int tag;
	uint32_t attr;
	uint32_t target;
} mvsoc_tags[] = {
	{ MARVELL_TAG_SDRAM_CS0,
	  MARVELL_ATTR_SDRAM_CS0,	MVSOC_UNITID_DDR },
	{ MARVELL_TAG_SDRAM_CS1,
	  MARVELL_ATTR_SDRAM_CS1,	MVSOC_UNITID_DDR },
	{ MARVELL_TAG_SDRAM_CS2,
	  MARVELL_ATTR_SDRAM_CS2,	MVSOC_UNITID_DDR },
	{ MARVELL_TAG_SDRAM_CS3,
	  MARVELL_ATTR_SDRAM_CS3,	MVSOC_UNITID_DDR },

#if defined(ORION)
	{ ORION_TAG_DEVICE_CS0,
	  ORION_ATTR_DEVICE_CS0,	MVSOC_UNITID_DEVBUS },
	{ ORION_TAG_DEVICE_CS1,
	  ORION_ATTR_DEVICE_CS1,	MVSOC_UNITID_DEVBUS },
	{ ORION_TAG_DEVICE_CS2,
	  ORION_ATTR_DEVICE_CS2,	MVSOC_UNITID_DEVBUS },
	{ ORION_TAG_DEVICE_BOOTCS,
	  ORION_ATTR_BOOT_CS,		MVSOC_UNITID_DEVBUS },
	{ ORION_TAG_FLASH_CS,
	  ORION_ATTR_FLASH_CS,		MVSOC_UNITID_DEVBUS },
	{ ORION_TAG_PEX0_MEM,
	  ORION_ATTR_PEX_MEM,		MVSOC_UNITID_PEX },
	{ ORION_TAG_PEX0_IO,
	  ORION_ATTR_PEX_IO,		MVSOC_UNITID_PEX },
	{ ORION_TAG_PEX1_MEM,
	  ORION_ATTR_PEX_MEM,		ORION_UNITID_PEX1 },
	{ ORION_TAG_PEX1_IO,
	  ORION_ATTR_PEX_IO,		ORION_UNITID_PEX1 },
	{ ORION_TAG_PCI_MEM,
	  ORION_ATTR_PCI_MEM,		ORION_UNITID_PCI },
	{ ORION_TAG_PCI_IO,
	  ORION_ATTR_PCI_IO,		ORION_UNITID_PCI },
	{ ORION_TAG_CRYPT,
	  ORION_ATTR_CRYPT,		ORION_UNITID_CRYPT },
#endif

#if defined(KIRKWOOD)
	{ KIRKWOOD_TAG_NAND,
	  KIRKWOOD_ATTR_NAND,		MVSOC_UNITID_DEVBUS },
	{ KIRKWOOD_TAG_SPI,
	  KIRKWOOD_ATTR_SPI,		MVSOC_UNITID_DEVBUS },
	{ KIRKWOOD_TAG_BOOTROM,
	  KIRKWOOD_ATTR_BOOTROM,	MVSOC_UNITID_DEVBUS },
	{ KIRKWOOD_TAG_PEX_MEM,
	  KIRKWOOD_ATTR_PEX_MEM,	MVSOC_UNITID_PEX },
	{ KIRKWOOD_TAG_PEX_IO,
	  KIRKWOOD_ATTR_PEX_IO,		MVSOC_UNITID_PEX },
	{ KIRKWOOD_TAG_PEX1_MEM,
	  KIRKWOOD_ATTR_PEX1_MEM,	MVSOC_UNITID_PEX },
	{ KIRKWOOD_TAG_PEX1_IO,
	  KIRKWOOD_ATTR_PEX1_IO,	MVSOC_UNITID_PEX },
	{ KIRKWOOD_TAG_CRYPT,
	  KIRKWOOD_ATTR_CRYPT,		KIRKWOOD_UNITID_CRYPT },
#endif

#if defined(MV78XX0)
	{ MV78XX0_TAG_DEVICE_CS0,
	  MV78XX0_ATTR_DEVICE_CS0,	MVSOC_UNITID_DEVBUS },
	{ MV78XX0_TAG_DEVICE_CS1,
	  MV78XX0_ATTR_DEVICE_CS1,	MVSOC_UNITID_DEVBUS },
	{ MV78XX0_TAG_DEVICE_CS2,
	  MV78XX0_ATTR_DEVICE_CS2,	MVSOC_UNITID_DEVBUS },
	{ MV78XX0_TAG_DEVICE_CS3,
	  MV78XX0_ATTR_DEVICE_CS3,	MVSOC_UNITID_DEVBUS },
	{ MV78XX0_TAG_DEVICE_BOOTCS,
	  MV78XX0_ATTR_BOOT_CS,		MVSOC_UNITID_DEVBUS },
	{ MV78XX0_TAG_SPI,
	  MV78XX0_ATTR_SPI,		MVSOC_UNITID_DEVBUS },
	{ MV78XX0_TAG_PEX0_MEM,
	  MV78XX0_ATTR_PEX_0_MEM,	MVSOC_UNITID_PEX },
	{ MV78XX0_TAG_PEX01_MEM,
	  MV78XX0_ATTR_PEX_1_MEM,	MVSOC_UNITID_PEX },
	{ MV78XX0_TAG_PEX02_MEM,
	  MV78XX0_ATTR_PEX_2_MEM,	MVSOC_UNITID_PEX },
	{ MV78XX0_TAG_PEX03_MEM,
	  MV78XX0_ATTR_PEX_3_MEM,	MVSOC_UNITID_PEX },
	{ MV78XX0_TAG_PEX0_IO,
	  MV78XX0_ATTR_PEX_0_IO,	MVSOC_UNITID_PEX },
	{ MV78XX0_TAG_PEX01_IO,
	  MV78XX0_ATTR_PEX_1_IO,	MVSOC_UNITID_PEX },
	{ MV78XX0_TAG_PEX02_IO,
	  MV78XX0_ATTR_PEX_2_IO,	MVSOC_UNITID_PEX },
	{ MV78XX0_TAG_PEX03_IO,
	  MV78XX0_ATTR_PEX_3_IO,	MVSOC_UNITID_PEX },
	{ MV78XX0_TAG_PEX1_MEM,
	  MV78XX0_ATTR_PEX_0_MEM,	MV78XX0_UNITID_PEX1 },
	{ MV78XX0_TAG_PEX11_MEM,
	  MV78XX0_ATTR_PEX_1_MEM,	MV78XX0_UNITID_PEX1 },
	{ MV78XX0_TAG_PEX12_MEM,
	  MV78XX0_ATTR_PEX_2_MEM,	MV78XX0_UNITID_PEX1 },
	{ MV78XX0_TAG_PEX13_MEM,
	  MV78XX0_ATTR_PEX_3_MEM,	MV78XX0_UNITID_PEX1 },
	{ MV78XX0_TAG_PEX1_IO,
	  MV78XX0_ATTR_PEX_0_IO,	MV78XX0_UNITID_PEX1 },
	{ MV78XX0_TAG_PEX11_IO,
	  MV78XX0_ATTR_PEX_1_IO,	MV78XX0_UNITID_PEX1 },
	{ MV78XX0_TAG_PEX12_IO,
	  MV78XX0_ATTR_PEX_2_IO,	MV78XX0_UNITID_PEX1 },
	{ MV78XX0_TAG_PEX13_IO,
	  MV78XX0_ATTR_PEX_3_IO,	MV78XX0_UNITID_PEX1 },
	{ MV78XX0_TAG_CRYPT,
	  MV78XX0_ATTR_CRYPT,		MV78XX0_UNITID_CRYPT },
#endif

#if defined(ARMADAXP)
	{ ARMADAXP_TAG_PEX00_MEM,
	  ARMADAXP_ATTR_PEXx0_MEM,	ARMADAXP_UNITID_PEX0 },
	{ ARMADAXP_TAG_PEX00_IO,
	  ARMADAXP_ATTR_PEXx0_IO,	ARMADAXP_UNITID_PEX0 },
	{ ARMADAXP_TAG_PEX01_MEM,
	  ARMADAXP_ATTR_PEXx1_MEM,	ARMADAXP_UNITID_PEX0 },
	{ ARMADAXP_TAG_PEX01_IO,
	  ARMADAXP_ATTR_PEXx1_IO,	ARMADAXP_UNITID_PEX0 },
	{ ARMADAXP_TAG_PEX02_MEM,
	  ARMADAXP_ATTR_PEXx2_MEM,	ARMADAXP_UNITID_PEX0 },
	{ ARMADAXP_TAG_PEX02_IO,
	  ARMADAXP_ATTR_PEXx2_IO,	ARMADAXP_UNITID_PEX0 },
	{ ARMADAXP_TAG_PEX03_MEM,
	  ARMADAXP_ATTR_PEXx3_MEM,	ARMADAXP_UNITID_PEX0 },
	{ ARMADAXP_TAG_PEX03_IO,
	  ARMADAXP_ATTR_PEXx3_IO,	ARMADAXP_UNITID_PEX0 },
	{ ARMADAXP_TAG_PEX2_MEM,
	  ARMADAXP_ATTR_PEX2_MEM,	ARMADAXP_UNITID_PEX2 },
	{ ARMADAXP_TAG_PEX2_IO,
	  ARMADAXP_ATTR_PEX2_IO,	ARMADAXP_UNITID_PEX2 },
	{ ARMADAXP_TAG_PEX3_MEM,
	  ARMADAXP_ATTR_PEX3_MEM,	ARMADAXP_UNITID_PEX3 },
	{ ARMADAXP_TAG_PEX3_IO,
	  ARMADAXP_ATTR_PEX3_IO,	ARMADAXP_UNITID_PEX3 },
#endif
};

#if defined(ARMADAXP)
#undef ARMADAXP
#define ARMADAXP(m)	MARVELL_ARMADAXP_ ## m
#endif
#if defined(ORION)
#define ORION_1(m)	MARVELL_ORION_1_ ## m
#define ORION_2(m)	MARVELL_ORION_2_ ## m
#endif
#if defined(KIRKWOOD)
#undef KIRKWOOD
#define KIRKWOOD(m)	MARVELL_KIRKWOOD_ ## m
#endif
#if defined(MV78XX0)
#undef MV78XX0
#define MV78XX0(m)	MARVELL_MV78XX0_ ## m
#endif
static struct {
	uint16_t model;
	uint8_t rev;
	const char *modelstr;
	const char *revstr;
	const char *typestr;
} nametbl[] = {
#if defined(ORION)
	{ ORION_1(88F1181),	0, "MV88F1181", NULL,	"Orion1" },
	{ ORION_1(88F5082),	2, "MV88F5082", "A2",	"Orion1" },
	{ ORION_1(88F5180N),	3, "MV88F5180N","B1",	"Orion1" },
	{ ORION_1(88F5181),	0, "MV88F5181",	"A0",	"Orion1" },
	{ ORION_1(88F5181),	1, "MV88F5181",	"A1",	"Orion1" },
	{ ORION_1(88F5181),	2, "MV88F5181",	"B0",	"Orion1" },
	{ ORION_1(88F5181),	3, "MV88F5181",	"B1",	"Orion1" },
	{ ORION_1(88F5181),	8, "MV88F5181L","A0",	"Orion1" },
	{ ORION_1(88F5181),	9, "MV88F5181L","A1",	"Orion1" },
	{ ORION_1(88F5182),	0, "MV88F5182",	"A0",	"Orion1" },
	{ ORION_1(88F5182),	1, "MV88F5182",	"A1",	"Orion1" },
	{ ORION_1(88F5182),	2, "MV88F5182",	"A2",	"Orion1" },
	{ ORION_1(88F6082),	0, "MV88F6082",	"A0",	"Orion1" },
	{ ORION_1(88F6082),	1, "MV88F6082",	"A1",	"Orion1" },
	{ ORION_1(88F6183),	0, "MV88F6183",	"A0",	"Orion1" },
	{ ORION_1(88F6183),	1, "MV88F6183",	"Z0",	"Orion1" },
	{ ORION_1(88W8660),	0, "MV88W8660",	"A0",	"Orion1" },
	{ ORION_1(88W8660),	1, "MV88W8660",	"A1",	"Orion1" },

	{ ORION_2(88F1281),	0, "MV88F1281",	"A0",	"Orion2" },
	{ ORION_2(88F5281),	0, "MV88F5281",	"A0",	"Orion2" },
	{ ORION_2(88F5281),	1, "MV88F5281",	"B0",	"Orion2" },
	{ ORION_2(88F5281),	2, "MV88F5281",	"C0",	"Orion2" },
	{ ORION_2(88F5281),	3, "MV88F5281",	"C1",	"Orion2" },
	{ ORION_2(88F5281),	4, "MV88F5281",	"D0",	"Orion2" },
#endif

#if defined(KIRKWOOD)
	{ KIRKWOOD(88F6180),	2, "88F6180",	"A0",	"Kirkwood" },
	{ KIRKWOOD(88F6180),	3, "88F6180",	"A1",	"Kirkwood" },
	{ KIRKWOOD(88F6192),	0, "88F619x",	"Z0",	"Kirkwood" },
	{ KIRKWOOD(88F6192),	2, "88F619x",	"A0",	"Kirkwood" },
	{ KIRKWOOD(88F6192),	3, "88F619x",	"A1",	"Kirkwood" },
	{ KIRKWOOD(88F6281),	0, "88F6281",	"Z0",	"Kirkwood" },
	{ KIRKWOOD(88F6281),	2, "88F6281",	"A0",	"Kirkwood" },
	{ KIRKWOOD(88F6281),	3, "88F6281",	"A1",	"Kirkwood" },
	{ KIRKWOOD(88F6282),	0, "88F6282",	"A0",	"Kirkwood" },
	{ KIRKWOOD(88F6282),	1, "88F6282",	"A1",	"Kirkwood" },
#endif

#if defined(MV78XX0)
	{ MV78XX0(MV78100),	1, "MV78100",	"A0",  "Discovery Innovation" },
	{ MV78XX0(MV78100),	2, "MV78100",	"A1",  "Discovery Innovation" },
	{ MV78XX0(MV78200),	1, "MV78200",	"A0",  "Discovery Innovation" },
#endif

#if defined(ARMADAXP)
	{ ARMADAXP(MV78130),	1, "MV78130",	"A0",  "Armada XP" },
	{ ARMADAXP(MV78160),	1, "MV78160",	"A0",  "Armada XP" },
	{ ARMADAXP(MV78230),	1, "MV78260",	"A0",  "Armada XP" },
	{ ARMADAXP(MV78260),	1, "MV78260",	"A0",  "Armada XP" },
	{ ARMADAXP(MV78460),	1, "MV78460",	"A0",  "Armada XP" },
	{ ARMADAXP(MV78460),	2, "MV78460",	"B0",  "Armada XP" },
#endif
};

#define OFFSET_DEFAULT	MVA_OFFSET_DEFAULT
#define IRQ_DEFAULT	MVA_IRQ_DEFAULT
static const struct mvsoc_periph {
	int model;
	const char *name;
	int unit;
	bus_size_t offset;
	int irq;
} mvsoc_periphs[] = {
#if defined(ORION)
#define ORION_IRQ_TMR		(32 + MVSOC_MLMB_MLMBI_CPUTIMER0INTREQ)

    { ORION_1(88F1181),	"mvsoctmr",0, MVSOC_TMR_BASE,	ORION_IRQ_TMR },
    { ORION_1(88F1181),	"mvsocgpp",0, MVSOC_GPP_BASE,	ORION_IRQ_GPIO7_0 },
    { ORION_1(88F1181),	"com",     0, MVSOC_COM0_BASE,	ORION_IRQ_UART0 },
    { ORION_1(88F1181),	"com",     1, MVSOC_COM1_BASE,	ORION_IRQ_UART1 },
    { ORION_1(88F1181),	"gttwsi",  0, MVSOC_TWSI_BASE,	ORION_IRQ_TWSI },
    { ORION_1(88F1181),	"mvpex",   0, MVSOC_PEX_BASE,	ORION_IRQ_PEX0INT },
    { ORION_1(88F1181),	"mvpex",   1, ORION_PEX1_BASE,	ORION_IRQ_PEX1INT },

    { ORION_1(88F5082),	"mvsoctmr",0, MVSOC_TMR_BASE,	ORION_IRQ_TMR },
    { ORION_1(88F5082),	"mvsocgpp",0, MVSOC_GPP_BASE,	ORION_IRQ_GPIO7_0 },
    { ORION_1(88F5082),	"com",     0, MVSOC_COM0_BASE,	ORION_IRQ_UART0 },
    { ORION_1(88F5082),	"com",     1, MVSOC_COM1_BASE,	ORION_IRQ_UART1 },
    { ORION_1(88F5082),	"ehci",    0, ORION_USB0_BASE,	ORION_IRQ_USBCNT0 },
    { ORION_1(88F5082),	"ehci",    1, ORION_USB1_BASE,	ORION_IRQ_USBCNT1 },
    { ORION_1(88F5082),	"gtidmac", 0, ORION_IDMAC_BASE,	IRQ_DEFAULT },
    { ORION_1(88F5082),	"gttwsi",  0, MVSOC_TWSI_BASE,	ORION_IRQ_TWSI },
    { ORION_1(88F5082),	"mvcesa",  0, ORION_CESA_BASE,	ORION_IRQ_SECURITYINTR},
    { ORION_1(88F5082),	"mvgbec",  0, ORION_GBE_BASE,	IRQ_DEFAULT },
    { ORION_1(88F5082),	"mvpex",   0, MVSOC_PEX_BASE,	ORION_IRQ_PEX0INT },
    { ORION_1(88F5082),	"mvsata",  0, ORION_SATAHC_BASE,ORION_IRQ_SATAINTR },

    { ORION_1(88F5180N),"mvsoctmr",0, MVSOC_TMR_BASE,	ORION_IRQ_TMR },
    { ORION_1(88F5180N),"mvsocgpp",0, MVSOC_GPP_BASE,	ORION_IRQ_GPIO7_0 },
    { ORION_1(88F5180N),"com",     0, MVSOC_COM0_BASE,	ORION_IRQ_UART0 },
    { ORION_1(88F5180N),"com",     1, MVSOC_COM1_BASE,	ORION_IRQ_UART1 },
    { ORION_1(88F5180N),"ehci",    0, ORION_USB0_BASE,	ORION_IRQ_USBCNT0 },
    { ORION_1(88F5180N),"gtidmac", 0, ORION_IDMAC_BASE,	IRQ_DEFAULT },
    { ORION_1(88F5180N),"gtpci",   0, ORION_PCI_BASE,	ORION_IRQ_PEX0INT },
    { ORION_1(88F5180N),"gttwsi",  0, MVSOC_TWSI_BASE,	ORION_IRQ_TWSI },
    { ORION_1(88F5180N),"mvgbec",  0, ORION_GBE_BASE,	IRQ_DEFAULT },
    { ORION_1(88F5180N),"mvpex",   0, MVSOC_PEX_BASE,	ORION_IRQ_PEX0INT },

    { ORION_1(88F5181),	"mvsoctmr",0, MVSOC_TMR_BASE,	ORION_IRQ_TMR },
    { ORION_1(88F5181),	"mvsocgpp",0, MVSOC_GPP_BASE,	ORION_IRQ_GPIO7_0 },
    { ORION_1(88F5181),	"com",     0, MVSOC_COM0_BASE,	ORION_IRQ_UART0 },
    { ORION_1(88F5181),	"com",     1, MVSOC_COM1_BASE,	ORION_IRQ_UART1 },
    { ORION_1(88F5181),	"ehci",    0, ORION_USB0_BASE,	ORION_IRQ_USBCNT0 },
    { ORION_1(88F5181),	"gtidmac", 0, ORION_IDMAC_BASE,	IRQ_DEFAULT },
    { ORION_1(88F5181),	"gtpci",   0, ORION_PCI_BASE,	ORION_IRQ_PEX0INT },
    { ORION_1(88F5181),	"gttwsi",  0, MVSOC_TWSI_BASE,	ORION_IRQ_TWSI },
    { ORION_1(88F5181),	"mvcesa",  0, ORION_CESA_BASE,	ORION_IRQ_SECURITYINTR},
    { ORION_1(88F5181),	"mvgbec",  0, ORION_GBE_BASE,	IRQ_DEFAULT },
    { ORION_1(88F5181),	"mvpex",   0, MVSOC_PEX_BASE,	ORION_IRQ_PEX0INT },

    { ORION_1(88F5182),	"mvsoctmr",0, MVSOC_TMR_BASE,	ORION_IRQ_TMR },
    { ORION_1(88F5182),	"mvsocgpp",0, MVSOC_GPP_BASE,	ORION_IRQ_GPIO7_0 },
    { ORION_1(88F5182),	"com",     0, MVSOC_COM0_BASE,	ORION_IRQ_UART0 },
    { ORION_1(88F5182),	"com",     1, MVSOC_COM1_BASE,	ORION_IRQ_UART1 },
    { ORION_1(88F5182),	"ehci",    0, ORION_USB0_BASE,	ORION_IRQ_USBCNT0 },
    { ORION_1(88F5182),	"ehci",    1, ORION_USB1_BASE,	ORION_IRQ_USBCNT1 },
    { ORION_1(88F5182),	"gtidmac", 0, ORION_IDMAC_BASE,	IRQ_DEFAULT },
    { ORION_1(88F5182),	"gtpci",   0, ORION_PCI_BASE,	ORION_IRQ_PEX0INT },
    { ORION_1(88F5182),	"gttwsi",  0, MVSOC_TWSI_BASE,	ORION_IRQ_TWSI },
    { ORION_1(88F5182),	"mvgbec",  0, ORION_GBE_BASE,	IRQ_DEFAULT },
    { ORION_1(88F5182),	"mvsata",  0, ORION_SATAHC_BASE,ORION_IRQ_SATAINTR },
    { ORION_1(88F5182),	"mvpex",   0, MVSOC_PEX_BASE,	ORION_IRQ_PEX0INT },

    { ORION_1(88F6082),	"mvsoctmr",0, MVSOC_TMR_BASE,	ORION_IRQ_TMR },
    { ORION_1(88F6082),	"mvsocgpp",0, MVSOC_GPP_BASE,	ORION_IRQ_GPIO7_0 },
    { ORION_1(88F6082),	"com",     0, MVSOC_COM0_BASE,	ORION_IRQ_UART0 },
    { ORION_1(88F6082),	"com",     1, MVSOC_COM1_BASE,	ORION_IRQ_UART1 },
    { ORION_1(88F6082),	"ehci",    0, ORION_USB0_BASE,	ORION_IRQ_USBCNT0 },
    { ORION_1(88F6082),	"gttwsi",  0, MVSOC_TWSI_BASE,	ORION_IRQ_TWSI },
    { ORION_1(88F6082),	"mvcesa",  0, ORION_CESA_BASE,	ORION_IRQ_SECURITYINTR},
    { ORION_1(88F6082),	"mvgbec",  0, ORION_GBE_BASE,	IRQ_DEFAULT },
    { ORION_1(88F6082),	"mvsata",  0, ORION_SATAHC_BASE,ORION_IRQ_SATAINTR },
    { ORION_1(88F6082),	"mvpex",   0, MVSOC_PEX_BASE,	ORION_IRQ_PEX0INT },

    { ORION_1(88F6183),	"mvsoctmr",0, MVSOC_TMR_BASE,	ORION_IRQ_TMR },
    { ORION_1(88F6183),	"mvsocgpp",0, MVSOC_GPP_BASE,	ORION_IRQ_GPIO7_0 },
    { ORION_1(88F6183),	"gttwsi",  0, MVSOC_TWSI_BASE,	ORION_IRQ_TWSI },
    { ORION_1(88F6183),	"mvpex",   0, MVSOC_PEX_BASE,	ORION_IRQ_PEX0INT },

    { ORION_1(88W8660),	"mvsoctmr",0, MVSOC_TMR_BASE,	ORION_IRQ_TMR },
    { ORION_1(88W8660),	"mvsocgpp",0, MVSOC_GPP_BASE,	ORION_IRQ_GPIO7_0 },
    { ORION_1(88W8660),	"com",     0, MVSOC_COM0_BASE,	ORION_IRQ_UART0 },
    { ORION_1(88W8660),	"com",     1, MVSOC_COM1_BASE,	ORION_IRQ_UART1 },
    { ORION_1(88W8660),	"ehci",    0, ORION_USB0_BASE,	ORION_IRQ_USBCNT0 },
    { ORION_1(88W8660),	"gtidmac", 0, ORION_IDMAC_BASE,	IRQ_DEFAULT },
    { ORION_1(88W8660),	"gtpci",   0, ORION_PCI_BASE,	ORION_IRQ_PEX0INT },
    { ORION_1(88W8660),	"gttwsi",  0, MVSOC_TWSI_BASE,	ORION_IRQ_TWSI },
    { ORION_1(88W8660),	"mvgbec",  0, ORION_GBE_BASE,	IRQ_DEFAULT },
    { ORION_1(88W8660),	"mvpex",   0, MVSOC_PEX_BASE,	ORION_IRQ_PEX0INT },

    { ORION_2(88F1281),	"mvsoctmr",0, MVSOC_TMR_BASE,	ORION_IRQ_TMR },
    { ORION_2(88F1281),	"mvsocgpp",0, MVSOC_GPP_BASE,	ORION_IRQ_GPIO7_0 },
    { ORION_2(88F1281),	"com",     0, MVSOC_COM0_BASE,	ORION_IRQ_UART0 },
    { ORION_2(88F1281),	"com",     1, MVSOC_COM1_BASE,	ORION_IRQ_UART1 },
    { ORION_2(88F1281),	"gttwsi",  0, MVSOC_TWSI_BASE,	ORION_IRQ_TWSI },
    { ORION_2(88F1281),	"mvpex",   0, MVSOC_PEX_BASE,	ORION_IRQ_PEX0INT },
    { ORION_2(88F1281),	"mvpex",   1, ORION_PEX1_BASE,	ORION_IRQ_PEX1INT },

    { ORION_2(88F5281),	"mvsoctmr",0, MVSOC_TMR_BASE,	ORION_IRQ_TMR },
    { ORION_2(88F5281),	"mvsocgpp",0, MVSOC_GPP_BASE,	ORION_IRQ_GPIO7_0 },
    { ORION_2(88F5281),	"com",     0, MVSOC_COM0_BASE,	ORION_IRQ_UART0 },
    { ORION_2(88F5281),	"com",     1, MVSOC_COM1_BASE,	ORION_IRQ_UART1 },
    { ORION_2(88F5281),	"ehci",    0, ORION_USB0_BASE,	ORION_IRQ_USBCNT0 },
    { ORION_2(88F5281),	"gtidmac", 0, ORION_IDMAC_BASE,	IRQ_DEFAULT },
    { ORION_2(88F5281),	"gtpci",   0, ORION_PCI_BASE,	ORION_IRQ_PEX0INT },
    { ORION_2(88F5281),	"gttwsi",  0, MVSOC_TWSI_BASE,	ORION_IRQ_TWSI },
    { ORION_2(88F5281),	"mvgbec",  0, ORION_GBE_BASE,	IRQ_DEFAULT },
    { ORION_2(88F5281),	"mvpex",   0, MVSOC_PEX_BASE,	ORION_IRQ_PEX0INT },
#endif

#if defined(KIRKWOOD)
#define KIRKWOOD_IRQ_TMR	(64 + MVSOC_MLMB_MLMBI_CPUTIMER0INTREQ)

    { KIRKWOOD(88F6180),"mvsoctmr",0, MVSOC_TMR_BASE,	KIRKWOOD_IRQ_TMR },
    { KIRKWOOD(88F6180),"mvsocgpp",0, MVSOC_GPP_BASE,	KIRKWOOD_IRQ_GPIOLO7_0},
    { KIRKWOOD(88F6180),"mvsocrtc",0, KIRKWOOD_RTC_BASE,IRQ_DEFAULT },
    { KIRKWOOD(88F6180),"com",     0, MVSOC_COM0_BASE,	KIRKWOOD_IRQ_UART0INT },
    { KIRKWOOD(88F6180),"com",     1, MVSOC_COM1_BASE,	KIRKWOOD_IRQ_UART1INT },
    { KIRKWOOD(88F6180),"ehci",    0, KIRKWOOD_USB_BASE,KIRKWOOD_IRQ_USB0CNT },
    { KIRKWOOD(88F6180),"gtidmac", 0, KIRKWOOD_IDMAC_BASE,IRQ_DEFAULT },
    { KIRKWOOD(88F6180),"gttwsi",  0, MVSOC_TWSI_BASE,	KIRKWOOD_IRQ_TWSI },
    { KIRKWOOD(88F6180),"mvcesa",  0, KIRKWOOD_CESA_BASE,KIRKWOOD_IRQ_SECURITYINT},
    { KIRKWOOD(88F6180),"mvgbec",  0, KIRKWOOD_GBE0_BASE,IRQ_DEFAULT },
    { KIRKWOOD(88F6180),"mvpex",   0, MVSOC_PEX_BASE,	KIRKWOOD_IRQ_PEX0INT },
    { KIRKWOOD(88F6180),"mvsdio",  0, KIRKWOOD_SDIO_BASE,KIRKWOOD_IRQ_SDIOINT },

    { KIRKWOOD(88F6192),"mvsoctmr",0, MVSOC_TMR_BASE,	KIRKWOOD_IRQ_TMR },
    { KIRKWOOD(88F6192),"mvsocgpp",0, MVSOC_GPP_BASE,	KIRKWOOD_IRQ_GPIOLO7_0},
    { KIRKWOOD(88F6192),"mvsocrtc",0, KIRKWOOD_RTC_BASE,IRQ_DEFAULT },
    { KIRKWOOD(88F6192),"com",     0, MVSOC_COM0_BASE,	KIRKWOOD_IRQ_UART0INT },
    { KIRKWOOD(88F6192),"com",     1, MVSOC_COM1_BASE,	KIRKWOOD_IRQ_UART1INT },
    { KIRKWOOD(88F6192),"ehci",    0, KIRKWOOD_USB_BASE,KIRKWOOD_IRQ_USB0CNT },
    { KIRKWOOD(88F6192),"gtidmac", 0, KIRKWOOD_IDMAC_BASE,IRQ_DEFAULT },
    { KIRKWOOD(88F6192),"gttwsi",  0, MVSOC_TWSI_BASE,	KIRKWOOD_IRQ_TWSI },
    { KIRKWOOD(88F6192),"mvcesa",  0, KIRKWOOD_CESA_BASE,KIRKWOOD_IRQ_SECURITYINT},
    { KIRKWOOD(88F6192),"mvgbec",  0, KIRKWOOD_GBE0_BASE,IRQ_DEFAULT },
    { KIRKWOOD(88F6192),"mvgbec",  1, KIRKWOOD_GBE1_BASE,IRQ_DEFAULT },
    { KIRKWOOD(88F6192),"mvpex",   0, MVSOC_PEX_BASE,	KIRKWOOD_IRQ_PEX0INT },
    { KIRKWOOD(88F6192),"mvsata",  0, KIRKWOOD_SATAHC_BASE,KIRKWOOD_IRQ_SATA },
    { KIRKWOOD(88F6192),"mvsdio",  0, KIRKWOOD_SDIO_BASE,KIRKWOOD_IRQ_SDIOINT },

    { KIRKWOOD(88F6281),"mvsoctmr",0, MVSOC_TMR_BASE,	KIRKWOOD_IRQ_TMR },
    { KIRKWOOD(88F6281),"mvsocgpp",0, MVSOC_GPP_BASE,	KIRKWOOD_IRQ_GPIOLO7_0},
    { KIRKWOOD(88F6281),"mvsocrtc",0, KIRKWOOD_RTC_BASE,IRQ_DEFAULT },
    { KIRKWOOD(88F6281),"com",     0, MVSOC_COM0_BASE,	KIRKWOOD_IRQ_UART0INT },
    { KIRKWOOD(88F6281),"com",     1, MVSOC_COM1_BASE,	KIRKWOOD_IRQ_UART1INT },
    { KIRKWOOD(88F6281),"ehci",    0, KIRKWOOD_USB_BASE,KIRKWOOD_IRQ_USB0CNT },
    { KIRKWOOD(88F6281),"gtidmac", 0, KIRKWOOD_IDMAC_BASE,IRQ_DEFAULT },
    { KIRKWOOD(88F6281),"gttwsi",  0, MVSOC_TWSI_BASE,	KIRKWOOD_IRQ_TWSI },
    { KIRKWOOD(88F6281),"mvcesa",  0, KIRKWOOD_CESA_BASE,KIRKWOOD_IRQ_SECURITYINT },
    { KIRKWOOD(88F6281),"mvgbec",  0, KIRKWOOD_GBE0_BASE,IRQ_DEFAULT },
    { KIRKWOOD(88F6281),"mvgbec",  1, KIRKWOOD_GBE1_BASE,IRQ_DEFAULT },
    { KIRKWOOD(88F6281),"mvpex",   0, MVSOC_PEX_BASE,	KIRKWOOD_IRQ_PEX0INT },
    { KIRKWOOD(88F6281),"mvsata",  0, KIRKWOOD_SATAHC_BASE,KIRKWOOD_IRQ_SATA },
    { KIRKWOOD(88F6281),"mvsdio",  0, KIRKWOOD_SDIO_BASE,KIRKWOOD_IRQ_SDIOINT },

    { KIRKWOOD(88F6282),"mvsoctmr",0, MVSOC_TMR_BASE,	KIRKWOOD_IRQ_TMR },
    { KIRKWOOD(88F6282),"mvsocgpp",0, MVSOC_GPP_BASE,	KIRKWOOD_IRQ_GPIOLO7_0},
    { KIRKWOOD(88F6282),"mvsocrtc",0, KIRKWOOD_RTC_BASE,IRQ_DEFAULT },
    { KIRKWOOD(88F6282),"mvsocts", 0, KIRKWOOD_TS_BASE,	IRQ_DEFAULT },
    { KIRKWOOD(88F6282),"com",     0, MVSOC_COM0_BASE,	KIRKWOOD_IRQ_UART0INT },
    { KIRKWOOD(88F6282),"com",     1, MVSOC_COM1_BASE,	KIRKWOOD_IRQ_UART1INT },
    { KIRKWOOD(88F6282),"ehci",    0, KIRKWOOD_USB_BASE,KIRKWOOD_IRQ_USB0CNT },
    { KIRKWOOD(88F6282),"gtidmac", 0, KIRKWOOD_IDMAC_BASE,IRQ_DEFAULT },
    { KIRKWOOD(88F6282),"gttwsi",  0, MVSOC_TWSI_BASE,	KIRKWOOD_IRQ_TWSI },
    { KIRKWOOD(88F6282),"gttwsi",  1, KIRKWOOD_TWSI1_BASE,KIRKWOOD_IRQ_TWSI1 },
    { KIRKWOOD(88F6282),"mvcesa",  0, KIRKWOOD_CESA_BASE,KIRKWOOD_IRQ_SECURITYINT},
    { KIRKWOOD(88F6282),"mvgbec",  0, KIRKWOOD_GBE0_BASE,IRQ_DEFAULT },
    { KIRKWOOD(88F6282),"mvgbec",  1, KIRKWOOD_GBE1_BASE,IRQ_DEFAULT },
    { KIRKWOOD(88F6282),"mvpex",   0, MVSOC_PEX_BASE,	KIRKWOOD_IRQ_PEX0INT },
    { KIRKWOOD(88F6282),"mvpex",   1, KIRKWOOD_PEX1_BASE,KIRKWOOD_IRQ_PEX1INT },
    { KIRKWOOD(88F6282),"mvsata",  0, KIRKWOOD_SATAHC_BASE,KIRKWOOD_IRQ_SATA },
    { KIRKWOOD(88F6282),"mvsdio",  0, KIRKWOOD_SDIO_BASE,KIRKWOOD_IRQ_SDIOINT },
#endif

#if defined(MV78XX0)
    { MV78XX0(MV78100),	"mvsoctmr",0, MVSOC_TMR_BASE,	MV78XX0_IRQ_TIMER0 },
    { MV78XX0(MV78100),	"mvsocgpp",0, MVSOC_GPP_BASE,	MV78XX0_IRQ_GPIO0_7 },
    { MV78XX0(MV78100),	"com",	   0, MVSOC_COM0_BASE,	MV78XX0_IRQ_UART0 },
    { MV78XX0(MV78100),	"com",	   1, MVSOC_COM1_BASE,	MV78XX0_IRQ_UART1 },
    { MV78XX0(MV78100),	"com",	   2, MV78XX0_COM2_BASE,MV78XX0_IRQ_UART2 },
    { MV78XX0(MV78100),	"com",	   3, MV78XX0_COM3_BASE,MV78XX0_IRQ_UART3 },
    { MV78XX0(MV78100),	"gttwsi",  0, MVSOC_TWSI_BASE,	MV78XX0_IRQ_TWSI0 },
    { MV78XX0(MV78100),	"gttwsi",  1, MV78XX0_TWSI1_BASE,MV78XX0_IRQ_TWSI1 },
    { MV78XX0(MV78100), "mvgbec",  0, MV78XX0_GBE0_BASE,IRQ_DEFAULT },
    { MV78XX0(MV78100), "mvgbec",  1, MV78XX0_GBE1_BASE,IRQ_DEFAULT },
    { MV78XX0(MV78100), "mvsata",  0, MV78XX0_SATAHC_BASE,MV78XX0_IRQ_SATA },

    { MV78XX0(MV78200),	"mvsoctmr",0, MVSOC_TMR_BASE,	MV78XX0_IRQ_TIMER0 },
    { MV78XX0(MV78200),	"mvsocgpp",0, MVSOC_GPP_BASE,	MV78XX0_IRQ_GPIO0_7 },
    { MV78XX0(MV78200),	"com",     0, MVSOC_COM0_BASE,	MV78XX0_IRQ_UART0 },
    { MV78XX0(MV78200),	"com",     1, MVSOC_COM1_BASE,	MV78XX0_IRQ_UART1 },
    { MV78XX0(MV78200),	"com",	   2, MV78XX0_COM2_BASE,MV78XX0_IRQ_UART2 },
    { MV78XX0(MV78200),	"com",	   3, MV78XX0_COM3_BASE,MV78XX0_IRQ_UART3 },
    { MV78XX0(MV78200),	"gttwsi",  0, MVSOC_TWSI_BASE,	MV78XX0_IRQ_TWSI0 },
    { MV78XX0(MV78200),	"gttwsi",  1, MV78XX0_TWSI1_BASE,MV78XX0_IRQ_TWSI1 },
    { MV78XX0(MV78200), "mvgbec",  0, MV78XX0_GBE0_BASE,IRQ_DEFAULT },
    { MV78XX0(MV78200), "mvgbec",  1, MV78XX0_GBE1_BASE,IRQ_DEFAULT },
    { MV78XX0(MV78200), "mvgbec",  2, MV78XX0_GBE2_BASE,IRQ_DEFAULT },
    { MV78XX0(MV78200), "mvgbec",  3, MV78XX0_GBE3_BASE,IRQ_DEFAULT },
    { MV78XX0(MV78200), "mvsata",  0, MV78XX0_SATAHC_BASE,MV78XX0_IRQ_SATA },
#endif

#if defined(ARMADAXP)
    { ARMADAXP(MV78130), "mvsoctmr",0,MVSOC_TMR_BASE,	ARMADAXP_IRQ_TIMER0 },
    { ARMADAXP(MV78130), "com",    0, MVSOC_COM0_BASE,	ARMADAXP_IRQ_UART0 },
    { ARMADAXP(MV78130), "com",    1, MVSOC_COM1_BASE,	ARMADAXP_IRQ_UART1 },
    { ARMADAXP(MV78130), "com",    2, ARMADAXP_COM2_BASE,ARMADAXP_IRQ_UART2 },
    { ARMADAXP(MV78130), "com",    3, ARMADAXP_COM3_BASE,ARMADAXP_IRQ_UART3 },
    { ARMADAXP(MV78130), "mvsocrtc",0,ARMADAXP_RTC_BASE,ARMADAXP_IRQ_RTC },
    { ARMADAXP(MV78130), "gttwsi", 0, MVSOC_TWSI_BASE,	ARMADAXP_IRQ_TWSI0 },
    { ARMADAXP(MV78130), "gttwsi", 1, ARMADAXP_TWSI1_BASE,ARMADAXP_IRQ_TWSI1 },
    { ARMADAXP(MV78130), "gtidmac",0, ARMADAXP_XORE0_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78130), "gtidmac",1, ARMADAXP_XORE1_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78130), "ehci",   0, ARMADAXP_USB0_BASE,ARMADAXP_IRQ_USB0 },
    { ARMADAXP(MV78130), "ehci",   1, ARMADAXP_USB1_BASE,ARMADAXP_IRQ_USB1 },
    { ARMADAXP(MV78130), "ehci",   2, ARMADAXP_USB2_BASE,ARMADAXP_IRQ_USB2 },
    { ARMADAXP(MV78130), "mvpex",  0, MVSOC_PEX_BASE,	ARMADAXP_IRQ_PEX00 },
    { ARMADAXP(MV78130), "mvpex",  1, ARMADAXP_PEX01_BASE,ARMADAXP_IRQ_PEX01 },
    { ARMADAXP(MV78130), "mvpex",  2, ARMADAXP_PEX02_BASE,ARMADAXP_IRQ_PEX02 },
    { ARMADAXP(MV78130), "mvpex",  3, ARMADAXP_PEX03_BASE,ARMADAXP_IRQ_PEX03 },
    { ARMADAXP(MV78130), "mvsata", 0, ARMADAXP_SATAHC_BASE,ARMADAXP_IRQ_SATA0 },
    { ARMADAXP(MV78130), "mvspi",  0, ARMADAXP_SPI_BASE,ARMADAXP_IRQ_SPI },
    { ARMADAXP(MV78130), "mvsdio", 0, ARMADAXP_SDIO_BASE,ARMADAXP_IRQ_SDIO },
    { ARMADAXP(MV78130), "mvgbec", 0, ARMADAXP_GBE0_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78130), "mvgbec", 1, ARMADAXP_GBE1_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78130), "mvgbec", 2, ARMADAXP_GBE2_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78130), "mvcesa", 0, ARMADAXP_CESA0_BASE,ARMADAXP_IRQ_CESA0 },
    { ARMADAXP(MV78130), "mvcesa", 1, ARMADAXP_CESA1_BASE,ARMADAXP_IRQ_CESA1 },

    { ARMADAXP(MV78160), "mvsoctmr",0,MVSOC_TMR_BASE,	ARMADAXP_IRQ_TIMER0 },
    { ARMADAXP(MV78160), "com",    0, MVSOC_COM0_BASE,	ARMADAXP_IRQ_UART0 },
    { ARMADAXP(MV78160), "com",    1, MVSOC_COM1_BASE,	ARMADAXP_IRQ_UART1 },
    { ARMADAXP(MV78160), "com",    2, ARMADAXP_COM2_BASE,ARMADAXP_IRQ_UART2 },
    { ARMADAXP(MV78160), "com",    3, ARMADAXP_COM3_BASE,ARMADAXP_IRQ_UART3 },
    { ARMADAXP(MV78160), "mvsocrtc",0,ARMADAXP_RTC_BASE,ARMADAXP_IRQ_RTC },
    { ARMADAXP(MV78160), "gttwsi", 0, MVSOC_TWSI_BASE,	ARMADAXP_IRQ_TWSI0 },
    { ARMADAXP(MV78160), "gttwsi", 1, ARMADAXP_TWSI1_BASE,ARMADAXP_IRQ_TWSI1 },
    { ARMADAXP(MV78160), "gtidmac",0, ARMADAXP_XORE0_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78160), "gtidmac",1, ARMADAXP_XORE1_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78160), "ehci",   0, ARMADAXP_USB0_BASE,ARMADAXP_IRQ_USB0 },
    { ARMADAXP(MV78160), "ehci",   1, ARMADAXP_USB1_BASE,ARMADAXP_IRQ_USB1 },
    { ARMADAXP(MV78160), "ehci",   2, ARMADAXP_USB2_BASE,ARMADAXP_IRQ_USB2 },
    { ARMADAXP(MV78160), "mvpex",  0, MVSOC_PEX_BASE,	ARMADAXP_IRQ_PEX00 },
    { ARMADAXP(MV78160), "mvpex",  1, ARMADAXP_PEX01_BASE,ARMADAXP_IRQ_PEX01 },
    { ARMADAXP(MV78160), "mvpex",  2, ARMADAXP_PEX02_BASE,ARMADAXP_IRQ_PEX02 },
    { ARMADAXP(MV78160), "mvpex",  3, ARMADAXP_PEX03_BASE,ARMADAXP_IRQ_PEX03 },
    { ARMADAXP(MV78160), "mvpex",  4, ARMADAXP_PEX2_BASE,ARMADAXP_IRQ_PEX2 },
    { ARMADAXP(MV78160), "mvsata", 0, ARMADAXP_SATAHC_BASE,ARMADAXP_IRQ_SATA0 },
    { ARMADAXP(MV78160), "mvspi",  0, ARMADAXP_SPI_BASE,ARMADAXP_IRQ_SPI },
    { ARMADAXP(MV78160), "mvsdio", 0, ARMADAXP_SDIO_BASE,ARMADAXP_IRQ_SDIO },
    { ARMADAXP(MV78160), "mvgbec", 0, ARMADAXP_GBE0_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78160), "mvgbec", 1, ARMADAXP_GBE1_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78160), "mvgbec", 2, ARMADAXP_GBE2_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78160), "mvgbec", 3, ARMADAXP_GBE3_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78160), "mvcesa", 0, ARMADAXP_CESA0_BASE,ARMADAXP_IRQ_CESA0 },
    { ARMADAXP(MV78160), "mvcesa", 1, ARMADAXP_CESA1_BASE,ARMADAXP_IRQ_CESA1 },

    { ARMADAXP(MV78230), "mvsoctmr",0,MVSOC_TMR_BASE,	ARMADAXP_IRQ_TIMER0 },
    { ARMADAXP(MV78230), "com",    0, MVSOC_COM0_BASE,	ARMADAXP_IRQ_UART0 },
    { ARMADAXP(MV78230), "com",    1, MVSOC_COM1_BASE,	ARMADAXP_IRQ_UART1 },
    { ARMADAXP(MV78230), "com",    2, ARMADAXP_COM2_BASE,ARMADAXP_IRQ_UART2 },
    { ARMADAXP(MV78230), "com",    3, ARMADAXP_COM3_BASE,ARMADAXP_IRQ_UART3 },
    { ARMADAXP(MV78230), "mvsocrtc",0,ARMADAXP_RTC_BASE,ARMADAXP_IRQ_RTC },
    { ARMADAXP(MV78230), "gttwsi", 0, MVSOC_TWSI_BASE,	ARMADAXP_IRQ_TWSI0 },
    { ARMADAXP(MV78230), "gttwsi", 1, ARMADAXP_TWSI1_BASE,ARMADAXP_IRQ_TWSI1 },
    { ARMADAXP(MV78230), "gtidmac",0, ARMADAXP_XORE0_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78230), "gtidmac",1, ARMADAXP_XORE1_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78230), "ehci",   0, ARMADAXP_USB0_BASE,ARMADAXP_IRQ_USB0 },
    { ARMADAXP(MV78230), "ehci",   1, ARMADAXP_USB1_BASE,ARMADAXP_IRQ_USB1 },
    { ARMADAXP(MV78230), "ehci",   2, ARMADAXP_USB2_BASE,ARMADAXP_IRQ_USB2 },
    { ARMADAXP(MV78230), "mvpex",  0, MVSOC_PEX_BASE,	ARMADAXP_IRQ_PEX00 },
    { ARMADAXP(MV78230), "mvpex",  1, ARMADAXP_PEX01_BASE,ARMADAXP_IRQ_PEX01 },
    { ARMADAXP(MV78230), "mvpex",  2, ARMADAXP_PEX02_BASE,ARMADAXP_IRQ_PEX02 },
    { ARMADAXP(MV78230), "mvpex",  3, ARMADAXP_PEX03_BASE,ARMADAXP_IRQ_PEX03 },
    { ARMADAXP(MV78230), "mvpex",  4, ARMADAXP_PEX2_BASE,ARMADAXP_IRQ_PEX2 },
    { ARMADAXP(MV78230), "mvsata", 0, ARMADAXP_SATAHC_BASE,ARMADAXP_IRQ_SATA0 },
    { ARMADAXP(MV78230), "mvspi",  0, ARMADAXP_SPI_BASE,ARMADAXP_IRQ_SPI },
    { ARMADAXP(MV78230), "mvsdio", 0, ARMADAXP_SDIO_BASE,ARMADAXP_IRQ_SDIO },
    { ARMADAXP(MV78230), "mvgbec", 0, ARMADAXP_GBE0_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78230), "mvgbec", 1, ARMADAXP_GBE1_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78230), "mvgbec", 2, ARMADAXP_GBE2_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78230), "mvcesa", 0, ARMADAXP_CESA0_BASE,ARMADAXP_IRQ_CESA0 },
    { ARMADAXP(MV78230), "mvcesa", 1, ARMADAXP_CESA1_BASE,ARMADAXP_IRQ_CESA1 },

    { ARMADAXP(MV78260), "mvsoctmr",0,MVSOC_TMR_BASE,	ARMADAXP_IRQ_TIMER0 },
    { ARMADAXP(MV78260), "com",    0, MVSOC_COM0_BASE,	ARMADAXP_IRQ_UART0 },
    { ARMADAXP(MV78260), "com",    1, MVSOC_COM1_BASE,	ARMADAXP_IRQ_UART1 },
    { ARMADAXP(MV78260), "com",    2, ARMADAXP_COM2_BASE,ARMADAXP_IRQ_UART2 },
    { ARMADAXP(MV78260), "com",    3, ARMADAXP_COM3_BASE,ARMADAXP_IRQ_UART3 },
    { ARMADAXP(MV78260), "mvsocrtc",0,ARMADAXP_RTC_BASE,ARMADAXP_IRQ_RTC },
    { ARMADAXP(MV78260), "gttwsi", 0, MVSOC_TWSI_BASE,	ARMADAXP_IRQ_TWSI0 },
    { ARMADAXP(MV78260), "gttwsi", 1, ARMADAXP_TWSI1_BASE,ARMADAXP_IRQ_TWSI1 },
    { ARMADAXP(MV78260), "gtidmac",0, ARMADAXP_XORE0_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78260), "gtidmac",1, ARMADAXP_XORE1_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78260), "ehci",   0, ARMADAXP_USB0_BASE,ARMADAXP_IRQ_USB0 },
    { ARMADAXP(MV78260), "ehci",   1, ARMADAXP_USB1_BASE,ARMADAXP_IRQ_USB1 },
    { ARMADAXP(MV78260), "ehci",   2, ARMADAXP_USB2_BASE,ARMADAXP_IRQ_USB2 },
    { ARMADAXP(MV78260), "mvpex",  0, MVSOC_PEX_BASE,	ARMADAXP_IRQ_PEX00 },
    { ARMADAXP(MV78260), "mvpex",  1, ARMADAXP_PEX01_BASE,ARMADAXP_IRQ_PEX01 },
    { ARMADAXP(MV78260), "mvpex",  2, ARMADAXP_PEX02_BASE,ARMADAXP_IRQ_PEX02 },
    { ARMADAXP(MV78260), "mvpex",  3, ARMADAXP_PEX03_BASE,ARMADAXP_IRQ_PEX03 },
    { ARMADAXP(MV78260), "mvpex",  4, ARMADAXP_PEX2_BASE,ARMADAXP_IRQ_PEX2 },
    { ARMADAXP(MV78260), "mvsata", 0, ARMADAXP_SATAHC_BASE,ARMADAXP_IRQ_SATA0 },
    { ARMADAXP(MV78260), "mvspi",  0, ARMADAXP_SPI_BASE,ARMADAXP_IRQ_SPI },
    { ARMADAXP(MV78260), "mvsdio", 0, ARMADAXP_SDIO_BASE,ARMADAXP_IRQ_SDIO },
    { ARMADAXP(MV78260), "mvgbec", 0, ARMADAXP_GBE0_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78260), "mvgbec", 1, ARMADAXP_GBE1_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78260), "mvgbec", 2, ARMADAXP_GBE2_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78260), "mvgbec", 3, ARMADAXP_GBE3_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78260), "mvcesa", 0, ARMADAXP_CESA0_BASE,ARMADAXP_IRQ_CESA0 },
    { ARMADAXP(MV78260), "mvcesa", 1, ARMADAXP_CESA1_BASE,ARMADAXP_IRQ_CESA1 },

    { ARMADAXP(MV78460), "mvsoctmr",0,MVSOC_TMR_BASE,	ARMADAXP_IRQ_TIMER0 },
    { ARMADAXP(MV78460), "com",    0, MVSOC_COM0_BASE,	ARMADAXP_IRQ_UART0 },
    { ARMADAXP(MV78460), "com",    1, MVSOC_COM1_BASE,	ARMADAXP_IRQ_UART1 },
    { ARMADAXP(MV78460), "com",    2, ARMADAXP_COM2_BASE,ARMADAXP_IRQ_UART2 },
    { ARMADAXP(MV78460), "com",    3, ARMADAXP_COM3_BASE,ARMADAXP_IRQ_UART3 },
    { ARMADAXP(MV78460), "mvsocrtc",0,ARMADAXP_RTC_BASE,ARMADAXP_IRQ_RTC },
    { ARMADAXP(MV78460), "gttwsi", 0, MVSOC_TWSI_BASE,	ARMADAXP_IRQ_TWSI0 },
    { ARMADAXP(MV78460), "gttwsi", 1, ARMADAXP_TWSI1_BASE,ARMADAXP_IRQ_TWSI1 },
    { ARMADAXP(MV78460), "gtidmac",0, ARMADAXP_XORE0_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78460), "gtidmac",1, ARMADAXP_XORE1_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78460), "ehci",   0, ARMADAXP_USB0_BASE,ARMADAXP_IRQ_USB0 },
    { ARMADAXP(MV78460), "ehci",   1, ARMADAXP_USB1_BASE,ARMADAXP_IRQ_USB1 },
    { ARMADAXP(MV78460), "ehci",   2, ARMADAXP_USB2_BASE,ARMADAXP_IRQ_USB2 },
    { ARMADAXP(MV78460), "mvpex",  0, MVSOC_PEX_BASE,	ARMADAXP_IRQ_PEX00 },
    { ARMADAXP(MV78460), "mvpex",  1, ARMADAXP_PEX01_BASE,ARMADAXP_IRQ_PEX01 },
    { ARMADAXP(MV78460), "mvpex",  2, ARMADAXP_PEX02_BASE,ARMADAXP_IRQ_PEX02 },
    { ARMADAXP(MV78460), "mvpex",  3, ARMADAXP_PEX03_BASE,ARMADAXP_IRQ_PEX03 },
    { ARMADAXP(MV78460), "mvpex",  4, ARMADAXP_PEX2_BASE,ARMADAXP_IRQ_PEX2 },
    { ARMADAXP(MV78460), "mvpex",  5, ARMADAXP_PEX3_BASE,ARMADAXP_IRQ_PEX3 },
    { ARMADAXP(MV78460), "mvsata", 0, ARMADAXP_SATAHC_BASE,ARMADAXP_IRQ_SATA0 },
    { ARMADAXP(MV78460), "mvspi",  0, ARMADAXP_SPI_BASE,ARMADAXP_IRQ_SPI },
    { ARMADAXP(MV78460), "mvsdio", 0, ARMADAXP_SDIO_BASE,ARMADAXP_IRQ_SDIO },
    { ARMADAXP(MV78460), "mvgbec", 0, ARMADAXP_GBE0_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78460), "mvgbec", 1, ARMADAXP_GBE1_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78460), "mvgbec", 2, ARMADAXP_GBE2_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78460), "mvgbec", 3, ARMADAXP_GBE3_BASE,IRQ_DEFAULT },
    { ARMADAXP(MV78460), "mvcesa", 0, ARMADAXP_CESA0_BASE,ARMADAXP_IRQ_CESA0 },
    { ARMADAXP(MV78460), "mvcesa", 1, ARMADAXP_CESA1_BASE,ARMADAXP_IRQ_CESA1 },
#endif
};


CFATTACH_DECL_NEW(mvsoc, sizeof(struct mvsoc_softc),
    mvsoc_match, mvsoc_attach, NULL, NULL);

/* ARGSUSED */
static int
mvsoc_match(device_t parent, struct cfdata *match, void *aux)
{

	return 1;
}

/* ARGSUSED */
static void
mvsoc_attach(device_t parent, device_t self, void *aux)
{
	struct mvsoc_softc *sc = device_private(self);
	struct marvell_attach_args mva;
	uint16_t model;
	uint8_t rev;
	int i;

	sc->sc_dev = self;
	sc->sc_iot = &mvsoc_bs_tag;
	sc->sc_addr = vtophys(regbase);
	sc->sc_dmat = &mvsoc_bus_dma_tag;
	if (bus_space_map(sc->sc_iot, sc->sc_addr, 0x100000, 0, &sc->sc_ioh) !=
	    0) {
		aprint_error_dev(self, "can't map registers\n");
		return;
	}

	model = mvsoc_model();
	rev = mvsoc_rev();
	for (i = 0; i < __arraycount(nametbl); i++)
		if (nametbl[i].model == model && nametbl[i].rev == rev)
			break;
	if (i >= __arraycount(nametbl))
		panic("unknown SoC: model 0x%04x, rev 0x%02x", model, rev);

	aprint_normal(": Marvell %s %s%s  %s\n",
	    nametbl[i].modelstr,
	    nametbl[i].revstr != NULL ? "Rev. " : "",
	    nametbl[i].revstr != NULL ? nametbl[i].revstr : "",
	    nametbl[i].typestr);
        aprint_normal("%s: CPU Clock %d.%03d MHz"
	    "  SysClock %d.%03d MHz  TClock %d.%03d MHz\n",
	    device_xname(self),
	    mvPclk / 1000000, (mvPclk / 1000) % 1000,
	    mvSysclk / 1000000, (mvSysclk / 1000) % 1000,
	    mvTclk / 1000000, (mvTclk / 1000) % 1000);
	aprint_naive("\n");

	mvsoc_intr_init();

	for (i = 0; i < __arraycount(mvsoc_periphs); i++) {
		if (mvsoc_periphs[i].model != model)
			continue;

		mva.mva_name = mvsoc_periphs[i].name;
		mva.mva_model = model;
		mva.mva_revision = rev;
		mva.mva_iot = sc->sc_iot;
		mva.mva_ioh = sc->sc_ioh;
		mva.mva_unit = mvsoc_periphs[i].unit;
		mva.mva_addr = sc->sc_addr;
		mva.mva_offset = mvsoc_periphs[i].offset;
		mva.mva_size = 0;
		mva.mva_dmat = sc->sc_dmat;
		mva.mva_irq = mvsoc_periphs[i].irq;

		/* Skip clock disabled devices */
		if (mvsoc_clkgating != NULL && mvsoc_clkgating(&mva)) {
			aprint_normal_dev(self, "%s%d clock disabled\n",
			    mvsoc_periphs[i].name, mvsoc_periphs[i].unit);
			continue;
		}

		config_found_sm_loc(sc->sc_dev, "mvsoc", NULL, &mva,
		    mvsoc_print, mvsoc_search);
	}
}

static int
mvsoc_print(void *aux, const char *pnp)
{
	struct marvell_attach_args *mva = aux;

	if (pnp)
		aprint_normal("%s at %s unit %d",
		    mva->mva_name, pnp, mva->mva_unit);
	else {
		if (mva->mva_unit != MVA_UNIT_DEFAULT)
			aprint_normal(" unit %d", mva->mva_unit);
		if (mva->mva_offset != MVA_OFFSET_DEFAULT) {
			aprint_normal(" offset 0x%04lx", mva->mva_offset);
			if (mva->mva_size > 0)
				aprint_normal("-0x%04lx",
				    mva->mva_offset + mva->mva_size - 1);
		}
		if (mva->mva_irq != MVA_IRQ_DEFAULT)
			aprint_normal(" irq %d", mva->mva_irq);
	}

	return UNCONF;
}

/* ARGSUSED */
static int
mvsoc_search(device_t parent, cfdata_t cf, const int *ldesc, void *aux)
{

	return config_match(parent, cf, aux);
}

/* ARGSUSED */
int
marvell_winparams_by_tag(device_t dev, int tag, int *target, int *attribute,
			 uint64_t *base, uint32_t *size)
{
	uint32_t base32;
	int rv;

	rv = mvsoc_target(tag, target, attribute, &base32, size);
	*base = base32;
	if (rv == -1)
		return -1;
	return 0;
}


/*
 * These functions is called before bus_space is initialized.
 */

void
mvsoc_bootstrap(bus_addr_t iobase)
{

	regbase = iobase;
	dsc_base = iobase + MVSOC_DSC_BASE;
	mlmb_base = iobase + MVSOC_MLMB_BASE;
	pex_base = iobase + MVSOC_PEX_BASE;
#ifdef MVSOC_CONSOLE_EARLY
	com_base = iobase + MVSOC_COM0_BASE;
	cn_tab = &mvsoc_earlycons;
	printf("Hello\n");
#endif
}

/*
 * We can read register of PCI configurations from (MVSOC_PEX_BASE + 0).
 */
uint16_t
mvsoc_model(void)
{
	/*
	 * We read product-id from vendor/device register of PCI-Express.
	 */
	uint32_t reg;
	uint16_t model;

	KASSERT(regbase != 0xffffffff);

	reg = *(volatile uint32_t *)(pex_base + PCI_ID_REG);
	model = PCI_PRODUCT(reg);

#if defined(ORION)
	if (model == PCI_PRODUCT_MARVELL_88F5182) {
		reg = *(volatile uint32_t *)(regbase + ORION_PMI_BASE +
		    ORION_PMI_SAMPLE_AT_RESET);
		if ((reg & ORION_PMISMPL_TCLK_MASK) == 0)
			model = PCI_PRODUCT_MARVELL_88F5082;
	}
#endif
#if defined(KIRKWOOD)
	if (model == PCI_PRODUCT_MARVELL_88F6281) {
		reg = *(volatile uint32_t *)(regbase + KIRKWOOD_MISC_BASE +
		    KIRKWOOD_MISC_DEVICEID);
		if (reg == 1)	/* 88F6192 is 1 */
			model = MARVELL_KIRKWOOD_88F6192;
	}
#endif

	return model;
}

uint8_t
mvsoc_rev(void)
{
	uint32_t reg;
	uint8_t rev;

	KASSERT(regbase != 0xffffffff);

	reg = *(volatile uint32_t *)(pex_base + PCI_CLASS_REG);
	rev = PCI_REVISION(reg);

	return rev;
}


int
mvsoc_target(int tag, uint32_t *target, uint32_t *attr, uint32_t *base,
	     uint32_t *size)
{
	int i;

	KASSERT(regbase != 0xffffffff);

	if (tag == MVSOC_TAG_INTERNALREG) {
		if (target != NULL)
			*target = 0;
		if (attr != NULL)
			*attr = 0;
		if (base != NULL)
			*base = read_mlmbreg(MVSOC_MLMB_IRBAR) &
			    MVSOC_MLMB_IRBAR_BASE_MASK;
		if (size != NULL)
			*size = 0;

		return 0;
	}

	/* sanity check */
	for (i = 0; i < __arraycount(mvsoc_tags); i++)
		if (mvsoc_tags[i].tag == tag)
			break;
	if (i >= __arraycount(mvsoc_tags))
		return -1;

	if (target != NULL)
		*target = mvsoc_tags[i].target;
	if (attr != NULL)
		*attr = mvsoc_tags[i].attr;

	if (mvsoc_tags[i].target == MVSOC_UNITID_DDR) {
		/*
		 * Read DDR SDRAM Controller Address Decode Registers
		 */
		uint32_t baseaddrreg, sizereg;
		int cs = 0;

		switch (mvsoc_tags[i].attr) {
		case MARVELL_ATTR_SDRAM_CS0:
			cs = 0;
			break;
		case MARVELL_ATTR_SDRAM_CS1:
			cs = 1;
			break;
		case MARVELL_ATTR_SDRAM_CS2:
			cs = 2;
			break;
		case MARVELL_ATTR_SDRAM_CS3:
			cs = 3;
			break;
		}
		sizereg = *(volatile uint32_t *)(dsc_base + MVSOC_DSC_CSSR(cs));
		if (sizereg & MVSOC_DSC_CSSR_WINEN) {
			baseaddrreg = *(volatile uint32_t *)(dsc_base +
			    MVSOC_DSC_CSBAR(cs));

			if (base != NULL)
				*base = baseaddrreg & MVSOC_DSC_CSBAR_BASE_MASK;
			if (size != NULL)
				*size = (sizereg & MVSOC_DSC_CSSR_SIZE_MASK) +
				    (~MVSOC_DSC_CSSR_SIZE_MASK + 1);
		} else {
			if (base != NULL)
				*base = 0;
			if (size != NULL)
				*size = 0;
		}
		return 0;
	} else {
		/*
		 * Read CPU Address Map Registers
		 */
		uint32_t basereg, ctrlreg, ta, tamask;

		ta = MVSOC_MLMB_WCR_TARGET(mvsoc_tags[i].target) |
		    MVSOC_MLMB_WCR_ATTR(mvsoc_tags[i].attr);
		tamask = MVSOC_MLMB_WCR_TARGET(MVSOC_UNITID_MASK) |
		    MVSOC_MLMB_WCR_ATTR(MARVELL_ATTR_MASK);

		if (base != NULL)
			*base = 0;
		if (size != NULL)
			*size = 0;

		for (i = 0; i < nwindow; i++) {
			ctrlreg = read_mlmbreg(MVSOC_MLMB_WCR(i));
			if ((ctrlreg & tamask) != ta)
				continue;
			if (ctrlreg & MVSOC_MLMB_WCR_WINEN) {
				basereg = read_mlmbreg(MVSOC_MLMB_WBR(i));

				if (base != NULL)
					*base =
					    basereg & MVSOC_MLMB_WBR_BASE_MASK;
				if (size != NULL)
					*size = (ctrlreg &
					    MVSOC_MLMB_WCR_SIZE_MASK) +
					    (~MVSOC_MLMB_WCR_SIZE_MASK + 1);
			}
			break;
		}
		return i;
	}
}
