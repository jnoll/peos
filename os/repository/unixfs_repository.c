/* unixfs_repository.c */

/* -----------------------------------------------------------
This file contains the source for the interface to the UNIX
file system.  This interface is used by PML to manipulate files
This interface is exported via a function in repository.c which
sets up a struct with pointers to all of the functions contained
here.

Last Modified: Summer 2000

The modifications made in summer 2000 to the pre-existing code 
are slight, but hopefully this code is now well documented,
which should be the start of a trend....
------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <malloc.h>
#include <math.h>
#include <time.h>

#include "repository.h"
#include "unixfs_repository.h"
#include "debug.h"
/*#include "C:\WINDOWS\DESKTOP\My Briefcase\CU Masters Andy\CSC 5728\repository.h"*/
/*#include "C:\WINDOWS\DESKTOP\My Briefcase\CU Masters Andy\CSC 5728\unixfs_repository.h"*/


/*------------------------------------------------------------
Function: UnixfsOpen
Purpose : to take a file and create a repository instance out of it.
          Summer 2000 changes:
          The first argument has been changed from a file descriptor to 
		  a path for comliance with the pml format.  It allowed
		  for the commenting out of code that serves to get the
		  path based on the file descriptor.  
		  *** calls to open a unixfs repostory will need to include 
		  the path now, where they did not previously.
------------------------------------------------------------*/
repository* UnixfsOpen(path, type)
		const char* path;
                int         type;
{
		repository* rep;
/*		char        wdir[256];
		int         i;*/
		RepListNode* temp;

		printf("unixfs_open\n");
		rep = malloc(sizeof(repository));

		/* Unable to allocate memory for repository structure. */
		if (rep == NULL)
		{
/*				close(fd);*/
				return (repository *)NULL;
		}

		rep->id = "UNIXFS";
		rep->info = (unixfs_info *)malloc(sizeof(unixfs_info));

		if (rep->info == NULL) 
		{
/*				close(fd);*/
				return (repository *)NULL;
		}

/*		i = 0;
		read(fd,wdir+i,1);

		while(wdir[i] !='\n') 
		{
				i++;
				read(fd,wdir+i,1);
		}
		wdir[i]='\0';

		debug("wdir = %s\n",wdir);
		((unixfs_info *) rep->info)->w_dir = wdir;*/

		/* *** added Summer 2000 */
		strcpy(((unixfs_info *) rep->info)->w_dir, path);

		rep->name = NULL;
		rep->type = type;

/*		close(fd);*/

	    /* Add this object to the list */

		temp = (RepListNode*) malloc(sizeof(RepListNode));
		temp->data  = rep;
		temp->link  = (RepListNode*)RepListHead->link;
		RepListHead = temp;

		return (repository *)rep;
}

/*------------------------------------------------------------
Function: UnixfsClose
Purpose : to free up the memory associated with a repository
------------------------------------------------------------*/
int UnixfsClose(rep)
		repository* rep;
{
                RepListNode* temp;
                RepListNode* iter;

		debug("unixfs_close\n");
		if (rep != NULL) 
		{
			    /* Remove this rep from the list. We start with the empty first node
			       because we need to be at the preceeding node in order to delete correctly*/
			        iter = RepListHead;
				while (iter->link != NULL && iter->link->data != rep)
				{
					/* find the correct node */
					iter = iter->link;
				}
				if (iter->link == NULL)
					debug("error, rep not found in list\n");
				else
				{
					/* delete the correct node */
				        temp = iter->link->link;
					free (iter->link);
					iter->link = temp;
				}

				/* Deallocate memory from repository structure. */
				free(rep->info);
				free(rep);
				return (int)0;
		}
		
		else
				/* Argument no repository. */
				return (int)-1;
}

/*------------------------------------------------------------
Function: UnixfsCreateObject
Purpose : To create a unixfs_object with the current name, date, size.
          This func is used internally by query and assert to build
		  responses to queries.
------------------------------------------------------------*/
unixfs_object * UnixfsCreateObject(name)
		char*          name;
{
		unixfs_object* ret;
		struct stat*   statbuf;
		struct tm*     tm_data;
		int	       year;
		int            month;
		int            day;

		ret = (unixfs_object *)malloc(sizeof(unixfs_object));
		statbuf = (struct stat *) malloc(sizeof(struct stat));

		ret->name = (char *)strdup(name);
		ret->date = (char *)malloc(15);
		ret->size = (char *)malloc(10);

		stat(name, statbuf);

		tm_data = gmtime(&statbuf->st_mtime);
		year    = 1900 + tm_data->tm_year;
		month   = tm_data->tm_mon + 1;
		day     = tm_data->tm_mday;

		sprintf(ret->date,"%d-%s%d-%s%d", year, (month < 10 ? "0" : ""), month, (day < 10 ? "0":""), day);
		sprintf(ret->size,"%d",statbuf->st_size);

		debug("object:%s\n",ret->name);
		debug("date:%s\n",ret->date);
		debug("size:%s\n",ret->size);

		return (unixfs_object *)ret;
}

/*------------------------------------------------------------
Function: UnixfsCreateRepositoryObject
Purpose : Turns a unixfs_object into a repository object.
          This func is used internally by query and assert to build
		  responses to queries.
------------------------------------------------------------*/
repository_object* UnixfsCreateRepositoryObject(object)
		unixfs_object*     object;
{
		repository_object* ret;

		ret = (repository_object *)malloc(sizeof(repository_object));
		ret->id = "UNIXFS";
		ret->object = object;

		return (repository_object *)ret;
}

/*------------------------------------------------------------
Function: UnixfsQueryOpen
Purpose : Checks all unixfs_repositories for a given attribute (name, 
          date, size) and adds the rep to the query result list 
		  (query_res) if it satisfies the query.
------------------------------------------------------------*/
int UnixfsQueryOpen(query_res, attribute, alen, operand, value, vlen)
		repository_queryresult** query_res;
		char*                    attribute;
		int                      alen; /* not used, a dummy to fit the shape of pmlquery */
		int                      operand;
		char*                    value;
		int                      vlen; /* not used, a dummy to fit the shape of pmlquery */
{
       	        repository*              rep;
		struct dirent*           dir_entry;
		unixfs_object*           obj;
		repository_object*       rep_obj;
		DIR*                     dir;
		char*                    dirname;
		char*                    cwd;
		char*                    cmp_str;
		int                      attr;
		int                      cmp_res;
		int                      op_cmp;
		int                      add_obj;
		int                      entries;
		repository_queryresult*  query_result;
		repository_queryresult*  query_resultpnt;
                RepListNode* iter;

		attr = -1;
		entries = 0;

		if (attribute == NULL)
				return (int)-1;

		if (strcmp(attribute,"name") == 0)
				attr = NAME;
		else if (strcmp(attribute,"date") == 0)
				attr = DATE;
		else if (strcmp(attribute,"size") == 0)
				attr = SIZE;

		if (attr < 0) 
				return (int)-1;

/*  LOOP FROM HERE TO BOTTOM OVER ALL REPS, SETTING rep EQUAL TO THE CURRENT ONE */
		iter = RepListHead->link;   /*points to first rep */
		while (iter->data != NULL)
		{
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

			while ((dir_entry = readdir(dir)) != NULL) 
			{
					op_cmp = 0;
					obj    = UnixfsCreateObject(dir_entry->d_name);

					debug("--> %s\n",obj->name);

					switch (attr)
					{
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

					switch (operand)
					{
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

					if (add_obj != 0)
					{
							rep_obj = UnixfsCreateRepositoryObject(obj);
							repository_addtoqueryresult(query_result, rep_obj);
							entries++;

							debug("found match\n");
					} 
					else
							free(obj);
			}

			closedir(dir);
			iter = iter->link;
		}
		chdir(cwd);
		*query_res = (repository_queryresult *)query_result;

		return (int)entries;
}

/*------------------------------------------------------------
Function: UnixfsAssert
Purpose : Checks to see if a query would be successful or not
------------------------------------------------------------*/
int UnixfsAssert(repository* rep, char* attribute, char* value)
{
		struct dirent*     dir_entry;
		unixfs_object*     obj;
		repository_object* rep_obj;
		DIR*               dir;
		char*              dirname;
		char*              cwd;
		char*              cmp_str;
		int                attr;
		int                cmp_res;
                RepListNode*       iter;

		attr = -1;
		if (attribute == NULL) 
				return (int)-1;

		if (strcmp(attribute,"name") == 0)
				attr = NAME;
		else if (strcmp(attribute,"date") == 0)
				attr = DATE;
		else if (strcmp(attribute,"size") == 0)
				attr = SIZE;

		if (attr < 0)
				return (int)-1;

/*  LOOP FROM HERE TO BOTTOM OVER ALL REPS, SETTING rep EQUAL TO THE CURRENT ONE */
		iter = RepListHead->link;   /*points to first rep */
		while (iter->data != NULL)
		{
			dirname = ((unixfs_info *) rep->info)->w_dir;

			debug("UNIXFS assert on external repository [%s]\n", dirname);

			dir = opendir(dirname);

			/* Repository directory not found. */
			if (dir == NULL)
					return (int)-1;

			cwd = (char *)getcwd(NULL, 128);
			chdir(dirname);

			debug("UNIXFS start reading directory.\n");

			while ((dir_entry = readdir(dir)) != NULL)
			{
					obj = UnixfsCreateObject(dir_entry->d_name);
					switch (attr) 
					{
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

					if (cmp_res == 0) 
					{
							closedir(dir);
							chdir(cwd);
							return (int)1;
					}
			}

			closedir(dir);
			iter = iter->link;
		}
		chdir(cwd);
		return (int)0;
}

/*------------------------------------------------------------
Function: UnixfsRead
Purpose : Reads a given attribute from a unixfs_repository object
------------------------------------------------------------*/
int UnixfsRead(void* objptr, void* attrib, size_t UnusedDummy1, void* val, size_t UnusedDummy2)
{
	    /* cast the void*(s) correctly */
		repository_object* obj       = (repository_object*) objptr;  
                char*              attribute = (char*) attrib;
                char**             value     = (char**) val;

		int                attr;
		char*              cmp_str;
		unixfs_object*     u_obj;

		attr = -1;

		if (strcmp(attribute,"name") == 0)
				attr = NAME;
		else if (strcmp(attribute,"date") == 0)
				attr = DATE;
		else if (strcmp(attribute,"size") == 0)
				attr = SIZE;

		switch (attr) 
		{
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

/*------------------------------------------------------------
Function: UnixfsInit
Purpose : To initialize this external repository and provide 
          pointers to all of its functions, NULL for I/F functions
		  not posessed by this repository type.
		  *** still need to determine Query vs. QueryOpen and QueryClose
------------------------------------------------------------*/
int UnixfsInit() 
{
		repository_initexternal("UNIXFS",
								NULL,
                                                                UnixfsOpen,
								NULL,     /* ***see comment below*/
								UnixfsClose,
								NULL, NULL, NULL, 
								UnixfsRead,
								NULL, NULL, 
								UnixfsQueryOpen,
								UnixfsAssert,
								NULL);

		/* Provide a dummy first node to start the list of unixfs reps */
		if (RepListHead == NULL)
		{
			RepListHead = malloc(sizeof(RepListNode));
			RepListHead->data = NULL;
			RepListHead->link = NULL;
		}

		return (int)0;
}

/* There are currently 2 close functions, because the pml version (which is also the 
generic version) has a (void) argument list and the unixfs version needs a repository*.
When the VM is changed, the generic version can be changed to accept a repository* which 
will not be passed along to pmlstate_close.   */
