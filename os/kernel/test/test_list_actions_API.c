#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>
#include "events.h"
#include "process_table.h"
#include "pmlheaders.h"
#include "graph_engine.h"
#include "process.h"
#include "unistd.h"


int create_process(char *model)
{
    int pid;
    int i;
    int num_resources;
    peos_resource_t *resources;


    resources = (peos_resource_t *) peos_get_resource_list(model,&num_resources);


    if (resources == NULL) {
        printf("error getting resources\n");
	return -1;
    }
    
    for(i = 0; i < num_resources; i++) {
	strcpy(resources[i].value, "$$");
    }
    
    printf("Executing %s:\n", model);
    
    if ((pid = peos_run(model,resources,num_resources)) < 0) {
	printf("couldn't create process\n");
	return -1;
    } 
    else {
	printf("Created pid = %d\n", pid);
	return 1;
    }
}


int
main(int argc, char *argv[])
{
    FILE *fp,*fp1,*fp2,*fp3;

    peos_action_t *alist;
    int num_actions;
    int j;


    fprintf(stderr, "Testing list_action API ...\n");
    
    /*create model file */

    fp = fopen("test_list_action_API.pml", "w");
    fprintf(fp, "process p {\n");
    fprintf(fp, "action act_1 {\n");
    fprintf(fp, "requires {test_list_action_API_res1}\n");
    fprintf(fp, "provides {test_list_action_API_res2}}\n");
    fprintf(fp, "action act_2 {\n");
    fprintf(fp, "requires {test_list_action_API_res2}\n");
    fprintf(fp, "provides {test_list_action_API_res3}}\n");
    fprintf(fp, "action act_3 {\n");
    fprintf(fp, "requires {test_list_action_API_res3}\n");
    fprintf(fp, "provides {test_list_action_API_res4}}}\n");
    fclose(fp);

    unlink("proc_table.dat");
    unlink("proc_table.dat.xml");

    /* Create a process */
    
    if (create_process("test_list_action_API.pml") < 0) {
        fprintf(stderr, "Error executing list instances API test\n");
        exit(EXIT_FAILURE);
    }


    /* bind resources */
    
    if (peos_set_resource_binding(0, "test_list_action_API_res1", "test_list_action_API_res1_val") < 0) { 
        fprintf(stderr, "Error executing list instances API test\n");
        exit(EXIT_FAILURE);
    }

    if (peos_set_resource_binding(0, "test_list_action_API_res2", "test_list_action_API_res2_val") < 0) { 
        fprintf(stderr, "Error executing list instances API test\n");
	exit(EXIT_FAILURE);
    }

    if (peos_set_resource_binding(0, "test_list_action_API_res3", "test_list_action_API_res3_val") < 0) { 
        fprintf(stderr, "Error executing list instances API test\n");
        exit(EXIT_FAILURE);
    }

    /* create the resources so that their predicates evaluate to true */
    fp1 = fopen("test_list_action_API_res1_val", "w");
    fp2 = fopen("test_list_action_API_res2_val", "w");
    fp3 = fopen("test_list_action_API_res3_val", "w");
    
    /*
     *  now do list instances -- the process should be updated to make 
     * actions act_1, act_2, and act_3 AVAILABLE 
     * 
     */
    
    
    alist = peos_list_actions(0,&num_actions);
    if(alist) {
        for(j = 0; j < num_actions; j++) {
            if(strcmp(alist[j].name,"act_1") == 0) {
	        if(alist[j].state != ACT_SATISFIED) {
		    fprintf(stderr, "LIST ACTION API TEST FAIL -- act_1 not READY");
		}
	    }
    		
            if(strcmp(alist[j].name,"act_2") == 0) {
	        if(alist[j].state != ACT_SATISFIED) {
		    fprintf(stderr, "LIST ACTION API TEST FAIL -- act_2 not AVAILABLE");
		}
	    }

            if(strcmp(alist[j].name,"act_3") == 0) {
	        if(alist[j].state != ACT_AVAILABLE) {
		    fprintf(stderr, "LIST ACTION API TEST FAIL -- act_3 not AVAILABLE");
		}
	    }
    		
        }
    }
    else {
        fprintf(stderr, "Error executing list instances API test\n");
	exit(EXIT_FAILURE);
    }

    /* clean up */

    fclose(fp1);
    fclose(fp2);
    fclose(fp3);
    unlink("test_list_action_API_res1_val");
    unlink("test_list_action_API_res2_val");
    unlink("test_list_action_API_res3_val");
    unlink("test_list_action_API.pml");
    unlink("proc_table.dat");
    unlink("proc_table.dat.xml");
    
    fprintf(stderr, "done .. \n");
    return 0;


}



