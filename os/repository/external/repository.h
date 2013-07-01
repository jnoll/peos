/* repository.h */

/* This file is the header for the generic repository interface information
   It was last modified Summer 2000.  
*/

#ifndef _REPOSITORY_H
#define _REPOSITORY_H

#include "debug.h"
#include "pml_state.h"
/*#include "C:\WINDOWS\DESKTOP\My Briefcase\CU Masters Andy\CSC 5728\pml_state.h"*/



#define EQ 1
#define NE 2
#define GT 3
#define LT 4
#define GE 5
#define LE 6

#define REPOSITORY_MASK 	0666

typedef struct 
{
  char *name;
  char *id;
  void *info;
  int type;
} repository;

typedef struct repository_entry repository_entry;

typedef struct 
{
  int type;
  char *id;
  void *object;
} repository_object;

/* -----------------------------------------------------------------
Typedefs and structs to hold the results of a query in the form
of a linked list
------------------------------------------------------------------*/
typedef struct repository_querynode repository_querynode;

struct repository_querynode 
{
  repository_object *entry;
  repository_querynode *next;
};

struct repository_queryresult 
{
  repository_querynode *start;
  repository_querynode *end;
  repository_querynode *pos;
  int entries;
};

typedef struct repository_queryresult repository_queryresult;

/*-----------------------------------------------------------------------
Struct defining all of the pointers to the interface functions for any
repository instance
-----------------------------------------------------------------------*/
struct repository_entry 
{
  char*        id;
  int          (*open)(const char*, pml_mode_t);
  repository*  (*openfs)(const char*, int);
  int          (*close)(void);
  int          (*closefs)(repository *); 
  pml_obj_t    (*create)(void);
  int          (*del)(pml_obj_t);
  int          (*pack)(void);
  int          (*read)(void*, void *, size_t, void *, size_t);
  int          (*write)(pml_obj_t, void *, size_t, void *, size_t);               
  int          (*queryopen)(void* **, void *, size_t, int, void *, size_t);
  int          (*queryclose)(void* **);
  int          (*assert)(repository *, char *, char *);
  int          (*query)(repository_queryresult **, repository *, char *, int, char *);
  int	nr;
  repository_entry* next;
};

/*---------------------------------------------------------------------------
Despite the non-generic names, the following functions are the generic interface 
functions used by all existing types of interfaces (currently UNIX FS and PML).  
These functions are called by the kernel and VM to access all types of repositories
and when called, they will dispatch a call to the appropriate function
for the repository in question.
---------------------------------------------------------------------------*/

int           pml_open_repository(const char* , pml_mode_t);
repository*   fs_open(const char*, int);
int           pml_close_repository(void);
int           fs_close(repository *);
pml_obj_t     pml_create_object(void);
int           pml_delete_object(pml_obj_t);
int           pml_pack_objects(void);
int           pml_read_attribute(void* , void *, size_t, void *, size_t);
int           pml_write_attribute(pml_obj_t, void *, size_t, void *, size_t);               
int           pml_query_open(void* **, void *, size_t, int, void *,
size_t);
int           pml_query_close(void* **);
int           assert(repository *, char *, char *);
int           query(repository_queryresult **, repository *, char *, int, char *);

/*-----------------------------------------------------------------------
This function will generate a struct containing pointers to all of the functions
that are used by a repository.  When a repository comes into existance, it 
gets a set of pointers to it's interface functions.  The fs_close is explained
with the init() functions in pmlstate.c and unixfs.c
-----------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C"{
#endif

int repository_initexternal(char *      id,
			    int         (*Open)(const char* , pml_mode_t),
                            repository* (*Openfs)(const char*, int),
			    int         (*Close)(void),
			    int         (*Closefs)(repository *),
			    pml_obj_t   (*CreateObj)(void),
			    int         (*DeleteObj)(pml_obj_t),
			    int         (*Pack)(void),
			    int         (*ReadAttr)(void* , void *, size_t, void *, size_t),
			    int         (*WriteAttr)(pml_obj_t, void *, size_t, void *, size_t),
			    int         (*QueryOpen)(void* **, void *, size_t, int, void *, size_t),
			    int         (*QueryClose)(void* **),
			    int         (*Assert)(repository *, char *, char *),
			    int         (*Query)(repository_queryresult **, repository *, char *, int, char *)
);

/* In Read, QueryOpen, and QueryClose, the first arg is a void* that will be cast to the 
appropriate type by the function that is called*/

int                     repository_init();

/* *** Still need to figure out what to do with these functions...  
       They are currently kept around because they do some of the work for
	   UNIXFS functions.  They may belong in unixfs.h, but that decision is 
	   not being made now   */

int                     repository_addtoqueryresult(repository_queryresult *, repository_object *);
repository_queryresult* repository_openquery();
int                     repository_closequery(repository_queryresult *);
repository_object*      repository_querynext(repository_queryresult *);
int                     repository_queryrewind(repository_queryresult *);

#ifdef __cplusplus
}
#endif
#endif



