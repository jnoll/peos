
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
static peos_context_t* global_context;

/* Stubs. */
/* XXX As far as I can tell, the tests don't directly exercise any
   function that actually calls this. -jn */
peos_context_t *peos_get_context(int pid)
{
    //static peos_context_t context;
    //return &context;
    
    return &global_context;
    
}

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
#ifdef TEST_PREDICATE_VERBOSE
    fprintf(stderr,"\n\t*** Leaving test_is_provides_true\n");
#endif
   
}
END_TEST
	
	
START_TEST(test_is_requires_true)
{

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
#ifdef TEST_PREDICATE_VERBOSE
    fprintf(stderr,"\t*** Leaving test_is_requires_true\n");
#endif
   
}
END_TEST
	

START_TEST(test_is_requires_true_1)
{

    peos_resource_t *resources = (peos_resource_t *) calloc(1, sizeof(peos_resource_t));

    strcpy(resources[0].name, "resource_0");
    strcpy(resources[0].value, "$$");
    strcpy(resources[0].qualifier, "abstract");

    global_resources = resources;
    global_num_resources = 1;

    fail_unless(is_requires_true(0, "some_name") == 1, "is_requires_true failed");
    if(resources) free(resources);

    unlink("resource_file");
    
#ifdef TEST_PREDICATE_VERBOSE
    fprintf(stderr,"\t*** Leaving test_is_requires_true_1\n");
#endif
}
END_TEST
	

START_TEST(test_is_provides_true_1)
{
    peos_resource_t *resources = (peos_resource_t *) calloc(1, sizeof(peos_resource_t));

    strcpy(resources[0].name, "resource_0");
    strcpy(resources[0].value, "$$");
    strcpy(resources[0].qualifier, "abstract");

    global_resources = resources;
    global_num_resources = 1;

    fail_unless(is_provides_true(0, "some_name") == 1, "is_provides_true failed");
    if(resources) free(resources);

    unlink("resource_file");
#ifdef TEST_PREDICATE_VERBOSE
    fprintf(stderr,"\t*** Leaving test_is_provides_true_1\n");
#endif
}
END_TEST
	
    
START_TEST(test_is_requires_true_2)
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

    fail_unless(is_requires_true(0, "some_name") == 0, "is_requires_true 2  failed");

    if(resources) free(resources);

    unlink("resource_file");
#ifdef TEST_PREDICATE_VERBOSE
    fprintf(stderr,"\t*** Leaving test_is_requires_true_2\n");
#endif  
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
#ifdef TEST_PREDICATE_VERBOSE
    fprintf(stderr,"\t*** Leaving test_is_provides_true_2\n");
#endif
}
END_TEST

START_TEST(test_contains_char)
{
    char* str = "abc";
    fail_unless(contains_char(str, 'a'), "contains_char failed");
    fail_unless(contains_char(str, 'b'), "contains_char failed");
    fail_unless(contains_char(str, 'c'), "contains_char failed");
    fail_unless(!contains_char(str, 'd'), "contains_char failed");
}
END_TEST

START_TEST(test_pe_get_resval)
{
    global_context = (peos_context_t*) malloc(sizeof(peos_context_t));
    global_context->resources = (peos_resource_t *) calloc(1, sizeof(peos_resource_t));
    strcpy(global_context->resources[0].name, "res1");
    strcpy(global_context->resources[0].value, "val1");
    //fail_unless(strcmp(pe_get_resval(0, "res1"), "val1") == 0, "pe_get_resval failed");
}
END_TEST

START_TEST(test_get_eval_result_exists)
{
    system("touch my_file");
    fail_unless(get_eval_result("./../../../os/kernel/peos_init.tcl", "exists", "my_file"), "get_eval_result_exists failed");
    system("rm my_file");
    fail_unless(!get_eval_result("./../../../os/kernel/peos_init.tcl", "exists", "my_file"), "get_eval_result_exists failed");
}
END_TEST
        
START_TEST(test_get_eval_result_filecount)
{
    system("rm -rf /tmp/my_dir");
    system("mkdir /tmp/my_dir");
    fail_unless(get_eval_result("./../../../os/kernel/peos_init.tcl", "filecount", "/tmp/my_dir") == 0, "get_eval_result_filecount failed");
    system("touch /tmp/my_dir/my_file_1");
    fail_unless(get_eval_result("./../../../os/kernel/peos_init.tcl", "filecount", "/tmp/my_dir") == 1, "get_eval_result_filecount failed");
    system("touch /tmp/my_dir/my_file_2");
    fail_unless(get_eval_result("./../../../os/kernel/peos_init.tcl", "filecount", "/tmp/my_dir") == 2, "get_eval_result_filecount failed");
    system("rm -rf /tmp/my_dir");
}
END_TEST
        
START_TEST(test_get_eval_result_filesize)
{
    FILE *fp;
    long before, after;
    fp = fopen("my_file", "w");
    fprintf(fp, "some text\n");
    fclose(fp);
    before = get_eval_result("./../../../os/kernel/peos_init.tcl", "filesize", "my_file");
    fp = fopen("my_file", "w");
    fprintf(fp, "some more text\n");
    fclose(fp);
    after = get_eval_result("./../../../os/kernel/peos_init.tcl", "filesize", "my_file");
    system("rm my_file");
    fail_unless(before < after, "get_eval_result_filesize failed");
}
END_TEST

START_TEST(test_get_eval_result_timestamp)
{
    long before, after;
    system("touch -t 200201311759.30 my_file");  //year is 2002
    before = get_eval_result("./../../../os/kernel/peos_init.tcl", "timestamp", "my_file");
    system("touch -t 200301311759.30 my_file"); //change year to 2003
    after = get_eval_result("./../../../os/kernel/peos_init.tcl", "timestamp", "my_file");
    system("rm my_file");
    fail_unless(before < after, "get_eval_result_timestamp failed");
}
END_TEST

START_TEST(test_get_eval_result_misspellcount)
{
    FILE* fp;
    fp = fopen("my_file", "w");
    fprintf(fp, "no error\n");
    fclose(fp);
    fail_unless(get_eval_result("./../../../os/kernel/peos_init.tcl", "misspellcount", "my_file") == 0, "get_eval_result_misspellcount failed");
    fp = fopen("my_file", "w");
    fprintf(fp, "aabb\n");
    fclose(fp);
    fail_unless(get_eval_result("./../../../os/kernel/peos_init.tcl", "misspellcount", "my_file") == 1, "get_eval_result_misspellcount failed");
    fp = fopen("my_file", "w");
    fprintf(fp, "ccdd\n");
    fclose(fp);
    system("rm my_file");
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
    
    tc = tcase_create("contains_char");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_contains_char);
    
    tc = tcase_create("pe_get_resval");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_pe_get_resval);  
    
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
    
    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    suite_free(s);
    //system("rm  tclf_*.tcl");	/* Clean up. */
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
