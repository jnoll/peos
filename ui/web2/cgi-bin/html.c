#include <stdio.h>
#include <kernel/action.h>
#include <kernel/events.h>
#include <kernel/process_table.h>

void
print_header(char *title)
{
    printf("%s%c%c\n","Content-Type:text/html;charset=iso-8859-1",13,10);

    printf("<html>\n");
    printf("<head>\n");
    printf("</head>\n");

    printf("<body class=\"myTable\">\n");

    printf("<table width=\"100%%\" bgcolor=\"#FFCC66\">\n");
    printf("<tr>");
    printf("<td><img src=\"../images/peos_logo.jpg\"></td>\n");
    printf("<td ><center><font size=\"6\" face=\"Arial, Helvetica, sans-serif\">\n");
    printf("PEOS Process Enactment Demonstration\n");
    printf("</font></center></td>");
    printf("</tr>");
    printf("</table\n");

    printf("<hr>\n");

    printf("<center><font face=\"Arial, Helvetica, sans-serif\">\n");
    printf("<h1>%s</h1>\n", title);
    printf("</font></center>\n");

}

void print_noscript()
{
    printf("<noscript>\n");
    printf("<b> This page requires Java Script Support. Please turn on Java Script and reload this page");
    printf("</noscript>");
}
		    

void print_action(int pid, char* action, char *state)
{
    printf("<a href=\"action_page.cgi?%d+%s=%s\">%s</a>", pid, action, state, action);
}

void print_script(int pid, char *action, int one_line)
{
    char *script;
    int i=0;

    script = (char *) peos_get_script(pid, action);

    if(script) {
	for(i=0; script[i] != '\0';  i++) {
	    if (one_line && (i >= 40 || script[i] == '\n')) {
		break;
	    }
	    if(script[i] == '\"') continue; /* Don't display quotes. */
	    putchar(script[i]);
	}
    }
}


void print_resource(int pid, char *action)
{
    int i,  num_resources;
    char temp[256], * p;
    peos_resource_t *resources;

    resources = (peos_resource_t *) peos_get_resource_list_action(pid, action, &num_resources);
    if(resources){
	for(i=0; i<num_resources; i++){
	    printf("<a href=\"%s\">", resources[i].value);
	    for (p = resources[i].name; *p != '\0'; p++) {
		switch (*p) {
		case '\"': 
		    continue;
		default: putchar(*p);
		    break;
		}
	    }
	    printf("</a>");
	    if(i != num_resources-1)
		printf(", ");
	}
    }
}

void print_process_button()
{
    printf("<tr>\n");
    printf("<td height=\"80\" width=\"800\" valign=\"top\" colspan=\"4\">\n");
    printf("<br><br><br><br><br><br>\n");
    printf("<a href=\"create.cgi\">\n"); 
    printf("<img src=\"../images/newprocess_button.jpg\" width=\"120\" height=\"30\" border=\"0\">\n");
    printf("</a>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("<tr>\n");
    printf("<td height=\"180\" width=\"848\" valign=\"top\" colspan=\"4\">\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("</table>\n");
    printf("</body>\n");
    printf("</html>\n");
}
