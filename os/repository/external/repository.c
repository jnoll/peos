/* repository.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "repository.h"

#include "unixfs_repository.h"

repository_entry *rep_list, *rep_listnext;
int nr_repositories;


int
repository_init()
{

		rep_list = NULL;
		rep_listnext = NULL;
		nr_repositories = 0;

		unixfs_init();

		return (int)0;
}

int
repository_initexternal(id, open, close, assert, query, read)
		char *id;
		repository  *(*open)(int,int);
		int (*close)(repository *);
		int (*assert)(repository *, char *, int, char *);
		int (*query)(repository_queryresult *, repository *, char *, int, char *);
		int (*read)(repository_object *, char *, char *);
{
		repository_entry *rep_pnt;

		rep_pnt = malloc(sizeof(repository_entry));
		if (rep_pnt == NULL)
				return (int)-1;
		rep_pnt->next = NULL;
		rep_pnt->open = open;
		rep_pnt->close = close;
		rep_pnt->assert = assert;
		rep_pnt->query = query;
		rep_pnt->read = read;
		rep_pnt->nr = nr_repositories;
		rep_pnt->id = id;
		nr_repositories++;

		if (rep_list == NULL) {
				rep_list = rep_pnt;
				rep_listnext = rep_pnt;
		} else {
				rep_listnext->next = rep_pnt;
				rep_listnext = rep_pnt;
		}
		return (int)0;
}

repository *
repository_open(path)
		char *path;
{
		char rep_id[256];
		int fd, i;
		repository_entry *rep_pnt;

		i = 0;
		/* Open repository file. */
		if ((fd = open(path, O_RDWR, REPOSITORY_MASK)) == -1)
				return (repository *)NULL;
		/* Read repository id from file. */
		read(fd,rep_id+i,1);
		while(rep_id[i] !='\n') {
				i++;
				read(fd,rep_id+i,1);
		}
		rep_id[i]='\0';

		printf("rep_id:[%s]\n",rep_id);

		rep_pnt = rep_list;

		/* No external repositories registered. */
		if (rep_pnt == NULL) {
				printf("no external repositores available.\n");
				close(fd);
				return (repository *)NULL;
		}

		/* Search registered repositores for match. */
		i = 0;
		while (rep_pnt != NULL && strcmp(rep_id,rep_pnt->id) != 0) {
				printf("rep id:%s\n",rep_pnt->id);
				i++;
				rep_pnt = rep_pnt->next;
		}

		/* No matching repositories. */
		if (rep_pnt == NULL) {
				printf("no matching repositories.\n");
				return (repository *)NULL;
		} else
				/* Use registered open function to open repository. */
				if (rep_pnt->open != NULL)
						return (repository *)rep_pnt->open(fd, i);
				else
						/* Open not supported by external repository. */
						return (repository *)NULL;

		return (int)0;
}

int
repository_close(rep)
		repository *rep;
{
		(rep_list+rep->type)->close(rep);
		return (int)0;
}

int
repository_assert(rep, attribute, operand, value)
		repository *rep;
		char *attribute;
		int operand;
		char *value;
{
		/* Assert not supported by external repository. */
		if ((rep_list + rep->type)->assert == NULL)
				return (int)-1;
		return (int)(rep_list + rep->type)->assert(rep, attribute, operand, value);
}

int
repository_query(query_result, rep, attribute, operand, value)
		repository_queryresult *query_result;
		repository *rep;
		char *attribute;
		int operand;
		char *value;
{
		if ((rep_list + rep->type)->query == NULL)
				return (int)-1;
		return (int)(rep_list + rep->type)->query(query_result, rep, attribute, operand, value);
}

int
repository_read(rep_object, rep, attribute, operand, value)
		repository_object *rep_object;
		repository *rep;
		char *attribute;
		int operand;
		char *value;
{
		/* Read not supported by external repository. */
		if ((rep_list + rep->type)->read == NULL)
				return (int)-1;
		return (int)(rep_list +rep->type)->read(rep_object, attribute, value);
}




