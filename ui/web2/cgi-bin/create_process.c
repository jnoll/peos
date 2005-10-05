#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <crypt.h>
#include <unistd.h>
#include "getcgi.h"
#include "kernel/events.h"
#include "kernel/action.h"
#include "kernel/process_table.h"
#include "html.h"
#include "xmlparse.h"


char *process_filename = NULL;

void list_models()
{
    int i, j, len;
    char* display_process_name;
    char **result = (char **)peos_list_models();

    printf("<h2 style=\"text-align: center;\">Create Process</h2>\n");
    printf("<div style=\"text-align: center;\">\n");
    printf("  [<a href=\"active_processes.cgi?action=continue&process_filename=%s\">Active Process</a>]\n", process_filename);
    printf("</div><br><br>\n");

    printf("<table border=\"1\" cellspacing=\"0\" cellpadding=\"2\" align=\"center\">\n");
    printf("  <tbody>\n");
    printf("    <tr>\n");
    printf("      <td><br>\n");
    printf("      <ul>\n");
    for (i = 0; result && result[i]; i++)
    {
        display_process_name = strdup(result[i]);
        len = strlen(display_process_name);
        for (j = 0; j < len; j++) {
            if (display_process_name[j] == '_')
                display_process_name[j] = ' ';
            else
                display_process_name[j] = toupper(display_process_name[j]);
        }
            
	printf("        <li><a href=\"active_processes.cgi?action=create&model=%s.pml&process_filename=%s\">%s</a></li>\n", result[i], process_filename, display_process_name);
        free(display_process_name);
    }
    printf("      </ul>\n");
    printf("      </td><br>\n");
    printf("    </tr>\n");
    printf("  </tbody>");
    printf("</table><br>\n");
}

int main()
{
    
    char **cgivars, *filename;
    int i;

    /** First, get the CGI variables into a list of strings **/ 
    cgivars = getcgivars();

    filename = (char *)getvalue("process_filename", cgivars);
    process_filename = (char *)malloc(strlen(filename) * sizeof(char));
    strcpy(process_filename, filename);

    peos_set_process_table_file(process_filename);
    peos_set_loginname(process_filename);
    
    print_header("Process Model List");
    print_banner("PEOS Process Enactment Demonstration");
 
    list_models();

    /** Free anything that needs to be freed **/
    for (i=0; cgivars[i]; i++) free(cgivars[i]);
    free(cgivars) ;

    print_footer();

    exit(0); 
}
