	.ident	"@(#)inst_sync.s	1.2	96/06/01 SMI"

	.file	"inst_sync.s"

	.text

	.globl	__cerror

_fwdef_(`inst_sync'):
	MCOUNT			/ subroutine entry counter if profiling
	movl	$INST_SYNC,%eax
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
