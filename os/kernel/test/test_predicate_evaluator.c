
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "test_util.h"

/* Globals. */
peos_resource_t *global_resources;
int global_num_resources;

/* Stubs. */

peos_resource_t *get_resource_list_action_provides(int pid, char *act_name, int *num_resources)
{
    
    *num_resources = global_num_resources;

    return global_resources;
} 
     

peos_resource_t *get_resource_list_action_requires(int pid, char *act_name, int *num_resources)
{ 
     
    *num_resources = global_num_resources;

    return global_resources;
}


START_TEST(test_is_provides_true)
{
/*
    FILE *fptr;

    peos_resource_t *resources = (peos_resource_t *) calloc(1, sizeof(peos_resource_t));

    strcpy(resources[0].name, "resource_0");
    strcpy(resources[0].value, "resource_file");
    strcpy(resources[0].qualifier, "abstract");

    global_resources = resources;
    global_num_resources = 1;
 
    fptr = fopen("resource_file", "w");

    fail_unless(is_provides_true(0, "some_name") == 1, "is_provides_true failed");

    if(resources) free(resources);

    unlink("resource_file");
    */
}
END_TEST
	
	
START_TEST(test_is_requires_true)
{
/*
    FILE *fptr;

    peos_resource_t *resources = (peos_resource_t *) calloc(1, sizeof(peos_resource_t));

    strcpy(resources[0].name, "resource_0");
    strcpy(resources[0].value, "resource_file");
    strcpy(resources[0].qualifier, "abstract");

    global_resources = resources;
    global_num_resources = 1;

    fptr = fopen("resource_file", "w");

    fail_unless(is_requires_true(0, "some_name") == 1, "is_requires_true failed");

    if(resources) free(resources);

    unlink("resource_file");
    */
}
END_TEST
	

START_TEST(test_is_requires_true_1)
{
/*
    peos_resource_t *resources = (peos_resource_t *) calloc(1, sizeof(peos_resource_t));

    strcpy(resources[0].name, "resource_0");
    strcpy(resources[0].value, "$$");
    strcpy(resources[0].qualifier, "abstract");

    global_resources = resources;
    global_num_resources = 1;

    fail_unless(is_requires_true(0, "some_name") == 1, "is_requires_true failed");

    if(resources) free(resources);

    unlink("resource_file");
    */
}
END_TEST
	

START_TEST(test_is_provides_true_1)
{
/*
    peos_resource_t *resources = (peos_resource_t *) calloc(1, sizeof(peos_resource_t));

    strcpy(resources[0].name, "resource_0");
    strcpy(resources[0].value, "$$");
    strcpy(resources[0].qualifier, "abstract");

    global_resources = resources;
    global_num_resources = 1;

    fail_unless(is_provides_true(0, "some_name") == 1, "is_provides_true failed");

    if(resources) free(resources);

    unlink("resource_file");
    */
}
END_TEST
	
    
START_TEST(test_is_requires_true_2)
{
/*
    peos_resource_t *resources = (peos_resource_t *) calloc(2, sizeof(peos_resource_t));

    strcpy(resources[0].name, "resource_0");
    strcpy(resources[0].value, "$$");
    strcpy(resources[0].qualifier, "abstract");

    strcpy(resources[0].name, "resource_0");
    strcpy(resources[0].value, "$$");
    strcpy(resources[0].qualifier, "$$");

    global_resources = resources;
    global_num_resources = 2;

    fail_unless(is_requires_true(0, "some_name") == 0, "is_requires_true 2  failed");

    if(resources) free(resources);

    unlink("resource_file");
    */
}
END_TEST


START_TEST(test_is_provides_true_2)
{
    peos_resource_t *resources = (peos_resource_t *) calloc(2, sizeof(peos_resource_t));

    strcpy(resources[0].name, "resource_0");
    strcpy(resources[0].value, "$$");
    strcpy(resources[0].qualifier, "abstract");

    strcpy(resources[0].name, "resource_0");
    strcpy(resources[0].value, "$$");
    strcpy(resources[0].qualifier, "$$");

    global_resources = resources;
    global_num_resources = 2;

    fail_unless(is_provides_true(0, "some_name") == 0, "is_provides_true 2  failed");

    if(resources) free(resources);

    unlink("resource_file");
}
END_TEST

//----------------------
START_TEST(test_pe_perform_predicate_eval)
{
/*
	int TREE_OP_t = EQ,
	    TREE_OP_t_left = DOT,
	    TREE_OP_t_right = DOT;
	    
	char* TREE_ID_t_left_right ="timestamp",* TREE_ID_t_right_right="timestamp";
	// normal case 
	if(TREE_OP_t >= EQ && TREE_OP_t <= GT){
		if(TREE_OP_t_left == DOT && TREE_OP_t_right == DOT){
			if(!strcmp("timestamp", TREE_ID_t_left_right) && !strcmp("timestamp", TREE_ID_t_right_right)){
				// this should be accessed
			}else 
				fail ("Should never have gotten here!");
		}
	}
	
	
	TREE_OP_t = EQ-1;
	// bad case 1 
	if(TREE_OP_t >= EQ && TREE_OP_t <= GT){
		if(TREE_OP_t_left == DOT && TREE_OP_t_right == DOT){
			if(!strcmp("timestamp", TREE_ID_t_left_right) && !strcmp("timestamp", TREE_ID_t_left_right)){
				fail ("Should never have gotten here!");
			}
		}
	}
	
	TREE_OP_t = GT;
	TREE_OP_t_left=EQ;
	
	
	// bad case 2 
	if(TREE_OP_t >= EQ && TREE_OP_t <= GT){
		if(TREE_OP_t_left == DOT && TREE_OP_t_right == DOT){
			if(!strcmp("timestamp", TREE_ID_t_left_right) && !strcmp("timestamp", TREE_ID_t_left_right)){
				fail ("Should never have gotten here!");
			}
		}
	}
*/
}
END_TEST

START_TEST(test_pe_eval)
{
/*

	-1 return conditions:
	* buf1 resource file is not found
	* buf2 resource file is not found
	* the TREE_IP was not found
	* condition and method of evaluation flags do not any one's implemented
	
	1 return conditions:
	* TREE OP condition evaluated to true
	0 return conditions:
	* TREE OP condition evaluated to false

	int resource_one_exists = 1,
	    resource_two_exists = 1,
	    cond_meth_satisfied = 1;
	    
	 
   	if(cond_meth_satisfied){
		if(resource_one_exists && resource_two_exists)
		
		fail_unless(resource_one_exists && resource_two_exists,
			"resources do not exist, no comparison should be made");
		
	}
*/
}
END_TEST
 
	
int
main(int argc, char *argv[])
{
    int nf;
    SRunner *sr;
    Suite *s = suite_create("predicate_evaluator");
    TCase *tc;

    parse_args(argc, argv);
    tc = tcase_create("is_provides_true");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_is_provides_true);
    tcase_add_test(tc,test_is_provides_true_1);
    tcase_add_test(tc,test_is_provides_true_2);
    
    
    tc = tcase_create("is_requires_true");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_is_requires_true);
    tcase_add_test(tc,test_is_requires_true_1);
    tcase_add_test(tc,test_is_requires_true_2);
    
    tc = tcase_create("testing_pe_eval");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_pe_eval);
        
    tc = tcase_create("testing_pe_perform_predicate_eval");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_pe_perform_predicate_eval);
    
    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    suite_free(s);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
