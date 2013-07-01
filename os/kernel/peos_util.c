#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "peos_util.h"

char* find_file(char* file) {
    char result[BUFSIZ];
    char* env;
    FILE* f;
    
    if (!file)
        return NULL;
    
    if ((strlen(file) > 0 && file[0] == '/') || (strlen(file) > 1 && strncmp(file, "./", 2) == 0)) {
        strcpy(result, file);
    }
    else {
        sprintf(result, "./%s", file);
    }
    if ((f = fopen(result, "r"))) {
        fclose(f);
        return strdup(result);
    }

    if ((env = getenv("COMPILER_DIR"))) {
        if (!(strlen(file) > 0 && file[0] == '/') && !(strlen(file) > 1 && strncmp(file, "./", 2) == 0)) {
            sprintf(result, "%s/%s", env, file);
            if ((f = fopen(result, "r"))) {
                fclose(f);
                return strdup(result);
            }
        }
    }
    return NULL;
}

#ifdef UNIT_TEST
#include "test_peos_util.c"
#endif

