#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "kernel/events.h"
#include "kernel/action.h"
#include "kernel/process_table.h"


int main()
{

    /* create a test process table */	
    int pid;
    int i;
    char *model = "test_process.pml";
    peos_resource_t *resources;
    int num_resources;
    char *process_filename = "test.dat"; 


    peos_set_process_table_file(process_filename);

    resources = (peos_resource_t *) peos_get_resource_list(model,&num_resources);
    pid = peos_run(model, resources, num_resources);
	
    exit(0);

}	
