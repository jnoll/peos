#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#undef TEST_PREDICATE_VERBOSE
#include "test_util.h"



/* Globals. */
peos_resource_t *global_resources;
int global_num_resources;

/* Stubs. */

peos_context_t *peos_get_context(int pid) {
    static peos_context_t context;
    return &context;
}

peos_resource_t *get_resource_list_action_provides(int pid, char *act_name, int *num_resources) {
    *num_resources = global_num_resources;
    return global_resources;
}

peos_resource_t *get_resource_list_action_requires(int pid, char *act_name, int *num_resources) {
    *num_resources = global_num_resources;
    return global_resources;
}

START_TEST(test_get_resource_index)
{
    peos_resource_t* resources = (peos_resource_t *) calloc(3, sizeof(peos_resource_t));
    strcpy(resources[0].name, "res0");
    strcpy(resources[1].name, "res1");
    strcpy(resources[2].name, "res2");
    fail_unless(get_resource_index(resources, 3, "res0") == 0, "get_resource_index failed");
    fail_unless(get_resource_index(resources, 3, "res1") == 1, "get_resource_index failed");
    fail_unless(get_resource_index(resources, 3, "res2") == 2, "get_resource_index failed");
}
END_TEST

START_TEST(test_get_tcl_file)
{
    system("touch peos_init.tcl");
    fail_unless(strcmp(get_tcl_file(), "./peos_init.tcl") == 0, "get_tcl_file");
    system("rm peos_init.tcl");
    fail_unless(!get_tcl_file(), "get_tcl_file");
}
END_TEST

START_TEST(test_get_eval_result_exists)
{
    system("more ./../peos_init.tcl > peos_init.tcl");
    fail_unless(get_eval_result("exists", "$res") == 0, "get_eval_result_exists failed");
    system("touch my_file");
    fail_unless(get_eval_result("exists", "my_file"), "get_eval_result_exists failed");
    system("rm my_file");
    fail_unless(!get_eval_result("exists", "my_file"), "get_eval_result_exists failed");
    system("rm peos_init.tcl");
}
END_TEST

START_TEST(test_get_eval_result_filecount)
{
    system("more ./../peos_init.tcl > peos_init.tcl");
    fail_unless(get_eval_result("filecount", "$res") == 0, "get_eval_result_filecount failed");
    system("rm -rf /tmp/my_dir");
    system("mkdir /tmp/my_dir");
    fail_unless(get_eval_result("filecount", "/tmp/my_dir") == 0, "get_eval_result_filecount failed");
    system("touch /tmp/my_dir/my_file_1");
    fail_unless(get_eval_result("filecount", "/tmp/my_dir") == 1, "get_eval_result_filecount failed");
    system("touch /tmp/my_dir/my_file_2");
    fail_unless(get_eval_result("filecount", "/tmp/my_dir") == 2, "get_eval_result_filecount failed");
    system("rm -rf /tmp/my_dir");
    fail_unless(get_eval_result("filecount", "not_exist_dir") == 0, "get_eval_result_filecount failed");
    system("rm peos_init.tcl");
}
END_TEST
        
START_TEST(test_get_eval_result_filesize)
{
    long before, after;
    system("more ./../peos_init.tcl > peos_init.tcl");
    fail_unless(get_eval_result("filesize", "$res") == 0, "get_eval_result_filesize failed");
    system("echo abc > my_file");
    before = get_eval_result("filesize", "my_file");
    system("echo abcdef > my_file");
    after = get_eval_result("filesize", "my_file");
    system("rm my_file");
    fail_unless(before < after, "get_eval_result_filesize failed");
    fail_unless(get_eval_result("filesize", "not_exist_file") == 0, "get_eval_result_filesize failed");
    system("rm peos_init.tcl");
}
END_TEST

START_TEST(test_get_eval_result_timestamp)
{
    long before, after;
    system("more ./../peos_init.tcl > peos_init.tcl");
    fail_unless(get_eval_result("timestamp", "$res") == 0, "get_eval_result_timestamp failed");
    system("touch -t 200201311759.30 my_file");  //year is 2002
    before = get_eval_result("timestamp", "my_file");
    system("touch -t 200301311759.30 my_file"); //change year to 2003
    after = get_eval_result("timestamp", "my_file");
    system("rm my_file");
    fail_unless(before < after, "get_eval_result_timestamp failed");
    fail_unless(get_eval_result("timestamp", "not_exists_file") == 0, "get_eval_result_timestamp failed");
    system("rm peos_init.tcl");
}
END_TEST

START_TEST(test_get_eval_result_misspellcount)
{
    system("more ./../peos_init.tcl > peos_init.tcl");
    fail_unless(get_eval_result("misspellcount", "$res") == 0, "get_eval_result_misspellcount failed");
    system("touch my_file");
    fail_unless(get_eval_result("misspellcount", "my_file") == 0, "get_eval_result_misspellcount failed");
    system("echo man > my_file");
    fail_unless(get_eval_result("misspellcount", "my_file") == 0, "get_eval_result_misspellcount failed");
    system("echo man aabb > my_file");
    fail_unless(get_eval_result("misspellcount", "my_file") == 1, "get_eval_result_misspellcount failed");
    system("echo ccdd man aabb > my_file");
    fail_unless(get_eval_result("misspellcount", "my_file") == 2, "get_eval_result_misspellcount failed");
    system("rm my_file");
    fail_unless(get_eval_result("misspellcount", "not_exists_file") == 0, "get_eval_result_misspellcount failed");
    system("rm peos_init.tcl");
}
END_TEST

START_TEST(test_eval_resource_list_bound) {
    peos_resource_t* resources = (peos_resource_t *) calloc(3, sizeof(peos_resource_t));
    
    strcpy(resources[0].name, "res0");
    strcpy(resources[0].value, "val0");
    strcpy(resources[1].name, "res1");
    strcpy(resources[1].value, "$res0/val1");
    strcpy(resources[2].name, "res2");
    strcpy(resources[2].value, "${res1}/val2");
    
    eval_resource_list(&resources, 3);
    
    fail_unless(strcmp(resources[0].value, "val0") == 0, "eval_resource_list_bound failed");
    fail_unless(strcmp(resources[1].value, "val0/val1") == 0, "eval_resource_list_bound failed");
    fail_unless(strcmp(resources[2].value, "val0/val1/val2") == 0, "eval_resource_list_bound failed");
}
END_TEST
        
START_TEST(test_eval_resource_list_unbound) {
    peos_resource_t* resources = (peos_resource_t *) calloc(3, sizeof(peos_resource_t));
    strcpy(resources[0].name, "res0");  //res0 is unbound
    strcpy(resources[1].name, "res1");
    strcpy(resources[1].value, "$res0/val1");
    strcpy(resources[2].name, "res2");
    strcpy(resources[2].value, "${res1}/val2");
    
    eval_resource_list(&resources, 3);
    
    fail_unless(strcmp(resources[0].value, "${res0}") == 0, "eval_resource_list_unbound failed");
    fail_unless(strcmp(resources[1].value, "${res0}/val1") == 0, "eval_resource_list_unbound failed");
    fail_unless(strcmp(resources[2].value, "${res0}/val1/val2") == 0, "eval_resource_list_unbound failed");
}
END_TEST

START_TEST(test_eval_predicate_abstract)
{
    peos_resource_t* resources;
    Tree t_res;
    
    resources = (peos_resource_t *) calloc(1, sizeof(peos_resource_t));
    strcpy(resources[0].name, "res");
    strcpy(resources[0].value, "my_file");
    strcpy(resources[0].qualifier, "abstract");

    t_res = make_tree("res", 0, NULL, NULL);
    
    fail_unless(eval_predicate(resources, 1, t_res), "eval_predicate_abstract_0 failed");
}
END_TEST
        
START_TEST(test_eval_predicate_single_node)
{
    peos_resource_t* resources;
    Tree t0, t1, t2, t3, t4, t5, t6;
    
    resources = (peos_resource_t *) calloc(1, sizeof(peos_resource_t));
    strcpy(resources[0].name, "res");
    strcpy(resources[0].value, "my_file");
    
    t0 = make_tree("\"True\"", 0, NULL, NULL);
    t1 = make_tree("\"Passed\"", 0, NULL, NULL);
    t2 = make_tree("\"1\"", 0, NULL, NULL);
    t3 = make_tree("\"False\"", 0, NULL, NULL);
    t4 = make_tree("\"Failed\"", 0, NULL, NULL);
    t5 = make_tree("\"0\"", 0, NULL, NULL);
    t6 = make_tree("res", 0, NULL, NULL);
    
    fail_unless(eval_predicate(NULL, 0, t0), "eval_predicate_single_node failed");
    fail_unless(eval_predicate(NULL, 0, t1), "eval_predicate_single_node failed");
    fail_unless(eval_predicate(NULL, 0, t2), "eval_predicate_single_node failed");
    fail_unless(!eval_predicate(NULL, 0, t3), "eval_predicate_single_node failed");
    fail_unless(!eval_predicate(NULL, 0, t4), "eval_predicate_single_node failed");
    fail_unless(!eval_predicate(NULL, 0, t5), "eval_predicate_single_node failed");
    
    system("more ./../peos_init.tcl > peos_init.tcl");
    system("touch my_file");
    fail_unless(eval_predicate(resources, 1, t6), "eval_predicate_single_node failed");
    system("rm my_file");
    fail_unless(!eval_predicate(resources, 1, t6), "eval_predicate_single_node failed");
    system("rm peos_init.tcl");
}
END_TEST
        
START_TEST(test_eval_predicate_dot)
{
    peos_resource_t* resources;
    Tree t_dot, t_res, t_exists;
    
    resources = (peos_resource_t *) calloc(1, sizeof(peos_resource_t));
    strcpy(resources[0].name, "res");
    strcpy(resources[0].value, "my_file");

    t_res = make_tree("res", 0, NULL, NULL);
    t_exists = make_tree("exists", 0, NULL, NULL);
    t_dot = make_tree(NULL, DOT, t_res, t_exists);
    
    system("more ./../peos_init.tcl > peos_init.tcl");
    system("touch my_file");
    fail_unless(eval_predicate(resources, 1, t_dot), "eval_predicate_dot failed");
    system("rm my_file");
    fail_unless(!eval_predicate(resources, 1, t_dot), "eval_predicate_dot failed");
    system("rm peos_init.tcl");
}
END_TEST

START_TEST(test_eval_predicate_and_or)
{
    peos_resource_t* resources;
    Tree t_and, t_or, t_res0, t_res1;
    
    resources = (peos_resource_t *) calloc(2, sizeof(peos_resource_t));
    strcpy(resources[0].name, "res0");
    strcpy(resources[0].value, "my_file0");
    strcpy(resources[1].name, "res1");
    strcpy(resources[1].value, "my_file1");

    t_res0 = make_tree("res0", 0, NULL, NULL);
    t_res1 = make_tree("res1", 0, NULL, NULL);
    t_and = make_tree(NULL, AND, t_res0, t_res1);
    t_or = make_tree(NULL, OR, t_res0, t_res1);
    
    system("more ./../peos_init.tcl > peos_init.tcl");
    system("touch my_file0");
    system("touch my_file1");
    fail_unless(eval_predicate(resources, 2, t_and), "eval_predicate_and_or failed");
    fail_unless(eval_predicate(resources, 2, t_or), "eval_predicate_and_or failed");
    system("rm my_file0");
    fail_unless(!eval_predicate(resources, 2, t_and), "eval_predicate_and_or failed");
    fail_unless(eval_predicate(resources, 2, t_or), "eval_predicate_and_or failed");
    system("rm my_file1");
    fail_unless(!eval_predicate(resources, 2, t_and), "eval_predicate_and_or failed");
    fail_unless(!eval_predicate(resources, 2, t_or), "eval_predicate_and_or failed");
    system("rm peos_init.tcl");
}
END_TEST
        
START_TEST(test_eval_predicate_eq_ne_gt_le_ge)
{
    peos_resource_t* resources;
    Tree t_eq, t_ne, t_lt, t_gt, t_le, t_ge, t_dot0, t_dot1, t_res0, t_res1, t_fz0, t_fz1;
    
    resources = (peos_resource_t *) calloc(2, sizeof(peos_resource_t));
    strcpy(resources[0].name, "res0");
    strcpy(resources[0].value, "my_file0");
    strcpy(resources[1].name, "res1");
    strcpy(resources[1].value, "my_file1");

    t_res0 = make_tree("res0", 0, NULL, NULL);
    t_fz0 = make_tree("filesize", 0, NULL, NULL);
    t_dot0 = make_tree(NULL, DOT, t_res0, t_fz0);
    
    t_res1 = make_tree("res1", 0, NULL, NULL);
    t_fz1 = make_tree("filesize", 0, NULL, NULL);
    t_dot1 = make_tree(NULL, DOT, t_res1, t_fz1);
    
    t_eq = make_tree(NULL, EQ, t_dot0, t_dot1); // res0.filesize == res1.filesize
    t_ne = make_tree(NULL, NE, t_dot0, t_dot1); // res0.filesize != res1.filesize
    t_lt = make_tree(NULL, LT, t_dot0, t_dot1); // res0.filesize < res1.filesize
    t_gt = make_tree(NULL, GT, t_dot0, t_dot1); // res0.filesize > res1.filesize
    t_le = make_tree(NULL, LE, t_dot0, t_dot1); // res0.filesize <= res1.filesize
    t_ge = make_tree(NULL, GE, t_dot0, t_dot1); // res0.filesize >= res1.filesize
    
    system("more ./../peos_init.tcl > peos_init.tcl");
    system("touch my_file0");
    system("touch my_file1");
    fail_unless(eval_predicate(resources, 2, t_eq), "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(!eval_predicate(resources, 2, t_ne), "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(!eval_predicate(resources, 2, t_lt), "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(!eval_predicate(resources, 2, t_gt), "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(eval_predicate(resources, 2, t_le), "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(eval_predicate(resources, 2, t_ge), "eval_predicate_eq_ne_gt_le_ge failed");
    system("echo \"12\" > my_file0");
    system("echo \"1234\" > my_file1");
    fail_unless(!eval_predicate(resources, 2, t_eq), "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(eval_predicate(resources, 2, t_ne), "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(eval_predicate(resources, 2, t_lt), "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(!eval_predicate(resources, 2, t_gt), "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(eval_predicate(resources, 2, t_le), "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(!eval_predicate(resources, 2, t_ge), "eval_predicate_eq_ne_gt_le_ge failed");
    system("echo \"1234\" > my_file0");
    system("echo \"12\" > my_file1");
    fail_unless(!eval_predicate(resources, 2, t_eq), "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(eval_predicate(resources, 2, t_ne), "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(!eval_predicate(resources, 2, t_lt), "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(eval_predicate(resources, 2, t_gt), "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(!eval_predicate(resources, 2, t_le), "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(eval_predicate(resources, 2, t_ge), "eval_predicate_eq_ne_gt_le_ge failed");
    system("rm my_file0");
    system("rm my_file1");
    system("rm peos_init.tcl");
}
END_TEST
        
int
main(int argc, char *argv[])
{
    int nf;
    SRunner *sr;
    Suite *s = suite_create("predicate_evaluator");
    TCase *tc;
    //system("cp -f ../tclf_*tcl `pwd`");
    parse_args(argc, argv);
    
    tc = tcase_create("get_resource_index");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_get_resource_index);
    
    tc = tcase_create("get_tcl_file");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_get_tcl_file);
    
    tc = tcase_create("get_eval_result_exists");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_get_eval_result_exists);
    
    tc = tcase_create("get_eval_result_filecount");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_get_eval_result_filecount);
    
    tc = tcase_create("get_eval_result_filesize");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_get_eval_result_filesize);
    
    tc = tcase_create("get_eval_result_timestamp");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_get_eval_result_timestamp);
    
    tc = tcase_create("get_eval_result_misspellcount");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_get_eval_result_misspellcount);
    
    tc = tcase_create("eval_resource_list_bound");
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_eval_resource_list_bound);
    
    tc = tcase_create("eval_resource_list_unbound");
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_eval_resource_list_unbound);
    
    tc = tcase_create("eval_predicate_abstract");
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_eval_predicate_abstract);
    
    tc = tcase_create("eval_predicate_single_node");
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_eval_predicate_single_node);
    
    tc = tcase_create("eval_predicate_dot");
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_eval_predicate_dot);
    
    tc = tcase_create("eval_predicate_and_or");
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_eval_predicate_and_or);
    
    tc = tcase_create("eval_predicate_eq_ne_gt_le_ge");
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_eval_predicate_eq_ne_gt_le_ge);
    
    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    suite_free(s);
    //system("rm  tclf_*.tcl");	/* Clean up. */
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
