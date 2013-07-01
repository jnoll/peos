#ifndef _ENGINE_H
#define _ENGINE_H

#include "glo.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int runPeos (char *cmd);
char *getPath(void);

#endif 
