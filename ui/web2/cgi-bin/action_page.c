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

static void indent(int spaces)
{
    int i;
    for (i = 0; i < spaces; i++)
	printf("  ");
}

void write_content()
{
    int i;
    char *xml_data_filename;
    _action_page *ap;
    int action_position = -1;
    int h_sp = 5;
    int prevIndex = -1, nextIndex = -1;

    xml_data_filename = (char *) malloc((strlen(process_filename) + strlen(".xml") + 1) * sizeof(char));
    strcpy(xml_data_filename, process_filename);
    strcat(xml_data_filename, ".xml");

    ap = get_action_page_details(xml_data_filename, pid, action_name);
    printf("<table style=\"width: 100%;\" border=\"0\" cellspacing=\"0\" cellpadding=\"2\">\n");
    printf("  <tbody>\n");
    printf("    <tr>\n");
    printf("      <td style=\"vertical-align: top; height: 400px; width: 25%; background-color: rgb(204, 204, 255);\">\n");
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
	    printf("<li>Iteration</li>\n");
	    indent(h_sp++);
	    printf("<ul>\n");
	} else if (!strcmp(ap->action_list[i], "!!iteration!!")) {
	    indent(--h_sp);
	    printf("</ul>\n");
	} else if (!strcmp(ap->action_list[i], "||selection||")) {
	    indent(h_sp);
	    printf("<li>Selection</li>\n");
	    indent(h_sp++);
	    printf("<ul>\n");
	} else if (!strcmp(ap->action_list[i], "!!selection!!")) {
	    indent(--h_sp);
	    printf("</ul>\n");
	} else if (strcmp(ap->action_list[i], action_name)) {
	    indent(h_sp);
	    printf("<li><a href=\"action_page.cgi?process_filename=%s&pid=%d&action_name=%s\">%s</a></li>\n", process_filename, pid, ap->action_list[i], ap->action_list[i]);
	    if (action_position == -1) {
		prevIndex = i;
	    } else if ((nextIndex == -1) && (i > action_position)) {
		nextIndex = i;
	    }
	} else {
	    indent(h_sp);
	    printf("<li>%s</li>\n", ap->action_list[i]);
	    action_position = i;
	}
    }
    printf("          </ul>\n");
    printf("        </ul>\n");
    printf("      </td>\n");
    printf("      <td style=\"vertical-align: top; text-align: center;\">\n");
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
    if (ap->reqd_resources) {
	if (!strcmp(ap->reqd_resources->qualifier, "new")) {
	    printf("              <td>%s = %s</td>\n", ap->reqd_resources->name, ap->reqd_resources->value);
	} else if ((strcmp(ap->reqd_resources->qualifier, "abstract")) && (!strcmp(ap->reqd_resources->value, "$$"))) {
	    printf("              <td>%s = %s</td>\n", ap->reqd_resources->name, ap->reqd_resources->value);
	} else {
	    printf("              <td>No resources required</td>\n");
	}
    } else {
	printf("              <td>No resources required</td>\n");
    }
    printf("            </tr>\n");
    printf("            <tr>\n");
    printf("              <td style=\"text-align: right;\"><b>Provided Resources:</b></td>\n");
    if (ap->prov_resources) {
	printf("              <td>%s = %s</td>\n", ap->prov_resources->name, ap->prov_resources->value);
    } else {
	printf("              <td>No resources provided</td>\n");
    }
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
    printf("                <input type=\"button\" value=\"Start\" onclick='alert(\"Hello\")'");
    if (!strcmp(ap->state, "RUN"))
	printf(" disabled");
    printf(">&nbsp;\n");
    printf("                <input type=\"button\" value=\"Finish\" onclick='alert(\"Hello\")'");
    if (!strcmp(ap->state, "DONE"))
	printf(" disabled");
    printf(">&nbsp;\n");
    printf("                <input type=\"button\" value=\"Suspend\" onclick='alert(\"Hello\")'");
    if (strcmp(ap->state, "RUN"))
	printf(" disabled");
    printf(">&nbsp;\n");
    printf("                <input type=\"button\" value=\"Abort\" onclick='alert(\"Hello\")'");
    if (strcmp(ap->state, "RUN"))
	printf(" disabled");
    printf(">\n");
    printf("              </td>\n");
    printf("              <td style=\"text-align: right;\">");
    if (prevIndex != -1)
	printf("<a href=\"action_page.cgi?process_filename=%s&pid=%d&action_name=%s\">&lt; Previous</a>", process_filename, pid, ap->action_list[prevIndex]);
    printf("&nbsp;&nbsp;");
    if (nextIndex != -1)
	printf("<a href=\"action_page.cgi?process_filename=%s&pid=%d&action_name=%s\">Next &gt;</a>", process_filename, pid, ap->action_list[nextIndex]);
    printf("</td>\n");
    printf("            </tr>\n");
    printf("          </tbody>\n");
    printf("        </table><br>\n");
    printf("      </td>\n");
    printf("    </tr>\n");
    printf("  </tbody>");
    printf("</table>\n");
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

    peos_set_process_table_file(process_filename);
    peos_set_loginname(process_filename);
    
    print_header("Action page");
    print_banner();
 
    write_content();

    /** Free anything that needs to be freed **/
    for (i=0; cgivars[i]; i++) free(cgivars[i]);
    free(cgivars) ;

    print_footer();

    exit(0); 
}
