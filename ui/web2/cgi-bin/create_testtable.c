#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "kernel/action.h"
#include "kernel/process_table.h"
#include "kernel/events.h"


int main()
{

  load_proc_table("proc_table.dat");
  peos_run("test_action", NULL, 0);
  save_proc_table("proc_table.dat");

  return 0;
}
