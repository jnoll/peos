#include <stdlib.h>
#include <stdio.h>
#include "events.h"
#include "process_table.h"
#include "vm.h"

int main()
{
  int i, j;
  char **result = (char **) peos_list_models();
  char **result2;

  load_proc_table("proc_table.dat");
  result2 = peos_list_instances(result2);
  save_proc_table("proc_table.dat");

  printf("%s%c%c\n","Content-Type:text/html;charset=iso-8859-1",13,10);

  printf("<body>\n");
  printf("<H1>TEST</H1>\n");

  printf("<table width=\"100%\" border=\"0\" celspacing=\"0\" cellpadding=\"0\">\n");
  printf("<tr>\n");
  printf("<td width=\"50%\">\n");
  printf(" <form name=\"model_form\" action=\"http://linux.students.engr.scu.edu/~wchu/cgi-bin/create.cgi\">\n");
  printf("<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">\n");
  printf("<tr>\n");
  printf("<td width=\"40%\">\n");
  printf("<select width=\"160\" style=\"width: 160px\" name=\"model_list\" size=\"10\">\n");
  printf("<option selected> Select Model </option>\n");
  for(j=0; result && result[j]; j++){
    printf("<option value=\"%s\">%s</option>\n",result[j], result[j]);
  }
  printf("</select>\n");
  printf("</td>\n");
  printf("<td width=\"60%\" valign=\"bottom\">\n");
  printf("<input type=\"submit\" value=\"Create Model\">\n");
  printf("</td>\n");
  printf("</tr>\n");
  printf("</table>\n");
  printf("</form>\n");
  printf("</td>\n");

  printf("<td>\n");
  printf("<form action=\"http://linux.students.engr.scu.edu/~wchu/cgi-bin/create.cgi\">\n");
  printf("<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">\n");
  printf("<td width=\"40%\">\n");
  printf("<select width=\"160\" style=\"width: 160px\" size=\"10\" name=\"model_list\">\n");
  printf("<option value=\"cont\" selected>Active Processes</option>\n");
  for(i=0; i <= PEOS_MAX_PID; i++){
    printf("<option value=\"cont\">%d&nbsp;&nbsp;&nbsp;%s</option>\n", i, result2[i]);
  }
  printf("</select>\n"); 
  printf("</td>\n");
  printf("<td width=\"60%\" valign=\"bottom\">\n");
  printf("<input type=\"submit\" value=\"Continue Process\">\n");
  printf("</td>\n");
  printf("</table>\n");
  printf("</form>\n");
  printf("</td>\n");
  printf("</tr>\n");
  printf("</table>\n");


  printf("</body>\n");
	  
  return 0;
}

