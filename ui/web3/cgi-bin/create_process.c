#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "getcgi.h"
#include "kernel/events.h"
#include "kernel/action.h"
#include "kernel/process_table.h"
#include "html.h" 


char *process_filename;


int main()
{
    
    char **cgivars;
    int i;
    int pid;
    char *model;
    peos_resource_t *resources;
    int num_resources;
    

    /** First, get the CGI variables into a list of strings         **/
    cgivars = getcgivars();
 
    model = (char *) getvalue("model", cgivars);
    
    process_filename = (char *) getvalue("process_filename", cgivars);
    
    peos_set_process_table_file(process_filename);

    resources = (peos_resource_t *) peos_get_resource_list(model,&num_resources);
    pid = peos_run(model, resources, num_resources);

    if((resources == NULL) || (pid < 0)) {
        printf("Location: error_page.html\n\n");
        exit(0);  
    }
    
    for (i=0; cgivars[i]; i++) free(cgivars[i]);
    free(cgivars) ;
    
    printf("Location: action_listing.cgi?process_filename=%s&start=false\n\n",process_filename);
    exit(0);


}	
