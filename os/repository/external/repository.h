/* repository.h */

#ifndef _REPOSITORY_H
#define _REPOSITORY_H

#include "debug.h"

#define EQ 1
#define NE 2
#define GT 3
#define LT 4
#define GE 5
#define LE 6

#define REPOSITORY_MASK 	0666

typedef struct {
		char *name;
		char *id;
		void *info;
		int type;
} repository;

typedef struct repository_entry repository_entry;

typedef struct {
		int type;
		char *id;
		void *object;
} repository_object;


/* Definitions for queries. */
typedef struct repository_querynode repository_querynode;

struct repository_querynode {
	repository_object *entry;
	repository_querynode *next;
};


struct repository_queryresult {
		repository_querynode *start;
		repository_querynode *end;
		repository_querynode *pos;
		int entries;
};

typedef struct repository_queryresult repository_queryresult;

/* Definitions for external repository entry. */
struct repository_entry {
		char *id;
		int	nr;
		repository *(*open)(int,int);
		int (*close)(repository *);
		int (*assert)(repository *, char *, char *);
		int (*query)(repository_queryresult **, repository *, char *, int operand, char *);
		int (*queryclose)(repository_queryresult *);
		int (*read)(repository_object *, char *, char **);
		repository_entry *next;
};

/* Repository function prototypes. */
repository * repository_open(char *);

int repository_close(repository *);

int repository_query(repository_queryresult **, repository *, char *, int, char *);

int repository_assert(repository *, char *, char *);

int repository_read(repository_object *, repository *, char *, char **);

int repository_initexternal(char *id,
		repository *(*open)(int,int),
		int (*close)(repository *),
		int (*assert)(repository *, char *, char *),
		int (*query)(repository_queryresult **, repository *, char *, int, char *),
		int (*read)(repository_object *, char *, char **));

int repository_init();

int repository_addtoqueryresult(repository_queryresult *, repository_object *);

repository_queryresult *repository_openquery();

int repository_closequery(repository_queryresult *);

repository_object *repository_querynext(repository_queryresult *);

int repository_queryrewind(repository_queryresult *);



#endif

