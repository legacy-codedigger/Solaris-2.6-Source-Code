	.ident	"@(#)seteguid.s	1.8	96/06/01 SMI"


	.file	"seteguid.s"

	.text

	.globl	__cerror

_fwdef_(`setegid'):
	MCOUNT			/ subroutine entry counter if profiling
	movl	$SETEGID,%eax
	lcall   $SYSCALL_TRAPNUM,$0
	jae	noerror
	_prologue_
_m4_ifdef_(`DSHLIB',
	`pushl	%eax',
	`'
)
	jmp	_fref_(__cerror)
noerror:
	ret

_fwdef_(`seteuid'):
	MCOUNT			/ subroutine entry counter if profiling
	movl	$SETEUID,%eax
	lcall   $SYSCALL_TRAPNUM,$0
	jae	noerror
	_prologue_
_m4_ifdef_(`DSHLIB',
	`pushl	%eax',
	`'
)
	jmp	_fref_(__cerror)
