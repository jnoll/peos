#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "getcgi.h"
#include "kernel/events.h"
#include "kernel/action.h"
#include "kernel/process_table.h"
#include "html.h" 


char *process_filename;

void list_models()
{
    int i;
    
    char ** result = (char **) peos_list_models();

    printf("<table cellpadding=\"2\" cellspacing=\"2\" border=\"1\" style=\"text-align: left; width: 100%;\">");
    printf("<tbody>");
    printf("<tr>");
    printf("<td style=\"vertical-align: top;\">Processes<br></td>");
    printf("</tr>");
     
    for(i=0; result && result[i]; i++) {
	char *model = strcat(result[i],".pml");    
        printf("<tr>");
        printf("<td style=\"vertical-align: top;\"><a href=create_process.cgi?model=%s&process_filename=%s>%s</a><br></td>",model, process_filename, result[i]);
        printf("</tr>");
	    
    }
    printf("</tbody>");
    printf("</table>");
	 
}



int main()
{
    
    char **cgivars;
    int i;
	                                                                                            
    /** First, get the CGI variables into a list of strings         **/
    cgivars = getcgivars();
    
    process_filename = (char *) getvalue("process_filename", cgivars);

    peos_set_process_table_file(process_filename);
					
    
    print_header("Process List");
        
    list_models();
    printf("</body>\n") ;
    printf("</html>\n") ;

    /** Free anything that needs to be freed **/

    for (i=0; cgivars[i]; i++) free(cgivars[i]);
    free(cgivars) ;
	
    exit(0) ;    
}	
