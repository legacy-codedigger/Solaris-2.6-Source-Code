	.ident	"@(#)modctl.s	1.7	96/06/01 SMI"


	.file	"modctl.s"

	.text

	.globl	modctl
	.globl	__cerror

_fgdef_(modctl):
	MCOUNT
	movl	$MODCTL,%eax
	lcall   $SYSCALL_TRAPNUM,$0
	jae	noerror
	_prologue_
_m4_ifdef_(`DSHLIB',
	`pushl	%eax',
	`'
)
	jmp	_fref_(__cerror)
noerror:
	xorl	%eax,%eax
	ret
