#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../../os/kernel/vm.h"
#include "../../../os/kernel/process_table.h"
#include "../../../os/kernel/events.h"

int main()
{
  int i, pid, num_resources;
  char *input, *model_name, *model_file, *temp;
  peos_resource_t *resources;

  printf("%s%c%c\n","Content-Type:text/html;charset=iso-8859-1",13,10);

  input = getenv("QUERY_STRING");

  temp = strchr(input, '=');
  temp = strtok(temp, "&");

  if(temp){
    temp++;
    model_name = (char *) calloc(strlen(temp)+1, sizeof(char));
    strcpy(model_name, temp);
  }
   
  load_proc_table("proc_table.dat");
  model_file = (char *)find_model_file(model_name);
    if(model_file == NULL){
      printf("error: model file not found\n");
       return;
    }

    resources = (peos_resource_t *) get_resource_list(model_file,&num_resources);
    if(resources == NULL){
      printf("error getting resources\n");
      return;
    }
  
  for(i=0; i<num_resources-1; i++){
    temp = strtok(NULL, "&");
    temp = strchr(temp, '=');
      temp++;
      if(temp[0] != '\0'){
        if(strlen(temp) < 256){
          sprintf(resources[i].value, "%s", temp);
        }
        else
          printf("Error: Resource value must be less than 256 characters.\n");
      }
      else
	sprintf(resources[i].value, "%s", "default_value");
  }
  temp = strtok(NULL, "");
  temp = strchr(temp, '=');
  temp++;
  if(temp[0] != '\0'){
    if(strlen(temp) < 256)
      sprintf(resources[i].value, "%s", temp);
    else
      printf("Error: Resource value must be less than 256 characters.\n");
  }
  else
    sprintf(resources[i].value, "%s", "default_value");

  load_proc_table("proc_table.dat");
  if((pid = peos_run(model_name, resources, num_resources)) < 0) {
    printf("<P>%d</P>\n", pid);
    printf("<P>Couldn't create %s</P>\n", model_name);
  }
  save_proc_table("proc_table.dat");



  printf("<html>\n");
  printf("<head>\n");
  printf("</head>\n");
  printf("<body onload=\"window.location.href='main.cgi'\">\n");
  printf("</body>\n");
  printf("</html>\n");
		  
  return 0;
}
