#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "vm.h"
#include "process_table.h"
#include "events.h"

void create_process(char *model);

int main()
{
  char *input, *model_name;

  printf("%s%c%c\n","Content-Type:text/html;charset=iso-8859-1",13,10);

  input = getenv("QUERY_STRING");

  if(sscanf(input,"model_list=%s", model_name) != 1)
  {
    printf("<P>Error! Error in passing variable.\n");
    exit(1);
  }
  else if(model_name == NULL)
  {
    printf("<P>Error! Error in passing variable.\n");
    exit(1);
  }
  else
  {
    create_process(model_name);
  }

  printf("<html>\n");
  printf("<head>\n");
  printf("<body onload=\"window.location.href='main.cgi'\"\n");
  printf("</body>\n");
  printf("</head>\n");
  printf("</html>\n");

  return 0;
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
