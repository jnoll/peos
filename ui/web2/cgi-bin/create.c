#include <stdlib.h>
#include <stdio.h>
#include <kernel/events.h>
#include <kernel/process_table.h>
#include <kernel/vm.h>

int main()
{
  int i, j;
  char **result = (char **) peos_list_models();
  char **result2;

  load_proc_table("proc_table.dat");
  result2 = peos_list_instances(result2);
  save_proc_table("proc_table.dat");

  print_header("Create New Process");
  printf("<h2>Select a Model to Instantiate:</h2>\n");

  printf("<center><table width=\"70%%\">\n");
  for(j=0; result && result[j]; j++){
    printf("<tr><td><a href=\"do_create.cgi?model_list=%s\">%s.pml</a></td</tr>\n", result[j], result[j]);
  }
  printf("</table></center>\n");

  printf("</body>\n");
	  
  return 0;
}

