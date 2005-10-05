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

void add_jscript()
{
    printf("<script language=\"javascript\">\n");
    printf("function checkUncheck() {\n");
    printf("  for (var i = 1; i < document.forms[0].list.length; i++) {\n");
    printf("    document.forms[0].list[i].checked = document.forms[0].list[0].checked\n");
    printf("  }\n");
    printf("}\n\n");
    printf("function confirmDel() {\n");
    printf("  var numChecked = 0\n");
    printf("  var delList = \"\"\n");
    printf("  for (var i = 1; i < document.forms[0].list.length; i++) {\n");
    printf("    if (document.forms[0].list[i].checked) {\n");
    printf("      delList += document.forms[0].list[i].value + \",\"\n");
    printf("      numChecked++\n");
    printf("    }\n");
    printf("  }\n");
    printf("  if (numChecked == 0) {\n");
    printf("    alert(\"Nothing selected\")\n");
    printf("    return\n");
    printf("  }\n");
    printf("  if (confirm(\"Do you want to delete the checked items?\")) {\n");
    printf("    window.location=\"active_processes.cgi?action=delete&process_filename=%s&list=\" + delList\n", process_filename);
    printf("  } else {\n");
    printf("    for (var i = 0; i < document.forms[0].list.length; i++) {\n");
    printf("      document.forms[0].list[i].checked = false\n");
    printf("    }\n");
    printf("  }\n");
    printf("}\n");
    printf("</script>\n");
}

void list_processes()
{
    char *xml_data_filename;
    _process_list *plist;
    int tcount = 0;

    /* Create the XML data file by calling this method */
    //peos_list_instances();
    load_process_table();
    save_process_table();

    xml_data_filename = (char *) malloc((strlen(process_filename) + strlen(".xml") + 1) * sizeof(char));
    strcpy(xml_data_filename, process_filename);
    strcat(xml_data_filename, ".xml");

    plist = get_processes_list(xml_data_filename);
    printf("<h2 style=\"text-align: center;\">Active Processes</h2>\n");
    printf("<div style=\"text-align: center;\">\n");
    printf("  [<a href=\"create_process.cgi?process_filename=%s\">Create Process</a>]\n", process_filename);
    printf("</div><br><br>\n");

    if (plist == NULL)
	return;

    add_jscript();
    printf("<form>\n");
    printf("<table border=\"1\" cellspacing=\"0\" cellpadding=\"2\" align=\"center\">\n");
    printf("  <tbody>\n");
    printf("    <tr>\n");
    printf("      <td style=\"vertical-align: middle;\">\n");
    printf("        <input type=\"checkbox\" name=\"list\" value=\"-1\" onclick=\"checkUncheck()\">\n");
    printf("      </td>\n");
    printf("      <td><b>PID</b></td>\n");
    printf("      <td><b>Process Name</b></td>\n");
    printf("      <td><b>Action Name</b></td>\n");
    printf("    </tr>\n");
    while(plist) {
	tcount++;
	printf("    <tr>\n");
	printf("      <td style=\"vertical-align: middle;\">\n");
	printf("        <input type=\"checkbox\" name=\"list\" value=\"%d\">\n", plist->pid);
	printf("      </td>\n");
	printf("      <td>%d</td>\n", plist->pid);
	if (plist->active_action_name) {
	    printf("      <td><a href=\"action_page.cgi?process_filename=%s&pid=%d&action_name=%s\">%s</a></td>\n", process_filename, plist->pid, plist->active_action_name, plist->name);
            printf("      <td><a href=\"action_page.cgi?process_filename=%s&pid=%d&action_name=%s\">", process_filename, plist->pid, plist->active_action_name);
            print_action_name(plist->active_action_name);
            printf("</a></td>\n");
	} else {
	    printf("      <td><a href=\"action_page.cgi?process_filename=%s&pid=%d&action_name=%s\">", process_filename, plist->pid, plist->first_action_name);
            print_action_name(plist->name);
            printf("</a></td>\n");
	    printf("      <td>&nbsp;</td>\n");
	}
	printf("    </tr>\n");
	plist = plist->next;
    }
    printf("    <tr>\n");
    printf("      <td colspan=\"4\"><center>\n");
    printf("        <input type=\"button\" value=\"Delete\" onclick=\"confirmDel()\">\n");
    printf("      </td>\n");
    printf("    </tr>\n");
    printf("  </tbody>\n");
    printf("</table>\n");
    printf("</form><br>\n");
}

int main()
{
    
    char **cgivars, *login_name, *action, *model, *del_list, *token;
    int i, pid, num_resources;
    peos_resource_t *resources;
    char* res_file;

    /** First, get the CGI variables into a list of strings **/ 
    cgivars = getcgivars();

    action = (char *)getvalue("action", cgivars);

    if (!strcmp(action, "continue")) {
	login_name = (char *)getvalue("process_filename", cgivars);
	process_filename = (char *)malloc(strlen(login_name) * sizeof(char));
	strcpy(process_filename, login_name);
	peos_set_process_table_file(process_filename);
	peos_set_loginname(process_filename);
    } else if (!strcmp(action, "start")) {
	char *enc_loginname;
	login_name = (char *)getenv("REMOTE_USER");
	enc_loginname = (char *)crypt(login_name, "df");
	for (i = 0; i < strlen(enc_loginname); i++) {
	    if (enc_loginname[i] == '/') enc_loginname[i] = '_';
	    else if (enc_loginname[i] == '.') enc_loginname[i] = '-';
	}
	process_filename = (char *)malloc((strlen(enc_loginname)
				+ strlen(".dat") + 1) * sizeof(char));
	strcpy(process_filename, enc_loginname);
	strcat(process_filename, ".dat");
	peos_set_process_table_file(process_filename);
	peos_set_loginname(process_filename);
    } else if (!strcmp(action, "create")) {
	process_filename = (char *)getvalue("process_filename", cgivars);
	model = (char *)getvalue("model", cgivars);
	peos_set_process_table_file(process_filename);
	peos_set_loginname(process_filename);
	resources = (peos_resource_t *) peos_get_resource_list(model,&num_resources);
	pid = peos_run(model,resources,num_resources);
        if ((res_file = peos_get_resource_file(model)))
            peos_bind_resource_file(pid, res_file);
	free(resources);
    } else if (!strcmp(action, "delete")) {
	process_filename = (char *)getvalue("process_filename", cgivars);
	del_list = (char *)getvalue("list", cgivars);
	peos_set_process_table_file(process_filename);
	peos_set_loginname(process_filename);
	token = strtok(del_list, ",");
	while (token) {
	    peos_delete_process_instance(atoi(token));
	    token = strtok(NULL, ",");
	}
    }
 
    print_header("Process List");
    print_banner("PEOS Process Enactment Demonstration");
 
    list_processes();

    /** Free anything that needs to be freed **/
    for (i=0; cgivars[i]; i++) free(cgivars[i]);
    free(cgivars) ;

    print_footer();

    exit(0) ; 
}
