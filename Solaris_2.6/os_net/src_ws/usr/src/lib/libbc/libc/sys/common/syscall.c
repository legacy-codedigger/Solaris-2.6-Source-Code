#include <errno.h>
#include <stdio.h>
#include <varargs.h>
#include <sys/syscall.h>
#include "xsyscall.h"

/* 
 * Array of SVR4 system call numbers. The 4.1 numbers are mapped
 * to their SVR4/5.0 equivalents before trapping into the kernel.
 */

int syscallnum[190] = {	SYS_syscall,	SYS_exit,	SYS_fork, 
	SYS_read,	SYS_write,	SYS_open,	SYS_close,
        -1,		SYS_creat,	SYS_link,	SYS_unlink,
	-1,		SYS_chdir,	0,		SYS_mknod,
        SYS_chmod,	SYS_lchown,	0,		0,
        SYS_lseek,	SYS_getpid,	0,		0,
	0,		SYS_getuid,	0,		SYS_ptrace,
	0,		0,		0,		0, 
	0,		0,		SYS_access, 	0,
	0,		SYS_sync,	SYS_kill,	SYS_stat,
	0,		SYS_lstat,	SYS_dup,	SYS_pipe,
	0,		SYS_profil,	0,		0,
	SYS_getgid,	0,		0,		0,
	SYS_acct,	0,		-1,		SYS_ioctl,
	-1 /*reboot*/,	0,		SYS_symlink,	SYS_readlink,
	SYS_execve,	SYS_umask,	SYS_chroot,	SYS_fstat,     
	0,		-1/*getpagesize*/,-1,		0,
	0,		0,		-1,		-1,
	SYS_mmap,	-1,		SYS_munmap,	SYS_mprotect,
	-1 /*advise*/,	SYS_vhangup,	0,		SYS_mincore,
	SYS_getgroups,	SYS_setgroups,	-1 /*getpgrp*/,	-1 /*setpgrp*/,
	SYS_setitimer,	0,		-1 /*swapon*/,	SYS_getitimer,
	-1/*gethostname*/,-1/*sethostname*/,-1/*getdtablesize*/,-1/*dup2*/,
	-1/*getdopt*/,	SYS_fcntl,	-1 /*select*/,	-1 /*setdopt*/,
	SYS_fsync,	-1 /*setprio*/,	-1 /*socket*/,	-1 /*connect*/,
	-1 /*accept*/,	-1 /*getprio*/,	-1 /*send*/,	-1 /*recv*/,
	0,		-1 /*bind*/,	-1 /*setsockopt*/,-1 /*listen*/, 
	0,		-1 /*sigvec*/,	-1 /*sigblock*/, -1 /*sigsetmask*/,
	-1 /*sigpause*/, -1 /*sigstack*/, -1 /*recvmsg*/, -1 /*sendmsg*/,
	-1 /*vtrace*/,	SYS_gettimeofday, -1 /*getrusage*/, -1 /*getsockopt*/,
	0,		SYS_readv,	SYS_writev,	-1 /*settimeofday*/,
	SYS_fchown,	SYS_fchmod,	-1 /*recvfrom*/, -1 /*setreuid*/,
	-1 /*getregid*/, SYS_rename,	-1 /*truncate*/, -1 /*ftruncate*/,
	-1 /*flock*/,	0,		-1 /*sendto*/,	-1 /*shutdown*/,
	-1 /*socketpair*/,SYS_mkdir,	SYS_rmdir,	SYS_utimes,
	0,		SYS_adjtime,	-1 /*getpeername*/,-1 /*gethostid*/, 
	0,		SYS_getrlimit,	SYS_setrlimit,	-1 /*killpg*/,
	0,		0,		0,		-1/*getsockname*/,
	SYS_getmsg,	SYS_putmsg,	SYS_poll,	0,
	-1/*nfssvc*/,	-1 /*getdirentries*/, SYS_statfs, SYS_fstatfs,
	SYS_umount,	-1 /*async_daemmon*/ -1 /*getfh*/, -1/*getdomain*/,
	-1/*setdomain*/, 0,		-1 /*quotactl*/, -1 /*exportfs*/,
	SYS_mount,	-1/*ustat*/,	SYS_semsys,	SYS_msgsys,
	SYS_shmsys,	-1 /*auditsys*/, -1 /*rfsys*/,	SYS_getdents,
	-1 /*setsid*/,	SYS_fchdir,	SYS_fchroot,	-1 /*vpixsys*/,
	-1 /*aioread*/,	-1 /*aiowrite*/, -1 /*aiocancel*/, SYS_sigpending,   
	0,		-1 /*setpgid*/, SYS_pathconf,	SYS_uname,
}; 


syscall(sysnum, va_alist)
int sysnum;
va_dcl
{
	va_list ap;
	int i1, i2, i3, i4;
	char *c1, *c2, *c3, *c4;

	va_start(ap);
	switch(sysnum) {
		case XSYS_read: 
			i1 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			i2 = va_arg(ap, int);
			return(bc_read(i1, c1, i2));
		case XSYS_write:
			i1 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			i2 = va_arg(ap, int);
			return(bc_write(i1, c1, i2));
		case XSYS_readv:         
			i1 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			i2 = va_arg(ap, int);
			return(bc_readv(i1, c1, i2));
		case XSYS_writev:
			i1 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			i2 = va_arg(ap, int);
			return(bc_writev(i1, c1, i2));
		case XSYS_open:         
			c1 = va_arg(ap, char *);
			i1 = va_arg(ap, int);
			i2 = va_arg(ap, int);
			if (i2)
				return(bc_open(c1, i1, i2));
			else
				return(bc_open(c1, i1));
		case XSYS_close:        
			i1 = va_arg(ap, int);
			return(bc_close(i1));
		case XSYS_fcntl:
			i1 = va_arg(ap, int);
			i2 = va_arg(ap, int);
			i3 = va_arg(ap, int);
			return(bc_fcntl(i1, i2, i3));
		case XSYS_select:
			i1 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			c2 = va_arg(ap, char *);
			c3 = va_arg(ap, char *);
			c4 = va_arg(ap, char *);
			return(_select(i1, c1, c2, c3, c4));
		case XSYS_ioctl :	        
			i1 = va_arg(ap, int);
			i2 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			return(bc_ioctl(i1, i2, c1));
		case XSYS_stat:          
			c1 = va_arg(ap, char *);
			c2 = va_arg(ap, char *);
			return(bc_stat(c1, c2));
		case XSYS_lstat:
			c1 = va_arg(ap, char *);
			c2 = va_arg(ap, char *);
			return(bc_lstat(c1, c2));
		case XSYS_fstat:     
			i1 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			return(bc_fstat(i1, c1));
        	case XSYS_getdents:     
			i1 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			i2 = va_arg(ap, int);
			return(bc_getdents(i1, c1, i2));
		case XSYS_kill:         
			i1 = va_arg(ap, int);
			i2 = va_arg(ap, int);
			return(bc_kill(i1, i2));
		case XSYS_mount:        
			c1 = va_arg(ap, char *);
			c2 = va_arg(ap, char *);
			i1 = va_arg(ap, int);
			c3 = va_arg(ap, char *);
			return(mount(c1, c2, i1, c3));
		case XSYS_getrlimit:
			i1 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			return(bc_getrlimit(i1, c1));
		case XSYS_setrlimit:    
			i1 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			return(bc_setrlimit(i1, c1));
		case XSYS_uname:
			c1 = va_arg(ap, char *);
			return(bc_uname(c1));
		case XSYS_creat:     
			c1 = va_arg(ap, char *);
			i1 = va_arg(ap, int);
			return(creat(c1, i1));
		case XSYS_unmount:        
			c1 = va_arg(ap, char *);
			return(umount(c1));
		case XSYS_link:         
			c1 = va_arg(ap, char *);
			c2 = va_arg(ap, char *);
			return(link(c1, c2));
		case XSYS_unlink:       
			c1 = va_arg(ap, char *);
			return(unlink(c1));
		case XSYS_chdir:
			c1 = va_arg(ap, char *);
			return(chdir(c1));
		case XSYS_mknod:        
			c1 = va_arg(ap, char *);
			i1 = va_arg(ap, int);
			i2 = va_arg(ap, int);
			return(mknod(c1, i1, i2));
		case XSYS_chmod:        
			c1 = va_arg(ap, char *);
			i1 = va_arg(ap, int);
			return(chmod(c1, i1));
		case XSYS_chown: 
			c1 = va_arg(ap, char *);
			i1 = va_arg(ap, int);
			i2 = va_arg(ap, int);
			return(chown(c1, i1, i2));
		case XSYS_lseek:  
			i1 = va_arg(ap, int);
			i2 = va_arg(ap, int);
			i3 = va_arg(ap, int);
			return(lseek(i1, i2, i3));
		case XSYS_access: 
			c1 = va_arg(ap, char *);
			i1 = va_arg(ap, int);
			return(access(c1, i1));
        	case XSYS_dup:          
			i1 = va_arg(ap, int);
			return(dup(i1));
		case XSYS_dup2:
			i1 = va_arg(ap, int);
			i2 = va_arg(ap, int);
			return(dup(i1, i2));
		case XSYS_pipe:         
			c1 = (char *)va_arg(ap, int *);
			return(pipe(c1));
		case XSYS_symlink:      
			c1 = va_arg(ap, char *);
			c2 = va_arg(ap, char *);
			return(symlink(c1, c2));
		case XSYS_readlink:     
			c1 = va_arg(ap, char *);
			c2 = va_arg(ap, char *);
			i1 = va_arg(ap, int);
			return(readlink(c1, c2, i1));
		case XSYS_execve:        
			c1 = va_arg(ap, char *);
			c2 = (char *)va_arg(ap, char **);
			c3 = (char *)va_arg(ap, char **);
			return(execve(c1, c2, c3));
		case XSYS_chroot:       
			c1 = va_arg(ap, char *);
			return(chroot(c1));
		case XSYS_getgroups:    
			i1 = va_arg(ap, int);
			c1 = (char *)va_arg(ap, int *);
			return(getgroups(i1, c1));
		case XSYS_setgroups:     
			i1 = va_arg(ap, int);
			c1 = (char *)va_arg(ap, int *);
			return(setgroups(i1, c1));
		case XSYS_fsync:        
			i1 = va_arg(ap, int);
			return(fsync(i1));
		case XSYS_gettimeofday:
			c1 = va_arg(ap, char *);
			c2 = va_arg(ap, char *);
			return(gettimeofday(c1, c2));
		case XSYS_settimeofday:
			c1 = va_arg(ap, char *);
			c2 = va_arg(ap, char *);
			return(settimeofday(c1, c2));
		case XSYS_rename:        
			c1 = va_arg(ap, char *);
			c2 = va_arg(ap, char *);
			return(rename(c1, c2));
		case XSYS_mkdir:        
			c1 = va_arg(ap, char *);
			i1 = va_arg(ap, int);
			return(mkdir(c1, i1));
		case XSYS_rmdir:         
			c1 = va_arg(ap, char *);
			return(rmdir(c1));
        	case XSYS_statfs:       
			c1 = va_arg(ap, char *);
			c2 = va_arg(ap, char *);
			return(statfs(c1, c2));
		case XSYS_fstatfs:      
			i1 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			return(fstatfs(i1, c1));
		case XSYS_getpagesize:
			return(getpagesize());
		case XSYS_gethostid:
			return(gethostid());
		case XSYS_getdtablesize:
			return(getdtablesize());
		case XSYS_pathconf:      
			c1 = va_arg(ap, char *);
			i1 = va_arg(ap, int);
			return(pathconf(c1, i1));
		case XSYS_gethostname:
			c1 = va_arg(ap, char *);
			i1 = va_arg(ap, int);
			return(gethostname(c1, i1));
		case XSYS_sethostname:
			c1 = va_arg(ap, char *);
			i1 = va_arg(ap, int);
			return(sethostname(c1, i1));
		case XSYS_setreuid:  
			i1 = va_arg(ap, int);
			i2 = va_arg(ap, int);
			return(setreuid(i1, i2));
		case XSYS_setregid:  
			i1 = va_arg(ap, int);
			i2 = va_arg(ap, int);
			return(setregid(i1, i2));
		case XSYS_getpriority:  
			i1 = va_arg(ap, int);
			i2 = va_arg(ap, int);
			return(getpriority(i1, i2));
		case XSYS_setpriority:  
			i1 = va_arg(ap, int);
			i2 = va_arg(ap, int);
			i3 = va_arg(ap, int);
			return(setpriority(i1, i2, i3));
		case XSYS_sigvec:
			i1 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			c2 = va_arg(ap, char *);
			return(sigvec(i1, c1, c2));
		case XSYS_sigblock:
			i1 = va_arg(ap, int);
			return(sigblock(i1));
		case XSYS_sigpending:
			c1 = va_arg(ap, char *);
			return(sigpending(c1));
		case XSYS_sigsetmask:
			i1 = va_arg(ap, int);
			return(sigsetmask(i1));
		case XSYS_sigpause:
			c1 = va_arg(ap, char *);
			return(sigpause(c1));
		case XSYS_sigstack:
			c1 = va_arg(ap, char *);
			c2 = va_arg(ap, char *);
			return(sigstack(c1, c2));
		case XSYS_truncate:
			c1 = va_arg(ap, char *);
			i1 = va_arg(ap, int);
			return(truncate(c1, i1));
		case XSYS_ftruncate:
			i1 = va_arg(ap, int);
			i2 = va_arg(ap, int);
			return(ftruncate(i1, i2));
		case XSYS_killpg:
			i1 = va_arg(ap, int);
			i2 = va_arg(ap, int);
			return(killpg(i1, i2));
		case XSYS_setpgid:
			i1 = va_arg(ap, int);
			i2 = va_arg(ap, int);
			return(setpgid(i1, i2));
		case XSYS_ptrace:       
			i1 = va_arg(ap, int);
			i2 = va_arg(ap, int);
			i3 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			i4 = va_arg(ap, int);
			c2 = va_arg(ap, char *);
			return(ptrace(i1, i2, i3, c1, i4, c2));	
#ifdef S5EMUL
		case XSYS_getpgrp:
			return(getpgrp());
		case XSYS_setpgrp:
			return(setpgrp());
#else
		case XSYS_getpgrp:
			i1 = va_arg(ap, int);
			return(getpgrp(i1));
		case XSYS_setpgrp:
			i1 = va_arg(ap, int);
			i2 = va_arg(ap, int);
			return(setpgrp(i1, i2));
#endif
		case XSYS_getrusage:
			i1 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			return(getrusage(i1, c1));
		case XSYS_setsid:
			return(setsid());

		case XSYS_flock:
			i1 = va_arg(ap, int);
			i2 = va_arg(ap, int);
			return(flock(i1, i2));
	
		/* the following system calls are now implemented in
		 * libsocket */
		case XSYS_accept:
			i1 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			c2 = (char *)va_arg(ap, int *);
			return(_accept(i1, c1, c2));
		case XSYS_bind:
			i1 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			i2 = va_arg(ap, int);
			return(_bind(i1, c1, i2));
		case XSYS_connect:
			i1 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			i2 = va_arg(ap, int);
			return(_connect(i1, c1, i2));
		case XSYS_getsockopt:
			i1 = va_arg(ap, int);
			i2 = va_arg(ap, int);
			i3 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			c2 = va_arg(ap, char *);
			return(_getsockopt(i1, i2, i3, c1, c2));
		case XSYS_getpeername:
			i1 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			c2 = va_arg(ap, char *);
			return(_getpeername(i1, c1, c2));
		case XSYS_getsockname:
			i1 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			c2 = va_arg(ap, char *);
			return(_getsockname(i1, c1, c2));
		case XSYS_getdomainname:
			c1 = va_arg(ap, char *);
			i1 = va_arg(ap, int);
			return(getdomainname(c1, i1));
		case XSYS_listen:
			i1 = va_arg(ap, int);
			i2 = va_arg(ap, int);
			return(_listen(i1, i2));
		case XSYS_recv:
			i1 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			i2 = va_arg(ap, int);
			i3 = va_arg(ap, int);
			return(_recv(i1, c1, i2, i3));
		case XSYS_recvfrom:
			i1 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			i2 = va_arg(ap, int);
			i3 = va_arg(ap, int);
			c2 = va_arg(ap, char *);
			c3 = va_arg(ap, char *);
			return(_recvfrom(i1, c1, i2, i3, c2, c3));
		case XSYS_recvmsg:
			i1 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			i2 = va_arg(ap, int);
			return(_recvmsg(i1, c1, i2));
		case XSYS_send:
			i1 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			i2 = va_arg(ap, int);
			i3 = va_arg(ap, int);
			return(_send(i1, c1, i2, i3));
		case XSYS_sendto:
			i1 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			i2 = va_arg(ap, int);
			i3 = va_arg(ap, int);
			c2 = va_arg(ap, char *);
			i4 = va_arg(ap, int);
			return(_sendto(i1, c1, i2, i3, c2, i4));
		case XSYS_sendmsg:
			i1 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			i2 = va_arg(ap, int);
			return(_sendmsg(i1, c1, i2));
		case XSYS_setdomainname:
			c1 = va_arg(ap, char *);
			i1 = va_arg(ap, int);
			return(setdomainname(c1 ,i1));
		case XSYS_setsockopt:
			i1 = va_arg(ap, int);
			i2 = va_arg(ap, int);
			i3 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			i4 = va_arg(ap, int);
			return(_setsockopt(i1, i2, i3, c1, i4));
		case XSYS_shutdown:
			i1 = va_arg(ap, int);
			i2 = va_arg(ap, int);
			return(_shutdown(i1, i2));
		case XSYS_socket:
			i1 = va_arg(ap, int);
			i2 = va_arg(ap, int);
			i3 = va_arg(ap, int);
			return(_socket(i1, i2, i3));
		case XSYS_socketpair:
			i1 = va_arg(ap, int);
			i2 = va_arg(ap, int);
			i3 = va_arg(ap, int);
			c1 = va_arg(ap, char *);
			return(_socketpair(i1, i2, i3, c1));


		/* The following can directly go through syscall */
		case XSYS_acct:
		case XSYS_adjtime:      
		case XSYS_exit: 
		case XSYS_fchdir:        
		case XSYS_fchmod:        
		case XSYS_fchown:    
		case XSYS_fchroot:
		case XSYS_fork: 
		case XSYS_getgid:    
		case XSYS_getitimer:
		case XSYS_getmsg:
		case XSYS_getpid:       
		case XSYS_getuid:       
		case XSYS_mincore:      
		case XSYS_mprotect:     
		case XSYS_munmap:       
		case XSYS_putmsg:       
		case XSYS_poll: 
		case XSYS_profil:       
		case XSYS_setitimer:
		case XSYS_sync:         
		case XSYS_umask: 
		case XSYS_utimes:
		case XSYS_semsys:
		case XSYS_msgsys:       
		case XSYS_shmsys:       
		case XSYS_mmap:          
		case XSYS_vhangup: 
			return(_syscall(syscallnum[sysnum], va_alist));

		case XSYS_aioread:
		case XSYS_aiowrite:
		case XSYS_aiocancel:
		case XSYS_swapon:
		case XSYS_async_daemon:
		case XSYS_getfh:     
		case XSYS_nfssvc:
		case XSYS_exportfs:  
		case XSYS_auditsys:   
        	case XSYS_vpixsys:   
		case XSYS_quotactl:
		case XSYS_getdopt:    
		case XSYS_setdopt:
		case XSYS_ustat:
		case XSYS_vtrace:
		case XSYS_reboot:
		case XSYS_madvise:
		case XSYS_vadvise:
		case XSYS_getdirentries:
			fprintf(stderr,"system call not supported\n");
			return(-1);
	}
}
