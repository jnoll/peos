
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "test_util.h"

/* Globals. */


/* Stubs. */

peos_resource_t *get_resource_list_action_provides(int pid, char *act_name, int *num_resources)
{
    peos_resource_t *resources = (peos_resource_t *) calloc(1, sizeof(peos_resource_t));

    strcpy(resources[0].name, "resource_0");
    strcpy(resources[0].value, "resource_file");
    
    *num_resources = 1;

    return resources;
}
    

peos_resource_t *get_resource_list_action_requires(int pid, char *act_name, int *num_resources)
{
    peos_resource_t *resources = (peos_resource_t *) calloc(1, sizeof(peos_resource_t));

    strcpy(resources[0].name, "resource_0");
    strcpy(resources[0].value, "resource_file");
     
    *num_resources = 1;

    return resources;
}


START_TEST(test_is_provides_true)
{
    FILE *fptr;

    fptr = fopen("resource_file", "w");

    fail_unless(is_provides_true(0, "some_name") == 1, "is_provides_true failed");

    unlink("resource_file");
}
END_TEST
	
	
START_TEST(test_is_requires_true)
{
    FILE *fptr;

    fptr = fopen("resource_file", "w");

    fail_unless(is_requires_true(0, "some_name") == 1, "is_requires_true failed");

    unlink("resource_file");
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
    
    tc = tcase_create("is_requires_true");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_is_requires_true);
    
    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    suite_free(s);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
