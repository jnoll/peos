/* repository.c */

/* -----------------------------------------------------------
This file contains the source for handling the generic exported
interfaces to various aspects of PML.  It will create structures
that hold pointers to the interface functions in other source files.

Last Modified: Summer 2000

The modifications made in summer 2000 to the pre-existing code 
include changing the generic function names to those used in 
pml_state.c.  We would have liked to keep more generic names 
(ie. not starting with pml_) but that would have required changing
function calls in the VM, and this summer there is no group 
modifying the VM.  In doing this, you can be sure that the 
names of functions would be consistent as well.  

One suggestion for implementation when the VM is modified is
to have a global enumeration of interface types, one of which will
be an argument to each generic I/F function call.  A switch on 
the enumeration could determine which function to call.
------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "repository.h"
/*#include "C:\WINDOWS\DESKTOP\My Briefcase\CU Masters Andy\CSC 5728\unixfs_repository.h"*/


/*------------------------------------------------------------
Global variable declarations
------------------------------------------------------------*/

repository_entry*  rep_list;
repository_entry*  rep_listnext;
int                nr_repositories;

/*------------------------------------------------------------
Function: repository_init
Purpose :  
------------------------------------------------------------*/
int repository_init()
{
		rep_list        = NULL;
		rep_listnext    = NULL;
		nr_repositories = 0;

		UnixfsInit();
		pmlstate_init();
		return (int)0;
}

/*------------------------------------------------------------
Function: repository_initexternal
Purpose : This function accepts the functions from other interfaces
          and creates a struct containing pointers to those functions
------------------------------------------------------------*/
int repository_initexternal(id, Open, Openfs, Close, Closefs, Create, Delete,
							Pack, ReadAttribute, WriteAttribute, 
							QueryOpen, QueryClose, Assert, Query)
		char*     id;
		int       (*Open)(const char*, pml_mode_t);                                /* Used only by pmlstate   */
                repository* (*Openfs)(const char*, int);                                   /* Used only by UNIXFS     */
		int       (*Close)(void);                                                  /* Used only by pmlstate   */
		int       (*Closefs)(repository *);                                        /* Used only by UNIXFS     */
		pml_obj_t (*Create)(void);                                                 /* Used only by pmlstate    */
                int       (*Delete)(pml_obj_t);                                            /* Used only by pmlstate    */
                int       (*Pack)(void);                                                   /* Used only by pmlstate    */
                int       (*ReadAttribute)(void*, void *, size_t, void *, size_t);
                int       (*WriteAttribute)(pml_obj_t, void *, size_t, void *, size_t);    /* Used only by pmlstate    */
                int       (*QueryOpen)(void* **, void *, size_t, int, void *, size_t);
                int       (*QueryClose)(void* **);
		int       (*Assert)(repository *, char *, char *);
		int       (*Query)(repository_queryresult **, repository *, char *, int, char *);
{

		repository_entry *rep_pnt;

		rep_pnt = malloc(sizeof(repository_entry));
		if (rep_pnt == NULL)
				return (int)-1;

		rep_pnt->next       = NULL;
		rep_pnt->open       = Open;
                rep_pnt->openfs     = Openfs;
		rep_pnt->close      = Close;
		rep_pnt->closefs    = Closefs;
		rep_pnt->create     = Create;
		rep_pnt->del        = Delete;
		rep_pnt->pack       = Pack;
		rep_pnt->read       = ReadAttribute;
		rep_pnt->write      = WriteAttribute;
		rep_pnt->queryopen  = QueryOpen;
		rep_pnt->queryclose = QueryClose;
		rep_pnt->assert     = Assert;
		rep_pnt->query      = Query;
		rep_pnt->nr         = nr_repositories;
		rep_pnt->id         = id;

		nr_repositories++;

		if (rep_list == NULL) 
		{
				rep_list     = rep_pnt;
				rep_listnext = rep_pnt;
		} 
		
		else
		{
				rep_listnext->next = rep_pnt;
				rep_listnext       = rep_pnt;
		}

		return (int)0;
}

/*------------------------------------------------------------
Function: pml_open_repository
Purpose : takes the given path and generates a repository object
------------------------------------------------------------*/
int  pml_open_repository(path, mode)
		const char*       path;
                pml_mode_t        mode;
{
		char              rep_id[256];
		int               fd;
		int               i;
		repository_entry* rep_pnt;

	i = 0;

	/* Open repository file. */
	if ((fd = open(path, O_RDWR, REPOSITORY_MASK)) == -1)
			return (int) -1;

	/* Read repository id from file. */
	read(fd,rep_id+i,1);
	while(rep_id[i] !='\n')
	{
		i++;
		read(fd,rep_id+i,1);
	}
	rep_id[i]='\0';

	debug("rep_id:[%s]\n",rep_id);

	rep_pnt = rep_list;

	/* No external repositories registered. */
	if (rep_pnt == NULL) 
	{
		debug("no external repositores available.\n");
		close(fd);
		return (int) -1;
	}

	/* Search registered repositores for match. */
	i = 0;
	while (rep_pnt != NULL && strcmp(rep_id,rep_pnt->id) != 0) 
	{
		debug("rep id:%s\n",rep_pnt->id);
		i++;
		rep_pnt = rep_pnt->next;
	}

	/* No matching repositories. */
	if (rep_pnt == NULL) 
	{
		debug("no matching repositories.\n");
		return (int) -1;
	} 
	else
	{
		/* Use registered open function to open repository. */
		if (rep_pnt->open != NULL)
			return (int)rep_pnt->open(path, mode);

		else
		{
			/* Open not supported by external repository. */
                        return (int) -1;
		}
	}

	return (int) -1;
}

/*------------------------------------------------------------
Function: pml_close_repository
Purpose : calls close for a pmlstate rep.  See comments in pmlstate.c 
          and unixfs.c with the close functions.  Since this 
          method is only for pml, we simply call that method.
------------------------------------------------------------*/
int pml_close_repository(void)
{

		return (int)pmlstate_close_repository();
}


/*-----------------------------------------------------------
Function: fs_open
Purpose : calls open for a UNIXFS rep.  See comments in pmlstate.c
          and unixfs.c with the close functions (a parallel to 
          the open functions
-----------------------------------------------------------*/
repository* fs_open(path, type)
                const char* path;
                int type;
{
               return (repository*)UnixfsOpen(path, type);
}

    
/*------------------------------------------------------------
Function: fs_close
Purpose : calls close for a UNIXFS rep.  See comments in pmlstate.c 
          and unixfs.c with the close functions.
------------------------------------------------------------*/
int fs_close(rep)
		repository *rep;
{
		return (int)UnixfsClose(rep);
}

/*------------------------------------------------------------
Function: repository_assert
Purpose : checks to see if a query would be successful.
          NOTE:  The name of this function was not changed to pml...
		         because there is no version of this in pmlstate.c.
				 It is only used by unixfs.
------------------------------------------------------------*/
int repository_assert(rep, attribute, value)
		repository* rep;
		char*       attribute;
		char*       value;
{
		/* Assert not supported by external repository. */
		if ((rep_list + rep->type)->assert == NULL)
				return (int)-1;
		return (int)(rep_list + rep->type)->assert(rep, attribute, value);
}

/*------------------------------------------------------------
Function: pml_create_object
Purpose : Creates an index record struct (a pml_rec_t)
------------------------------------------------------------*/
pml_obj_t pml_create_object(void)
{
		return (pml_obj_t)pmlstate_create_object();
}

/*------------------------------------------------------------
Function: pml_delete_object
Purpose : marks objects for deletion
------------------------------------------------------------*/
int pml_delete_object(pml_obj_t objp)
{
		return (int)pmlstate_delete_object(objp);
}


/*------------------------------------------------------------
Function: pml_pack_objects
Purpose : deletes marked objects
------------------------------------------------------------*/
int pml_pack_objects(void)
{
		return (int)pmlstate_pack_objects();
}

/*------------------------------------------------------------
Function: pml_read_attribute
Purpose : Read a specified attribute from a given repository object
          the void* is cast as the correct type of repository in
	  that rep's specific read function.  Currently, the caller
          intending to read a unisfx rep obj must set alen and
          vlen = 0.
------------------------------------------------------------*/
int pml_read_attribute(void* objptr, void* atrp, size_t alen, void* valp, size_t vlen)
{
	/* It is a  unixfs call */
	if ((alen == 0) && (vlen == 0))
	     return (int)UnixfsRead(objptr, atrp, alen, valp, vlen);
        else 
	     return (int)pmlstate_read_attribute(objptr, atrp, alen,
valp, vlen);
}

/*------------------------------------------------------------
Function: pml_write_attribute
Purpose : writes a given attribute to a repository object.
          Currently only used for pmlstate.
------------------------------------------------------------*/
int pml_write_attribute(pml_obj_t objp, void* atrp, size_t alen, void* valp, size_t vlen)
{
	return (int)pmlstate_write_attribute(objp, atrp, alen, valp,
vlen);
}

/*------------------------------------------------------------
Function: pml_query_open
Purpose : Initiates a query of all repositories.  Either all pml
          or all Unixfs.  For Unixfs, the caller must set alen
          and vlen = 0.
------------------------------------------------------------*/
int pml_query_open(void* ** objptr, void* atrp, size_t alen, int op, void* valp, size_t vlen)
{
	if ((alen == 0) && (vlen == 0))
        	return (int)UnixfsQueryOpen(objptr, atrp, alen,
op, valp, vlen);
        else
                return (int)pmlstate_query_open(objptr, atrp, alen,
op, valp, vlen);
}

/*------------------------------------------------------------
Function: pml_query_close
Purpose : free the memory allocated by a successful query.  
          Currently, this is only used for pmlstate.
------------------------------------------------------------*/
int pml_query_close(void* ** objptr)
{
		return (int)pmlstate_query_close(objptr);
}
/*____________________________________________________________
The following functions are used by unixfs_repository.c functions
to do some of their work.  They may be better suited to exist in
that file, rather than here, but that decision is not being made
at this time.
____________________________________________________________*/

/*------------------------------------------------------------
Function: repository_addtoqueryresult
Purpose : 
------------------------------------------------------------*/
int repository_addtoqueryresult(query_result, rep_object)
		repository_queryresult* query_result;
		repository_object*      rep_object;
{
		if (query_result == NULL || rep_object == NULL) 
		{
				debug("did not add, error\n");
				return (int)-1;
		}

		if (query_result->start != NULL) 
		{
				debug("adding more elements\n");
				query_result->end->next    = (repository_querynode *)malloc(sizeof(repository_querynode));
				query_result->end          = query_result->end->next;
		} 
		
		else
		{
				debug("first element in result\n");
				query_result->start        = (repository_querynode *)malloc(sizeof(repository_querynode));
				query_result->end          = query_result->start;
				query_result->pos          = query_result->start;
				query_result->start->entry = rep_object;
				query_result->start->next  = NULL;
		}

		query_result->end->entry = rep_object;
		query_result->end->next  = NULL;
		query_result->entries++;

		return (int)0;
}

/*------------------------------------------------------------
Function: repository_openquery
Purpose : 
------------------------------------------------------------*/
repository_queryresult* repository_openquery()
{
		repository_queryresult *result;

		result = (repository_queryresult *) malloc(sizeof(repository_queryresult));

		if (result == NULL)
				return NULL;

		result->entries = 0;
		result->start   = NULL;
		result->end     = NULL;
		result->pos     = NULL;

		return (repository_queryresult *)result;
}

/*------------------------------------------------------------
Function: repository_closequery
Purpose : 
------------------------------------------------------------*/
int repository_closequery(query_result)
		repository_queryresult* query_result;
{
		repository_querynode* rep_node;
		repository_querynode* tmp;

		if (query_result == NULL)
				return (int)-1;

		rep_node = query_result->start;

		while (rep_node != NULL) 
		{
				tmp = rep_node->next;
				free(rep_node->entry);
				free(rep_node);
				rep_node = tmp;
		}

		free(query_result);

		return (int)0;
}

/*------------------------------------------------------------
Function: repository_querynext
Purpose : 
------------------------------------------------------------*/
repository_object *repository_querynext(query_result)
        repository_queryresult* query_result;
{
		repository_querynode* res;

		if (query_result == NULL || query_result->pos == NULL)
				return NULL;

		res               = query_result->pos;
		query_result->pos = query_result->pos->next;

		return (repository_object *)res->entry;
}

/*------------------------------------------------------------
Function: repository_queryrewind
Purpose : 
------------------------------------------------------------*/
int repository_queryrewind(query_result)
		repository_queryresult* query_result;
{
		if (query_result == NULL)
				return (int)-1;

		query_result->pos = query_result->start;

		return (int)0;
}

/* THIS IS FOR TESTING PURPOSES, IF YOU STILL SEE IT,
   IT'S BECAUSE I FORGOT TO DELETE IT !!!     */

int main()
{
return 0;
}
