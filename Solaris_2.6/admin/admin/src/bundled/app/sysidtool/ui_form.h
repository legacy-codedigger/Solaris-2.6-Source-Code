#ifndef UI_FORM_H
#define	UI_FORM_H

/*
 * Copyright (c) 1993-1996 Sun Microsystems, Inc.  All Rights Reserved.
 */

#pragma	ident	"@(#)ui_form.h 1.7 96/02/05"

typedef enum field_type {
	FIELD_NONE,
	FIELD_TEXT,
	FIELD_EXCLUSIVE_CHOICE,
	FIELD_NONEXCLUSIVE_CHOICE,
	FIELD_CONFIRM
} Field_type;

typedef	struct menu Menu;
struct menu {
	char	**labels;	/* array of labels (for convenience) */
	void	*values;	/* array of values */
	int	nitems;		/* number of items */
	int	selected;	/* selected item */
	void	*client_data;	/* a holding place for any additional info */
};

typedef struct field_help Field_help;
struct field_help {
	char		*howto;
	char		*topics;
	char		*reference;
};

typedef	struct field_desc Field_desc;
struct field_desc {
	Field_type	type;		/* text, choice, etc. */
	void		*user_data;	/* user data (usually attribute) */
	Field_help	*help;		/* help structure for field */
	char		*label;		/* label for field */
	void		*value;		/* depends on field type */
	int		field_length;	/* maximum field display length */
	int		value_length;	/* maximum length of value */
	int		vmin;		/* minimum value */
	int		vmax;		/* maximum value */
	int		flags;		/* see below */
	int		(*validate)(struct field_desc *);
					/* field validation function */
};

/*
 * Field flags
 */
#define	FF_RDONLY		0x1
#define	FF_VALREQ		0x2
#define	FF_SUMMARY		0x4
#define	FF_POPUP		0x8	/* really a "form" flag */

#define	FF_LAB_LJUST		0x10
#define	FF_LAB_RJUST		0x20
#define	FF_LAB_CENTER		0x40
#define	FF_LAB_ALIGN		0x80	/* align on right edge of label */

#define	FF_VAL_LJUST		0x100
#define	FF_VAL_RJUST		0x200
#define	FF_VAL_CENTER		0x400

#define	FF_CANCEL		0x1000
#define	FF_CHANGE		0x2000
#define	FF_GOBACK		0x4000

#define	FF_HIGHLIGHT		0x10000
#define	FF_ENDGROUP		0x20000	/* end of group -- add space */
#define	FF_TYPE_TO_WIPE		0x40000
#define	FF_FORCE_SCROLLABLE	0x80000

#define	FF_KEYFOCUS		0x100000 /* keyboard focus (gui) */

/*
 * Misc. constants
 */
#ifndef TRUE
#define	TRUE			1
#endif
#ifndef FALSE
#define	FALSE			0
#endif

typedef	int	(Validate_proc)(Field_desc *);

extern void	free_menu(Menu *);

extern char	**format_text(char *, int);

#endif	/* !UI_FORM_H */
