/*
 * This is an SMC LMAC driver.  It will periodically be updated with
 * new versions from SMC.  It is important that minimal changes be
 * made to this file, to facilitate maintenance and running diffs with
 * new versions as they arrive.  DO NOT cstyle or lint this file, or
 * make any other unnecessary changes.
 *
 * This file should be identical in the PS and Realmode source trees.
 */

#ident	"@(#)ebm.h 1.2	95/07/18 SMI"

/*---

       ETH_EISA.H    (C) Copyright 1992 Standard Microsystems Corp.
                       All rights reserved.

       LMI lower routine includes for
       Ethernet EISA adapter.


---*/

#define	EISA_CONFIG          0
#define	EISA_DMA             0x10
#define	EISA_790             0x20
#define  EBM_PAGE_SIZE			0x2000
#define  DMA_CUT_OFF	     32 

/* eisa config registers
*/

#define	EISA_ID1             EISA_CONFIG+0
#define	EISA_ID2             EISA_CONFIG+1
#define	EISA_ID3             EISA_CONFIG+2
#define	EISA_ID4             EISA_CONFIG+3
#define	EISA_EBC             EISA_CONFIG+4
#define	EISA_RIML            EISA_CONFIG+6
#define	EISA_RIMH            EISA_CONFIG+7
#define	EISA_RAM             EISA_CONFIG+8
#define	EISA_ROM             EISA_CONFIG+9
#define	EISA_INT             EISA_CONFIG+0x0a
#define	EISA_STAT            EISA_CONFIG+0x0b
#define	EISA_TOUT            EISA_CONFIG+0x0c
#define	EISA_TRC             EISA_CONFIG+0x0d
#define	EISA_EE              EISA_CONFIG+0x0e

/* eisa dma registers
*/

#define	EISA_AA              EISA_DMA+0
#define	EISA_AS              EISA_DMA+4



/* config register values
*/

#define	EISA_ENB             0x01

#define	EISA_RAM_MASK        0x0fc
#define	EISA_RAM_PAGE_MASK   0x003

#define	EISA_ROM_MASK        0x0f0
#define	EISA_ROM_PAGE_MASK   0x007



/* EISA_INT
*/

#define	EISA_CLRD            0x80
#define	EISA_MENB            0x40
#define	EISA_ROMEN           0x20
#define	EISA_G4              0x10
#define	EISA_POL             0x08
#define	EISA_IRQ_MASK        0x07



/* EISA_STAT
*/

#define	EISA_EIL             0x80
#define	EISA_DIEN            0x40
#define	EISA_STATI           0x20
#define	EISA_STATD           0x10
#define	EISA_RNIC            0x08
#define	EISA_SDMA            0x02



/* EISA_TOUT
*/

#define	EISA_TEISA           0x10
#define	EISA_TNIC1           0x08
#define	EISA_TNIC0           0x04



/* EISA_TRC
*/

#define	EISA_RP3             0x80
#define	EISA_RP2             0x40
#define	EISA_HWCS            0x08


/* read only
*/

#define	EISA_GIN1            0x02
#define	EISA_GIN0            0x01


/* write only
*/

#define	EISA_CLRI            0x04
#define	EISA_CA              0x01



/* EISA_EE
*/

#define	EISA_SA16            0x80
#define	EISA_SZ15            0x40
#define	EISA_SZ14            0x20
#define	EISA_IM16            0x10
#define	EISA_EECS            0x08
#define	EISA_EESK            0x04
#define	EISA_EEDI            0x02
#define	EISA_EEDO            0x01



/* EISA_DMA
*/

#define	EISA_SZ_SDMA         0x8000
#define	EISA_SZ_DIR          0x4000



/* Misc defs...
*/

#define	CRC_SIZE					4
#define	NIC_HEADER_SIZE		4		// Size of NIC header
#define	TX_BUF_ONE				1
#define	TX_BUF_TWO				2



#define	START_TX_THRESHOLD	20
#define	START_RX_THRESHOLD	64
#define	DEFAULT_TX_RETRIES	3

#define	MANUF_ID             0x04da3
#ifndef EISA_BRD_ID
#define	EISA_BRD_ID          0x08010
#endif
