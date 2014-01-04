/*	$NetBSD: fhc_central.c,v 1.3 2012/03/18 05:26:58 mrg Exp $	*/
/*	$OpenBSD: fhc_central.c,v 1.5 2004/09/27 18:32:35 jason Exp $	*/

/*
 * Copyright (c) 2004 Jason L. Wright (jason@thought.net).
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
__KERNEL_RCSID(0, "$NetBSD: fhc_central.c,v 1.3 2012/03/18 05:26:58 mrg Exp $");

#include <sys/types.h>
#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/device.h>
#include <sys/conf.h>
#include <sys/bus.h>

#include <machine/autoconf.h>
#include <machine/openfirm.h>

#include <sparc64/dev/centralvar.h>
#include <sparc64/dev/fhcreg.h>
#include <sparc64/dev/fhcvar.h>

int	fhc_central_match(device_t, cfdata_t, void *);
void	fhc_central_attach(device_t, device_t, void *);

CFATTACH_DECL_NEW(fhc_central, sizeof(struct fhc_softc),
    fhc_central_match, fhc_central_attach, NULL, NULL);

int
fhc_central_match(device_t parent, cfdata_t match, void *aux)
{
	struct central_attach_args *ca = aux;

	if (strcmp(ca->ca_name, "fhc") == 0)
		return (1);
	return (0);
}

void
fhc_central_attach(device_t parent, device_t self, void *aux)
{
	struct fhc_softc *sc = device_private(self);
	struct central_attach_args *ca = aux;
	u_int32_t board;

	sc->sc_node = ca->ca_node;
	sc->sc_bt = ca->ca_bustag;
	sc->sc_is_central = 1;

	if (central_bus_map(sc->sc_bt, ca->ca_reg[0].cbr_slot,
	    ca->ca_reg[0].cbr_offset, ca->ca_reg[0].cbr_size, 0,
	    &sc->sc_preg)) {
		printf(": failed to map preg\n");
		return;
	}

	if (central_bus_map(sc->sc_bt, ca->ca_reg[1].cbr_slot,
	    ca->ca_reg[1].cbr_offset, ca->ca_reg[1].cbr_size, 0,
	    &sc->sc_ireg)) {
		printf(": failed to map ireg\n");
		return;
	}

	if (central_bus_map(sc->sc_bt, ca->ca_reg[2].cbr_slot,
	    ca->ca_reg[2].cbr_offset, ca->ca_reg[2].cbr_size,
	    BUS_SPACE_MAP_LINEAR, &sc->sc_freg)) {
		printf(": failed to map freg\n");
		return;
	}

	if (central_bus_map(sc->sc_bt, ca->ca_reg[3].cbr_slot,
	    ca->ca_reg[3].cbr_offset, ca->ca_reg[3].cbr_size,
	    BUS_SPACE_MAP_LINEAR, &sc->sc_sreg)) {
		printf(": failed to map sreg\n");
		return;
	}

	if (central_bus_map(sc->sc_bt, ca->ca_reg[4].cbr_slot,
	    ca->ca_reg[4].cbr_offset, ca->ca_reg[4].cbr_size,
	    BUS_SPACE_MAP_LINEAR, &sc->sc_ureg)) {
		printf(": failed to map ureg\n");
		return;
	}

	if (central_bus_map(sc->sc_bt, ca->ca_reg[5].cbr_slot,
	    ca->ca_reg[5].cbr_offset, ca->ca_reg[5].cbr_size,
	    BUS_SPACE_MAP_LINEAR, &sc->sc_treg)) {
		printf(": failed to map treg\n");
		return;
	}

	board = bus_space_read_4(sc->sc_bt, sc->sc_preg, FHC_P_BSR);
	sc->sc_board = ((board >> 16) & 0x1) | ((board >> 12) & 0xe);
	sc->sc_dev = self;

	fhc_attach(sc);
	return;
}
