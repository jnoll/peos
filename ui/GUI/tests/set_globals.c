#include "set_globals.h"

void
set_CUR_PID(int value) 
{

   cur_pid = value;

}

char *
set_CWD(char *str)
{

  char *temp;
  int size = 0;

  if(str == NULL) {
  	temp = NULL;
	cwd = temp;
	return temp;

  } else {
	size = strlen(str) + 1;
	temp = (char *) malloc(size);
  	strcpy(temp, str);
	cwd = temp;
	return temp;
  }

}
