#include <check.h>
#include <stdlib.h>
#include <unistd.h>		/* unlink() */
#include "test_util.h"

/* Stub (bogus) globals. */




START_TEST(test_find_actions)
{}
END_TEST

START_TEST(test_set_act_state)
{
    /* Pre: action list is initialized. */
    peos_action_t *actions = make_actions(10, ACT_NONE);
    
    /* Action. */
    set_act_state("act_0", ACT_READY, actions, 10);

    /* Post: specified action is in desired state. */
    fail_unless(actions[0].state == ACT_READY, "act_0 not READY");
    
    /* Action. */
    set_act_state("act_9", ACT_READY, actions, 10);

    /* Post: specified action is in desired state. */
    fail_unless(actions[9].state == ACT_READY, "act_5 not READY");
    
    /* Action. */
    set_act_state("act_5", ACT_READY, actions, 10);

    /* Post: specified action is in desired state. */
    fail_unless(actions[5].state == ACT_READY, "act_9 not READY");
    fail_unless(set_act_state("none", ACT_READY, actions, 10) == -1,
		"set action 'none' to READY");
    free_actions(actions, 10);
}
END_TEST


START_TEST(test_list_actions)
{
    peos_action_t *acts = 0, *p, *actions;

    /* Empty result. */
    /*  Pre: action list is initialized. */
    actions = make_actions(10, ACT_NONE);

    /* Action. */
    acts = peos_find_actions(ACT_READY, actions, 10);

    /* Post: list of actions in READY state. */
    for (p = acts; p->name; p++) {
	fail_unless(p->state == ACT_READY, NULL);
    }
    fail_unless((p - acts) == 0, "action count wrong");

    /* One ready, at beginning. */
    /* Pre: action list is initialized. */
    actions[0].state = ACT_READY;

    /* Action. */
    acts = peos_find_actions(ACT_READY, actions, 10);

    /* Post: one action in READY state. */
    for (p = acts; p->name; p++) {
	fail_unless(p->state == ACT_READY, NULL);
    }
    fail_unless((p - acts) == 1, "action count wrong");


    /* Two ready, beginning and end. */
    /* Pre: action list is initialized. */
    actions[9].state = ACT_READY;

    /* Action. */
    acts = peos_find_actions(ACT_READY, actions, 10);

    /* Post: two actions in READY state. */
    for (p = acts; p->name; p++) {
	fail_unless(p->state == ACT_READY, NULL);
    }
    fail_unless((p - acts) == 2, "action count wrong");

    /* Three ready, including middle. */
    /* Pre: action list is initialized. */
    actions[5].state = ACT_READY;

    /* Action. */
    acts = peos_find_actions(ACT_READY, actions, 10);

    /* Post: three actions in READY state. */
    for (p = acts; p->name; p++) {
	fail_unless(p->state == ACT_READY, NULL);
    }
    fail_unless((p - acts) == 3, "action count wrong");

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

    tc = tcase_create("find");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_find_actions);

    tc = tcase_create("list");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_list_actions);


    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    suite_free(s);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
