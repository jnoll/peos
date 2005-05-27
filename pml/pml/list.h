/************************************************************************
 * File:	list.h							*
 *									*
 * Description:	This file contains the public function and type		*
 *		declarations for the list abstract data type.		*
 ************************************************************************/

# ifndef LIST_H
# define LIST_H
# include <pml/features.h>


/* Type declarations */

typedef void *Item;
typedef struct list *List;


/* Function declarations */

BEGIN_PROTOTYPES

extern List ListConcat (
# ifdef ANSI_PROTOTYPES
    List		/* head */,
    List		/* tail */
# endif
);


extern List ListCopy (
# ifdef ANSI_PROTOTYPES
    List		/* list */
# endif
);


extern List ListCreate (
# ifdef ANSI_PROTOTYPES
    void		/* none */
# endif
);


extern void ListDestroy (
# ifdef ANSI_PROTOTYPES
    List		/* list */
# endif
);


extern Item ListGet (
# ifdef ANSI_PROTOTYPES
    List		/* list */
# endif
);


extern Item ListIndex (
# ifdef ANSI_PROTOTYPES
    List		/* list  */,
    int			/* index */
# endif
);


extern Item ListInsert (
# ifdef ANSI_PROTOTYPES
    List		/* list  */,
    int			/* index */,
    Item		/* item  */
# endif
);


extern Item ListPop (
# ifdef ANSI_PROTOTYPES
    List		/* list */
# endif
);


extern Item ListPull (
# ifdef ANSI_PROTOTYPES
    List		/* list */
# endif
);


extern Item ListPush (
# ifdef ANSI_PROTOTYPES
    List		/* list */,
    Item		/* item */
# endif
);


extern Item ListPut (
# ifdef ANSI_PROTOTYPES
    List		/* list */,
    Item		/* item */
# endif
);


extern int ListSize (
# ifdef ANSI_PROTOTYPES
    List		/* list */
# endif
);

END_PROTOTYPES

# endif /* LIST_H */
