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
int pid;
char *action_name;
char *resource_type = NULL;
char *action = NULL;
peos_resource_t *resources;
peos_resource_t* proc_resources;
int num_proc_resources;

static void indent(int spaces)
{
    int i;
    for (i = 0; i < spaces; i++)
	printf("  ");
}

void add_jscript()
{
    printf("<script language=\"javascript\">\n");
    printf("function validate() {\n");
    printf("var i;\n");
    printf("for (i = 0; i < document.pickform.elements.length; i++) {\n");
    printf("if (document.pickform.elements[i].type == \"text\") {\n");
    printf("if (document.pickform.elements[i].value == \" \") {\n");
    printf("alert(\"Please enter all resource bindings\");\n");
    printf("return false;\n");
    printf("}\n");
    printf("}\n");
    printf("}\n");
    printf("return true;\n");
    printf("}\n");
    printf("</script>\n");
}

void add_action_list(_action_page *ap)
{
    int i;
    int h_sp = 5;

    
    printf("        <div style=\"text-align: center;\"><small>\n");
    printf("          [<a href=\"create_process.cgi?process_filename=%s\">Create Process</a>]&nbsp;\n", process_filename);
    printf("          [<a href=\"active_processes.cgi?action=continue&process_filename=%s\">Active Process</a>]&nbsp;\n", process_filename);
    printf("        </small></div><br>\n");
    printf("        <ul>\n");
    printf("          <li>%s (Pid: %d)</li>\n", ap->model, ap->pid);
    printf("          <ul>\n");
    for (i = 0; i < ap->total_actions; i++) {
	if (!strcmp(ap->action_list[i], "||iteration||")) {
	    indent(h_sp);
	    printf("<li>Repeat:</li>\n");
	    indent(h_sp++);
	    printf("<ul>\n");
	} else if (!strcmp(ap->action_list[i], "!!iteration!!")) {
	    indent(--h_sp);
	    printf("</ul>\n");
	} else if (!strcmp(ap->action_list[i], "||selection||")) {
	    indent(h_sp);
	    printf("<li>Select:</li>\n");
	    indent(h_sp++);
	    printf("<ul>\n");
	} else if (!strcmp(ap->action_list[i], "!!selection!!")) {
	    indent(--h_sp);
	    printf("</ul>\n");
	} else if (!strcmp(ap->action_list[i], "||sequences||")) {
	    indent(h_sp);
	    printf("<li>Choice:</li>\n");
	    indent(h_sp++);
	    printf("<ul>\n");
	} else if (!strcmp(ap->action_list[i], "!!sequences!!")) {
	    indent(--h_sp);
	    printf("</ul>\n");
	} else if (!strcmp(ap->action_list[i], "||sequenceb||")) {
	    indent(h_sp);
	    printf("<li>Path:</li>\n");
	    indent(h_sp++);
	    printf("<ul>\n");
	} else if (!strcmp(ap->action_list[i], "!!sequenceb!!")) {
	    indent(--h_sp);
	    printf("</ul>\n");
	} else if (!strcmp(ap->action_list[i], "||branch||")) {
	    indent(h_sp);
	    printf("<li>Do Concurrently:</li>\n");
	    indent(h_sp++);
	    printf("<ul>\n");
	} else if (!strcmp(ap->action_list[i], "!!branch!!")) {
	    indent(--h_sp);
	    printf("</ul>\n");
	} else if (strcmp(ap->action_list[i], action_name)) {
	    indent(h_sp);
	    printf("<li><a href=\"action_page.cgi?process_filename=%s&pid=%d&action_name=%s\">%s</a></li>\n", process_filename, pid, ap->action_list[i], ap->action_list[i]);
	} else {
	    indent(h_sp);
	    printf("<li>%s</li>\n", ap->action_list[i]);
	}
    }
    printf("          </ul>\n");
    printf("        </ul>\n");
}

void add_prev_next(_action_page *ap, int prevIndex, int nextIndex) {
    printf("<table style=\"text-align: left; background-color: rgb(255, 204, 51); width: 100%; cellspacing=\"0\" cellpadding=\"1\">\n");
        printf("  <tbody>\n");
        printf("    <tr>\n");
        printf("      <td style=\"width:100px;\">\n");
    if (prevIndex == -1)
        printf("&lt;&lt;Prev");
    else
        printf("<a href=\"action_page.cgi?process_filename=%s&pid=%d&action_name=%s\">&lt;&lt;Prev</a>", process_filename, pid, ap->action_list[prevIndex]);
    printf("&nbsp;&nbsp;");
    if (nextIndex == -1)
        printf("Next&gt;&gt;\n");
    else
        printf("<a href=\"action_page.cgi?process_filename=%s&pid=%d&action_name=%s\">Next&gt;&gt;</a>", process_filename, pid, ap->action_list[nextIndex]);
    printf("&nbsp;&nbsp;");
    printf("<a href=\"http://www.cse.scu.edu/~jnoll/286/projects/index.html\">Home</a>\n");
    printf("      </td>\n");
    printf("    </tr>\n");
    printf("  </tbody>\n");
    printf("</table>\n");
}

void write_content()
{
    int i, j;
    char *xml_data_filename;
    _action_page *ap;
    int action_position = -1;
    int prevIndex = -1, nextIndex = -1;
    int cb_reqd = 0, cb_prov = 0;
    int num_unbound_resources;
    int num_resources;
    char unbound_value[256];
    
    xml_data_filename = (char *) malloc((strlen(process_filename) + strlen(".xml") + 1) * sizeof(char));
    strcpy(xml_data_filename, process_filename);
    strcat(xml_data_filename, ".xml");

    ap = get_action_page_details(xml_data_filename, pid, action_name);
    
    for (i = 0; i < ap->total_actions; i++) {
        if (strcmp(ap->action_list[i], action_name)) {
            if (strcmp(ap->action_list[i], "||iteration||") && strcmp(ap->action_list[i], "!!iteration!!") &&
                strcmp(ap->action_list[i], "||selection||") && strcmp(ap->action_list[i], "!!selection!!") &&
                strcmp(ap->action_list[i], "||branch||")    && strcmp(ap->action_list[i], "!!branch!!") &&
                strcmp(ap->action_list[i], "||sequences||") && strcmp(ap->action_list[i], "!!sequences!!") &&
                strcmp(ap->action_list[i], "||sequenceb||") && strcmp(ap->action_list[i], "!!sequenceb!!")) {
                if (action_position == -1) {
                    prevIndex = i;
                } else if ((nextIndex == -1) && (i > action_position)) {
                    nextIndex = i;
                }
            }
        }
        else {
            action_position = i;
        }
    }
    if ((resource_type) && ((strcmp(resource_type,"requires") != 0) || (strcmp(resource_type,"provides") != 0))) {
        if (strcmp(resource_type,"requires") == 0) {
            resources = peos_get_resource_list_action_requires(pid, action_name, &num_resources);
        }
        else {
            resources = peos_get_resource_list_action_provides(pid, action_name, &num_resources);
        }
        num_unbound_resources = 0;
        for (i=0; i < num_resources; i++) {
            if((strcmp(resources[i].qualifier,"any") == 0) || (strcmp(resources[i].qualifier,"new") == 0)) {
                num_unbound_resources++;
            }
            else {
                sprintf(unbound_value, "${%s}", resources[i].name);
                if ((strcmp(resources[i].value,unbound_value) == 0) && (strcmp(resources[i].qualifier, "abstract") != 0)) {
                    num_unbound_resources++;
                }
            }
        }
        if (((num_unbound_resources == 0) || (num_resources == 0)) && (action == NULL)) {
            if(strcmp(resource_type,"requires") == 0) {
                peos_notify(pid, action_name, PEOS_EVENT_START);
                printf("Location: action_page.cgi?process_filename=%s&pid=%d&action_name=%s\r\n\r\n", process_filename, pid, action_name);
                exit(0);
            }
            else {
                peos_notify(pid, action_name, PEOS_EVENT_FINISH);
                printf("Location: active_processes.cgi?action=continue&process_filename=%s\r\n\r\n", process_filename);
            }
            //add_action_list(ap);
        }
        else {
            print_header("Action page");
            print_banner();
            add_prev_next(ap, prevIndex, nextIndex);
            printf("<table style=\"width: 100%;\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\">\n");
            printf("  <tbody>\n");
            printf("    <tr>\n");
            printf("      <td style=\"vertical-align: top; height: 400px; width: 25%; background-color: rgb(204, 204, 255);\">\n");

            add_action_list(ap);

            printf("      </td>\n");
            printf("      <td style=\"vertical-align: top; text-align: left;\">\n");
            printf("        <h2>%s</h2><br>\n", action_name);
            
            
            
            add_jscript();
            printf("        <form name=\"pickform\" action=\"bind_resources.cgi?pid=%d&process_filename=%s&act_name=%s&resource_type=%s\">\n", pid, process_filename, action_name, resource_type);
            printf("        <table style=\"width: 100%;\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\">\n");
            printf("          <tbody>\n");
            printf("            <tr>\n");
            printf("              <td style=\"vertical-align: top; height: 60px; \" colspan=\"2\" align=\"center\">\n");
            printf("                <h4>Supply Resource Bindings</h4>\n");
            printf("              </td>\n");
            printf("            </tr>\n");
            for (i = 0; i < num_resources; i++) {
                if (strcmp(resources[i].qualifier, "abstract") != 0) {
                    printf("            <tr>\n");
                    printf("              <td style=\"vertical-align: top; text-align: right;\">%s<br></td>\n", resources[i].name);
                    sprintf(unbound_value, "${%s}", resources[i].name);
                    for (j = 0; j < num_proc_resources; j++) {
                        if (strcmp(proc_resources[j].name, resources[i].name) == 0)
                            break;
                    }
                    //display actual value not evaluated value
                    printf("              <td style=\"vertical-align: top; text-align: left;\"><input type=\"text\" size=\"100\" name=\"resource%d\" value=\"%s\" maxlength=\"256\"><td><br><br>\n", i, proc_resources[j].value);
                    printf("            </tr>\n");
                }
            }
            printf("            <tr>\n");
            printf("              <td style=\"vertical-align: top;\" colspan=\"2\" align=\"center\">\n");
            printf("                <input type=\"Submit\" name=\"Submit\" value=\"Submit\" onclick=\"return validate()\">&nbsp;&nbsp;\n");
            printf("                <input type=\"Reset\" name=\"Reset\" value=\"Reset\">\n");
            printf("              </td><br>\n");
            printf("            </tr>\n");
            printf("          </tbody>\n");
            printf("        </table>\n");
            printf("        <input type = \"hidden\" name =\"pid\" value=\"%d\">\n", pid);
            printf("        <input type = \"hidden\" name =\"process_filename\" value=\"%s\">\n", process_filename);
            printf("        <input type = \"hidden\" name =\"act_name\" value=\"%s\">\n", action_name);
            printf("        <input type = \"hidden\" name =\"resource_type\" value=\"%s\">\n", resource_type);
            if (action && (strcmp(action, "change") == 0)) {
                printf("        <input type = \"hidden\" name =\"action\" value=\"%s\">\n", action);
            } else {
                printf("        <input type = \"hidden\" name =\"action\" value=\"new\">\n");
            }
            printf("        </form>\n");
        }
    }
    else {
        print_header("Action page");
        print_banner();
        add_prev_next(ap, prevIndex, nextIndex);
        printf("<table style=\"width: 100%;\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\">\n");
        printf("  <tbody>\n");
        
        
        
        printf("    <tr>\n");
        printf("      <td style=\"vertical-align: top; height: 400px; width: 25%; background-color: rgb(204, 204, 255);\">\n");
        
        add_action_list(ap);
        
        
        printf("      </td>\n");
        printf("      <td style=\"vertical-align: top; text-align: left;\">\n");
        printf("        <h2>%s</h2><br>\n", action_name);
        
        
        printf("        <table style=\"width: 100%;\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\">\n");
        printf("          <tbody>\n");
        printf("            <tr>\n");
        printf("              <td style=\"width: 20%; text-align: right;\"><b>State:</b></td>\n");
        printf("              <td style=\"width: 10px;\" rowspan=\"4\">&nbsp;</td>\n");
        printf("              <td>%s</td>\n", ap->state);
        printf("            </tr>\n");
        printf("            <tr>\n");
        printf("              <td style=\"text-align: right;\"><b>Required Resources:</b></td>\n");
        printf("              <td>");
        for (i = 0; i < ap->total_reqd_resources; i++) {
            printf("%s = %s", ap->reqd_resources[i]->name, ap->reqd_resources[i]->value);
            if (i < (ap->total_reqd_resources-1)) {
                printf(", ");
            }
            sprintf(unbound_value, "${%s}", ap->reqd_resources[i]->name);
            if (strcmp(ap->reqd_resources[i]->value, unbound_value) != 0) {
                cb_reqd = 1;
            }
        }
        if (ap->total_reqd_resources == 0) {
            printf("No resources required");
        }
        else if (cb_reqd) {
            printf("&nbsp;&nbsp;&nbsp;<a href=\"action_page.cgi?resource_type=requires&action=change&process_filename=%s&pid=%d&action_name=%s\">Change  bindings</a>", process_filename, pid, action_name);
        }
        printf("</td>\n");
        printf("            </tr>\n");
        printf("            <tr>\n");
        printf("              <td style=\"text-align: right;\"><b>Provided Resources:</b></td>\n");
        printf("              <td>");
        for (i = 0; i < ap->total_prov_resources; i++) {
            printf("%s = %s", ap->prov_resources[i]->name, ap->prov_resources[i]->value);
            if (i < (ap->total_prov_resources-1)) {
                printf(", ");
            }
            sprintf(unbound_value, "${%s}", ap->prov_resources[i]->name);
            if (strcmp(ap->prov_resources[i]->value, unbound_value) != 0) {
                cb_prov = 1;
            }
        }
        if (ap->total_prov_resources == 0) {
            printf("No resources provided");
        }
        else if (cb_prov) {
            printf("&nbsp;&nbsp;&nbsp;<a href=\"action_page.cgi?resource_type=provides&action=change&process_filename=%s&pid=%d&action_name=%s\">Change bindings</a>", process_filename, pid, action_name);
        }
        printf("</td>\n");
        printf("            </tr>\n");
        printf("            <tr>\n");
        printf("              <td style=\"text-align: right; vertical-align: top;\"><b>Script:</b></td>\n");
        printf("              <td>%s</td>\n", ap->script);
        printf("            </tr>\n");
        printf("          </tbody>\n");
        printf("        </table><br><br>\n");
        printf("        <table style=\"width: 100%;\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\">\n");
        printf("          <tbody>\n");
        printf("            <tr>\n");
        printf("              <td>\n");
        printf("                <input type=\"button\" value=\"Start\" onclick='window.location=\"action_event.cgi?action_event=Run&pid=%d&act_name=%s&process_filename=%s\"'\n", pid, action_name, process_filename);
        if (!strcmp(ap->state, "RUN"))
            printf(" disabled");
        printf(">&nbsp;\n");
        printf("                <input type=\"button\" value=\"Finish\" onclick='window.location=\"action_event.cgi?action_event=Finish&pid=%d&act_name=%s&process_filename=%s\"'\n", pid, action_name, process_filename);
        if (!strcmp(ap->state, "DONE"))
            printf(" disabled");
        printf(">&nbsp;\n");
        printf("                <input type=\"button\" value=\"Suspend\" onclick='window.location=\"action_event.cgi?action_event=Suspend&pid=%d&act_name=%s&process_filename=%s\"'\n", pid, action_name, process_filename);
        if (strcmp(ap->state, "RUN"))
            printf(" disabled");
        printf(">&nbsp;\n");
        printf("                <input type=\"button\" value=\"Abort\" onclick='window.location=\"action_event.cgi?action_event=Abort&pid=%d&act_name=%s&process_filename=%s\"'\n", pid, action_name, process_filename);
        if (strcmp(ap->state, "RUN"))
            printf(" disabled");
        printf(">\n");
        printf("              </td>\n");
	/*printf("              <td style=\"text-align: right;\">");
	if (prevIndex != -1)
	    printf("<a href=\"action_page.cgi?process_filename=%s&pid=%d&action_name=%s\">&lt; Previous</a>", process_filename, pid, ap->action_list[prevIndex]);
	printf("&nbsp;&nbsp;");
	if (nextIndex != -1)
	    printf("<a href=\"action_page.cgi?process_filename=%s&pid=%d&action_name=%s\">Next &gt;</a>", process_filename, pid, ap->action_list[nextIndex]);
 printf("</td>\n");*/
        printf("            </tr>\n");
        printf("          </tbody>\n");
        printf("        </table><br>\n");
    }
    printf("      </td>\n");
    printf("    </tr>\n");
    printf("  </tbody>");
    printf("</table>\n");
    add_prev_next(ap, prevIndex, nextIndex);
}

int main()
{
    
    char **cgivars, *filename;
    char *tpid;
    int i;

    /** First, get the CGI variables into a list of strings **/ 
    cgivars = getcgivars();

    filename = (char *)getvalue("process_filename", cgivars);
    process_filename = (char *)malloc(strlen(filename) * sizeof(char));
    strcpy(process_filename, filename);
    action_name = (char *)getvalue("action_name", cgivars);
    tpid = (char *)getvalue("pid", cgivars);
    pid = atoi(tpid);
    resource_type = (char *)getvalue("resource_type", cgivars);
    action = (char *)getvalue("action", cgivars);

    peos_set_process_table_file(process_filename);
    peos_set_loginname(process_filename);
    
    proc_resources = peos_get_resource_list_context(pid, &num_proc_resources);
    
    write_content();

    
    if (resources)
        free(resources);
    if (proc_resources)
        free(proc_resources);

    /** Free anything that needs to be freed **/
    for (i=0; cgivars[i]; i++) free(cgivars[i]);
    free(cgivars) ;

    print_footer();

    exit(0); 
}
