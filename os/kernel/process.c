#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "process.h"
#include "events.h"
#include "graph.h"
#include "graph_engine.h"

/* Globals. */


/* Forward declarations. */
extern peos_context_t *find_free_entry();



char *act_state_name(vm_act_state state) 
{
    switch (state) {
      case ACT_READY:
  	    return "READY";
	    break;
      case ACT_RUN:
  	    return "RUN";
	    break;
      case ACT_DONE:
  	    return "DONE";
	    break;
      case ACT_SUSPEND:
	    return "SUSPEND";
	    break;
      case ACT_NONE:
	    return "NONE";
	    break;
      case ACT_BLOCKED:
            return "BLOCKED";
            break;
      case ACT_PENDING:
	    return "PENDING";
	    break;
      case ACT_AVAILABLE:
	    return "AVAILABLE";
	    break;
      case ACT_SATISFIED:
	    return "SATISFIED";
	    break;
      default:
	    return "unknown syscall";
	    break;
      }
}


char *find_model_file(char *model)
{
    char *ext, model_file[BUFSIZ];
    char *model_dir;
    FILE *f;

    /* XXX FIXME! there is a potential buffer overrun here! */
    if (model[0] == '/' || strncmp(model, "./", 2) == 0) {
	model_file[0] = '\0';
    } else {
	model_dir = getenv("COMPILER_DIR");
	if (model_dir == NULL) {
	    model_dir = ".";
	}
	sprintf(model_file, "%s/", model_dir);
    }

    ext = strrchr(model, '.');
    if (ext != NULL) {
	strncat(model_file, model, ext - model);
    } else {
	strncat(model_file, model, strlen(model));
    }

    strcat(model_file, ".pml");
    if ((f = fopen(model_file, "r"))) {
        fclose(f);
        return strdup(model_file);
    } else {
        return NULL;
    }
}

int peos_create_instance(char *model_file,peos_resource_t *resources,int num_resources)
{
    peos_context_t *context;

    if ((context = find_free_entry()) == NULL) {
        return -1;
    }

    if ((context->process_graph = makegraph(model_file)) != NULL) {
	context->pid = peos_get_pid(context);
	context->num_resources = num_resources;
	context -> resources = resources;
        strcpy(context->model, model_file);
        context->status = PEOS_READY;
	initialize_graph(context->process_graph, context->pid);
	return (context->pid); 
    }
    
    return -1;
}


void log_event(char *message)
{
    FILE *file;
    struct tm *current_info;
    time_t current;
    char times[50];
    int filedes;
    
    time(&current);
    current_info = localtime(&current);
    current = mktime(current_info);
    strftime(times,25,"%b %d %Y %H:%M",localtime(&current));

    filedes = open("event.log", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    if (filedes < 0) {
        fprintf(stderr, "Cannot Get Event Log File Descriptor\n");
	exit(EXIT_FAILURE);
    }
    
    if(get_lock(filedes) < 0) {
        fprintf(stderr, "Cannot Obtain Event Log File Lock\n");
	exit(EXIT_FAILURE);
    }
    
    file = fdopen(filedes, "a");

    fprintf(file, "%s %s\n", times,message);
    release_lock(filedes);
    fclose(file);
    close(filedes);
}
		    



#ifdef UNIT_TEST
#include "test_process.c"
#endif
