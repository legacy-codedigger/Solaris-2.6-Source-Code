#if !defined(lint)

	.file	"tables2.c"
	.version	"01.01"
	.ident	"@(#)kern-ml:tables2.c	1.3.2.1"
	.ident	"@(#)head.sys:sys/types.h	11.33.3.1"
	.ident	"@(#)head.sys:sys/select.h	1.2.3.1"
	.ident	"@(#)head.sys:sys/seg.h	11.1.9.1"
	.ident	"@(#)head.sys:sys/seg.h	11.1.9.1"
	.data
	.globl	idt
	.align	4
idt:
	.long	div0trap
	.long	-1895825392
	.long	dbgtrap
	.long	-1895825392
	.long	nmiint
	.long	-1912602608
	.long	brktrap
	.long	-285212656
	.long	ovflotrap
	.long	-285212656
	.long	boundstrap
	.long	-1895825392
	.long	invoptrap
	.long	-1895825392
	.long	ndptrap0
	.long	-1895825392
	.long	dbfault
	.long	-1895825392
/	.long	-2063597208
	.long	overrun
	.long	-1895825392
	.long	invtsstrap
	.long	-1895825392
	.long	segnptrap
	.long	-1895825392
	.long	stktrap
	.long	-1895825392
	.long	gptrap
	.long	-1895825392
	.long	pftrap
	.long	-1895825392
	.long	resvtrap
	.long	-1895825392
	.long	ndperr
	.long	-285212656
	.long	inval17
	.long	-1895825392
	.long	inval18
	.long	-1895825392
	.long	inval19
	.long	-1895825392
	.long	progent
	.long	-1895825392
	.long	inval21
	.long	-1895825392
	.long	inval22
	.long	-1895825392
	.long	inval23
	.long	-1895825392
	.long	inval24
	.long	-1895825392
	.long	inval25
	.long	-1895825392
	.long	inval26
	.long	-1895825392
	.long	inval27
	.long	-1895825392
	.long	inval28
	.long	-1895825392
	.long	inval29
	.long	-1895825392
	.long	inval30
	.long	-1895825392
	.long	inval31
	.long	-1895825392
	.long	ndptrap2
	.long	-285212656
	.long	inval33
	.long	-1895825392
	.long	inval34
	.long	-1895825392
	.long	inval35
	.long	-1895825392
	.long	inval36
	.long	-1895825392
	.long	inval37
	.long	-1895825392
	.long	inval38
	.long	-1895825392
	.long	inval39
	.long	-1895825392
	.long	inval40
	.long	-1895825392
	.long	inval41
	.long	-1895825392
	.long	inval42
	.long	-1895825392
	.long	inval43
	.long	-1895825392
	.long	inval44
	.long	-1895825392
	.long	inval45
	.long	-1895825392
	.long	inval46
	.long	-1895825392
	.long	inval47
	.long	-1895825392
	.long	inval48
	.long	-1895825392
	.long	inval49
	.long	-1895825392
	.long	inval50
	.long	-1895825392
	.long	inval51
	.long	-1895825392
	.long	inval52
	.long	-1895825392
	.long	inval53
	.long	-1895825392
	.long	inval54
	.long	-1895825392
	.long	inval55
	.long	-1895825392
	.long	inval56
	.long	-1895825392
	.long	inval57
	.long	-1895825392
	.long	inval58
	.long	-1895825392
	.long	inval59
	.long	-1895825392
	.long	inval60
	.long	-1895825392
	.long	inval61
	.long	-1895825392
	.long	inval62
	.long	-1895825392
	.long	inval63
	.long	-1895825392
	.long	ivctM0
	.long	-1912602608
	.long	ivctM1
	.long	-1912602608
	.long	ivctM2
	.long	-1912602608
	.long	ivctM3
	.long	-1912602608
	.long	ivctM4
	.long	-1912602608
	.long	ivctM5
	.long	-1912602608
	.long	ivctM6
	.long	-1912602608
	.long	ivctM7
	.long	-1912602608
	.long	ivctM0S0
	.long	-1912602608
	.long	ivctM0S1
	.long	-1912602608
	.long	ivctM0S2
	.long	-1912602608
	.long	ivctM0S3
	.long	-1912602608
	.long	ivctM0S4
	.long	-1912602608
	.long	ivctM0S5
	.long	-1912602608
	.long	ivctM0S6
	.long	-1912602608
	.long	ivctM0S7
	.long	-1912602608
	.long	ivctM1S0
	.long	-1912602608
	.long	ivctM1S1
	.long	-1912602608
	.long	ivctM1S2
	.long	-1912602608
	.long	ivctM1S3
	.long	-1912602608
	.long	ivctM1S4
	.long	-1912602608
	.long	ivctM1S5
	.long	-1912602608
	.long	ivctM1S6
	.long	-1912602608
	.long	ivctM1S7
	.long	-1912602608
	.long	ivctM2S0
	.long	-1912602608
	.long	ivctM2S1
	.long	-1912602608
	.long	ivctM2S2
	.long	-1912602608
	.long	ivctM2S3
	.long	-1912602608
	.long	ivctM2S4
	.long	-1912602608
	.long	ivctM2S5
	.long	-1912602608
	.long	ivctM2S6
	.long	-1912602608
	.long	ivctM2S7
	.long	-1912602608
	.long	ivctM3S0
	.long	-1912602608
	.long	ivctM3S1
	.long	-1912602608
	.long	ivctM3S2
	.long	-1912602608
	.long	ivctM3S3
	.long	-1912602608
	.long	ivctM3S4
	.long	-1912602608
	.long	ivctM3S5
	.long	-1912602608
	.long	ivctM3S6
	.long	-1912602608
	.long	ivctM3S7
	.long	-1912602608
	.long	ivctM4S0
	.long	-1912602608
	.long	ivctM4S1
	.long	-1912602608
	.long	ivctM4S2
	.long	-1912602608
	.long	ivctM4S3
	.long	-1912602608
	.long	ivctM4S4
	.long	-1912602608
	.long	ivctM4S5
	.long	-1912602608
	.long	ivctM4S6
	.long	-1912602608
	.long	ivctM4S7
	.long	-1912602608
	.long	ivctM5S0
	.long	-1912602608
	.long	ivctM5S1
	.long	-1912602608
	.long	ivctM5S2
	.long	-1912602608
	.long	ivctM5S3
	.long	-1912602608
	.long	ivctM5S4
	.long	-1912602608
	.long	ivctM5S5
	.long	-1912602608
	.long	ivctM5S6
	.long	-1912602608
	.long	ivctM5S7
	.long	-1912602608
	.long	ivctM6S0
	.long	-1912602608
	.long	ivctM6S1
	.long	-1912602608
	.long	ivctM6S2
	.long	-1912602608
	.long	ivctM6S3
	.long	-1912602608
	.long	ivctM6S4
	.long	-1912602608
	.long	ivctM6S5
	.long	-1912602608
	.long	ivctM6S6
	.long	-1912602608
	.long	ivctM6S7
	.long	-1912602608
	.long	ivctM7S0
	.long	-1912602608
	.long	ivctM7S1
	.long	-1912602608
	.long	ivctM7S2
	.long	-1912602608
	.long	ivctM7S3
	.long	-1912602608
	.long	ivctM7S4
	.long	-1912602608
	.long	ivctM7S5
	.long	-1912602608
	.long	ivctM7S6
	.long	-1912602608
	.long	ivctM7S7
	.long	-1912602608
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.long	invaltrap
	.long	-1895825392
	.type	idt,@object
	.size	idt,2048
	.globl	gdt
gdt:
	.long	0
	.long	0
	.long	gdt
	.long	-1828715825
	.long	idt
	.long	-1845491713
	.long	1280
	.long	-2113929121
	.long	1600
	.long	-1996488601
	.long	0
	.long	0
	.long	1472
	.long	-385875865
	.long	3584
	.long	-218103445
	.long	-26758
	.long	-33554297
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	-2113927161
	.long	0
	.long	-1996488704
	.long	0
	.long	-1996488704
	.long	0
	.long	-1697644545
	.long	0
	.long	-1831862273
	.long	0
	.long	-1996488704
	.long	0
	.long	-1996488704
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	-1706033153
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.zero	280
	.type	gdt,@object
	.size	gdt,720
	.globl	gdtend
gdtend:
	.long	0
	.type	gdtend,@object
	.size	gdtend,4
	.ident	"acomp: (SCDE) 5.0  02/22/91"
/REGAL	0	EXTDEF	gdtend	4

#endif	/* !defined(lint) */
