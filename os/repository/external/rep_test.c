/* repository test- rep_test.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "repository.h"
#include "unixfs_repository.h"

int
main(argc, argv)
		int argc;
		char **argv;
{
		repository *rep;
		repository_queryresult *result;
		repository_object *r_obj;
		char *tst;
		int i;

		debug_flag = 0;

		repository_init();
		rep = repository_open("test.rep");
		if ((i = repository_query(&result, rep, "name",NE,"test.rep")) != -1) {
				printf("entries = %d\n",i);
				while ((r_obj = repository_querynext(result)) != NULL) {
					repository_read(r_obj,rep,"name",&tst);
					printf("object name = %s\n",tst);
				}
		}
		repository_closequery(result);
		repository_close(rep);

		return (int)0;
}




