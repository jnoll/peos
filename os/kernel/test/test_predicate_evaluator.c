#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <check.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../predicate_evaluator.h"
#undef TEST_PREDICATE_VERBOSE
#include "test_util.h"

/* Globals. */
peos_resource_t *global_resources;
int global_num_resources;
char* global_tcl_file;
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

char* find_file(char* file) {
    return global_tcl_file;
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
    free(resources);
}
END_TEST

START_TEST(test_gen_expr)
{
    Tree t_and, t_or, t_eq, t_ne, t_lt, t_gt, t_le, t_ge, t_dot0, t_dot1,  t_res0, t_res1, t_attr0, t_attr1, t_true, t_string;
    Tcl_DString buf;
    char *e;

    t_true = make_tree("\"true\"", 0, NULL, NULL);
    t_string = make_tree("\"a string\"", 0, NULL, NULL);
    t_res0 = make_tree("res0", 0, NULL, NULL);
    t_attr0 = make_tree("filesize", 0, NULL, NULL);
    t_dot0 = make_tree(NULL, DOT, t_res0, t_attr0);
    
    t_res1 = make_tree("res1", 0, NULL, NULL);
    t_attr1 = make_tree("filesize", 0, NULL, NULL);
    t_dot1 = make_tree(NULL, DOT, t_res1, t_attr1);
    
    t_eq = make_tree(NULL, EQ, t_dot0, t_dot1); // res0.filesize == res1.filesize
    t_ne = make_tree(NULL, NE, t_dot0, t_dot1); // res0.filesize != res1.filesize
    t_lt = make_tree(NULL, LT, t_dot0, t_dot1); // res0.filesize < res1.filesize
    t_gt = make_tree(NULL, GT, t_dot0, t_dot1); // res0.filesize > res1.filesize
    t_le = make_tree(NULL, LE, t_dot0, t_dot1); // res0.filesize <= res1.filesize
    t_ge = make_tree(NULL, GE, t_dot0, t_dot1); // res0.filesize >= res1.filesize

    t_and = make_tree(NULL, AND, t_eq, t_ne);
    t_or = make_tree(NULL, OR, t_eq, t_ne);

    Tcl_DStringInit(&buf);
    e = gen_expr(t_res0, &buf); 
    fail_unless(strcmp(e, "${res0}") == 0, e);

    Tcl_DStringFree(&buf); Tcl_DStringInit(&buf); 
    e = gen_expr(t_dot0, &buf);
    fail_unless(strcmp(e, "[filesize ${res0}]") == 0, e);

    Tcl_DStringFree(&buf); Tcl_DStringInit(&buf); 
    e = gen_expr(t_eq, &buf);
    fail_unless(strcmp(e, "([filesize ${res0}] == [filesize ${res1}])") == 0, e);

    Tcl_DStringFree(&buf); Tcl_DStringInit(&buf); 
    e = gen_expr(t_ne, &buf);
    fail_unless(strcmp(e, "([filesize ${res0}] != [filesize ${res1}])") == 0, e);

    Tcl_DStringFree(&buf); Tcl_DStringInit(&buf); 
    e = gen_expr(t_lt, &buf);
    fail_unless(strcmp(e, "([filesize ${res0}] < [filesize ${res1}])") == 0, e);

    Tcl_DStringFree(&buf); Tcl_DStringInit(&buf); 
    e = gen_expr(t_gt, &buf);
    fail_unless(strcmp(e, "([filesize ${res0}] > [filesize ${res1}])") == 0, e);

    Tcl_DStringFree(&buf); Tcl_DStringInit(&buf); 
    e = gen_expr(t_le, &buf);
    fail_unless(strcmp(e, "([filesize ${res0}] <= [filesize ${res1}])") == 0, e);

    Tcl_DStringFree(&buf); Tcl_DStringInit(&buf); 
    e = gen_expr(t_ge, &buf);
    fail_unless(strcmp(e, "([filesize ${res0}] >= [filesize ${res1}])") == 0, e);

    Tcl_DStringFree(&buf); Tcl_DStringInit(&buf); 
    e = gen_expr(t_and, &buf);
    fail_unless(strcmp(e, "(([filesize ${res0}] == [filesize ${res1}]) && ([filesize ${res0}] != [filesize ${res1}]))") == 0, e);

    Tcl_DStringFree(&buf); Tcl_DStringInit(&buf); 
    e = gen_expr(t_or, &buf);
    fail_unless(strcmp(e, "(([filesize ${res0}] == [filesize ${res1}]) || ([filesize ${res0}] != [filesize ${res1}]))") == 0, e);

    Tcl_DStringFree(&buf); Tcl_DStringInit(&buf); 
    e = gen_expr(make_tree(NULL, EQ, t_dot0, t_true), &buf);
    fail_unless(strcmp(e, "[isTrue [filesize ${res0}]]") == 0, e);

    Tcl_DStringFree(&buf); Tcl_DStringInit(&buf); 
    e = gen_expr(make_tree(NULL, EQ, t_dot0, t_string), &buf);
    fail_unless(strcmp(e, "([filesize ${res0}] == \"a string\")") == 0, e);



    Tcl_DStringFree(&buf);
}
END_TEST

START_TEST(test_get_eval_result_error)
{
    int error;
    fail_unless(get_eval_result("not_exist_procedure", "res", &error) == 0, "get_eval_result_error failed");
    fail_unless(error, "get_eval_result_error failed");
    strcpy(global_tcl_file, "./../not_exist.tcl");
    fail_unless(get_eval_result("exists", "res", &error) == 0, "get_eval_result_error failed");
    fail_unless(error, "get_eval_result_error failed");
    strcpy(global_tcl_file, "./../peos_init.tcl");
}
END_TEST

START_TEST(test_get_eval_result_exists)
{
    int error;
    fail_unless(get_eval_result("exists", "$res", &error) == 0, "get_eval_result_exists failed");
    fail_unless(!error, "get_eval_result_error failed");
    system("touch my_file");
    fail_unless(get_eval_result("exists", "my_file", &error), "get_eval_result_exists failed");
    fail_unless(!error, "get_eval_result_error failed");
    system("rm my_file");
    fail_unless(!get_eval_result("exists", "my_file", &error), "get_eval_result_exists failed");
    fail_unless(!error, "get_eval_result_error failed");
}
END_TEST

START_TEST(test_get_eval_result_filecount)
{
    int error;
    fail_unless(get_eval_result("filecount", "$res", &error) == 0, "get_eval_result_filecount failed");
    fail_unless(!error, "get_eval_result_error failed");
    system("rm -rf /tmp/my_dir");
    system("mkdir /tmp/my_dir");
    fail_unless(get_eval_result("filecount", "/tmp/my_dir", &error) == 0, "get_eval_result_filecount failed");
    fail_unless(!error, "get_eval_result_error failed");
    system("touch /tmp/my_dir/my_file_1");
    fail_unless(get_eval_result("filecount", "/tmp/my_dir", &error) == 1, "get_eval_result_filecount failed");
    fail_unless(!error, "get_eval_result_error failed");
    system("touch /tmp/my_dir/my_file_2");
    fail_unless(get_eval_result("filecount", "/tmp/my_dir", &error) == 2, "get_eval_result_filecount failed");
    fail_unless(!error, "get_eval_result_error failed");
    system("rm -rf /tmp/my_dir");
    fail_unless(get_eval_result("filecount", "not_exist_dir", &error) == 0, "get_eval_result_filecount failed");
    fail_unless(!error, "get_eval_result_error failed");
}
END_TEST

START_TEST(test_get_eval_result_filesize)
{
    int error;
    long before, after;
    fail_unless(get_eval_result("filesize", "$res", &error) == 0, "get_eval_result_filesize failed");
    fail_unless(!error, "get_eval_result_error failed");
    system("echo abc > my_file");
    before = get_eval_result("filesize", "my_file", &error);
    fail_unless(!error, "get_eval_result_error failed");
    system("echo abcdef > my_file");
    after = get_eval_result("filesize", "my_file", &error);
    fail_unless(!error, "get_eval_result_error failed");
    system("rm my_file");
    fail_unless(before < after, "get_eval_result_filesize failed");
    fail_unless(get_eval_result("filesize", "not_exist_file", &error) == 0, "get_eval_result_filesize failed");
    fail_unless(!error, "get_eval_result_error failed");
}
END_TEST

START_TEST(test_get_eval_result_timestamp)
{
    int error;
    long before, after;
    fail_unless(get_eval_result("timestamp", "$res", &error) == 0, "get_eval_result_timestamp failed");
    fail_unless(!error, "get_eval_result_error failed");
    system("touch -t 200201311759.30 my_file");  //year is 2002
    before = get_eval_result("timestamp", "my_file", &error);
    fail_unless(!error, "get_eval_result_error failed");
    system("touch -t 200301311759.30 my_file"); //change year to 2003
    after = get_eval_result("timestamp", "my_file", &error);
    fail_unless(!error, "get_eval_result_error failed");
    system("rm my_file");
    fail_unless(before < after, "get_eval_result_timestamp failed");
    fail_unless(get_eval_result("timestamp", "not_exists_file", &error) == 0, "get_eval_result_timestamp failed");
    fail_unless(!error, "get_eval_result_error failed");
}
END_TEST

START_TEST(test_get_eval_result_misspellcount)
{
    int error;
    fail_unless(get_eval_result("misspellcount", "$res", &error) == 0, "get_eval_result_misspellcount failed");
    fail_unless(!error, "get_eval_result_error failed");
    system("touch my_file");
    fail_unless(get_eval_result("misspellcount", "my_file", &error) == 0, "get_eval_result_misspellcount failed");
    fail_unless(!error, "get_eval_result_error failed");
    system("echo man > my_file");
    fail_unless(get_eval_result("misspellcount", "my_file", &error) == 0, "get_eval_result_misspellcount failed");
    fail_unless(!error, "get_eval_result_error failed");
    system("echo man aabb > my_file");
    fail_unless(get_eval_result("misspellcount", "my_file", &error) == 1, "get_eval_result_misspellcount failed");
    fail_unless(!error, "get_eval_result_error failed");
    system("echo ccdd man aabb > my_file");
    fail_unless(get_eval_result("misspellcount", "my_file", &error) == 2, "get_eval_result_misspellcount failed");
    fail_unless(!error, "get_eval_result_error failed");
    system("rm my_file");
    fail_unless(get_eval_result("misspellcount", "not_exists_file", &error) == 0, "get_eval_result_misspellcount failed");
    fail_unless(!error, "get_eval_result_error failed");
}
END_TEST

START_TEST(test_eval_resource_list_error)
{
    peos_resource_t* resources;
    fail_unless(eval_resource_list(&resources, 1) == 0, "eval_resource_list_error failed");
    resources = (peos_resource_t*)calloc(1, sizeof(peos_resource_t));
    strcpy(resources[0].name, "res0");
    fail_unless(eval_resource_list(&resources, 1) == 1, "eval_resource_list_error failed");
    fail_unless(eval_resource_list(&resources, 0) == 0, "eval_resource_list_error failed");
    strcpy(resources[0].name, "");
    fail_unless(eval_resource_list(&resources, 1) == 0, "eval_resource_list_error failed");
}
END_TEST

START_TEST(test_eval_resource_list_bound) {
    peos_resource_t* resources = (peos_resource_t*)calloc(3, sizeof(peos_resource_t));
    
    strcpy(resources[0].name, "res0");
    strcpy(resources[0].value, "val0");
    strcpy(resources[1].name, "res1");
    strcpy(resources[1].value, "$res0/val1");
    strcpy(resources[2].name, "res2");
    strcpy(resources[2].value, "${res1}/val2");
    
    fail_unless(eval_resource_list(&resources, 3) == 1, "eval_resource_list_bound failed");
    
    fail_unless(strcmp(resources[0].value, "val0") == 0, "eval_resource_list_bound failed");
    fail_unless(strcmp(resources[1].value, "val0/val1") == 0, "eval_resource_list_bound failed");
    fail_unless(strcmp(resources[2].value, "val0/val1/val2") == 0, "eval_resource_list_bound failed");
    free(resources);
}
END_TEST
        
START_TEST(test_eval_resource_list_unbound) {
    peos_resource_t* resources = (peos_resource_t *) calloc(3, sizeof(peos_resource_t));
    strcpy(resources[0].name, "res0");  //res0 is unbound
    strcpy(resources[1].name, "res1");
    strcpy(resources[1].value, "$res0/val1");
    strcpy(resources[2].name, "res2");
    strcpy(resources[2].value, "${res1}/val2");
    
    fail_unless(eval_resource_list(&resources, 3) == 1, "eval_resource_list_unbound failed");
    
    fail_unless(strcmp(resources[0].value, "${res0}") == 0, "eval_resource_list_unbound failed");
    fail_unless(strcmp(resources[1].value, "${res0}/val1") == 0, "eval_resource_list_unbound failed");
    fail_unless(strcmp(resources[2].value, "${res0}/val1/val2") == 0, "eval_resource_list_unbound failed");
    free(resources);
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
    
    fail_unless(eval_predicate(resources, 1, t_res) == 1, "eval_predicate_abstract_0 failed");
    free(resources);
}
END_TEST

START_TEST(test_eval_predicate_null_tree)
{
    fail_unless(eval_predicate(NULL, 0, NULL) == 1, "eval_predicate_null_tree failed");
}
END_TEST

START_TEST(test_eval_predicate_error)
{

    peos_resource_t* resources = NULL;
    Tree t_dot, t_op, t0, t1, t_true;
    t0 = make_tree("res", 0, NULL, NULL);
    //test resources == NULL
    fail_unless(eval_predicate(resources, 0, t0) == -1, "null resource list failed");
    //test num_resources == 0
    resources = (peos_resource_t *) calloc(1, sizeof(peos_resource_t));
    fail_unless(eval_predicate(resources, 0, t0) == -1, "empty resource list failed");
    //resource name not found
    t1 = make_tree("exists", 0, NULL, NULL);
    t_dot = make_tree(NULL, DOT, t0, t1);
    strcpy(resources[0].name, "res_not_in_tree");
    strcpy(resources[0].value, "res_not_in_tree");
    fail_unless(eval_predicate(resources, 1, t_dot) == -1, "unbound resource failed");

    //test tcl attr procedure not exists
    strcpy(resources[0].name, "res");
    strcpy(resources[0].value, "val");
    free(t1);
    free(t_dot);
    t1 = make_tree("not_exist_tcl_proc", 0, NULL, NULL);
    t_dot = make_tree(NULL, DOT, t0, t1);
    fail_unless(eval_predicate(resources, 1, t_dot) == -1, "missing tcl attr procedure failed");

    //test operators that operate on error tree
    t_true = make_tree("\"True\"", 0, NULL, NULL);
    t_op = make_tree(NULL, EQ, t_true, t_dot);
    fail_unless(eval_predicate(resources, 1, t_op) == -1, "missing tcl attr procedure EQ failed");
    t_op->ival = NE;
    fail_unless(eval_predicate(resources, 1, t_op) == -1, "missing tcl attr procedure NE failed");
    t_op->ival = LT;
    fail_unless(eval_predicate(resources, 1, t_op) == -1, "missing tcl attr procedure LT failed");
    t_op->ival = GT;
    fail_unless(eval_predicate(resources, 1, t_op) == -1, "missing tcl attr procedure GT failed");
    t_op->ival = LE;
    fail_unless(eval_predicate(resources, 1, t_op) == -1, "missing tcl attr procedure LE failed");
    t_op->ival = GE;
    fail_unless(eval_predicate(resources, 1, t_op) == -1, "missing tcl attr procedure GE failed");
    t_op->ival = AND;
    fail_unless(eval_predicate(resources, 1, t_op) == -1, "missing tcl attr procedure AND failed");
    t_op->ival = OR;
    /* "True" || res.not_exist_tcl_proc is true */
    fail_unless(eval_predicate(resources, 1, t_op) == 1, "missing tcl attr procedure OR failed");
    free(resources);
}
END_TEST

START_TEST(test_eval_predicate_single_node)
{
    peos_resource_t* resources;
    Tree t;
    
    resources = (peos_resource_t *) calloc(1, sizeof(peos_resource_t));
    strcpy(resources[0].name, "res");
    strcpy(resources[0].value, "my_file");
    

    t = make_tree("\"True\"", 0, NULL, NULL);
    fail_unless(eval_predicate(NULL, 0, t) == 1, "\"True\" failed");
    free(t);

    t = make_tree("\"true\"", 0, NULL, NULL); 
    fail_unless(eval_predicate(NULL, 0, t) == 1, "\"true\" failed");
    free(t);

    t = make_tree("\"1\"", 0, NULL, NULL);
    fail_unless(eval_predicate(NULL, 0, t) == 1, "\"1\" failed");
    free(t);

    t = make_tree("\"False\"", 0, NULL, NULL);
    fail_unless(eval_predicate(NULL, 0, t) == 0, "\"False\" failed");
    free(t);

    t = make_tree("\"false\"", 0, NULL, NULL);
    fail_unless(eval_predicate(NULL, 0, t) == 0, "\"false\" failed");
    free(t);

    t = make_tree("\"0\"", 0, NULL, NULL);
    fail_unless(eval_predicate(NULL, 0, t) == 0, "\"0\" failed");
    free(t);

    t = make_tree("res", 0, NULL, NULL);
    system("touch my_file");
    fail_unless(eval_predicate(resources, 1, t) == 1, "[exists ${res}] (= my_file) failed");
    system("rm my_file");
    fail_unless(eval_predicate(resources, 1, t) == 0, "not [exists ${res}] (= my_file0) failed");
    free(t);    

    free(resources);
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
    
    /* Happy path: requires { res.exists } */
    system("touch my_file");
    fail_unless(eval_predicate(resources, 1, t_dot) == 1, "eval_predicate_dot 1 failed");
    system("rm my_file");
    fail_unless(eval_predicate(resources, 1, t_dot) == 0, "eval_predicate_dot 2 failed");


    /* requires { doc.title } -> [isTrue [title ${doc}]] */
    /* requires { doc.spellchecked } -> [isTrue [spellchecked ${doc}] */
    /* requires { doc.spellchecked == "True" } [spellchecked ${doc}] == true */

    free(resources);
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
    
    system("touch my_file0");
    system("touch my_file1");
    fail_unless(eval_predicate(resources, 2, t_and) == 1, "res0 AND res1 (both exist) failed");
    fail_unless(eval_predicate(resources, 2, t_or) == 1, "res0 OR res1 (both exist) failed");
    system("rm my_file0");
    fail_unless(eval_predicate(resources, 2, t_and) == 0, "res0 AND res1 (res0 doesn't exist) failed");
    fail_unless(eval_predicate(resources, 2, t_or) == 1, "res0 OR res1 (res0 doesn't exist) failed");
    system("rm my_file1");
    fail_unless(eval_predicate(resources, 2, t_and) == 0, "res0 AND res1 (neither exists) failed");
    fail_unless(eval_predicate(resources, 2, t_or) == 0, "res0 OR res1 (neither exists) failed");
    free(resources);
}
END_TEST
        
START_TEST(test_eval_predicate_eq_ne_gt_le_ge)
{
    peos_resource_t* resources;
    Tree t_eq, t_ne, t_lt, t_gt, t_le, t_ge, t_dot0, t_dot1, t_res0, t_res1, t_attr0, t_attr1;
    
    resources = (peos_resource_t *) calloc(2, sizeof(peos_resource_t));
    strcpy(resources[0].name, "res0");
    strcpy(resources[0].value, "my_file0");
    strcpy(resources[1].name, "res1");
    strcpy(resources[1].value, "my_file1");

    t_res0 = make_tree("res0", 0, NULL, NULL);
    t_attr0 = make_tree("filesize", 0, NULL, NULL);
    t_dot0 = make_tree(NULL, DOT, t_res0, t_attr0);
    
    t_res1 = make_tree("res1", 0, NULL, NULL);
    t_attr1 = make_tree("filesize", 0, NULL, NULL);
    t_dot1 = make_tree(NULL, DOT, t_res1, t_attr1);
    
    t_eq = make_tree(NULL, EQ, t_dot0, t_dot1); // res0.filesize == res1.filesize
    t_ne = make_tree(NULL, NE, t_dot0, t_dot1); // res0.filesize != res1.filesize
    t_lt = make_tree(NULL, LT, t_dot0, t_dot1); // res0.filesize < res1.filesize
    t_gt = make_tree(NULL, GT, t_dot0, t_dot1); // res0.filesize > res1.filesize
    t_le = make_tree(NULL, LE, t_dot0, t_dot1); // res0.filesize <= res1.filesize
    t_ge = make_tree(NULL, GE, t_dot0, t_dot1); // res0.filesize >= res1.filesize
    
    system("touch my_file0");
    system("touch my_file1");
    fail_unless(eval_predicate(resources, 2, t_eq)  == 1, "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(eval_predicate(resources, 2, t_ne) == 0, "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(eval_predicate(resources, 2, t_lt) == 0, "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(eval_predicate(resources, 2, t_gt) == 0, "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(eval_predicate(resources, 2, t_le) == 1, "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(eval_predicate(resources, 2, t_ge) == 1, "eval_predicate_eq_ne_gt_le_ge failed");
    system("echo \"12\" > my_file0");
    system("echo \"1234\" > my_file1");
    fail_unless(eval_predicate(resources, 2, t_eq) == 0, "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(eval_predicate(resources, 2, t_ne) == 1, "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(eval_predicate(resources, 2, t_lt) == 1, "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(eval_predicate(resources, 2, t_gt) == 0, "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(eval_predicate(resources, 2, t_le) == 1, "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(eval_predicate(resources, 2, t_ge) == 0, "eval_predicate_eq_ne_gt_le_ge failed");
    system("echo \"1234\" > my_file0");
    system("echo \"12\" > my_file1");
    fail_unless(eval_predicate(resources, 2, t_eq) == 0, "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(eval_predicate(resources, 2, t_ne) == 1, "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(eval_predicate(resources, 2, t_lt) == 0, "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(eval_predicate(resources, 2, t_gt) == 1, "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(eval_predicate(resources, 2, t_le) == 0, "eval_predicate_eq_ne_gt_le_ge failed");
    fail_unless(eval_predicate(resources, 2, t_ge) == 1, "eval_predicate_eq_ne_gt_le_ge failed");
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

    parse_args(argc, argv);
    
    global_tcl_file = strdup("./../peos_init.tcl");
    
    tc = tcase_create("get_resource_index");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_get_resource_index);
    
    tc = tcase_create("gen_expr");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_gen_expr);
       
    tc = tcase_create("get_eval_result_error");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_get_eval_result_error);
    
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
    
    tc = tcase_create("eval_resource_list_error");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_eval_resource_list_error);
    
    tc = tcase_create("eval_resource_list_bound");
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_eval_resource_list_bound);
    
    tc = tcase_create("eval_resource_list_unbound");
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_eval_resource_list_unbound);
    
    tc = tcase_create("eval_predicate_error");
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_eval_predicate_error);
    
    tc = tcase_create("eval_predicate_abstract");
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_eval_predicate_abstract);
    
    tc = tcase_create("eval_predicate_null_tree");
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_eval_predicate_null_tree);
    
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
    /*suite_free(s);*/
    if (global_tcl_file)
        free(global_tcl_file);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
