#include <stdlib.h>
#include <stdio.h>
#include "../../../os/kernel/events.h"
#include "../../../os/kernel/process_table.h"
#include "../../../os/kernel/vm.h"

int main()
{
  int i, j;
  char **result = (char **) peos_list_models();
  char **result2;

  load_proc_table("proc_table.dat");
  result2 = peos_list_instances(result2);
  save_proc_table("proc_table.dat");

  printf("%s%c%c\n\n","Content-Type:text/html;charset=iso-8859-1",13,10);
  printf("<html>\n");
  printf("<head>\n");
  printf("<style type=\"text/css\">\n");
  printf("<!--\n");
  printf(".myTable {\n");
  printf("background-image:url(../images/bg_logoblack_full.jpg);\n");
  printf("background-repeat:no-repeat;\n");
  printf("background-position:center top; }\n");
  printf("-->\n");
  printf("</style>\n");
  printf("</head>\n");

  printf("<body class=\"myTable\">\n");
  printf("<br><br><br><br><br><br><br><br><br><br>\n");
  printf("<table width=\"820px\" align=\"center\" border=\"0\" celspacing=\"0\" cellpadding=\"0\">\n");
  printf("<tr>\n");
  printf("<td width=\"50%\">\n");
  printf(" <form name=\"model_form\" action=\"resource.cgi\">\n");
  printf("<table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">\n");
  printf("<tr>\n");
  printf("<td width=\"40%\">\n");
  printf("&nbsp;&nbsp&nbsp&nbsp\n");
  printf("<select width=\"160\" style=\"width: 160px\" name=\"model_list\" size=\"10\">\n");
  printf("<option selected> Select Model </option>\n");
  for(j=0; result && result[j]; j++){
    printf("<option value=\"%s\">%s.pml</option>\n",result[j], result[j]);
  }
  printf("</select>\n");
  printf("</td>\n");
  printf("<td width=\"60%\" valign=\"bottom\">\n");
  printf("&nbsp;&nbsp;\n");
  printf("<input type=\"submit\" value=\"Create Model\">\n");
  printf("</td>\n");
  printf("</tr>\n");
  printf("</table>\n");
  printf("</form>\n");
  printf("</td>\n");

  printf("</tr>\n");
  printf("</table>\n");


  printf("</body>\n");
	  
  return 0;
}

