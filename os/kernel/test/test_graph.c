/*
 * $ID$
 * Graph operation unit tests.
 * John Noll, SCU.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../graph.h"
#include "test_util.h"


START_TEST(test_makegraph)
{
    char *file = TEST_PROC_NAME;

    Graph g;
    Node child;

    /* pre: a correct pml file exists. */
    make_pml_file(file, BASELINE_PML);

    /* action. */
    g = makegraph(file);
    unlink(file);

    /* post: constructed graph contains actions, in order. */
    fail_unless(g != NULL, "no graph made from correct pml file");
    child = (Node) ListIndex(g->source->successors, 0);
    fail_unless(child != NULL, "constructed graph contains no actions");
    fail_unless(strcmp(child->name, "a") == 0,
		"constructed graph does not contain specified actions");

}
END_TEST

START_TEST(test_makegraph_nofile)
{
    char *file = "no_file";
    Graph g;
    g = makegraph(file);
    fail_unless(g == NULL, "graph made from non existing file");
}
END_TEST

START_TEST(test_makegraph_error)
{
    char *file = TEST_PROC_NAME;
    FILE *old_stderr;
    Graph g;

    /* pre: a pml file exists, with syntax errors. */
    make_pml_file(file, "process p {\n	action a {}\n	acti b {}\n}\n");

    /* redirect stderr to suppress parser error messages. */
    if (verbosity != CK_VERBOSE) {
	old_stderr = stderr;
	stderr = fopen("/dev/null", "w");
    }

    /* action */
    g = makegraph(file);

    /* restore stderr. */
    if (verbosity != CK_VERBOSE) {    
	fclose(stderr);
	stderr = old_stderr;
    }
    unlink(file);

    /* post: no graph created. */
    fail_unless(g == NULL, "graph made from file with parse errors.");
}
END_TEST


START_TEST(test_find_node)
{
    Node n;
    char *file = TEST_PROC_NAME, *act_name = "act_0";
    Graph g;

    /* Pre: Graph exists */
    make_pml_file(file, "process p {\n  action act_0 {}\n  action act_1 {}\n}\n");

    g = makegraph(file);

    /* Action */
    n = find_node(g, act_name);
    unlink(file);

    /* Post: node not found. */
    fail_unless(n != NULL, "no node found.");
    fail_unless(strcmp(n->name, act_name) == 0, "correct node not found.");
}
END_TEST


START_TEST(test_find_node_non_existent)
{
    Node n;
    char *file = TEST_PROC_NAME;
    Graph g;

    /* Pre : Graph exists */
    make_pml_file(file, "process p {\n  action act_0 {}\n  action act_1 {}\n}\n");

    g = makegraph(file);
    unlink(file);

    /* Action */
    n = find_node(g,"act_2");

    /* Post */
    fail_unless(n == NULL,"non existent node found");
}
END_TEST



int
main(int argc, char *argv[])
{
    int nf;
    SRunner *sr;
    Suite *s = suite_create("graph");
    TCase *tc;

    parse_args(argc, argv);

    tc = tcase_create("makegraph");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_makegraph);
    tcase_add_test(tc,test_makegraph_nofile);
    tcase_add_test(tc,test_makegraph_error);

    tc = tcase_create("find_node");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_find_node);
    tcase_add_test(tc, test_find_node_non_existent);



    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    suite_free(s);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

