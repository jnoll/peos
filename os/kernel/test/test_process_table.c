#include <unistd.h>		/* unlink() */
#include <sys/stat.h>		/* mkdir() */
#include <sys/types.h>		/* mkdir() */
#include <check.h>
#include "graph.h"		/* sanitize_node() */
#include "process_table.h"
#define PROCESS_TABLE
#include "test_util.h"


/* Stubs */
extern Graph stub_makegraph(char *file);
extern Node make_node(char *name, vm_act_state state, int type,int order);


char *get_script_graph(Graph g, char *act_name)
{
    return "script";
}

void initialize_graph(Graph g)
{
    Node n;

    for(n = g -> source; n != NULL; n = n -> next) {
	n -> data = (void *) malloc (sizeof (struct data));
	sanitize_node(n);
    }
}


START_TEST(test_get_pid)
{
    int pid;
    peos_context_t *context = &(process_table[0]);
    pid = peos_get_pid(context);
    fail_unless(pid == 0, "pid");
}
END_TEST

START_TEST(test_get_pid_last)
{
    int pid;
    peos_context_t *context = &(process_table[PEOS_MAX_PID]);
    pid = peos_get_pid(context);
    fail_unless(pid == PEOS_MAX_PID, "pid");
}
END_TEST

START_TEST(test_get_lock)
{
    struct flock lck,lck1;
    int get_lock_status = 0;

    FILE *old_stderr;

    int fd,fd1;

    pid_t pid;
    int status, died;

    
    lck.l_type = F_WRLCK;
    lck.l_whence = 0;
    lck.l_start = 0;
    lck.l_len = 0; 

    fd = open("some_file", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    fcntl(fd, F_SETLK, &lck);

    switch(pid=fork()) {
            case -1: fprintf(stderr, "Cannot Complete Tests\n");
                     exit(-1);
		     break;
		     
            case 0 :
		     /* redirect stderr to suppress lock error messages. */
		     if (verbosity != CK_VERBOSE) {
		         old_stderr = stderr;
		         stderr = fopen("/dev/null", "w");
		     }
		     		     
		     lck1.l_type = F_WRLCK; 
                     lck1.l_whence = 0;
	             lck1.l_start = 0;
		     lck1.l_len = 0; 
		
                     fd1 = open("some_file", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
		     get_lock_status = get_lock(fd1);

		      /* restore stderr. */
		     if (verbosity != CK_VERBOSE) {
		         fclose(stderr);
		         stderr = old_stderr;
		     }

                     if ((get_lock_status == -1) && (errno == EAGAIN || errno == EACCES))
			     exit(0);
		     else
			    exit(EXIT_FAILURE); 
		     break;
		     
            default:  	     
		     died= wait(&status); // this is the code the parent runs 
		     /* fail unless child exited with status 0 */
		     fail_unless(status == 0, "Access to locked file");
		     
    }
	
    close(fd);
    close(fd1);   
    unlink("some_file");
   
}
END_TEST
   
				    

START_TEST(test_release_lock)
{
    struct flock lck,lck1;

    int fd,fd1;

    pid_t pid;
    int status, died;
    
    lck.l_type = F_WRLCK;
    lck.l_whence = 0;
    lck.l_start = 0;
    lck.l_len = 0; 

    fd = open("some_file", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    fcntl(fd, F_SETLK, &lck);
    release_lock(fd);

    
    switch(pid=fork()) {
            case -1: fprintf(stderr, "Cannot Complete Tests\n");
                     exit(-1);
		     
            case 0 :
		     lck1.l_type = F_WRLCK; 
                     lck1.l_whence = 0;
	             lck1.l_start = 0;
		     lck1.l_len = 0; 
		
                     fd1 = open("some_file", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

		     if ((fcntl(fd1, F_SETLK, &lck1) == 0) && (errno != EAGAIN || errno != EACCES))
		         exit(0);
		     else
		         exit(EXIT_FAILURE);
								       
            default: died= wait(&status); // this is the code the parent runs 
		     fail_unless(status == 0 ,"Lock failed");
    }
	
    close(fd);
    close(fd1);   
    unlink("some_file");
   
}
END_TEST


START_TEST(test_peos_set_resource_value)
{
    int i;
    peos_resource_t *resources = (peos_resource_t *) calloc(3,sizeof(peos_resource_t));
    peos_context_t *context = &(process_table[0]);

    context -> pid = 0;
    context -> resources = resources;
    context -> num_resources = 3;

    for(i = 0; i < 3; i++){
	sprintf(resources[i].name, "r%d",i);
	sprintf(resources[i].value, "val");
    }

    fail_unless(peos_set_resource_value(0, "r2", "new_val") == 1, "return value");
    fail_unless(strcmp(context->resources[0].value, "val") == 0, "r0 value");
    fail_unless(strcmp(context->resources[1].value, "val") == 0, "r1 value");
    fail_unless(strcmp(context->resources[2].value, "new_val") == 0, "r2 value");
	
}
END_TEST;


START_TEST(test_peos_set_resource_value_no_resource)
{
   int i;
   peos_resource_t *resources = (peos_resource_t *) calloc(3,sizeof(peos_resource_t));
   peos_context_t *context = &(process_table[0]);
  
   context -> pid = 0;
   context -> resources = resources;
   context -> num_resources = 3;
                                                                        
   for(i = 0; i < 3; i++) {
       sprintf(resources[i].name, "r%d",i);
       sprintf(resources[i].value, "val");
   }

   fail_unless(peos_set_resource_value(0, "r4", "new_val") == -1," unknown resource bound");
}
END_TEST

START_TEST(test_make_node_lists)
{
    Graph g;
    peos_action_t *actions;
    peos_other_node_t *other_nodes;
    int num_actions,num_other_nodes;

    g = stub_makegraph("some file");

    /* action */
    fail_unless(make_node_lists(g, &actions, &num_actions, &other_nodes, &num_other_nodes) == 1, "return value");

    /* post */

    fail_unless(num_actions == 2,"num actions wrong");
    fail_unless(num_other_nodes == 1 ," num_other_nodes wrong");

   
    fail_unless(strcmp(actions[0].name,"act_0") == 0, "act_0 name wrong");
    fail_unless(strcmp(actions[1].name,"act_1") == 0, "act_1 name wrong");
    fail_unless(strcmp(other_nodes[0].name,"sel") == 0, "sel name wrong");
    fail_unless(actions[0].state == ACT_NONE, "act_0 state wrong");
    fail_unless(actions[0].requires_state == FALSE, "act_0 requires_state wrong");
    fail_unless(actions[0].provides_state == FALSE, "act_0 provides_state wrong");
    fail_unless(actions[1].state == ACT_NONE, "act_1 state wrong");
    fail_unless(other_nodes[0].state == ACT_NONE, "sel state wrong");
    fail_unless(actions[1].requires_state == FALSE, "act_1 requires_state wrong");
    fail_unless(actions[1].provides_state == FALSE, "act_1 provides_state wrong");
    fail_unless(strcmp(actions[0].script,"test script") == 0, "act_0 script wrong");
    fail_unless(strcmp(actions[1].script,"test script") == 0, "act_1 script wrong");
}
END_TEST

START_TEST(test_annotate_graph)
{
	int i;
	int num_actions,num_other_nodes;
	peos_action_t *actions;
	peos_other_node_t *other_nodes;
	
	Graph g = (Graph) malloc (sizeof (struct graph));
	Node source,sink,act_0,act_1,sel,branch,join,rendezvous;
	num_actions = 2;
	actions = (peos_action_t *) calloc(num_actions, sizeof(peos_action_t));

	for(i = 0; i < num_actions ; i++)
	{
		sprintf(actions[i].name,"act_%d",i);
		actions[i].state = ACT_RUN;
	}

        num_other_nodes = 2;
        other_nodes = (peos_other_node_t *) calloc(num_other_nodes, sizeof(peos_other_node_t));
                                                                       
          sprintf(other_nodes[0].name,"sel");
	  sprintf(other_nodes[1].name,"br");
          other_nodes[0].state = ACT_RUN;
	  other_nodes[1].state = ACT_READY;
     
	source = make_node("p",ACT_NONE,PROCESS,0);
	sink = make_node("p",ACT_NONE,PROCESS,0);
	act_0 = make_node("act_0",ACT_NONE,ACTION,0);
	act_1 = make_node("act_1",ACT_NONE,ACTION,0);
	sel = make_node("sel",ACT_NONE,SELECTION,0);
	join = make_node("sel",ACT_NONE,JOIN,0);
	branch = make_node("br",ACT_NONE,BRANCH,0);
	rendezvous = make_node("br",ACT_NONE,RENDEZVOUS,0);

	g -> source = source;
	g -> sink = sink;
	source -> next = act_0;
	act_0 -> next = act_1;
	act_1 -> next = branch;
	branch -> next = sel;
	sel -> next = rendezvous;
	rendezvous -> next = join;
	join -> next = sink;
	sink -> next = NULL;

	sel -> matching = join;
	branch -> matching = rendezvous;

	fail_unless(annotate_graph(g,actions,num_actions,other_nodes,num_other_nodes) == 1, "return value");
	fail_unless(STATE(act_0) == ACT_RUN, "act 0 state not run");
	fail_unless(STATE(act_1) == ACT_RUN, "act 1 state not run");
	fail_unless(STATE(sel) == ACT_RUN, "sel not run");
	fail_unless(STATE(join) == ACT_RUN, "join not run");
	fail_unless(STATE(branch) == ACT_READY, "branch not ready");
       fail_unless(STATE(rendezvous) == ACT_READY, "rendezvous not run");

}
END_TEST


START_TEST(test_save_proc_table)
{
    int i, j, nbytes, abytes;
    char expected[BUFSIZ], actual[BUFSIZ];
    FILE *f;
    int t_num_actions,t_num_other_nodes;
    peos_action_t *t_actions;
    peos_other_node_t *t_other_nodes;

    /* Pre: process table initialized. */
    peos_context_t *context = &(process_table[0]);

    f = fopen("expected_proc_table.dat", "w");
    for (j = 0; j < PEOS_MAX_PID+1; j++) {
	context = &(process_table[j]);
	context->process_graph = (Graph) stub_makegraph("some file");
	sprintf(context->model, "test.pml");
	fprintf(f, "pid: %d\nmodel: %s\n", j, context->model);
	context->status = PEOS_RUNNING;
	fprintf(f, "status: %d\n", context->status);
	t_num_actions = 2;
	fprintf(f, "actions: "); 
	fprintf(f, "%d ",t_num_actions);
	t_actions = (peos_action_t *)calloc(t_num_actions, sizeof(peos_action_t));
	for (i = 0; i < t_num_actions; i++) {
	    sprintf(t_actions[i].name, "act_%d",i);
	    t_actions[i].state = ACT_NONE;
	    t_actions[i].requires_state = FALSE;
	    t_actions[i].provides_state = FALSE;
	    fprintf(f, " %s %d %d %d", t_actions[i].name, t_actions[i].state, t_actions[i].requires_state, t_actions[i].provides_state); 
	}
        fprintf(f, "\n");
        t_num_other_nodes = 1;
        fprintf(f, "other_nodes: ");
	fprintf(f, "%d ", t_num_other_nodes);
	t_other_nodes = (peos_other_node_t *)calloc(t_num_other_nodes, sizeof(peos_other_node_t));
	for (i = 0; i < t_num_other_nodes; i++) {
	    strcpy(t_other_nodes[i].name, "sel");
	    t_other_nodes[i].state = ACT_NONE;
	   fprintf(f, " %s %d", t_other_nodes[i].name, t_other_nodes[i].state);
	}

        fprintf(f, "\n");
	context->num_resources = 2;
        fprintf(f, "resources: ");
        fprintf(f, "%d ", context->num_resources);
        context->resources = (peos_resource_t *)calloc(context->num_resources, sizeof(peos_resource_t));
        for (i = 0; i < context->num_resources; i++) {
          strcpy(context->resources[i].name, "some_resource");
            strcpy(context->resources[i].value,"some_value");
            fprintf(f, " %s %s", context->resources[i].name, context->resources[i].value);
          }
	
	fprintf(f, "\n\n"); 
    }
    fclose(f);

    mark_point();

    f = fopen("expected_proc_table.dat", "r");
    memset(expected, 0, BUFSIZ);
    nbytes = fread(expected, sizeof(char), BUFSIZ, f);
    fclose(f);
    mark_point();

    /* Action */
    save_proc_table("proc_table1.dat");
    mark_point();

    /* Post: process table file exists and contains proc table. */
    f = fopen("proc_table1.dat", "r");
    memset(actual, 0, BUFSIZ);
    abytes = fread(actual, sizeof(char), BUFSIZ, f);
    fail_unless(abytes == nbytes, "file size");
    fclose(f);
    mark_point();

    fail_unless (strcmp(actual, expected) == 0, "proc table contents differ");
 
      unlink("proc_table1.dat");
      unlink("expected_proc_table.dat");
}
END_TEST

START_TEST(test_load_proc_table) 
{
    int i, j;
    char  *model = TEST_PROC_NAME;

    peos_context_t ctx;
    FILE *f;
    int num_actions,num_other_nodes;
    peos_action_t *actions;
    peos_other_node_t *other_nodes;

    /* Pre: saved process table file. */
    peos_context_t *context = &(ctx);

    f = fopen("proc_table_test.dat", "w");
    for (j = 0; j <= PEOS_MAX_PID; j++) {
	sprintf(context->model, model);
	fprintf(f, "pid: %d\nmodel: %s\n", j, context->model);
	context->status = PEOS_RUNNING;
	fprintf(f, "status: %d\n", context->status);
	num_actions = 2;
	fprintf(f, "actions: "); 
	fprintf(f, "%d ", num_actions);
	actions = (peos_action_t *)calloc(num_actions, sizeof(peos_action_t));
	for (i = 0; i < num_actions; i++) {
	    sprintf(actions[i].name, "act_%d", i);
	    actions[i].state = ACT_NONE;
	    actions[i].script = "test script";
	    actions[i].requires_state = FALSE;
	    actions[i].provides_state = TRUE;
	    fprintf(f, " %s %d %d %d", actions[i].name, actions[i].state, actions[i].requires_state, actions[i].provides_state); 
	}

        
	fprintf(f,"\n");

	num_other_nodes = 1;
        fprintf(f, "other_nodes: ");
        fprintf(f, "%d ", num_other_nodes);
        other_nodes = (peos_other_node_t *)calloc(num_other_nodes, sizeof(peos_other_node_t));
        for (i = 0; i < num_other_nodes; i++) {
            sprintf(other_nodes[i].name, "sel");
            other_nodes[i].state = ACT_NONE;
            fprintf(f, " %s %d", other_nodes[i].name, other_nodes[i].state);
        }
	
	fprintf(f,"\n");

        context->num_resources = 2;
        fprintf(f, "resources: ");
        fprintf(f, "%d ", context->num_resources);
        context->resources = (peos_resource_t *)calloc(context->num_resources, sizeof(peos_resource_t));
        for (i = 0; i < context->num_resources; i++) {
	    strcpy(context->resources[i].name, "some_resource");
	    strcpy(context->resources[i].value, "some_value"); 
            fprintf(f, " %s %s", context->resources[i].name, context->resources[i].value);
	}
	
	fprintf(f, "\n\n"); 
    }
    fclose(f);
    mark_point();

    memset(process_table, 0, (PEOS_MAX_PID+1) * sizeof(peos_context_t));
    mark_point();

    /* Load_proc_table requires a real model file to load actions from. */
    make_pml_file(model, "process sample5 {\n  action act_0 {\n    script {\"test script\"}\n  }\n  action act_1 {\n    script {\"test script\"}\n  }\n  action act_2 {\n    script {\"test script\"}\n  }\n}\n");

    /* Action */
    load_proc_table("proc_table_test.dat");
    mark_point();
    unlink(model);

    /* Post: process table reflects file contents; other globals
       initialized. */
    for (j = 0; j <= PEOS_MAX_PID; j++) {
	context = &(process_table[j]);
	fail_unless(context->pid == j, "pid");
	fail_unless(strcmp(context->model, model) == 0, "model name");
	fail_unless(context->status == PEOS_RUNNING, "process status");
	fail_unless(context->process_graph != NULL, "process graph null");	
	
	for (i=0; i < context->num_resources;i++) {
	    fail_unless(context->resources[i].pid == j, "resources pid");
	    fail_unless(strcmp(context->resources[i].name, "some_resource") == 0, "resource name");
	    fail_unless(strcmp(context->resources[i].value, "some_value") == 0, "resource value");
        }
	
    }

    unlink("proc_table_test.dat");
}
END_TEST


START_TEST(test_list_actions_0)
{
    
    int i, j;
    char  *model = TEST_PROC_NAME;

    peos_context_t ctx;
    FILE *f;
    int num_actions,num_other_nodes;
    peos_action_t *actions;
    peos_other_node_t *other_nodes;

    /* Pre: saved process table file. */
    peos_context_t *context = &(ctx);

    f = fopen("proc_table.dat", "w");
    for (j = 0; j <= PEOS_MAX_PID; j++) {
	sprintf(context->model, model);
	fprintf(f, "pid: %d\nmodel: %s\n", j, context->model);
	context->status = PEOS_RUNNING;
	fprintf(f, "status: %d\n", context->status);
	num_actions = 2;
	fprintf(f, "actions: "); 
	fprintf(f, "%d ", num_actions);
	actions = (peos_action_t *)calloc(num_actions, sizeof(peos_action_t));
	for (i = 0; i < num_actions; i++) {
	    sprintf(actions[i].name, "act_%d", i);
	    actions[i].state = ACT_NONE;
	    actions[i].script = "test script";
	    fprintf(f, " %s %d", actions[i].name, actions[i].state); 
	}
        
	fprintf(f,"\n");

	num_other_nodes = 1;
        fprintf(f, "other_nodes: ");
        fprintf(f, "%d ", num_other_nodes);
        other_nodes = (peos_other_node_t *)calloc(num_other_nodes, sizeof(peos_other_node_t));
        for (i = 0; i < num_other_nodes; i++) {
            sprintf(other_nodes[i].name, "sel");
            other_nodes[i].state = ACT_NONE;
            fprintf(f, " %s %d", other_nodes[i].name, other_nodes[i].state);
        }
	
	fprintf(f,"\n");

        context->num_resources = 2;
        fprintf(f, "resources: ");
        fprintf(f, "%d ", context->num_resources);
        context->resources = (peos_resource_t *)calloc(context->num_resources, sizeof(peos_resource_t));
        for (i = 0; i < context->num_resources; i++) {
	    strcpy(context->resources[i].name, "some_resource");
	    strcpy(context->resources[i].value, "some_value"); 
            fprintf(f, " %s %s", context->resources[i].name, context->resources[i].value);
	}
	
	fprintf(f, "\n\n"); 
    }
    fclose(f);

    make_pml_file(model, "process sample5 {\n  action act_0 {\n    script {\"test script\"}\n  }\n  action act_1 {\n    script {\"test script\"}\n  }\n }\n");

    
    actions = peos_list_actions(0,&num_actions);

    fail_unless(actions != NULL,"actions null");
    fail_unless(num_actions == 2, "num_actions wrong");
    fail_unless(strcmp(actions[0].name,"act_0") == 0, "act_0 name wrong");
    fail_unless(actions[0].pid == 0, "act_0 pid wrong");
    fail_unless(actions[0].state == ACT_NONE, "act_0 state wrong");
    fail_unless(strcmp(actions[1].name,"act_1") == 0, "act_1 name wrong"); 
    fail_unless(actions[1].pid == 0, "act_1 pid wrong");
    fail_unless(actions[1].state == ACT_NONE, "act_1 state wrong");
	    
   
    free(actions);
    unlink("proc_table.dat");
}
END_TEST


/* Sets up a full table. */
void setup_find_free_entry()
{
    int i;

    for (i = 0; i < PEOS_MAX_PID + 1; i++) {
	process_table[i].status = PEOS_RUNNING;
    }
}


START_TEST(test_find_free_entry)
{
    int i;
    peos_context_t *context; 
    /* Pre: an empty process table. */
    for (i = 0; i < PEOS_MAX_PID + 1; i++) {
	process_table[i].status = PEOS_NONE;
    }
    context = find_free_entry();

    /* Post: first entry is returned. */
    fail_unless(context == &(process_table[0]), "wrong context returned");
}
END_TEST

START_TEST(test_find_free_entry_full)
{
    peos_context_t *context; 
    /* Pre: an full process table. */
    setup_find_free_entry();
    context = find_free_entry();

    /* Post: first entry is returned. */
    fail_unless(context == NULL, "full: wrong context returned");
}
END_TEST

START_TEST(test_find_free_entry_full2)
{
    int i;
    peos_context_t *context; 
    /* Pre: an full process table. */
    for (i = 0; i < PEOS_MAX_PID + 1; i++) {
	process_table[i].status = PEOS_READY;
    }
    context = find_free_entry();

    /* Post: first entry is returned. */
    fail_unless(context == NULL, "full2: wrong context returned");
}
END_TEST


START_TEST(test_find_free_entry_first)
{
    peos_context_t *context; 
    /* Pre: an full process table. */
    setup_find_free_entry();
    process_table[0].status = PEOS_NONE;

    context = find_free_entry();

    /* Post: first entry is returned. */
    fail_unless(context == &(process_table[0]), "first: wrong context returned");
}
END_TEST

START_TEST(test_find_free_entry_last)
{
    peos_context_t *context; 
    /* Pre: an full process table. */
    setup_find_free_entry();
    process_table[PEOS_MAX_PID].status = PEOS_NONE;

    context = find_free_entry();

    /* Post: first entry is returned. */
    fail_unless(context == &(process_table[PEOS_MAX_PID]), "last: wrong context returned");
}
END_TEST

START_TEST(test_find_free_entry_middle)
{
    peos_context_t *context; 
    /* Pre: an full process table. */
    setup_find_free_entry();
    process_table[PEOS_MAX_PID/2].status = PEOS_NONE;

    context = find_free_entry();

    /* Post: first entry is returned. */
    fail_unless(context == &(process_table[PEOS_MAX_PID/2]), "middle: wrong context returned");
}
END_TEST


int
main(int argc, char *argv[])
{
    int nf;
    SRunner *sr;
    Suite *s = suite_create("process_table");
    TCase *tc;

    parse_args(argc, argv);

    tc = tcase_create("get pid");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_get_pid);
    tcase_add_test(tc, test_get_pid_last);

    tc = tcase_create("lock file");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_get_lock);
    tcase_add_test(tc,test_release_lock);
	
    
    tc = tcase_create("make node lists");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_make_node_lists);
    
    tc = tcase_create("annotate graph");
    suite_add_tcase(s,tc);
    tcase_add_test(tc,test_annotate_graph);

    tc = tcase_create("table io");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_load_proc_table);
    tcase_add_test(tc, test_save_proc_table);

    tc = tcase_create("set resource value");
    suite_add_tcase(s, tc);
    tcase_add_test(tc,test_peos_set_resource_value);
    tcase_add_test(tc,test_peos_set_resource_value_no_resource);

    tc = tcase_create("list actions");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_list_actions_0);

    tc = tcase_create("find free entry");
    suite_add_tcase(s, tc);
    tcase_add_test(tc, test_find_free_entry);
    tcase_add_test(tc, test_find_free_entry_full);
    tcase_add_test(tc, test_find_free_entry_full2);
    tcase_add_test(tc, test_find_free_entry_first);
    tcase_add_test(tc, test_find_free_entry_last);
    tcase_add_test(tc, test_find_free_entry_middle);

    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    suite_free(s);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
