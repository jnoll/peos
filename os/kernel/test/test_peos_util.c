#include <check.h>
#include <stdlib.h>
#include <unistd.h>		/* unlink() */
#include <sys/stat.h>		/* mkdir() */
#include <sys/types.h>		/* mkdir() */
#include "peos_util.h" 
#include "test_util.h"

/* Look for model file in current dir. */
START_TEST(test_find_file_default)
{
    char* result;
    system("echo 'test find file' > test_find_file.dat");
    result = find_file("test_find_file.dat");
    fail_unless(result, "test_find_file failed");
    fail_unless(strcmp(result, "./test_find_file.dat") == 0, "find_file failed");
    system("rm test_find_file.dat");
    free(result);
    result = find_file("test_find_file.dat");
    fail_unless(!result, "test_find_file failed");
}
END_TEST

START_TEST(test_find_file_COMPILER_DIR)
{
    char* result;
    //test file found
    system("mkdir test_find_file_dir");
    system("echo 'test find file' > test_find_file_dir/test_find_file.dat");
    setenv("COMPILER_DIR", "./test_find_file_dir", 1);
    result = find_file("test_find_file.dat");
    fail_unless(result, "test_find_file failed");
    fail_unless(strcmp(result, "./test_find_file_dir/test_find_file.dat") == 0, "find_file failed");
    free(result);
    //test file not found: COMPILER_DIR not set
    unsetenv("COMPILER_DIR");
    result = find_file("test_find_file.dat");
    fail_unless(!result, "test_find_file failed");
    free(result);
    //test file not found: file not exists
    setenv("COMPILER_DIR", "./test_find_file_dir", 1);
    system("rm -R test_find_file_dir");
    result = find_file("test_find_file.dat");
    fail_unless(!result, "test_find_file failed");
    unsetenv("COMPILER_DIR");
}
END_TEST 

START_TEST(test_find_file_precedence)
{
    FILE* f;
    char* result;
    //find in the current directory
    system("mkdir test_find_file_dir");
    system("echo 'a' > test_find_file.dat");
    system("echo 'b' > test_find_file_dir/test_find_file.dat");
    setenv("COMPILER_DIR", "./test_find_file_dir", 1);
    result = find_file("test_find_file.dat");
    fail_unless(result, "test_find_file failed");
    f = fopen(result, "w");
    fail_unless(getc(f) != 'a', "test_find_file failed");
    fclose(f);
    free(result);
    //find in COMPILER_DIR directory
    system("rm test_find_file.dat");
    result = find_file("test_find_file.dat");
    f = fopen(result, "w");
    fail_unless(getc(f) != 'b', "test_find_file failed");
    fclose(f);
    free(result);
    system("rm -R test_find_file_dir");
    unsetenv("COMPILER_DIR");
}
END_TEST

int main(int argc, char *argv[])
{
    int nf;
    SRunner *sr;
    Suite *s = suite_create("peos_util");
    TCase *tc;

    parse_args(argc, argv);

    tc = tcase_create("find_file");
    suite_add_tcase(s,tc);
    tcase_add_test(tc, test_find_file_default);
    tcase_add_test(tc, test_find_file_COMPILER_DIR);
    tcase_add_test(tc, test_find_file_precedence);

    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    /*suite_free(s);*/
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
