#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "getcgi.h"
#include "html.h" 



int main()
{
    
    char **cgivars;
    char *process_filename; 

    cgivars = getcgivars();
    
    process_filename = (char *) getvalue("process_filename", cgivars);
    
    if (unlink(process_filename) == 0) {
	printf("<p>Process table '%s' deleted.\n", process_filename);
    } else {
        goto_error_page(process_filename, "failed to delete process table.");
    }
    exit(0);
}	
