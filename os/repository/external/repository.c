/* repository.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "repository.h"

//#include "unixfs_repository.h"

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
		int (*assert)(repository *, char *, char *);
		int (*query)(repository_queryresult **, repository *, char *, int, char *);
		int (*read)(repository_object *, char *, char **);
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

		debug("rep_id:[%s]\n",rep_id);

		rep_pnt = rep_list;

		/* No external repositories registered. */
		if (rep_pnt == NULL) {
				debug("no external repositores available.\n");
				close(fd);
				return (repository *)NULL;
		}

		/* Search registered repositores for match. */
		i = 0;
		while (rep_pnt != NULL && strcmp(rep_id,rep_pnt->id) != 0) {
				debug("rep id:%s\n",rep_pnt->id);
				i++;
				rep_pnt = rep_pnt->next;
		}

		/* No matching repositories. */
		if (rep_pnt == NULL) {
				debug("no matching repositories.\n");
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
repository_assert(rep, attribute, value)
		repository *rep;
		char *attribute;
		char *value;
{
		/* Assert not supported by external repository. */
		if ((rep_list + rep->type)->assert == NULL)
				return (int)-1;
		return (int)(rep_list + rep->type)->assert(rep, attribute, value);
}

int
repository_query(query_result, rep, attribute, operand, value)
		repository_queryresult **query_result;
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
repository_read(rep_object, rep, attribute, value)
		repository_object *rep_object;
		repository *rep;
		char *attribute;
		char **value;
{
		/* Read not supported by external repository. */
		if ((rep_list + rep->type)->read == NULL)
				return (int)-1;
		return (int)(rep_list + rep->type)->read(rep_object, attribute, value);
}


int
repository_addtoqueryresult(query_result, rep_object)
		repository_queryresult *query_result;
		repository_object *rep_object;
{
		if (query_result == NULL || rep_object == NULL) {
				debug("did not add, error\n");
				return (int)-1;
		}
		if (query_result->start != NULL) {
				debug("adding more elements\n");
				query_result->end->next = (repository_querynode *)malloc(sizeof(repository_querynode));
				query_result->end = query_result->end->next;
		} else {
				debug("first element in result\n");
				query_result->start = (repository_querynode *)malloc(sizeof(repository_querynode));
				query_result->end = query_result->start;
				query_result->pos = query_result->start;
				query_result->start->entry = rep_object;
				query_result->start->next = NULL;
		}
		query_result->end->entry = rep_object;
		query_result->end->next = NULL;
		query_result->entries++;
		return (int)0;
}

repository_queryresult *
repository_openquery()
{
		repository_queryresult *result;

		result = (repository_queryresult *) malloc(sizeof(repository_queryresult));
		if (result == NULL)
				return NULL;
		result->entries = 0;
		result->start = NULL;
		result->end = NULL;
		result->pos = NULL;
		return (repository_queryresult *)result;
}

int
repository_closequery(query_result)
		repository_queryresult *query_result;
{
		repository_querynode *rep_node,*tmp;

		if (query_result == NULL)
				return (int)-1;

		rep_node = query_result->start;
		while (rep_node != NULL) {
				tmp = rep_node->next;
				free(rep_node->entry);
				free(rep_node);
				rep_node = tmp;
		}
		free(query_result);
		return (int)0;
}

repository_object *repository_querynext(query_result)
		repository_queryresult *query_result;
{
		repository_querynode *res;

		if (query_result == NULL || query_result->pos == NULL)
				return NULL;
		res = query_result->pos;
		query_result->pos = query_result->pos->next;
		return (repository_object *)res->entry;
}

int repository_queryrewind(query_result)
		repository_queryresult *query_result;
{
		if (query_result == NULL)
				return (int)-1;
		query_result->pos = query_result->start;
		return (int)0;
}

