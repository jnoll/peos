#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <kernel/action.h>
#include <kernel/process_table.h>
#include <kernel/events.h>

void get_resources(char *model);

int main()
{
  char *input, *model_name, *temp;

  printf("%s%c%c\n","Content-Type:text/html;charset=iso-8859-1",13,10);

  printf("<html>\n");
  printf("<head>\n");
  printf("<style type=\"text/css\">\n");
  printf("<!--\n");
  printf(".myTable {\n");
  printf("background-image:url(../images/peos_bar.jpg);\n");
  printf("background-repeat:no-repeat;\n");
  printf("background-position:center top; }\n");
  printf("-->\n");
  printf("</style>\n");
  printf("</head>\n"); 
  printf("<body class=\"myTable\" text=\"FFFFFF\" link=\"#FFCC66\" vlink=\"#FFCC66\" alink=\"#FFCC66\">\n");
  printf("<br><br><br><br><br><br><br><br><br>\n");

  input = getenv("QUERY_STRING");
  temp = strchr(input, '=');
  if(temp){
    temp++;
    model_name = (char *) calloc(strlen(temp) + 1, sizeof(char));
    strcpy(model_name, temp);
    get_resources(model_name);
  }
  else{ /*Model name is NULL*/
    printf("<p>Error! Error in passing variable.\n");
    exit(1);
  }
  
  printf("</body>\n");
  printf("</html>\n");
  return 0;
}


void get_resources(char *model)
{
  int i, j, pid, num_resources;
  char *model_file;
  peos_resource_t *resources;

  load_proc_table("proc_table.dat");
  model_file = (char *)find_model_file(model);
  if(model_file == NULL){
    printf("error: model file not found\n");
    return;
  }

  resources = (peos_resource_t *) get_resource_list(model_file,&num_resources);
  if(resources == NULL){
    printf("error getting resources\n");
    return;
  }
  save_proc_table("proc_table.dat");

  if(num_resources < 1){
    printf("<form action=\"do_create.cgi\">\n");
    printf("<table width=\"840\" align=\"center\">\n");
    printf("<tr>\n");
    printf("<td>\n");
    printf("<font size=\"5\"><b>No Resources</b></font>\n");
    printf("<input type=\"hidden\" name=\"model\" value=\"%s\">\n", model);
    printf("</td>\n");
    printf("</tr>\n");
    printf("<tr>\n");
    printf("<td>\n");
    printf("<br><br><br>\n");
    printf("<input type=\"submit\" value=\"Create Process\">\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("</table>\n");
    printf("</form>\n");
  }
  else{
    printf("<form action=\"do_create.cgi\">\n");
    printf("<table width=\"840px\" height=\"510\" align=\"center\" background=\"../images/center.jpg\">\n");
    printf("<tr>\n");
    printf("<td height=\"1\" valign=\"top\">\n");
    printf("<input type=\"hidden\" name=\"model\" value=\"%s\">\n", model);
    printf("</td>\n");
    printf("</tr>\n");
    printf("<tr>\n");
    for(i=0; i<num_resources; i++){
      printf("<td width=\"150\" height=\"1\" valign=\"top\">\n");
      for(j=0; ;j++){
        if(resources[i].name[j] == '\0' || resources[i].name[j] == '\n')
          break;
        if(resources[i].name[j] == '\"') continue; /* Don't display quotes. */
        printf("<b>%c</b>",resources[i].name[j]);
      }
      printf("<b>:</b>\n");
      printf("</td>\n");
      printf("<td width=\"600\" height=\"1\" valign=\"top\">\n");
      printf("<input  name=\"%s\" size=\"30\">\n", "value");
      printf("</td>\n");
      printf("</tr>\n");
      if(i == num_resources-1)
        break;
      else
        printf("<tr>\n");
    } 
    printf("<tr>\n");
    printf("<td>\n");
    printf("<input type=\"submit\" value=\"Submit Values\">\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("</table>\n");
    printf("<table width=\"1017\" align=\"center\" height=\"62\" background=\"../images/bg_logoblack_bottom.jpg\">\n");
    printf("<tr>\n");
    printf("<td>\n");
    printf("</td>\n");
    printf("</tr>\n");
    printf("</table>\n");
    printf("</form>\n");
  }
}
