#include <check.h>
#include <stdlib.h>
#include <unistd.h>		/* unlink() */
#include "test_util.h"

/* Stub (bogus) globals. */




START_TEST(test_set_act_state)
{
    /* Pre: action list is initialized. */
    peos_action_t *actions = make_actions(10, ACT_NONE);
    
    /* Action. */
    set_act_state("act_0", ACT_READY, actions, 10);

    /* Post: specified action is in desired state. */
    fail_unless(actions[0].state == ACT_READY, "act_0 not READY");
    free_actions(actions, 10);
}
END_TEST
    
START_TEST(test_set_act_state_last)
{
    /* Pre: action list is initialized. */
    peos_action_t *actions = make_actions(10, ACT_NONE);
    
    /* Action. */
    set_act_state("act_9", ACT_READY, actions, 10);

    /* Post: specified action is in desired state. */
    fail_unless(actions[9].state == ACT_READY, "act_9 not READY");
    free_actions(actions, 10);
}
END_TEST
    
START_TEST(test_set_act_state_middle)
{
    /* Pre: action list is initialized. */
    peos_action_t *actions = make_actions(10, ACT_NONE);
    
    /* Action. */
    set_act_state("act_5", ACT_READY, actions, 10);

    /* Post: specified action is in desired state. */
    fail_unless(actions[5].state == ACT_READY, "act_5 not READY");
    free_actions(actions, 10);
}
END_TEST

START_TEST(test_set_act_state_none)
{
    /* Pre: action list is initialized. */
    peos_action_t *actions = make_actions(10, ACT_NONE);
    fail_unless(set_act_state("none", ACT_READY, actions, 10) == -1,
		"set action 'none' to READY");
    free_actions(actions, 10);
}
END_TEST





int
main(int argc, char *argv[])
{
    int nf;
    SRunner *sr;
    TCase *tc;
    Suite *s = suite_create("action");

    parse_args(argc, argv);

    tc = tcase_create("set");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_set_act_state);
    tcase_add_test(tc, test_set_act_state_last);
    tcase_add_test(tc, test_set_act_state_middle);
    tcase_add_test(tc, test_set_act_state_none);


    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    suite_free(s);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
