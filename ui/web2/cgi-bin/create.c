#include <stdio.h>
#include <stdlib.h>
#include "events.h"
#include "vm.h"
#include "process_table.h"


void load_actions();
void create_process(char *model);
void print_action(peos_action_t *action);


int main()
{
  char *input, *model_name, cont_proc;


  printf("%s%c%c\n","Content-Type:text/html;charset=iso-8859-1",13,10);
  printf("<html>\n");
  printf("<head>\n");
  printf("</head>\n");

  printf("<body>\n");
  input = getenv("QUERY_STRING");

  if(strcmp(input,"") == 0)
  {}
  else if(sscanf(input,"model_list=%s", model_name) != 1)
  {
    printf("<P>Error! Error in passing variable.\n");
    exit(1);
  }
  else if(model_name == NULL)
  {
    printf("<P>Error! Error in passing variable.\n");
    exit(1);
  }
  else if(strcmp(model_name,"cont") == 0)
  {}
  else
  {
    load_proc_table("proc_table.dat");
    create_process(model_name);
    save_proc_table("proc_table.dat");
  }
  

  printf("<P><br><br><br></P>\n"); 
  load_actions();

  printf("<script language=\"JavaScript\">\n");
  printf("<!--\n");
  printf("function enable_run()\n");
  printf("{document.formMain.run.disabled=false;\n");
  printf("document.formMain.done.disabled=true;\n");
  printf("document.formMain.suspend.disabled=true;\n");
  printf("document.formMain.resume.disabled=true;}\n");
  printf("function enable_done()\n");
  printf("{document.formMain.done.disabled=false;\n");
  printf("document.formMain.suspend.disabled=false;\n");
  printf("document.formMain.run.disabled=true;\n");
  printf("document.formMain.resume.disabled=true;}\n");
  printf("function enable_resume()\n");
  printf("{document.formMain.resume.disabled=false;\n");
  printf("document.formMain.run.disabled=true;\n");
  printf("document.formMain.suspend.disabled=true;\n");
  printf("document.formMain.done.disabled=true;}\n");
  printf("//-->\n");
  printf("</script>\n");
  printf("</body>\n");
  printf("</html>\n");


  return 0;
}  

void load_actions()
{
  int i;
  peos_action_t **alist;

  printf("<form name=\"formMain\" action=\"http://linux.students.engr.scu.edu/~wchu/cgi-bin/run.cgi\">\n");

  printf("<table width=\"100\%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">\n");
  printf("<tr>\n");
  printf("Active\n");
  printf("</tr>\n");
  printf("<tr>\n");
  printf("<td width=\"33%\">\n");
  printf("<select width=\"160\" style=\"width: 160px\" name=\"action\" size=\"2\"
		        onchange=\"enable_done()\">\n");
  load_proc_table("proc_table.dat");
  alist = peos_list_actions(ACT_RUN);
  if (alist && alist[0]){
    for(i=0; alist[i]; i++){
      print_action(alist[i]);
    } 
  }
  save_proc_table("proc_table.dat");
  printf("</select>\n");
  printf("<input type=\"submit\" size=\"20\" name=\"done\" value=\"Done\" disabled>");
  printf("<input type=\"submit\" size=\"20\" name=\"suspend\" value=\"Suspend\" disabled>");
  printf("</td>\n");
  printf("<td align=\"center\">\n");
  printf("<p><textarea width=\"720\" style=\"width: 720px\" rows=\"4\" disabled>\n");
  printf("</textarea></p>\n");
  printf("</td>\n");
  printf("</tr>\n");
  printf("</table>\n");

  printf("<table width=\"100\%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">\n");
  printf("<br><br><br>\n");
  printf("<tr>\n");
  printf("<td width=\"33%\">\n");
  printf("Ready\n");
  printf("</td>\n");
  printf("<td width=\"67%\">\n");
  printf("Suspended\n");
  printf("</td>\n");
  printf("</tr>\n");

  printf("<tr>\n");

  printf("<td width=\"33%\">\n"); 
  printf("<select width=\"160\" style=\"width: 160px\" name=\"action\" size=\"10\" 
	  onclick=\"enable_run()\">\n"); 
  load_proc_table("proc_table.dat");
  alist = peos_list_actions(ACT_READY);
  if (alist && alist[0]){
    for(i=0; alist[i]; i++){
      print_action(alist[i]);
    }
  }
  save_proc_table("proc_table.dat");
  printf("</select>\n");
  printf("<input type=\"submit\" size=\"20\" name=\"run\" value=\"Run\" disabled 
          onclick=\"run_process()\">");
  printf("</td>\n");

  printf("<td width=\"34%\">\n");
  printf("<select width=\"160\" style=\"width: 160px\" name=\"action\" size=\"10\"
          onchange=\"enable_resume()\">\n");
  load_proc_table("proc_table.dat");
  alist = peos_list_actions(ACT_SUSPEND);
  if (alist && alist[0]){
    for(i=0; alist[i]; i++){
      print_action(alist[i]);
    }
  }
  save_proc_table("proc_table.dat");
  printf("</select>\n");
  printf("<input type=\"submit\" name=\"resume\" value=\"Resume\" disabled>");
  printf("</td>\n");
  printf("</tr>\n");
  printf("</table>\n");
  printf("</form>\n");

  printf("<br><br>\n");
  printf("<table width=\"100\%\" border=\"0\" cellspacing=\"0\" cellpadding=\"0\">\n");
  printf("<tr>\n");
  printf("<td align=\"left\">\n");
  printf("<br>\n");
  printf("<form action=\"http://linux.students.engr.scu.edu/~wchu/cgi-bin/test.cgi\">\n");
  printf("<input type=\"submit\" value=\"Create Process\">\n"); 
  printf("</td>\n");
  printf("</tr>\n");
  printf("</form>\n");
  printf("</table>\n");
}

void create_process(char *model)
{
  int pid;

  load_proc_table("proc_table.dat");
  if ((pid = peos_run(model, 0)) < 0) {
     printf("<P>%d</P>\n", pid);
     printf("<P>Couldn't create %s</P>\n", model);
  }
  save_proc_table("proc_table.dat");
}

void print_action(peos_action_t *action)
{
  printf("<option value=\"%d %s\">%d %s</option>\n"
         , action->pid, action->name, action->pid, action->name);
}
