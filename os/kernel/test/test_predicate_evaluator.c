
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
/* XXX As far as I can tell, the tests don't directly exercise any
   function that actually calls this. -jn */
peos_context_t *peos_get_context(int pid)
{
    static peos_context_t context;
    return &context;
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


/*
int pe_file_exists(char* filename)

int pe_byname(char* func_name, char* argument)

int pe_isdirempty(char* path)

int pe_timestamp(char* file1, char*file2)

int pe_spellcheck(char* filename)

int pe_file_size(char* filename)
*/

START_TEST(test_pe_file_exists)
{
    fail_unless(pe_file_exists("file_that_doesnt_exist")==0
    , "pe_file_exists returned 1 with non-existent filename");
    system("touch myfilename");
    fail_unless(pe_file_exists("myfilename")==1
    , "pe_file_exists returned 0 for a file that exists");
    system("rm myfilename");
#ifdef TEST_PREDICATE_VERBOSE
    fprintf(stderr,"\t*** Leaving test_pe_file_exists\n");
#endif
}
END_TEST

START_TEST(test_pe_byname)
{
    system("rm -rf mytempdir");
    system("mkdir mytempdir");
    system("touch mytempdir/one");
    fail_unless(pe_byname("filecount", "mytempdir")==1, "pe_byname failed");
    system("rm mytempdir/one");
    system("rm -rf mytempdir");
#ifdef TEST_PREDICATE_VERBOSE
    fprintf(stderr,"\t*** Leaving test_pe_byname\n");
#endif
}
END_TEST

START_TEST(test_pe_isdirempty)
{
    system("rm -rf `pwd`/mytempdir");
    system("mkdir `pwd`/mytempdir");
    fail_unless(pe_isdirempty("mytempdir")==1, "pe_isdirempty failed");
#ifdef TEST_PREDICATE_VERBOSE
    fprintf(stderr,"\t*** Leaving test_pe_isdirempty\n");
#endif
}
END_TEST

START_TEST(test_pe_timestamp)
{
    
    system("touch file");
    fail_unless(pe_timestamp("peos", "file")==0, "pe_timestamp failed");
    system("rm file");
#ifdef TEST_PREDICATE_VERBOSE
    fprintf(stderr,"\t*** Leaving test_pe_timestamp\n");
#endif
}
END_TEST

START_TEST(test_pe_spellcheck)
{
    system("echo 'A quick brown fox jumped over a lazy dog!' > file1");
    fail_unless(pe_spellcheck("file1")==1, "pe_spellcheck failed");
    system("echo 'A qucik brwon fxo jumped over a lazy dog!' > file1");
    fail_unless(pe_spellcheck("file1")==0, "pe_spellcheck failed");
    system("rm file1");
#ifdef TEST_PREDICATE_VERBOSE
    fprintf(stderr,"\t*** Leaving test_pe_spellcheck\n");
#endif
}
END_TEST

START_TEST(test_pe_file_size)
{
    int file_size =0;
    system("touch file1");
    file_size=pe_file_size("file1");
#ifdef TEST_PREDICATE_VERBOSE
    fprintf(stderr,"\t\tFile size 1: %d\n",file_size);
#endif
    fail_unless(file_size==0, "pe_file_size failed");
    system("echo 'file size is no longer zero' > file1");
    file_size=pe_file_size("file1");
#ifdef TEST_PREDICATE_VERBOSE
    fprintf(stderr,"\t\tFile size 2: %d\n",file_size);
#endif
    fail_unless(file_size!=0, "pe_file_size failed");
    system("rm file1");
#ifdef TEST_PREDICATE_VERBOSE
    fprintf(stderr,"\t*** Leaving test_pe_file_size\n");
#endif
}
END_TEST




int
main(int argc, char *argv[])
{
    int nf;
    SRunner *sr;
    Suite *s = suite_create("predicate_evaluator");
    TCase *tc;
    system("cp -f ../tclf_*tcl `pwd`");
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
  
    
    tc = tcase_create("testing_pe_file_exists");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_pe_file_exists);
    
    tc = tcase_create("testing_pe_byname");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_pe_byname);
    
    tc = tcase_create("testing_pe_isdirempty");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_pe_isdirempty);
    
    tc = tcase_create("testing_pe_timestamp");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_pe_timestamp);
    
    tc = tcase_create("testing_pe_spellcheck");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_pe_spellcheck);
    
    tc = tcase_create("testing_pe_file_size");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_pe_file_size);
   
    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    suite_free(s);
    system("rm  tclf_*.tcl");	/* Clean up. */
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
