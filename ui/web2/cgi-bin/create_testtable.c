#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../../../os/kernel/vm.h"
#include "../../../os/kernel/process_table.h"
#include "../../../os/kernel/events.h"


int main()
{

  load_proc_table("proc_table.dat");
  peos_run("test_action", 0);
  save_proc_table("proc_table.dat");

  return 0;
}
