
#define YYDEBUG (0)

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
/* Kernel includes */
#include <events.h>
#include <process_table.h>
#include <pmlheaders.h>
#include <graph_engine.h>
#include <process.h>
#include <peos_util.h>
#include "getcgi.h"
#include "util.h"

/* XXX This is a hack made possible by the fact that process_table_filename defined in process_table.c is not static. */
extern char *process_table_filename;

int create_process(char *model) {
    int pid;
    int num_resources;
    peos_resource_t *resources;
    char *res_file;
    char *model_file = calloc(strlen(model) + strlen(".pml") + 1, sizeof(char));
    
    strcpy(model_file, model);
    strcat(model_file, ".pml");
    resources = (peos_resource_t *) peos_get_resource_list(model_file,&num_resources);    //see events.c

    if (resources == NULL) {
        fprintf(stderr, "error getting resources for model '%s'\n", model_file);
        return -1;
    }
    
    fprintf(stderr, "Executing %s:\n", model);
    
    if ((pid = peos_run(model_file,resources,num_resources)) < 0) {    //see events.c
        fprintf(stderr, "error running process\n");
        return -1;
    }
    
    if ((res_file = peos_get_resource_file(model_file)))
        peos_bind_resource_file(pid, res_file);

    fprintf(stderr, "created pid = %d\n", pid);
    return 1;
}




void emit_xml() {
    FILE *in;
    int c;
    char **p, **models, *xml_data_filename = (char *) calloc((strlen(process_table_filename) + strlen(".xml") + 1), sizeof(char));

    strcpy(xml_data_filename, process_table_filename);
    strcat(xml_data_filename, ".xml");
    printf("Content-type: text/xml; charset=UTF-8\r\n\r\n");
    printf("<peos>\n");
    printf("<models>\n");
    if ((models = peos_list_models())) {
	for (p = models; *p; p++) {
	    printf("<model>%s</model>\n", *p);
	}
    }
    printf("</models>\n");

    if ((in = fopen(xml_data_filename, "r"))) {
	while ((c = getc(in)) != EOF) {
	    putchar(c);
	} 
	fclose(in);
    } else {
	printf("<process_table/>\r\n");
    }
    printf("</peos>\n");
}


int
main (int argc, char **argv)
{
    int status;
    char *pid;
    char *action;
    char *model;
    char *login = "proc_table"; /* default login name */
    char *request_method= getenv("REQUEST_METHOD") ;
    char *event, **cgivars = getcgivars();

    setenv("COMPILER_DIR", ".", 1);

    if (strcmp(request_method, "POST") == 0) {
	set_login_name(login);
	event = getvalue("event", cgivars);


	if (strcmp(event, "create") == 0) {
	    model = getvalue("model", cgivars);

	    if (create_process(model) != 1) {
		fprintf(stderr, "Could not Create Process\n");
		exit(EXIT_FAILURE);
	    }

	} else if (strcmp(event, "delete") == 0) {
	    pid = getvalue("pid", cgivars);

	    if (peos_delete_process_instance(atoi(pid)) < 0) {
		fprintf(stderr, "Could not delete process instance\n");
		exit(EXIT_FAILURE);
	    }
	} else if (strcmp(event, "start") == 0) {
	    pid = getvalue("pid", cgivars);
	    action  = getvalue("action", cgivars);

	    if ((status = peos_notify(atoi(pid), action, PEOS_EVENT_START)) == VM_ERROR 
		|| (status == VM_INTERNAL_ERROR)) {
		fprintf(stderr, "unable to start action '%s' in process instance '%d'\n", action, atoi(pid));
		return -1;
	    }

	} else if (strcmp(event, "finish") == 0) {
	    pid = getvalue("pid", cgivars);
	    action  = getvalue("action", cgivars);

	    if ((status = peos_notify(atoi(pid), action,PEOS_EVENT_FINISH)) == VM_ERROR 
		|| status == VM_INTERNAL_ERROR) {
		fprintf(stderr, "process executed an illegal instruction and has been terminated\n");
		return -1;
	    }

	} else if (strcmp(event, "abort") == 0) {
	    pid = getvalue("pid", cgivars);
	    action  = getvalue("action", cgivars);

	    if ((status = peos_notify(atoi(pid), action,PEOS_EVENT_ABORT)) == VM_ERROR 
		|| status == VM_INTERNAL_ERROR) {
		fprintf(stderr, "process encountered an illegal event and has been terminated\n");
		return -1;
	    }

	} else if (strcmp(event, "suspend") == 0) {
	    pid = getvalue("pid", cgivars);
	    action  = getvalue("action", cgivars);

	    if ((status = peos_notify(atoi(pid), action,PEOS_EVENT_SUSPEND)) == VM_ERROR 
		|| status == VM_INTERNAL_ERROR) {
		fprintf(stderr, "process encountered an illegal event and has been terminated\n");
		return -1;
	    }
	}
    }

    /* GET method means just emit XML. */
    emit_xml();
    return (0);
}

