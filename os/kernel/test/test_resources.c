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
extern Tree make_tree(char *sval,int ival, Tree left, Tree right);

peos_context_t *peos_get_context(int pid)
{
    static peos_context_t context;
    return &context;
}


/* Test growing resource list. */
START_TEST(test_insert_resource)
{
    int num_resources = 0, rsize = 2, i, num_expected;
    char rname[32];
    /* Pre: a possibly empty resource list exists. */
    peos_resource_t *resource_list = (peos_resource_t *) calloc(rsize, sizeof(peos_resource_t));

    /* Action: insert a number of resources into the list. */
    num_expected = (32 * rsize) + 1;
    for (i = 0; i < num_expected; i++) {
	sprintf(rname, "%s_%d", "resource", i);
	insert_resource(rname, &resource_list, &num_resources, &rsize);
    }

    /* Post: all resources have been inserted in order. */
    fail_unless(num_resources == num_expected, "num_resources wrong");
    for (i = 0; i < num_expected; i++) {
	sprintf(rname, "%s_%d", "resource", i);
	fail_unless(strcmp(resource_list[i].name, rname) == 0, "resource name wrong");
    }
}	
END_TEST

START_TEST(test_make_resource_list)
{
	int num_resources = 0, rsize = 256;
	char *ptr = "y";
	peos_resource_t *resource_list = (peos_resource_t *) calloc(rsize,sizeof(peos_resource_t));
	
	Tree t1 = make_tree("y",0,NULL,NULL);
	Tree t2 = make_tree("modified",0,NULL,NULL);
	Tree t3 = make_tree(NULL,DOT,t1,t2);
	Tree t4 = make_tree("true",0,NULL,NULL);
	Tree t5 = make_tree(NULL,EQ,t3,t4);

	make_resource_list(t5, &resource_list, &num_resources, &rsize);

	fail_unless(num_resources == 1,"num_resources wrong");
    fail_unless(strcmp(resource_list[0].name,ptr) == 0,"resource name wrong");

}	
END_TEST


/* Test growing resource list. */
START_TEST(test_make_resource_list_realloc)
{
    int num_resources = 0, rsize = 2, i, num_expected;
    char rname[32];
    peos_resource_t *resource_list = (peos_resource_t *) calloc(rsize,sizeof(peos_resource_t));
    Tree t[256];

    num_expected = (32 * rsize) + 1;
    for (i = 0; i < num_expected; i++) {
	sprintf(rname, "%s_%d", "resource", i);
	t[i] = make_tree(strdup(rname), 0, NULL, NULL);
    }

    for (i = 0; i < num_expected; i++) {
	make_resource_list(t[i], &resource_list, &num_resources, &rsize);
    }

    fail_unless(num_resources == num_expected, "num_resources wrong");
    for (i = 0; i < num_expected; i++) {
	sprintf(rname, "%s_%d", "resource", i);
	fail_unless(strcmp(resource_list[i].name, rname) == 0, "resource name wrong");
    }
}	
END_TEST


int
main(int argc, char *argv[])
{
    int nf;
    SRunner *sr;
    Suite *s = suite_create("resources");
    TCase *tc;

    parse_args(argc, argv);
    tc = tcase_create("make resource list");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_insert_resource);
    tcase_add_test(tc,test_make_resource_list);
    tcase_add_test(tc,test_make_resource_list_realloc);
    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    suite_free(s);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
