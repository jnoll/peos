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

START_TEST(test_get_eval_result_exists)
{
    fail_unless(get_eval_result("../peos_init.tcl", "exists", "$res") == 0, "get_eval_result_exists failed");
    system("touch my_file");
    fail_unless(get_eval_result("../peos_init.tcl", "exists", "my_file"), "get_eval_result_exists failed");
    system("rm my_file");
    fail_unless(!get_eval_result("../peos_init.tcl", "exists", "my_file"), "get_eval_result_exists failed");
}
END_TEST
        
START_TEST(test_get_eval_result_filecount)
{
    fail_unless(get_eval_result("../peos_init.tcl", "filecount", "$res") == 0, "get_eval_result_filecount failed");
    system("rm -rf /tmp/my_dir");
    system("mkdir /tmp/my_dir");
    fail_unless(get_eval_result("../peos_init.tcl", "filecount", "/tmp/my_dir") == 0, "get_eval_result_filecount failed");
    system("touch /tmp/my_dir/my_file_1");
    fail_unless(get_eval_result("../peos_init.tcl", "filecount", "/tmp/my_dir") == 1, "get_eval_result_filecount failed");
    system("touch /tmp/my_dir/my_file_2");
    fail_unless(get_eval_result("../peos_init.tcl", "filecount", "/tmp/my_dir") == 2, "get_eval_result_filecount failed");
    system("rm -rf /tmp/my_dir");
    fail_unless(get_eval_result("../peos_init.tcl", "filecount", "not_exist_dir") == 0, "get_eval_result_filecount failed");
}
END_TEST
        
START_TEST(test_get_eval_result_filesize)
{
    FILE *fp;
    long before, after;
    fail_unless(get_eval_result("../peos_init.tcl", "filesize", "$res") == 0, "get_eval_result_filesize failed");
     fp = fopen("my_file", "w");
    fprintf(fp, "some text\n");
    fclose(fp);
    before = get_eval_result("../peos_init.tcl", "filesize", "my_file");
    fp = fopen("my_file", "w");
    fprintf(fp, "some more text\n");
    fclose(fp);
    after = get_eval_result("../peos_init.tcl", "filesize", "my_file");
    system("rm my_file");
    fail_unless(before < after, "get_eval_result_filesize failed");
    fail_unless(get_eval_result("../peos_init.tcl", "filesize", "not_exist_file") == 0, "get_eval_result_filesize failed");
}
END_TEST

START_TEST(test_get_eval_result_timestamp)
{
    long before, after;
    fail_unless(get_eval_result("../peos_init.tcl", "timestamp", "$res") == 0, "get_eval_result_timestamp failed");
    system("touch -t 200201311759.30 my_file");  //year is 2002
    before = get_eval_result("../peos_init.tcl", "timestamp", "my_file");
    system("touch -t 200301311759.30 my_file"); //change year to 2003
    after = get_eval_result("../peos_init.tcl", "timestamp", "my_file");
    system("rm my_file");
    fail_unless(before < after, "get_eval_result_timestamp failed");
    fail_unless(get_eval_result("../peos_init.tcl", "timestamp", "not_exists_file") == 0, "get_eval_result_timestamp failed");
}
END_TEST

START_TEST(test_get_eval_result_misspellcount)
{
    FILE* fp;
    fail_unless(get_eval_result("../peos_init.tcl", "misspellcount", "$res") == 0, "get_eval_result_misspellcount failed");
    fp = fopen("my_file", "w");
    fprintf(fp, "no error\n");
    fclose(fp);
    fail_unless(get_eval_result("../peos_init.tcl", "misspellcount", "my_file") == 0, "get_eval_result_misspellcount failed");
    fp = fopen("my_file", "w");
    fprintf(fp, "aabb\n");
    fclose(fp);
    fail_unless(get_eval_result("../peos_init.tcl", "misspellcount", "my_file") == 1, "get_eval_result_misspellcount failed");
    fp = fopen("my_file", "w");
    fprintf(fp, "ccdd\n");
    fclose(fp);
    system("rm my_file");
    fail_unless(get_eval_result("../peos_init.tcl", "misspellcount", "not_exists_file") == 0, "get_eval_result_misspellcount failed");
}
END_TEST

START_TEST(test_eval_resource_list_0) {
    peos_resource_t* resources = (peos_resource_t *) calloc(3, sizeof(peos_resource_t));
    
    strcpy(resources[0].name, "res0");
    strcpy(resources[0].value, "val0");
    strcpy(resources[1].name, "res1");
    strcpy(resources[1].value, "$res0/val1");
    strcpy(resources[2].name, "res2");
    strcpy(resources[2].value, "${res1}/val2");
    
    eval_resource_list(&resources, 3);
    
    fail_unless(strcmp(resources[0].value, "val0") == 0, "eval_resource_list failed");
    fail_unless(strcmp(resources[1].value, "val0/val1") == 0, "eval_resource_list failed");
    fail_unless(strcmp(resources[2].value, "val0/val1/val2") == 0, "eval_resource_list failed");
}
END_TEST
        
START_TEST(test_eval_resource_list_1) {
    peos_resource_t* resources = (peos_resource_t *) calloc(3, sizeof(peos_resource_t));
    strcpy(resources[0].name, "res0");  //res0 is unbound
    strcpy(resources[1].name, "res1");
    strcpy(resources[1].value, "$res0/val1");
    strcpy(resources[2].name, "res2");
    strcpy(resources[2].value, "${res1}/val2");
    
    eval_resource_list(&resources, 3);
    
    fail_unless(strcmp(resources[0].value, "${res0}") == 0, "eval_resource_list failed");
    fail_unless(strcmp(resources[1].value, "${res0}/val1") == 0, "eval_resource_list failed");
    fail_unless(strcmp(resources[2].value, "${res0}/val1/val2") == 0, "eval_resource_list failed");
}
END_TEST

START_TEST(test_eval_predicate_abstract_0)
{
    peos_resource_t* resources;
    Tree t_res;
    
    resources = (peos_resource_t *) calloc(3, sizeof(peos_resource_t));
    strcpy(resources[0].name, "res");
    strcpy(resources[0].value, "my_file");
    strcpy(resources[0].qualifier, "abstract");

    t_res = make_tree("res", 0, NULL, NULL);
    
    fail_unless(eval_predicate("../peos_init.tcl", resources, 1, t_res), "eval_predicate_abstract_0 failed");
}
END_TEST
        
START_TEST(test_eval_predicate_0)
{
    peos_resource_t* resources;
    Tree t_res;
    
    resources = (peos_resource_t *) calloc(3, sizeof(peos_resource_t));
    strcpy(resources[0].name, "res");
    strcpy(resources[0].value, "my_file");

    t_res = make_tree("res", 0, NULL, NULL);
    
    system("touch my_file");
    fail_unless(eval_predicate("../peos_init.tcl", resources, 1, t_res), "eval_predicate_0 failed");
    system("rm my_file");
    fail_unless(!eval_predicate("../peos_init.tcl", resources, 1, t_res), "eval_predicate_0 failed");
}
END_TEST
        
START_TEST(test_eval_predicate_1)
{
    peos_resource_t* resources;
    Tree t_dot, t_res, t_exists;
    
    resources = (peos_resource_t *) calloc(3, sizeof(peos_resource_t));
    strcpy(resources[0].name, "res");
    strcpy(resources[0].value, "my_file");

    t_res = make_tree("res", 0, NULL, NULL);
    t_exists = make_tree("exists", 0, NULL, NULL);
    t_dot = make_tree(NULL, DOT, t_res, t_exists);
    
    system("touch my_file");
    fail_unless(eval_predicate("../peos_init.tcl", resources, 1, t_dot), "eval_predicate_1 failed");
    system("rm my_file");
    fail_unless(!eval_predicate("../peos_init.tcl", resources, 1, t_dot), "eval_predicate_1 failed");
}
END_TEST

START_TEST(test_eval_predicate_2)
{
    peos_resource_t* resources;
    Tree t_and, t_or, t_res0, t_res1;
    
    resources = (peos_resource_t *) calloc(3, sizeof(peos_resource_t));
    strcpy(resources[0].name, "res0");
    strcpy(resources[0].value, "my_file0");
    strcpy(resources[1].name, "res1");
    strcpy(resources[1].value, "my_file1");

    t_res0 = make_tree("res0", 0, NULL, NULL);
    t_res1 = make_tree("res1", 0, NULL, NULL);
    t_and = make_tree(NULL, AND, t_res0, t_res1);
    t_or = make_tree(NULL, OR, t_res0, t_res1);
    
    system("touch my_file0");
    system("touch my_file1");
    fail_unless(eval_predicate("../peos_init.tcl", resources, 2, t_and), "eval_predicate_2 failed");
    fail_unless(eval_predicate("../peos_init.tcl", resources, 2, t_or), "eval_predicate_2 failed");
    system("rm my_file0");
    fail_unless(!eval_predicate("../peos_init.tcl", resources, 2, t_and), "eval_predicate_2 failed");
    fail_unless(eval_predicate("../peos_init.tcl", resources, 2, t_or), "eval_predicate_2 failed");
    system("rm my_file1");
    fail_unless(!eval_predicate("../peos_init.tcl", resources, 2, t_and), "eval_predicate_2 failed");
    fail_unless(!eval_predicate("../peos_init.tcl", resources, 2, t_or), "eval_predicate_2 failed");
}
END_TEST
        
START_TEST(test_eval_predicate_3)
{
    peos_resource_t* resources;
    Tree t_eq, t_ne, t_lt, t_gt, t_le, t_ge, t_dot0, t_dot1, t_res0, t_res1, t_fz0, t_fz1;
    
    resources = (peos_resource_t *) calloc(3, sizeof(peos_resource_t));
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
    
    system("touch my_file0");
    system("touch my_file1");
    fail_unless(eval_predicate("../peos_init.tcl", resources, 2, t_eq), "eval_predicate_3 failed");
    fail_unless(!eval_predicate("../peos_init.tcl", resources, 2, t_ne), "eval_predicate_3 failed");
    fail_unless(!eval_predicate("../peos_init.tcl", resources, 2, t_lt), "eval_predicate_3 failed");
    fail_unless(!eval_predicate("../peos_init.tcl", resources, 2, t_gt), "eval_predicate_3 failed");
    fail_unless(eval_predicate("../peos_init.tcl", resources, 2, t_le), "eval_predicate_3 failed");
    fail_unless(eval_predicate("../peos_init.tcl", resources, 2, t_ge), "eval_predicate_3 failed");
    system("echo \"12\" > my_file0");
    system("echo \"1234\" > my_file1");
    fail_unless(!eval_predicate("../peos_init.tcl", resources, 2, t_eq), "eval_predicate_3 failed");
    fail_unless(eval_predicate("../peos_init.tcl", resources, 2, t_ne), "eval_predicate_3 failed");
    fail_unless(eval_predicate("../peos_init.tcl", resources, 2, t_lt), "eval_predicate_3 failed");
    fail_unless(!eval_predicate("../peos_init.tcl", resources, 2, t_gt), "eval_predicate_3 failed");
    fail_unless(eval_predicate("../peos_init.tcl", resources, 2, t_le), "eval_predicate_3 failed");
    fail_unless(!eval_predicate("../peos_init.tcl", resources, 2, t_ge), "eval_predicate_3 failed");
    system("echo \"1234\" > my_file0");
    system("echo \"12\" > my_file1");
    fail_unless(!eval_predicate("../peos_init.tcl", resources, 2, t_eq), "eval_predicate_3 failed");
    fail_unless(eval_predicate("../peos_init.tcl", resources, 2, t_ne), "eval_predicate_3 failed");
    fail_unless(!eval_predicate("../peos_init.tcl", resources, 2, t_lt), "eval_predicate_3 failed");
    fail_unless(eval_predicate("../peos_init.tcl", resources, 2, t_gt), "eval_predicate_3 failed");
    fail_unless(!eval_predicate("../peos_init.tcl", resources, 2, t_le), "eval_predicate_3 failed");
    fail_unless(eval_predicate("../peos_init.tcl", resources, 2, t_ge), "eval_predicate_3 failed");
    system("rm my_file0");
    system("rm my_file1");
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
    
    tc = tcase_create("eval_predicate_abstract_0");
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_eval_predicate_abstract_0);
    
    tc = tcase_create("eval_resource_list_0");
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_eval_resource_list_0);
    
    tc = tcase_create("eval_resource_list_1");
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_eval_resource_list_1);
    
    tc = tcase_create("eval_predicate_0");
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_eval_predicate_0);
    
    tc = tcase_create("eval_predicate_1");
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_eval_predicate_1);
    
    tc = tcase_create("eval_predicate_2");
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_eval_predicate_2);
    
    tc = tcase_create("eval_predicate_3");
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_eval_predicate_3);
    
    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    suite_free(s);
    //system("rm  tclf_*.tcl");	/* Clean up. */
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
