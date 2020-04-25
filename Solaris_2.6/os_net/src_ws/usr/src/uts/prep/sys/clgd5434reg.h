/*
 * Copyright 1988-1989, Sun Microsystems, Inc.
 */

#ifndef	_SYS_CLGD5434REG_H
#define	_SYS_CLGD5434REG_H

#pragma ident	"@(#)clgd5434reg.h	1.8	96/03/15 SMI"

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * General registers - 3c0-3df set
 */
#define	CLGD5434_HDR	0x06
#define		CLGD5434_HDR_ENABLE_555			0x80
#define		CLGD5434_HDR_ENABLE_EXT			0x40
#define		CLGD5434_HDR_EXT_SEL_MASK		0x0f
#define		CLGD5434_HDR_EXT_SEL_PALETTE_85MHZ	0x0a

/*
 * CRT Controller registers
 */
#define	CLGD5434_CRTC_CR19	0x19
#define	CLGD5434_CRTC_CR1A	0x1a
#define		CLGD5434_CRTC_CR1A_INTERLACED		0x01
#define		CLGD5434_CRTC_CR1A_NO_INTERLACE		0x00
#define		CLGD5434_CRTC_CR1A_HBE_67		4
#define		CLGD5434_CRTC_CR1A_VBE_89		6
#define		CLGD5434_CRTC_CR1A_PUT_HBE(n) \
			((((n)>>6)&3)<<CLGD5434_CRTC_CR1A_HBE_67)
#define		CLGD5434_CRTC_CR1A_PUT_VBE(n) \
			((((n)>>8)&3)<<CLGD5434_CRTC_CR1A_VBE_89)
#define	CLGD5434_CRTC_CR1B	0x1b
#define		CLGD5434_CRTC_CR1B_EXT_START16		0
#define		CLGD5434_CRTC_CR1B_ENABLE_EXT_ADR_WRAP	0x02
#define		CLGD5434_CRTC_CR1B_EXT_START17		2
#define		CLGD5434_CRTC_CR1B_EXT_START18		3
#define		CLGD5434_CRTC_CR1B_OFFSET8		4
#define		CLGD5434_CRTC_CR1B_BLANKING		0x20
#define		CLGD5434_CRTC_CR1B_PUT_OFFSET(n) \
			((((n)>>8)&1)<<CLGD5434_CRTC_CR1B_OFFSET8)
#define	CLGD5434_CRTC_CR27	0x27
#define		CLGD5434_CRTC_CR27_ID			2

/*
 * Sequencer registers
 */
#define	CLGD5434_SEQ_SR6	0x06
#define		CLGD5434_SEQ_SR6_KEY			0x12
#define	CLGD5434_SEQ_SR7	0x07
#define		CLGD5434_SEQ_SR7_PCI_LINEAR		0x10
#define		CLGD5434_SEQ_SR7_CLOCK			0x0e
#define			CLGD5434_SEQ_SR7_NORMAL_CLOCK	0x00
#define			CLGD5434_SEQ_SR7_CLOCK_DIV2	0x02
#define			CLGD5434_SEQ_SR7_CLOCK_DIV3	0x04
#define			CLGD5434_SEQ_SR7_CLOCK_16BPP	0x06
#define			CLGD5434_SEQ_SR7_CLOCK_32BPP	0x08
#define		CLGD5434_SEQ_SR7_HIRES			0x01
#define	CLGD5434_SEQ_SR12	0x12
#define		CLGD5434_SEQ_SR12_CURSOR_ENABLE		0x01
#define	CLGD5434_SEQ_SR1F	0x1f

/*
 * Graphics Controller
 */
#define	CLGD5434_GRC_GRB	0x0b
#define	CLGD5434_GRC_GR31	0x31
#define		CLGD5434_GRC_GR31_BLT_STATUS		0x01
#define		CLGD5434_GRC_GR31_BLT_GO		0x02
#define		CLGD5434_GRC_GR31_BLT_RESET		0x04
#define		CLGD5434_GRC_GR31_BLT_PROGRESS		0x08

/*
 * Virtual (mmap offsets) addresses
 */
#define	CLGD5434_VADDR_FB	0
#define	CLGD5434_VADDR_MMIO	0x10000000

#define	CLGD5434_MMIO_SZ	0x20000

/*
 * Device-specific ioctls
 */
#define	XIOC	('X'<<8)
#define	CLGD5434_LOCK	(XIOC|0)
#define	CLGD5434_UNLOCK	(XIOC|1)

#ifdef	__cplusplus
}
#endif

#endif /* _SYS_CLGD5434REG_H */