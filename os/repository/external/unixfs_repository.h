/* unixfs_repository.h */

#define NAME 		1
#define USER_ID 	2
#define GROUP_ID	3
#define DATE		4
#define	SIZE		5

typedef struct {
		char *w_dir;
} unixfs_info;

typedef struct {
		char *name;
		char *user_id;
		char *group_id;
		char *size;
		char *date;
} unixfs_object;

int unixfs_init();


