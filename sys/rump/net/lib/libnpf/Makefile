#	$NetBSD: Makefile,v 1.9 2013/11/08 00:38:27 rmind Exp $
#
# Public Domain.
#

.PATH:	${.CURDIR}/../../../../net/npf

LIB=	rumpnet_npf

SRCS=	npf.c npf_alg.c npf_conf.c npf_ctl.c npf_handler.c
SRCS+=	npf_bpf.c npf_if.c npf_inet.c npf_mbuf.c npf_nat.c
SRCS+=	npf_ruleset.c npf_rproc.c npf_sendpkt.c npf_session.c
SRCS+=	npf_state.c npf_state_tcp.c npf_tableset.c
SRCS+=	npf_tableset_ptree.c npf_worker.c if_npflog.c

SRCS+=	npf_alg_icmp.c

SRCS+=	npf_ext_log.c npf_ext_normalize.c npf_ext_rndblock.c

SRCS+=	component.c

CPPFLAGS+=	-D_NPF_TESTING
CPPFLAGS+=	-I${.CURDIR}/../../../librump/rumpvfs

.include <bsd.lib.mk>
.include <bsd.klinks.mk>
