#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "repository.h"

int main(argc, argv)
{
	int read, write, init, open, delete, pack, pml_open;
	pml_obj_t open_obj;	
	char s[30], t[30];

	printf("Summer 2000 Repository Interface.\n");

	printf("Attempting repository_init()...\n");
	init = repository_init();
	if( init < 0 )
	{
		printf("init failed.\n");
	}	
	else
	{
		printf("init succeeded!\n");
	}

	pml_open = pml_open_repository("reptest.rep", TRUNCATE);
        if( pml_open < 0 )
        {
                printf("Open pml state repository failed.\n");
                exit(-1);
        }
        else
        {
                printf("Open pml state repository succeeded!\n");
        }

	printf("Attempt to create an object...\n");
	open_obj = pml_create_object();
	printf("pml object created.\n");

	printf("Attempt to write to the object...\n");
	s[0] = 's';
	t[0] = 't';

	write = pml_write_attribute( open_obj, s, 30, t, 30 ); 
	if( write < 0 )
	{
		printf("Object write failed.\n");
		exit(-1);
	}
	else
	{
		printf("Object write succeeded.\n");
	}	

	printf("Attempt to read it back...\n");
	read = pml_read_attribute( open_obj, s, 30, t, 30 );
	if( read < 0 )
        {
                printf("Object read failed.\n");
                exit(-1);
        }
        else
        {
                printf("Object read succeeded.\n");
        }


	printf("Attempt to mark the object for deletion...\n");
	delete = pml_delete_object(open_obj);
	if( delete < 0 )
	{
		printf("delete mark failed.\n");
	}
	else
	{
		printf("delete mark succeeded.\n");
	}	

	printf("Attempt to delete the marked objects...\n");
	pack = pml_pack_objects();
	if( pack < 0 )
	{
		printf("delete marked objects failed.\n");
	}
	else
	{
		printf("delete marked objects succeeded.\n");
	}
	
}
