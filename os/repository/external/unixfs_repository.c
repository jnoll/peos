/* unixfs_repository.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <malloc.h>
#include <time.h>

#include "repository.h"
#include "unixfs_repository.h"


repository *
unixfs_open(fd, type)
		int fd, type;
{
		repository *rep;
		char wdir[256];
		int i;

		printf("unixfs_open\n");
		rep = malloc(sizeof(repository));

		/* Unable to allocate memory for repository structure. */
		if (rep == NULL) {
				close(fd);
				return (repository *)NULL;
		}

		rep->id = "UNIXFS";
		rep->info = (unixfs_info *)malloc(sizeof(unixfs_info));
		if (rep->info == NULL) {
				close(fd);
				return (repository *)NULL;
		}

		i = 0;
		read(fd,wdir+i,1);
		while(wdir[i] !='\n') {
				i++;
				read(fd,wdir+i,1);
		}
		wdir[i]='\0';

		debug("wdir = %s\n",wdir);
		((unixfs_info *) rep->info)->w_dir = wdir;
		rep->name = NULL;
		rep->type = type;

		close(fd);

		return (repository *)rep;
}

int
unixfs_close(rep)
		repository *rep;
{
		debug("unixfs_close\n");
		if (rep != NULL) {
				/* Deallocate memory from repository structure. */
				free(rep->info);
				free(rep);
				return (int)0;
		} else
				/* Argument no repository. */
				return (int)-1;
}


unixfs_object *
unixfs_createobject(name)
		char *name;
{
		unixfs_object *ret;
		struct stat *statbuf;
		struct tm *tm_data;
		int	year, month, day;

		ret = (unixfs_object *)malloc(sizeof(unixfs_object));
		statbuf = (struct stat *) malloc(sizeof(struct stat));

		ret->name = (char *)strdup(name);
		ret->date = (char *)malloc(15);
		ret->size = (char *)malloc(10);

		stat(name, statbuf);

		tm_data = gmtime(&statbuf->st_mtime);
		year = 1900 + tm_data->tm_year;
		month = tm_data->tm_mon + 1;
		day = tm_data->tm_mday;

		sprintf(ret->date,"%d-%s%d-%s%d", year, (month < 10 ? "0" : ""), month, (day < 10 ? "0":""), day);
		sprintf(ret->size,"%d",statbuf->st_size);
		debug("object:%s\n",ret->name);
		debug("date:%s\n",ret->date);
		debug("size:%s\n",ret->size);

		return (unixfs_object *)ret;
}

repository_object *
unixfs_createrepositoryobject(object)
		unixfs_object *object;
{
		repository_object *ret;
		ret = (repository_object *)malloc(sizeof(repository_object));
		ret->id = "UNIXFS";
		ret->object = object;
		return (repository_object *)ret;
}

int
unixfs_query(query_res, rep, attribute, operand, value)
		repository_queryresult **query_res;
		repository *rep;
		char *attribute;
		int operand;
		char *value;
{
		struct dirent *dir_entry;
		unixfs_object *obj;
		repository_object *rep_obj;
		DIR *dir;
		char *dirname;
		char *cwd,*cmp_str;
		int attr, cmp_res, op_cmp, add_obj, entries;

		repository_queryresult *query_result, *query_resultpnt;

		attr = -1;
		entries = 0;

		if (attribute == NULL) {
				return (int)-1;
		}

		if (strcmp(attribute,"name") == 0)
				attr = NAME;
		else if (strcmp(attribute,"date") == 0)
				attr = DATE;
		else if (strcmp(attribute,"size") == 0)
				attr = SIZE;

		if (attr < 0) {
				return (int)-1;
		}

		dirname = ((unixfs_info *) rep->info)->w_dir;
		debug("UNIXFS query on external repository [%s]\n", dirname);

		dir = opendir(dirname);
		/* Repository directory not found. */
		if (dir == NULL)
				return (int)-1;

		cwd = (char *) getcwd(NULL, 128);
		chdir(dirname);

		query_result = repository_openquery();

		debug("UNIXFS start reading directory.\n");
		while ((dir_entry = readdir(dir)) != NULL) {
				op_cmp = 0;
				obj = unixfs_createobject(dir_entry->d_name);
				debug("--> %s\n",obj->name);
				switch (attr) {
				case NAME:
					cmp_str = obj->name;
					break;
				case DATE:
					cmp_str = obj->date;
					break;
				case SIZE:
					cmp_str = obj->size;
					break;
				default:
					cmp_str = NULL;
					break;
				}
				add_obj = 0;
				cmp_res = strcmp(dir_entry->d_name, value);
				switch (operand) {
				case EQ:
						if (cmp_res == 0)
								add_obj = 1;
						break;
				case NE:
						if (cmp_res != 0)
								add_obj = 1;
						break;
				case GT:
						if (cmp_res > 0)
								add_obj = 1;
						break;
				case LT:
						if (cmp_res < 0)
								add_obj = 1;
						break;
				case GE:
						if (cmp_res >= 0)
								add_obj = 1;
						break;
				case LE:
						if (cmp_res <= 0)
								add_obj = 1;
						break;
				}

				if (add_obj != 0) {
						rep_obj = unixfs_createrepositoryobject(obj);
						repository_addtoqueryresult(query_result, rep_obj);
						entries++;
						debug("found match\n");
				} else
						free(obj);

		}
		closedir(dir);
		chdir(cwd);
		*query_res = (repository_queryresult *)query_result;
		return (int)entries;
}

int
unixfs_assert(rep, attribute, value)
		repository *rep;
		char *attribute;
		char *value;
{
		struct dirent *dir_entry;
		unixfs_object *obj;
		repository_object *rep_obj;
		DIR *dir;
		char *dirname;
		char *cwd,*cmp_str;
		int attr, cmp_res;

		attr = -1;

		if (attribute == NULL) {
				return (int)-1;
		}
		if (strcmp(attribute,"name") == 0)
				attr = NAME;
		else if (strcmp(attribute,"date") == 0)
				attr = DATE;
		else if (strcmp(attribute,"size") == 0)
				attr = SIZE;

		if (attr < 0) {
				return (int)-1;
		}

		dirname = ((unixfs_info *) rep->info)->w_dir;
		debug("UNIXFS assert on external repository [%s]\n", dirname);

		dir = opendir(dirname);
		/* Repository directory not found. */
		if (dir == NULL)
				return (int)-1;

		cwd = (char *) getcwd(NULL, 128);
		chdir(dirname);

		debug("UNIXFS start reading directory.\n");
		while ((dir_entry = readdir(dir)) != NULL) {
				obj = unixfs_createobject(dir_entry->d_name);
				switch (attr) {
				case NAME:
					cmp_str = obj->name;
					break;
				case DATE:
					cmp_str = obj->date;
					break;
				case SIZE:
					cmp_str = obj->size;
					break;
				default:
					cmp_str = NULL;
					break;
				}

				cmp_res = strcmp(dir_entry->d_name, value);
				free(obj);
				if (cmp_res == 0) {
						closedir(dir);
						chdir(cwd);
						return (int)1;
				}
		}
		closedir(dir);
		chdir(cwd);
		return (int)0;
}


int
unixfs_read(obj, attribute, value)
		repository_object *obj;
		char *attribute;
		char **value;
{
		int attr;
		char *cmp_str;
		unixfs_object *u_obj;

		attr = -1;
		if (strcmp(attribute,"name") == 0)
				attr = NAME;
		else if (strcmp(attribute,"date") == 0)
				attr = DATE;
		else if (strcmp(attribute,"size") == 0)
				attr = SIZE;
		switch (attr) {
				case NAME:
					cmp_str = ((unixfs_object *)obj->object)->name;
					break;
				case DATE:
					cmp_str = ((unixfs_object *)obj->object)->date;
					break;
				case SIZE:
					cmp_str = ((unixfs_object *)obj->object)->size;
					break;
				default:
					cmp_str = NULL;
					break;
		}
		*value = strdup(cmp_str);
		return (int)0;
}

int
unixfs_init() {
		repository_initexternal("UNIXFS",
								unixfs_open,
								unixfs_close,
								unixfs_assert,
								unixfs_query,
								unixfs_read);
		return (int)0;
}
