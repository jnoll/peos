#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <kernel/events.h>
#include <kernel/action.h>
#include <kernel/process_table.h>
#include "getcgi.h"



int main()
{

    char **cgivars;
    int i;
    int pid;
    char *action_name;
    char *state;
    char *script;
    char *process_filename;
    int num_req_resources;
    int num_prov_resources;
    peos_resource_t *req_resources;
    peos_resource_t *prov_resources;
    

    /** First, get the CGI variables into a list of strings  **/
    cgivars = getcgivars();
 
    pid = atoi((char *) getvalue("pid", cgivars));
    action_name = (char *) getvalue("act_name", cgivars);
    process_filename = (char *) getvalue("process_filename", cgivars);
   
    peos_set_process_table_file(process_filename);

    state = peos_get_act_state(pid, action_name);
    script = peos_get_script(pid, action_name);
    
     
    print_header("Action Details");


    printf("<form name=\"actionform\" action=\"action_event.cgi\">");

    printf("<table cellpadding=\"2\" cellspacing=\"2\" border=\"1\" style=\"text-align: left; width: 100%;\">");
    printf("<tbody>");
    printf("<tr>");
    printf("<td style=\"vertical-align: top;\">Name<br></td>");
    printf("<td style=\"vertical-align: top;\">%s<br></td>", action_name);
    printf("</tr>");

    printf("<tr>");
    printf("<td style=\"vertical-align: top;\">State<br></td>");
    printf("<td style=\"vertical-align: top;\">%s<br></td>", state);
    printf("</tr>");

    printf("<tr>");
    printf("<td style=\"vertical-align: top;\">Script<br></td>");
    printf("<td style=\"vertical-align: top;\">%s<br></td>",script);
    printf("</tr>");

    printf("</tbody>");
    printf("</table>");

    printf("<table cellpadding=\"2\" cellspacing=\"2\" border=\"1\" style=\"text-align: left; width: 100%;\">");
    printf("<tbody>");
    printf("<tr>");
    printf("<td style=\"vertical-align: top;\">");
    printf("<input type=\"Submit\" name=\"action_event\" value=\"Run\">");
    printf("<input type=\"Submit\" name=\"action_event\" value=\"Finish\">");
    printf("<input type=\"Submit\" name=\"action_event\" value=\"Suspend\">");
    printf("<input type=\"Submit\" name=\"action_event\" value=\"Abort\">");
    printf("<br></td>");
    printf("</tr>");
    printf("</tbody>");
    printf("</table>");


    printf("<input type=\"hidden\" name=\"pid\" value=\"%d\">", pid);
    printf("<input type=\"hidden\" name=\"act_name\" value=\"%s\">", action_name);
    printf("<input type=\"hidden\" name=\"process_filename\" value=\"%s\">", process_filename);
    
    printf("</form");   
    printf("</body>\n");
    printf("</html>\n");

    /** Free anything that needs to be freed **/
    for (i=0; cgivars[i]; i++) free(cgivars[i]) ;
    free(cgivars) ;


}
