/* unixfs_repository.c */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <malloc.h>

#include "repository.h"
#include "unixfs_repository.h"


repository *
unixfs_open(fd, type)
		int fd, type;
{
		repository *rep;


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

		((unixfs_info *) rep->info)->w_dir = ".";
		//printf("UNIXFS - repository directory [%s]\n",rep->info->w_dir);
		rep->name = NULL;
		rep->type = type;

		close(fd);

		return (repository *)rep;
}

int
unixfs_close(rep)
		repository *rep;
{
		printf("unixfs_close\n");
		if (rep != NULL) {
				/* Deallocate memory from repository structure. */
				//free(rep->id);
				free(rep->info);
				//free(rep->name);
				free(rep);
				return (int)0;
		} else
				/* Argument no repository. */
				return (int)-1;
}

int
unixfs_addtoquery(query_res, fname)
		repository_queryresult *query_res;
		char *fname;
{
		query_res->entry = malloc(sizeof(unixfs_object));
}


int
unixfs_query(query_res, rep, attribute, operand, value)
		repository_queryresult *query_res;
		repository *rep;
		char *attribute;
		int operand;
		char *value;
{
		struct dirent *dir_entry;
		DIR *dir;
		char *dirname;
		char *cwd;
		int attr, cmp_res, op_cmp;

		repository_queryresult *query_result, *query_resultpnt;

		attr = -1;

		if (attribute == NULL) {
				return (int)-1;
		}

		if (strcmp(attribute,"name") == 0)
				attr = NAME;
		else if (strcmp(attribute,"user_id") == 0)
				attr = USER_ID;
		else if (strcmp(attribute,"group_id") == 0)
				attr = GROUP_ID;
		else if (strcmp(attribute,"date") == 0)
				attr = DATE;
		else if (strcmp(attribute,"size") == 0)
				attr = SIZE;

		if (attr < 0) {
				return (int)-1;
		}

		dirname = ((unixfs_info *) rep->info)->w_dir;
		printf("UNIXFS query on external repository [%s]\n", dirname);

		dir = opendir(dirname);
		/* Repository directory not found. */
		if (dir == NULL)
				return (int)-1;

		cwd = (char *) getcwd(NULL, 128);
		chdir(dirname);

		query_result = malloc(sizeof(repository_queryresult));
		query_resultpnt = query_result;

		printf("UNIXFS start reading directory.\n");
		while ((dir_entry = readdir(dir)) != NULL) {
				op_cmp = 0;
				switch (attr) {
				case NAME:
					// printf("comparing %s %s\n",dir_entry->d_name, value);
					cmp_res = strcmp(dir_entry->d_name, value);
					switch (operand) {
					case EQ:
						if (cmp_res == 0) {
								printf("[%s]\n",dir_entry->d_name);
								unixfs_addtoquery(query_resultpnt, dir_entry->d_name);
						}
						break;
					}
					break;
				case USER_ID:
					break;
				case GROUP_ID:
					break;
				case DATE:
					break;
				case SIZE:
					break;
				default:
					closedir(dir);
					chdir(cwd);
					return (int)-1;
					break;
				}
		}
		closedir(dir);
		chdir(cwd);
}

int
unixfs_init() {
		repository_initexternal("UNIXFS",unixfs_open,unixfs_close,NULL,unixfs_query,NULL);
		return (int)0;
}
