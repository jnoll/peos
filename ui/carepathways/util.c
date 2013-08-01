#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <process_table.h>

void set_login_name(char *loginname)
{
    char *process_filename;

    process_filename = (char *) malloc((strlen(loginname) + strlen(".dat") +1) * sizeof(char));
    strcpy(process_filename, loginname);
    strcat(process_filename, ".dat"); 

    peos_set_process_table_file(process_filename);
    peos_set_loginname(loginname);
}
