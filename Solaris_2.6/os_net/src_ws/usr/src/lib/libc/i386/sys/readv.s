	.ident	"@(#)readv.s	1.8	96/06/01 SMI"

/ gid = readv();
/ returns effective gid

	.file	"readv.s"

	.text

	.globl  __cerror

_fwdef_(`readv'):
	MCOUNT
	movl	$READV,%eax
	lcall   $SYSCALL_TRAPNUM,$0
	jae	noerror
	cmpb	$ERESTART,%al
	je	readv
	_prologue_
_m4_ifdef_(`DSHLIB',
	`pushl	%eax',
	`'
)
	jmp	_fref_(__cerror)
noerror:
	ret
