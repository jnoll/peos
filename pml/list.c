/************************************************************************
 * File:	list.c							*
 *									*
 * Description:	This file contains the public and private function and	*
 *		type declarations for the list abstract data type.	*
 ************************************************************************/

# include <stdio.h>
# include <stdlib.h>
# include <pml/list.h>


/* Private type declarations */

struct list {
    int   size;
    int   count;
    int   first;
    Item *array;
};


/* Private function declarations */

static void Expand (
# ifdef ANSI_PROTOTYPES
    List		/* list */
# endif
);


static int Index (
# ifdef ANSI_PROTOTYPES
    List		/* list  */,
    int			/* index */
# endif
);


/************************************************************************
 * Function:	Expand							*
 *									*
 * Description:	Expands a list by doubling the size of the array if the	*
 *		list is full.						*
 ************************************************************************/

static void Expand (list)
    List list;
{
    int i;
    int num_bytes;


    if (list -> count == list -> size) {
	num_bytes = sizeof (Item) * list -> size * 2;
	list -> array = (Item *) realloc (list -> array, num_bytes);

	if (list -> first != 0) {
	    for (i = list -> first; i < list -> size; i ++)
		list -> array [i + list -> size] = list -> array [i];

	    list -> first += list -> size;
	}

	list -> size <<= 1;
    }
}


/************************************************************************
 * Function:	Index							*
 *									*
 * Description:	Returns the corresponding array index for the specified	*
 *		list and index.						*
 ************************************************************************/

static int Index (list, index)
    List list;
    int  index;
{
    return (list -> first + index) % list -> size;
}


/************************************************************************
 * Function:	ListConcat						*
 *									*
 * Description:	Returns a new list that is the concatenation of the	*
 *		two specified lists.					*
 ************************************************************************/

List ListConcat (head, tail)
    List head;
    List tail;
{
    int  i;
    List concat;


    concat = ListCopy (head);

    for (i = 0; i < ListSize (tail); i ++)
	ListPut (concat, ListIndex (tail, i));

    return concat;
}


/************************************************************************
 * Function:	ListCopy						*
 *									*
 * Description:	Returns a copy of the specified list.			*
 ************************************************************************/

List ListCopy (list)
    List list;
{
    int  i;
    List copy;


    copy = ListCreate ( );

    for (i = 0; i < ListSize (list); i ++)
	ListPut (copy, ListIndex (list, i));

    return copy;
}


/************************************************************************
 * Function:	ListCreate						*
 *									*
 * Description:	Creates, initializes, and returns a list.		*
 ************************************************************************/

List ListCreate ( )
{
    List list;


    list = (List) malloc (sizeof (struct list));

    list -> size = 1;
    list -> count = 0;
    list -> first = 0;
    list -> array = (Item *) malloc (sizeof (Item) * list -> size);

    return list;
}


/************************************************************************
 * Function:	ListDestroy						*
 *									*
 * Description:	Destroys the specified list by deallocating its		*
 *		associated memory.					*
 ************************************************************************/

void ListDestroy (list)
    List list;
{
    free (list -> array);
    free (list);
}


/************************************************************************
 * Function:	ListGet							*
 *									*
 * Description:	Removes and returns the first item in the specified	*
 *		list.  If the list if empty, then NULL is returned.	*
 ************************************************************************/

Item ListGet (list)
    List list;
{
    Item item;


    if (list -> count == 0)
	return NULL;

    item = list -> array [list -> first ++];

    if (list -> first == list -> size)
	list -> first = 0;

    list -> count --;
    return item;
}


/************************************************************************
 * Function:	ListIndex						*
 *									*
 * Description:	Returns the item at the specified index in the		*
 *		specified list.  If the index is negative, then it	*
 *		refers to the number of items from the end of the	*
 *		list.  If the index is out of range, then NULL is	*
 *		returned.						*
 ************************************************************************/

Item ListIndex (list, index)
    List list;
    int  index;
{
    if (index < 0)
	index = list -> count + index;

    if (index < 0 || index >= list -> count)
	return NULL;

    return list -> array [Index (list, index)];
}


/************************************************************************
 * Function:	ListInsert						*
 *									*
 * Description:	Inserts an item in the specified list at the specified	*
 *		index.  If the index is negative, then it refers to the	*
 *		number of items from the end of	the list.  If the index	*
 *		is out of range, then NULL is returned.			*
 ************************************************************************/

Item ListInsert (list, index, item)
    List list;
    int  index;
    Item item;
{
    if (index < 0)
	index = list -> count + index;

    if (index < 0 || index >= list -> count)
	return NULL;

    list -> array [Index (list, index)] = item;
    return item;
}


/************************************************************************
 * Function:	ListPop							*
 *									*
 * Description:	Removes and returns the first item in the specified	*
 *		list.  If the list if empty, then NULL is returned.	*
 ************************************************************************/

Item ListPop (list)
    List list;
{
    return ListGet (list);
}


/************************************************************************
 * Function:	ListPull						*
 *									*
 * Description:	Removes and returns the last item in the specified	*
 *		list.  If the list is empty, then NULL is returned.	*
 ************************************************************************/

Item ListPull (list)
    List list;
{
    if (list -> count == 0)
	return NULL;

    return list -> array [Index (list, -- list -> count)];
}


/************************************************************************
 * Function:	ListPush						*
 *									*
 * Description:	Adds an item to the front of the specified list and	*
 *		returns the item.					*
 ************************************************************************/

Item ListPush (list, item)
    List list;
    Item item;
{
    Expand (list);

    if (list -> first == 0)
	list -> first = list -> size;

    list -> array [-- list -> first] = item;
    list -> count ++;

    return item;
}


/************************************************************************
 * Function:	ListPut							*
 *									*
 * Description:	Adds an item to the end of the specified list and	*
 *		return the item.					*
 ************************************************************************/

Item ListPut (list, item)
    List list;
    Item item;
{
    Expand (list);
    list -> array [Index (list, list -> count ++)] = item;

    return item;
}


/************************************************************************
 * Function:	ListSize						*
 *									*
 * Description:	Returns the size of the specified list.			*
 ************************************************************************/

int ListSize (list)
    List list;
{
    return list -> count;
}
