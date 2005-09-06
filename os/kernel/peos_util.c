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
            
    //printf("\nenv(%s) file(%s) result(%s)\n", env, file, result);

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

/*char *find_resource_file(char *model)
{
    char *ext, model_file[BUFSIZ];
    char *model_dir;
    FILE *f;
    
    // XXX FIXME! there is a potential buffer overrun here!
    if (model[0] == '/' || strncmp(model, "./", 2) == 0) {
        model_file[0] = '\0';
    }
    else {
        model_dir = getenv("COMPILER_DIR");
        if (model_dir == NULL) {
            model_dir = ".";
        }
        sprintf(model_file, "%s/", model_dir);
    }
    ext = strrchr(model, '.');
    if (ext != NULL) {
        strncat(model_file, model, ext - model);
    }
    else {
        strncat(model_file, model, strlen(model));
    }
    strcat(model_file, ".res");
    if ((f = fopen(model_file, "r"))) {
        fclose(f);
        return strdup(model_file);
    }
    return NULL;
}*/

#ifdef UNIT_TEST
#include "test_peos_util.c"
#endif

