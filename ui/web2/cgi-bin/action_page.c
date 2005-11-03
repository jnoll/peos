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
_action_page *ap;
char *xml_data_filename;
char* display_process_name;

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

void add_action_list()
{
    int i;
    int h_sp = 5;
    
    printf("        <div style=\"text-align: center;\"><small>\n");
    printf("          [<a href=\"create_process.cgi\">Create Process</a>]&nbsp;\n");
    printf("          [<a href=\"active_processes.cgi?action=continue\">Active Process</a>]&nbsp;\n");
    printf("        </small></div><br>\n");
    printf("        <ul>\n");
    printf("          <li>%s (PID: %d)</li>\n", display_process_name, ap->pid);
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
            printf("<li>");
            printf("<a href=\"action_page.cgi?pid=%d&action_name=%s\">", pid, ap->action_list[i]);
            print_action_name(ap->action_list[i]);
            printf("</a>");
            if (strcmp(ap->state_list[i], "NONE") != 0)
                printf(" <small>(%s)</small>", ap->state_list[i]);
            printf("</li>\n");
	} else {
            indent(h_sp);
            printf("<li>");
            print_action_name(action_name);
            if (strcmp(ap->state, "NONE") != 0)
                printf(" <small>(%s)</small>", ap->state);
            printf("</li>\n");
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
        printf("<a href=\"action_page.cgi?pid=%d&action_name=%s\">&lt;&lt;Prev</a>", pid, ap->action_list[prevIndex]);
    printf("&nbsp;&nbsp;");
    if (nextIndex == -1)
        printf("Next&gt;&gt;\n");
    else
        printf("<a href=\"action_page.cgi?pid=%d&action_name=%s\">Next&gt;&gt;</a>", pid, ap->action_list[nextIndex]);
    printf("&nbsp;&nbsp;");
    printf("<a href=\"http://www.cse.scu.edu/~jnoll/286/projects/index.html\">Home</a>\n");
    printf("      </td>\n");
    printf("    </tr>\n");
    printf("  </tbody>\n");
    printf("</table>\n");
}

void add_action_control_buttons() {
    
    printf("        <table style=\"width: 100%;\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\">\n");
    printf("          <tbody>\n");
    printf("            <tr>\n");
    printf("              <td>\n");
    printf("                <input type=\"button\" value=\"Start\" onclick='window.location=\"action_event.cgi?action_event=Run&pid=%d&act_name=%s\"'\n", pid, action_name);
    if (!strcmp(ap->state, "RUN"))
        printf(" disabled");
    printf(">&nbsp;\n");
    printf("                <input type=\"button\" value=\"Finish\" onclick='window.location=\"action_event.cgi?action_event=Finish&pid=%d&act_name=%s&\"'\n", pid, action_name);
    if (!strcmp(ap->state, "DONE"))
        printf(" disabled");
    printf(">&nbsp;\n");
    printf("                <input type=\"button\" value=\"Suspend\" onclick='window.location=\"action_event.cgi?action_event=Suspend&pid=%d&act_name=%s\"'\n", pid, action_name);
    if (strcmp(ap->state, "RUN"))
        printf(" disabled");
    printf(">&nbsp;\n");
    printf("                <input type=\"button\" value=\"Abort\" onclick='window.location=\"action_event.cgi?action_event=Abort&pid=%d&act_name=%s\"'\n", pid, action_name);
    if (strcmp(ap->state, "RUN"))
        printf(" disabled");
    printf(">\n");
    printf("              </td>\n");
    printf("            </tr>\n");
    printf("          </tbody>\n");
    printf("        </table><br>\n");
}

void write_content()
{
    int i, j;
    int action_position = -1;
    int prevIndex = -1, nextIndex = -1;
    int cb_reqd = 0, cb_prov = 0;
    int num_unbound_resources;
    int num_resources;
    char unbound_value[256];
    
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
                printf("Location: action_page.cgi?pid=%d&action_name=%s\r\n\r\n", pid, action_name);
            }
            else {
                _action_page *new_ap;
                peos_notify(pid, action_name, PEOS_EVENT_FINISH);
                new_ap = get_action_page_details(xml_data_filename, pid, action_name);
                if (!new_ap)
                    printf("Location: active_processes.cgi?action=continue\r\n\r\n");
                else {
                    if (nextIndex != -1)
                        printf("Location: action_page.cgi?pid=%d&action_name=%s\r\n\r\n", pid, ap->action_list[nextIndex]);
                    else
                        printf("Location: action_page.cgi?pid=%d&action_name=%s\r\n\r\n", pid, ap->action_list[ap->total_actions - 1]);
                }
            }
        }
        else {
            print_header("Action page");
            print_banner(display_process_name);
            add_prev_next(ap, prevIndex, nextIndex);
            printf("<table style=\"width: 100%;\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\">\n");
            printf("  <tbody>\n");
            printf("    <tr>\n");
            printf("      <td style=\"vertical-align: top; height: 400px; width: 25%; background-color: rgb(204, 204, 255);\">\n");

            add_action_list();

            printf("      </td>\n");
            printf("      <td style=\"vertical-align: top; text-align: left;\">\n");
            printf("        <h2>%");
            print_action_name(action_name);
            if (strcmp(ap->state, "NONE") != 0)
                printf(" (%s)", ap->state);
            printf("</h2><br>\n");
            
            
            add_jscript();
            printf("        <form name=\"pickform\" action=\"bind_resources.cgi?pid=%d&process_filename=%s&act_name=%s&resource_type=%s\">\n", pid, process_filename, action_name, resource_type);
            printf("        <table style=\"width: 100%;\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\">\n");
            printf("          <tbody>\n");
            printf("            <tr>\n");
            printf("              <td style=\"vertical-align: top; height: 60px; \" colspan=\"2\" align=\"left\">\n");
            printf("                <h4>Enter a value for the following resources:</h4>\n");
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
            printf("              <td style=\"vertical-align: top;\" colspan=\"2\" align=\"left\">\n");
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
        print_banner(display_process_name);
        add_prev_next(ap, prevIndex, nextIndex);
        printf("<table style=\"width: 100%;\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\">\n");
        printf("  <tbody>\n");
        printf("    <tr>\n");
        printf("      <td style=\"vertical-align: top; height: 400px; width: 25%; background-color: rgb(204, 204, 255);\">\n");
        add_action_list();
        printf("      </td>\n");
        printf("      <td style=\"vertical-align: top; text-align: left;\">\n");
        printf("        <h2>");
        print_action_name(action_name);
        if (strcmp(ap->state, "NONE") != 0)
            printf(" (%s)", ap->state);
        printf("</h2><br>\n");
        printf("        <table style=\"width: 100%;\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\">\n");
        printf("          <tbody>\n");
        printf("            <tr>\n");
        printf("              <td style=\"width: 150px;\"><b>Required Resources:</b></td>\n");
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
            printf("&nbsp;&nbsp;&nbsp;<a href=\"action_page.cgi?resource_type=requires&action=change&pid=%d&action_name=%s\">Change  bindings</a>", pid, action_name);
        }
        printf("</td>\n");
        printf("            </tr>\n");
        printf("            <tr>\n");
        printf("              <td style=\"width: 150px;\"><b>Provided Resources:</b></td>\n");
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
            printf("&nbsp;&nbsp;&nbsp;<a href=\"action_page.cgi?resource_type=provides&action=change&pid=%d&action_name=%s\">Change bindings</a>", pid, action_name);
        }
        printf("</td>\n");
        printf("            </tr>\n");
        printf("          </tbody>\n");
        printf("        </table><br><br>\n");
        
        add_action_control_buttons();
        
        if (ap->script && strcmp(ap->script, "\n(null)\n") != 0) {
            int len, found_res;
            char res_name[256];
            len = strlen(ap->script);
            
            ap->script[len - 2] = '\0';
            ap->script += 2;
            len = strlen(ap->script);
            for (i = 0, j = 0; i < len; i++, j++) {
                if (ap->script[i] == '$') {
                    strcpy(res_name, "");
                    for (i++, j = 0; ap->script[i] != ' ' && ap->script[i] != '\n' && ap->script[i] != '\0' && ap->script[i] != '$'; i++, j++) {
                        res_name[j] = ap->script[i];
                    }
                    res_name[j] = '\0';
                    found_res = 0;
                    for (j = 0; j < ap->total_reqd_resources; j++) {
                        if (strcmp(res_name, ap->reqd_resources[j]->name) == 0) {
                            sprintf(unbound_value, "${%s}", ap->reqd_resources[j]->name);
                            if (strcmp(ap->reqd_resources[j]->value, unbound_value) != 0) {
                                printf("<a href=\"display_file.cgi?%s\">%s</a>", ap->reqd_resources[j]->value, ap->reqd_resources[j]->value);
                            }
                            else
                                printf("%s", ap->reqd_resources[j]->value);
                            found_res = 1;
                            break;
                        }
                    }
                    if (!found_res) {
                        for (j = 0; j < ap->total_prov_resources; j++) {
                            if (strcmp(res_name, ap->prov_resources[j]->name) == 0) {
                                sprintf(unbound_value, "${%s}", ap->prov_resources[j]->name);
                                if (strcmp(ap->prov_resources[j]->value, unbound_value) != 0) {
                                    printf("<a href=\"display_file.cgi?%s\">%s</a>", ap->prov_resources[j]->value, ap->prov_resources[j]->value);
                                }
                                else
                                    printf("%s", ap->prov_resources[j]->value);
                                found_res = 1;
                                break;
                            }
                        }
                    }
                    if (!found_res) {
                        printf("$%s", res_name);
                    }
                }
                if (ap->script[i] != '\0')
                    printf("%c", ap->script[i]);
                    
            }
            ap->script -= 2;
        }
        printf("\n");
    }
    printf("      </td>\n");
    printf("    </tr>\n");
    printf("  </tbody>\n");
    printf("</table>\n");
    add_prev_next(ap, prevIndex, nextIndex);
}

int main()
{
    
    char **cgivars;//, *filename;
    char *tpid;
    int i;
    int len;

    /** First, get the CGI variables into a list of strings **/ 
    cgivars = getcgivars();

    process_filename = get_process_filename();
    action_name = (char *)getvalue("action_name", cgivars);
    
    tpid = (char *)getvalue("pid", cgivars);
    pid = atoi(tpid);
    resource_type = (char *)getvalue("resource_type", cgivars);
    action = (char *)getvalue("action", cgivars);

    peos_set_process_table_file(process_filename);
    peos_set_loginname(process_filename);
    
    proc_resources = peos_get_resource_list_context(pid, &num_proc_resources);
    
    xml_data_filename = (char *) malloc((strlen(process_filename) + strlen(".xml") + 1) * sizeof(char));
    strcpy(xml_data_filename, process_filename);
    strcat(xml_data_filename, ".xml");

    ap = get_action_page_details(xml_data_filename, pid, action_name);
    
    display_process_name = strdup(strtok(ap->model, "."));
    len = strlen(display_process_name);
    for (i = 0; i < len; i++) {
        if (display_process_name[i] == '_')
            display_process_name[i] = ' ';
        else
            display_process_name[i] = toupper(display_process_name[i]);
    }
    
    write_content();
    print_footer();
    
    if (resources)
        free(resources);
    if (proc_resources)
        free(proc_resources);
    if (display_process_name)
        free(display_process_name);
    if (xml_data_filename)
        free(xml_data_filename);

    /** Free anything that needs to be freed **/
    for (i=0; cgivars[i]; i++) free(cgivars[i]);
    free(cgivars) ;
    exit(0); 
}
