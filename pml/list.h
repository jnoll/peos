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
) KRNL_SECTION ;


extern List ListCopy (
# ifdef ANSI_PROTOTYPES
    List		/* list */
# endif
) KRNL_SECTION ;


extern List ListCreate (
# ifdef ANSI_PROTOTYPES
    void		/* none */
# endif
) KRNL_SECTION ;


extern void ListDestroy (
# ifdef ANSI_PROTOTYPES
    List		/* list */
# endif
) KRNL_SECTION ;


extern Item ListGet (
# ifdef ANSI_PROTOTYPES
    List		/* list */
# endif
) KRNL_SECTION ;


extern Item ListIndex (
# ifdef ANSI_PROTOTYPES
    List		/* list  */,
    int			/* index */
# endif
) KRNL_SECTION ;


extern Item ListInsert (
# ifdef ANSI_PROTOTYPES
    List		/* list  */,
    int			/* index */,
    Item		/* item  */
# endif
) KRNL_SECTION ;


extern Item ListPop (
# ifdef ANSI_PROTOTYPES
    List		/* list */
# endif
) KRNL_SECTION ;


extern Item ListPull (
# ifdef ANSI_PROTOTYPES
    List		/* list */
# endif
) KRNL_SECTION ;


extern Item ListPush (
# ifdef ANSI_PROTOTYPES
    List		/* list */,
    Item		/* item */
# endif
) KRNL_SECTION ;


extern Item ListPut (
# ifdef ANSI_PROTOTYPES
    List		/* list */,
    Item		/* item */
# endif
) KRNL_SECTION ;


extern int ListSize (
# ifdef ANSI_PROTOTYPES
    List		/* list */
# endif
) KRNL_SECTION ;

END_PROTOTYPES

# endif /* LIST_H */
