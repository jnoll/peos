/* unixfs_repository.h */

#ifndef _UNIXFS_REPOSITORY_H
#define _UNIXFS_REPOSITORY_H

#define NAME 		1
#define DATE		2
#define	SIZE		3

typedef struct {
		char *w_dir;
} unixfs_info;

typedef struct {
		char *name;
		char *size;
		char *date;
} unixfs_object;

int unixfs_init();

#endif


