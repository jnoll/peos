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
    printf("  <title>%s</title>\n",title);
    printf("</head>\n");

    printf("<body class=\"myTable\">\n");

    printf("<table width=\"100%%\" bgcolor=\"#FFCC66\">\n");
    printf("  <tr>\n");
    printf("    <td><img src=\"../images/peos_logo.jpg\"></td>\n");
    printf("    <td ><center><font size=\"6\" face=\"Arial, Helvetica, sans-serif\">");
    printf("PEOS Process Enactment Demonstration</font></center></td>\n");
    printf("  </tr>\n");
    printf("</table>\n");

    printf("<hr>\n");

    printf("<center><font face=\"Arial, Helvetica, sans-serif\">\n");
    printf("<h1>%s</h1>\n", title);
    printf("</font></center>\n");

}

void print_noscript()
{
    printf("<noscript>\n");
    printf("<b> This page requires Java Script Support. Please turn on Java Script and reload this page.</b>\n");
    printf("</noscript>\n");
}
	
void print_actionlist_link(char *process_filename)
{

    printf("<table cellpadding=\"2\" cellspacing=\"2\" border=\"0\" style=\"text-align: left; width: 100%%;\">\n");
    printf("  <tbody>\n");
    printf("    <tr>\n");
    printf("      <td style=\"vertical-align: top;\">\n");
    printf("        <a href=\"action_list.cgi?process_filename=%s&start=false\">Back to Action List</a><br>\n",
           process_filename);
    printf("      </td>\n");
    printf("    </tr>\n");
    printf("  </tbody>\n");
    printf("</table>\n");
}


void print_createprocess_link(char *process_filename)
{

    printf("<table cellpadding=\"2\" cellspacing=\"2\" border=\"0\" style=\"text-align: left; width: 100%%;\">\n");
    printf("  <tbody>\n");
    printf("    <tr>\n");
    printf("      <td style=\"vertical-align: top;\">\n");
    printf("        <a href=\"process_listing.cgi?process_filename=%s\">Create Process</a><br>\n",
	   process_filename);
    printf("      </td>\n");
    printf("    </tr>\n");
    printf("  </tbody>\n");
    printf("</table>\n");
}

void goto_error_page(char *process_filename, char *msg)
{
    printf("Location: error_page.cgi?process_filename=%s&msg=%s\r\n\r\n",process_filename,msg);
}
    
