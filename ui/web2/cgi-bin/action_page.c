#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <kernel/events.h>
#include <kernel/action.h>
#include <kernel/process_table.h>
#include "proc_graph.h"

extern Graph make_graph(char *file);
extern void makedot(Node n, char *filename, char *action, char *state);
void print_resource(int pid, char *action);

int main()
{
    char *input, *temp, *action, *state;
    char *gifname, **instances, *fname;
    int pid;
    Graph graph_name;


    input = getenv("QUERY_STRING");

    temp = strtok(input,"+");
    pid = atoi(temp);
    temp = strtok(NULL, "=");
    action = temp;
    temp = strtok(NULL, "");
    state = temp;

    print_header(action);

    load_proc_table("proc_table.dat");

    printf("<table width=\"90%%\" align=\"center\">\n");
    printf("<tr>\n");
    printf("</tr>\n");
    printf("<td width=\"10%%\">Status:</td><td>%s</td>\n", state);
    printf("<tr>\n");
    printf("</tr>\n");

    printf("<tr><td>Resources:</td>");
    printf("<td>");
    print_resource(pid, action);
    printf("</td>");
    printf("</tr>\n");

    printf("<tr><td>Description</td><td>\n");
    print_script(pid, action);
    printf("</td>");
    printf("</tr>\n");
    printf("</table>\n");

    printf("<br><br><br><br>\n");
    printf("<table width=\"90%%\" align=\"center\">\n");
    printf("<tr>\n");
    
    instances = peos_list_instances();
    gifname = (char *)calloc(strlen(instances[pid]) + 1, sizeof(char));
    fname = (char *)calloc(strlen(instances[pid]) + 1, sizeof(char));
    strcpy(fname, instances[pid]);
    graph_name = make_graph(fname); 
    gifname = strchr(fname, '/');
    gifname++;
    makedot(graph_name->source, fname, action, state);
    strtok(gifname, ".");
    strcat(gifname, ".gif");
    printf("<img src=%s>", gifname); 
    printf("<br><br><br>\n");
    printf("</tr>\n");
    
    printf("<tr>\n");

    if(strcmp(state,"active") == 0) {
	printf("<td width=\"100\">\n");
	printf("<a href=\"do.cgi?%d+%s=done\">\n", pid, action);
	printf("<img src=\"../images/done_button.jpg\" border=\"0\">\n");
	printf("</a>\n");
	printf("</td>\n");
	printf("<td width=\"100\">\n");
	printf("<a href=\"do.cgi?%d+%s=suspend\">\n", pid, action);
	printf("<img src=\"../images/suspend_button.jpg\" border=\"0\">\n");
	printf("</a>\n");
    } else if(strcmp(state, "suspend") == 0) {
	printf("<td width=\"100\">\n");
	printf("<a href=\"do.cgi?%d+%s=resume\">\n", pid, action);
	printf("<img src=\"../images/perform_button.jpg\" border=\"0\">\n");
	printf("</a>\n");
    } else if(strcmp(state, "ready") == 0) {
	printf("<td width=\"100\">\n");
	printf("<a href=\"do.cgi?%d+%s=run\">\n", pid, action);
	printf("<img src=\"../images/perform_button.jpg\" border=\"0\">\n");
	printf("</a>\n");
    } else {
	printf("Error in action state.\n");
    }
    printf("</td>\n");

    printf("<td>\n"); 
    printf("<a href=\"main.cgi\">\n");
    printf("<img src=\"../images/back_button.jpg\" border=\"0\">\n");
    printf("</a>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("</table>\n");
  
    printf("</td>\n");
    printf("</tr>\n");
    printf("</table>\n");

    printf("</body>\n");
    printf("</html>\n");

    return 0;
}
