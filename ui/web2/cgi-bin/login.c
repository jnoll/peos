#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "kernel/events.h"
#include "kernel/action.h"
#include "kernel/process_table.h"
#include "html.h" 



int main()
{
    print_header("Login Page");

    printf("<br><br><div style=\"text-align: center;\">\n");
    printf("<form name=\"name\" action=\"action_list.cgi\"> Login Name <input type=\"text\" name=\"process_filename\"><br><br>");
    
    printf("<input type=\"hidden\" name=\"start\" value=\"true\"><br><br>");
    
    printf("<input type=\"submit\" name=\"Submit\" value=\"Submit\"><br><br></form></div>");
    exit(0);
    
}	
