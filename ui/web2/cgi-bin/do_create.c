#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../../os/kernel/vm.h"
#include "../../../os/kernel/process_table.h"
#include "../../../os/kernel/events.h"

void create_process(char *model);

int main()
{
  char *input, *model_name, *temp;

  printf("%s%c%c\n","Content-Type:text/html;charset=iso-8859-1",13,10);

  input = getenv("QUERY_STRING");
  temp = strchr(input, '=');
  if(temp){
    temp++;
    model_name = (char *) calloc(strlen(temp) + 1, sizeof(char));
    strcpy(model_name, temp);
    create_process(model_name);
  }
  else{ /*Model name is NULL*/
    printf("<p>Error! Error in passing variable.\n");
    exit(1);
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
  if ((pid = peos_run(model, 0, 0)) < 0) {
    printf("<P>%d</P>\n", pid);
    printf("<P>Couldn't create %s</P>\n", model);
  }
  save_proc_table("proc_table.dat");
}
