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
  int i, pid, num_resources;
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
  for(i=0; i<num_resources; i++){
    sprintf(resources[i].value,"value_%d", i);
  }

  if ((pid = peos_run(model, resources, num_resources)) < 0) {
    printf("<P>%d</P>\n", pid);
    printf("<P>Couldn't create %s</P>\n", model);
  }
  save_proc_table("proc_table.dat");
  
}
