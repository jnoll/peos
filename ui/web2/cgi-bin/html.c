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
		    
