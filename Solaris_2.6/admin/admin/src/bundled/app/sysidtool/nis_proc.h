/* 
 * SCCSID  = "@(#)nis_proc.h	1.1	92/11/30 SMI"
 *
 *	nis_proc.h
 *
 * This module contains definitions used by the NIS service.
 *
 */

/*
 * Structure definitions used by the various functions
 */
struct nis_db_result {
	nis_error	status;	/* Status of the result 	   */
	nis_object	*obj;	/* Pointer to the object returned  */
	unsigned long	ticks;	/* Number of clock ticks used 	   */
};

typedef struct nis_db_result nis_db_result;

struct obj_list {
	u_char		r;	/* readable flag	*/
	nis_object	*o;	/* object		*/
};

typedef struct obj_list	obj_list;
struct nis_db_list_result { 
	nis_error	status;	/* return status */ 
	obj_list	*objs;	/* Objects returned */ 
	int		numo;	/* Number of objects */ 
	unsigned long	ticks;	/* Database ticks */
}; 

typedef struct nis_db_list_result nis_db_list_result;

struct nis_fn_result {
	nis_error	status;	/* Return status */
	nis_object	*obj;	/* The object returned */
	netobj		cookie; /* The magic cookie (db state) */
	unsigned long	ticks;	/* the obligatory ticks value */
};

typedef struct nis_fn_result nis_fn_result;

struct limits {
	int	max_attrval;	/* Maximum length of 1 attribute */
	int	max_attr;	/* Maximum number of attributes  */
	int	max_columns;	/* Maximum number of columns	 */
};

struct ticks {
	u_long	aticks;
	u_long	dticks;
	u_long	zticks;
	u_long	cticks;
};

struct object_list {
	int		num;
	nis_object	*objs;
};

struct checkpoint {
	char			table[80];	/* Table to checkpoint */
	struct checkpoint	*next;	/* Next checkpoint */
};

struct cleanup {
	void		(*func)();
	void		*data;
	char		*tag;
	int		id;
	struct cleanup *next;
};
typedef struct cleanup cleanup;

struct xdr_clean_data {
	bool_t		(*xdr_func)();
	char		*xdr_data;
};
typedef struct xdr_clean_data xdr_clean_data;


struct ping_item {
	NIS_HASH_ITEM	item;	/* Item struct for hash functions */
	u_long		mtime;	/* Time of the update		  */
	nis_object	*obj;	/* Directory object.		  */
};

typedef struct ping_item ping_item;

struct sdata {
	void	*buf;	/* Memory allocation pointer */
	u_long	size;	/* Size of allocated data    */
};

/* operations statistics vector */
struct ops_stats {
	u_long	op;		/* Operation (procedure number)  */
	u_long	calls;		/* Times called			 */
	u_long	errors;		/* Error encountered in call	*/
	int	cursamp;	/* current sample index		 */
	u_long	tsamps[16];	/* last 16 time measurements	 */
};

/* 
 * Forward reference declarations for functions in the service and in
 * the nis library
 */

#ifdef __STDC__
/* in nis_db.c */
extern void		__make_legal(char *);
extern nis_db_result 	*db_add(nis_name, nis_object *, int);
extern nis_error 	__db_add(nis_name, nis_object *, int);
extern nis_db_result 	*db_remove(nis_name, nis_object *, u_long);
extern nis_error 	__db_remove(nis_name, nis_object *);
extern nis_db_result 	*db_lookup(nis_name);
extern nis_db_list_result *db_list(nis_name, int, nis_attr *);
extern void		free_db_list(nis_db_list_result *);
extern nis_db_result 	*db_addib(nis_name, int, nis_attr *, nis_object *, 
								    nis_object *);
extern nis_error 	__db_addib(nis_name, int, nis_attr *, nis_object *);
extern nis_db_result 	*db_remib(nis_name, int, nis_attr *, obj_list *, int, 
							nis_object *, u_long);
extern nis_error 	__db_remib(nis_name, int, nis_attr *);
extern nis_fn_result 	*db_firstib(nis_name, int, nis_attr *, int);
extern nis_fn_result 	*db_nextib(nis_name, netobj *, int);
extern void		db_flush(nis_name, netobj *);
extern void		add_checkpoint(nis_name);
extern cp_result	*do_checkpoint(nis_name);
extern nis_name		__table_name(nis_name);

/* in nis_subr_proc.c */
extern int		nis_strcasecmp(char *, char *);
extern int		nis_isserving(nis_object *);
extern nis_error	__directory_object(nis_name, struct ticks *, int, 
								nis_object **);
extern void		nis_getprincipal(char *, int, caddr_t);
extern nis_error	get_object_safely(nis_name, u_long, nis_object **);
extern nis_result	*svc_lookup(nis_name, u_long);
extern int		__can_do(u_long, u_long, nis_object *, nis_name);
extern u_char		*__get_xdr_buf(int);
extern char		*__get_string_buf(int);
extern entry_col	*__get_entry_col(int);
extern table_col	*__get_table_col(int);
extern nis_attr		*__get_attrs(int);
extern int		nis_isstable(log_entry *, int);
extern u_long		nis_cptime(nis_server *, nis_name);
extern void		make_stamp(nis_name, u_long);
extern int		replica_update(ping_item *);
extern int		ping_replicas(ping_item *);
extern void		add_pingitem(nis_object *, u_long, NIS_HASH_TABLE *);
extern void		do_cleanup(cleanup *);
extern void		add_cleanup(void (*)(), void *, char *);
extern void		do_xdr_cleanup(xdr_clean_data *);
extern void		add_xdr_cleanup(bool_t (*)(), char *, char *);

/* in nis_ib_proc.c */
extern nis_object	*nis_censor_object(nis_object *, table_col *, nis_name);

/* in nis_log_svc.c */
extern u_long		last_update(nis_name);
extern int		apply_transaction(log_entry *);
extern int		checkpoint_log(void);
extern u_long		add_update(log_entry *);
extern int		begin_transaction(nis_name);
extern int		abort_transaction(int);
extern int		end_transaction(int);
extern void 		entries_since(nis_name, u_long, log_result *);

/* in nis_log_common.c */
extern int		map_log(int);
#else

/* in nis_db.c */
extern void		__make_legal();
extern nis_db_result 	*db_add();
extern nis_error 	__db_add();
extern nis_db_result 	*db_remove();
extern nis_error 	__db_remove();
extern nis_db_result 	*db_lookup();
extern nis_db_list_result *db_list();
extern void		free_db_list();
extern nis_db_result 	*db_addib();
extern nis_error 	*__db_addib();
extern nis_db_result 	*db_remib();
extern nis_error 	__db_remib();
extern nis_fn_result 	*db_firstib();
extern nis_fn_result 	*db_nextib();
extern void		db_flush();
extern void		add_checkpoint();
extern cp_result	*do_checkpoint();
extern nis_name		__table_name();

/* in nis_subr_proc.c */
extern int		nis_strcasecmp();
extern int		nis_isserving();
extern nis_error	__directory_object();
extern void		nis_getprincipal();
extern nis_error	get_object_safely();
extern nis_result	*svc_lookup();
extern int		__can_do();
extern u_char		*__get_xdr_buf();
extern char		*__get_string_buf();
extern entry_col	*__get_entry_col();
extern table_col	*__get_table_col();
extern nis_attr		*__get_attrs();
extern int		nis_isstable();
extern u_long		nis_cptime();
extern void		make_stamp();
extern void		replica_update();
extern void		add_pingitem();
extern void		do_cleanup();
extern void		add_cleanup();
extern void		do_xdr_cleanup();
extern void		add_xdr_cleanup();

/* in nis_ib_proc.c */
extern nis_object	*nis_censor_object();

/* in nis_log_svc.c */
extern u_long		last_update();
extern int		apply_transaction();
extern int		checkpoint_log();
extern u_long		add_update();
extern int		begin_transaction();
extern int		abort_transaction();
extern int		end_transaction();
extern void		entries_since();

/* in nis_log_common.c */
extern int		map_log();
extern int		__log_resync();
extern char		*__make_name();
#endif

/*
 * Declarations for global state used by the service.
 */
extern int root_server;		/* set by the -r switch 	*/
extern int verbose;		/* set by the -v switch 	*/
extern int static_root;		/* set by network partition 	*/
extern int secure_level;	/* set by the -S switch 	*/
extern NIS_HASH_TABLE upd_list;	/* added too during a ping.	*/
extern NIS_HASH_TABLE ping_list;/* added too during an update.	*/
extern int ping_pid, upd_pid;	/* Processes doing the work	*/
/*
 * MACRO definitions for the various source modules.
 */
/* #define STATUS(r) r.status */
#define OBJECT(r) r.objects.objects_val
#define NUMOBJ(r) r.objects.objects_len
#define CBDATA(r) r.cookie
#define COOKIE(r) r.cookie
#define same_oid(o1,o2)	(((o1)->zo_oid.mtime == (o2)->zo_oid.mtime) &&\
			 ((o1)->zo_oid.ctime == (o2)->zo_oid.ctime))

#define ROOT_OBJ	"root.object"
#define PARENT_OBJ	"parent.object"

#define ENVAL ec_value.ec_value_val
#define ENLEN ec_value.ec_value_len
#define ZAVAL zattr_val.zattr_val_val
#define ZALEN zattr_val.zattr_val_len

#define FN_NOMANGLE	0	/* don't mangle objects on first/next calls */
#define FN_MANGLE	1	/* mangle objects on first/next calls */
#define	FN_NORAGS	2	/* don't put malloc data on the rag list    */
#define FN_NOERROR	4	/* don't generate error on missing table    */

#define	ADD_OP		1
#define REM_OP		2
#define MOD_OP		3
#define FIRST_OP 	4
#define NEXT_OP 	5

#define GETSERVER(o, n) (((o)->DI_data.do_servers.do_servers_val) + n)
#define MAXSERVER(o)    (o)->DI_data.do_servers.do_servers_len
#define MASTER(o) 	GETSERVER(o, 0)

#define CHILDPROC	(getpid() != master_pid)

/* Global variables defined in nis_main.c */
extern cleanup	*looseends;	  /* Data that needs to be freed	      */
extern cleanup	*free_rags;	  /* free cleanup structs that are available  */
extern int	children;	  /* Server load control 		      */
extern int	max_children;	  /* Max number of forked children 	      */
extern int	readonly;	  /* When true the service is read only       */
extern int	need_checkpoint;  /* When true the log should be checkpointed */
extern int	force_checkpoint; /* When true the log WILL be checkpointed   */
extern int	auth_verbose; /* verbose authorization messages */
extern pid_t	master_pid;	/* Process id of the controlling process */
extern table_obj tbl_prototype; /* directory "table" format */
/*
 * A set of defines for tracking memory problems. A simple memory allocator
 * and checker is part of nis_malloc.c and when DEBUG is defined and this
 * file is linked in, we generate messages on allocs/frees and check for
 * freeing free memory, etc.
 */
#ifdef MEM_DEBUG
#define XFREE		xfree
#define XMALLOC 	xmalloc
#define XCALLOC 	xcalloc
#define XSTRDUP 	xstrdup
#ifdef __STDC__
extern void xfree(void *);
extern char *xstrdup(char *);
extern char *xmalloc(int);
extern char *xcalloc(int, int);
#else
extern void xfree();
extern char *xstrdup();
extern char *xmalloc();
extern char *xcalloc();
#endif /* __STDC__ */
#else
#define XFREE 		free
#define XMALLOC		malloc
#define XCALLOC	 	calloc
#define XSTRDUP		strdup
#endif