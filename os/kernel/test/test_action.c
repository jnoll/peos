#include <check.h>
#include <stdlib.h>
#include <unistd.h>		/* unlink() */
#include "test_util.h"
#include "graph_engine.h"

/* Stub (bogus) globals. */



START_TEST(test_get_act_state)
{
	/* Pre: action list initialized. */
	peos_action_t *actions = make_actions(10, ACT_NONE);
	fail_unless(get_act_state("act_0",actions,10) == ACT_NONE, "act_0 not none");
	free_actions(actions,10);
}
END_TEST


START_TEST(test_get_act_state_middle)
{
	/* pre : actions list initialized */
	peos_action_t *actions = make_actions(10, ACT_NONE);
	fail_unless(get_act_state("act_5", actions, 10) == ACT_NONE, "act_5 not none");
	free_actions(actions, 10);
}
END_TEST


START_TEST(test_get_act_state_last)
{
	/* pre : actions list initialized */
	peos_action_t *actions = make_actions(10,ACT_NONE);
	fail_unless(get_act_state("act_9", actions, 10) == ACT_NONE, "act_9 not none");
	free_actions(actions,10);
}
END_TEST


START_TEST(test_get_act_state_none)
{
	peos_action_t *actions = make_actions(10, ACT_NONE);
	fail_unless(get_act_state("none",actions,10) == -1, "act none's state found");
	free_actions(actions,10);
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

    tc = tcase_create("get");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_get_act_state);
    tcase_add_test(tc,test_get_act_state_last);
    tcase_add_test(tc,test_get_act_state_middle);
    tcase_add_test(tc,test_get_act_state_none);

    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    suite_free(s);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
