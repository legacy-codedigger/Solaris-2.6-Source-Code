.ident	"@(#)fstat.s	1.10	96/06/01 SMI"

	.file	"fstat.s"

	.text

	.globl	__cerror
_m4_ifdef_(`_LARGEFILE_INTERFACE',
`_fwdef_(`fstat64'):
	MCOUNT		
	movl	$FSTAT64,%eax
	lcall   $SYSCALL_TRAPNUM,$0
	jae	noerror64
	_prologue_
_m4_ifdef_(`DSHLIB',
	`pushl	%eax',
	`'
)
	jmp	_fref_(__cerror)
noerror64:
	xorl	%eax,%eax
	ret',
`_fwdef_(`fstat'):
	MCOUNT		
	movl	$FSTAT,%eax
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
	ret'
)
