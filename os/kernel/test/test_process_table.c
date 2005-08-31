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

void initialize_graph(Graph g, int pid)
{
    Node n;
    for(n = g -> source; n != NULL; n = n -> next) {
        n -> data = (void *) malloc (sizeof (struct data));
	sanitize_node(n);
    }
}

vm_exit_code update_process_state(int pid)
{
    return VM_CONTINUE;
}

char *act_state_name(vm_act_state s)
{
    return "READY";
}

peos_resource_t *get_resource_list_action_requires(int pid, char *act_name, int *num_resources)
{
    peos_resource_t *resources = (peos_resource_t *) calloc(2, sizeof(peos_resource_t));
    *num_resources = 2;

    strcpy(resources[0].name, "r1");
    strcpy(resources[0].value, "r1val");
    strcpy(resources[0].qualifier, "abstract");
    strcpy(resources[1].name, "r2");
    strcpy(resources[1].value, "r2val");
    strcpy(resources[1].qualifier, "\0");

    return resources;
}

peos_resource_t *get_resource_list_action_provides(int pid, char *act_name, int *num_resources)
{
    peos_resource_t *resources = (peos_resource_t *) calloc(2, sizeof(peos_resource_t));
    *num_resources = 2;

    strcpy(resources[0].name, "r1");
    strcpy(resources[0].value, "r1val");
    strcpy(resources[0].qualifier, "abstract");
    strcpy(resources[1].name, "r2");
    strcpy(resources[1].value, "r2val");
    strcpy(resources[1].qualifier, "\0");

    return resources;
}

int eval_resource_list(peos_resource_t** resources, int num_resources) {
    return 1;
}

int fill_resource_list_value(peos_resource_t* source, int num_source, peos_resource_t** destination, int num_destination) {
    int i;
    peos_resource_t* res = *destination;

    for (i = 0; i < num_destination; i++) {
        strcat(res[i].value, "_eval");
    }
    return 1;
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


START_TEST(test_set_resource_binding)
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

    fail_unless(set_resource_binding(0, "r2", "new_val") == 1, "return value");
    fail_unless(strcmp(context->resources[0].value, "val") == 0, "r0 value");
    fail_unless(strcmp(context->resources[1].value, "val") == 0, "r1 value");
    fail_unless(strcmp(context->resources[2].value, "new_val") == 0, "r2 value");
	
}
END_TEST;


START_TEST(test_set_resource_binding_overflow)
{
    int i;
    char long_res[400];
    FILE *old_stderr;
    
    peos_resource_t *resources = (peos_resource_t *) calloc(3,sizeof(peos_resource_t));
    peos_context_t *context = &(process_table[0]);


    if (verbosity != CK_VERBOSE) {
        old_stderr = stderr;
	stderr = fopen("/dev/null", "w");
    }
    
    
    for(i = 0; i < 399; i++) {
        long_res[i] = 'a';
    }
    long_res[i] = '\0';    
 
    context -> pid = 0;
    context -> resources = resources;
    context -> num_resources = 3;

    for(i = 0; i < 3; i++){
	sprintf(resources[i].name, "r%d",i);
	sprintf(resources[i].value, "val");
    }

    fail_unless(set_resource_binding(0, "r2", long_res) == -1, "overflow value");
    if (verbosity != CK_VERBOSE) {
        fclose(stderr);
        stderr = old_stderr;
    }
    
}
END_TEST

START_TEST(test_get_resource_binding)
{
    int i;
    peos_resource_t *resources = (peos_resource_t *) calloc(3,sizeof(peos_resource_t));
    peos_context_t *context = &(process_table[0]);

    context -> pid = 0;
    context -> resources = resources;
    context -> num_resources = 3;

    for(i = 0; i < 3; i++){
	sprintf(resources[i].name, "r%d",i);
	sprintf(resources[i].value, "val%d",i);
    }

    fail_unless(strcmp(get_resource_binding(0, "r2"), "val2") == 0 , "get binding");
}
END_TEST

START_TEST(test_get_resource_qualifier)
{
    int i;
    peos_resource_t *resources = (peos_resource_t *) calloc(3,sizeof(peos_resource_t));
    peos_context_t *context = &(process_table[0]);

    context -> pid = 0;
    context -> resources = resources;
    context -> num_resources = 3;

    for(i = 0; i < 3; i++){
	sprintf(resources[i].name, "r%d",i);
	sprintf(resources[i].value, "val%d",i);
	sprintf(resources[i].qualifier, "qualifier%d",i);
    }

    fail_unless(strcmp(get_resource_qualifier(0, "r2"), "qualifier2") == 0 , "get qualifier wrong");
}
END_TEST

    
START_TEST(test_set_resource_binding_no_resource)
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

   fail_unless(set_resource_binding(0, "r4", "new_val") == -1," unknown resource bound");
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
    fail_unless(actions[1].state == ACT_NONE, "act_1 state wrong");
    fail_unless(other_nodes[0].state == ACT_NONE, "sel state wrong");
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

START_TEST(test_save_context)
{
    FILE* f;
    char line[600];
    char res_line[600];
    int i;
    
    peos_context_t* context = &(process_table[0]);
    context->process_graph = (Graph)stub_makegraph("some file");
    strcpy(context->model, "test.pml");
    context->status = PEOS_RUNNING;
    context->num_resources = 2;
    context->resources = (peos_resource_t *)calloc(context->num_resources, sizeof(peos_resource_t));
    
    //test for unbound resource
    strcpy(context->resources[0].name, "res0");
    strcpy(context->resources[1].name, "res1");
    f = fopen("test_save_context.dat", "w");
    fail_unless(save_context(0, context, f), "save_context failed");
    fclose(f);
    f = fopen("test_save_context.dat", "r");
    fgets(line, 600, f);
    fail_unless(strcmp(line, "pid: 0\n") == 0, "save_context failed");
    fgets(line, 600, f);
    fail_unless(strcmp(line, "model: test.pml\n") == 0, "save_context failed");
    fgets(line, 600, f);
    fail_unless(strcmp(line, "status: 4\n") == 0, "save_context failed");
    fgets(line, 600, f);
    fail_unless(strcmp(line, "actions: 2  act_0 0 act_1 0\n") == 0, "save_context failed");
    fgets(line, 600, f);
    fail_unless(strcmp(line, "other_nodes: 1  sel 0\n") == 0, "save_context failed");
    fgets(line, 600, f);
    fail_unless(strcmp(line, "resources: 2  res0 \"\" res1 \"\"\n") == 0, "save_context failed");
    fclose(f);

    //test for bounded resources with single character as their value
    strcpy(context->resources[0].value, "a");
    strcpy(context->resources[1].value, "b");
    f = fopen("test_save_context.dat", "w");
    fail_unless(save_context(0, context, f), "save_context failed");
    fclose(f);
    f = fopen("test_save_context.dat", "r");
    //skip to resource line
    for (i = 0; i < 6; i++)
        fgets(line, 600, f);
    fclose(f);
    fail_unless(strcmp(line, "resources: 2  res0 \"a\" res1 \"b\"\n") == 0, "save_context failed");

    //test for bounded resources with their value equal maximum number of characters
    for (i = 0; i < 255; i++) {
        context->resources[0].value[i] = 'a';
        context->resources[1].value[i] = 'b';
    }
    context->resources[0].value[++i] = '\0';
    context->resources[1].value[i] = '\0';

    f = fopen("test_save_context.dat", "w");
    fail_unless(save_context(0, context, f), "save_context failed");
    fclose(f);
    f = fopen("test_save_context.dat", "r");
    //skip to resource line
    for (i = 0; i < 6; i++)
        fgets(line, 600, f);
    fclose(f);
    sprintf(res_line, "resources: 2  res0 \"%s\" res1 \"%s\"\n", context->resources[0].value, context->resources[1].value);
    fail_unless(strcmp(line, res_line) == 0, "save_context failed");
    unlink("test_save_context.dat");
}
END_TEST

START_TEST(test_load_context)
{
    int i;
    char res_value[257];
    FILE* f;
    Node node;
    peos_context_t ctx;
    peos_context_t* context = &(ctx);
    
    make_pml_file("test.pml", "process sample5 { action act_0 { script {\"test script\"} } action act_1 { script {\"test script\"} } }");
    
    //test for unbound resource
    f = fopen("test_load_context.dat", "w");
    fprintf(f, "pid: 0\n");
    fprintf(f, "model: test.pml\n");
    fprintf(f, "status: 4\n");
    fprintf(f, "actions: 2  act_0 0 act_1 0\n");
    fprintf(f, "other_nodes: 1  sel 0\n");
    fprintf(f, "resources: 2  res0 \"\" res1 \"\"\n");
    fprintf(f, " \n");
    fclose(f);
    f = fopen("test_load_context.dat", "r");
    fail_unless(load_context(f, context), "load_context failed");
    fclose(f);
    fail_unless(context->pid == 0, "load_context failed");
    fail_unless(strcmp(context->model, "test.pml") == 0, "load_context failed");
    fail_unless(context->status == PEOS_RUNNING, "load_context failed");
    fail_unless(context->process_graph, "load_context failed");
    node = context->process_graph->source->next;
    fail_unless(node, "load_context failed");
    fail_unless(node->type == ACTION, "load_context failed");
    fail_unless(strcmp(node->name, "act_0") == 0, "load_context failed");
    node = node->next;
    fail_unless(node, "load_context failed");
    fail_unless(node->type == ACTION, "load_context failed");
    fail_unless(strcmp(node->name, "act_1") == 0, "load_context failed");
    fail_unless(context->num_resources == 2, "load_context failed");
    fail_unless(strcmp(context->resources[0].name, "res0") == 0, "load_context failed");
    fail_unless(strcmp(context->resources[0].value, "") == 0, "load_context failed");
    fail_unless(strcmp(context->resources[1].name, "res1") == 0, "load_context failed");
    fail_unless(strcmp(context->resources[1].value, "") == 0, "load_context failed");
    
    //test for bounded resources with single character as their value
    f = fopen("test_load_context.dat", "w");
    fprintf(f, "pid: 0\n");
    fprintf(f, "model: test.pml\n");
    fprintf(f, "status: 4\n");
    fprintf(f, "actions: 2  act_0 0 act_1 0\n");
    fprintf(f, "other_nodes: 1  sel 0\n");
    fprintf(f, "resources: 2  res0 \"a\" res1 \"b\"\n");
    fprintf(f, " \n");
    fclose(f);
    f = fopen("test_load_context.dat", "r");
    fail_unless(load_context(f, context), "load_context failed");
    fclose(f);
    fail_unless(strcmp(context->resources[0].value, "a") == 0, "load_context failed");
    fail_unless(strcmp(context->resources[1].value, "b") == 0, "load_context failed");
    
    //test for bounded resources with their value equal maximum number of characters
    for (i = 0; i < 255; i++) {
        res_value[i] = 'a';
    }
    res_value[++i] = '\0';
    
    f = fopen("test_load_context.dat", "w");
    fprintf(f, "pid: 0\n");
    fprintf(f, "model: test.pml\n");
    fprintf(f, "status: 4\n");
    fprintf(f, "actions: 2  act_0 0 act_1 0\n");
    fprintf(f, "other_nodes: 1  sel 0\n");
    fprintf(f, "resources: 2  res0 \"%s\" res1 \"%s\"\n", res_value, res_value);
    fprintf(f, " \n");
    fclose(f);
    f = fopen("test_load_context.dat", "r");
    fail_unless(load_context(f, context), "load_context failed");
    fclose(f);
    fail_unless(strcmp(context->resources[0].value, res_value) == 0, "load_context failed");
    fail_unless(strcmp(context->resources[1].value, res_value) == 0, "load_context failed");
    
    //test for bounded resources with their value beyond maximum number of characters
    for (i = 0; i < 256; i++) {
        res_value[i] = 'a';
    }
    res_value[++i] = '\0';
    f = fopen("test_load_context.dat", "w");
    fprintf(f, "pid: 0\n");
    fprintf(f, "model: test.pml\n");
    fprintf(f, "status: 4\n");
    fprintf(f, "actions: 2  act_0 0 act_1 0\n");
    fprintf(f, "other_nodes: 1  sel 0\n");
    fprintf(f, "resources: 2  res0 \"%s\" res1 \"%s\"\n", res_value, res_value);
    fprintf(f, " \n");
    fclose(f);
    f = fopen("test_load_context.dat", "r");
    fail_unless(!load_context(f, context), "load_context failed");
    fclose(f);
    unlink("test_load_context.dat");
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
	    fprintf(f, " %s %d", t_actions[i].name, t_actions[i].state); 
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
            fprintf(f, " %s \"%s\"", context->resources[i].name, context->resources[i].value);
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
      unlink("proc_table.dat.xml");
}
END_TEST

START_TEST(test_load_proc_table) 
{
    int i, j;
    char  *model = TEST_PROC_NAME;

    peos_context_t ctx;
    FILE *f;
    int num_actions, num_other_nodes;
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
            fprintf(f, " %s \"%s\"", context->resources[i].name, context->resources[i].value);
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
            fprintf(f, " %s \"%s\"", context->resources[i].name, context->resources[i].value);
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
    unlink("proc_table.dat.xml");
}
END_TEST


START_TEST(test_print_action_node_0)
{
    int abytes, nbytes;	
    FILE *expected, *actual;	
    char expectedmem[BUFSIZ], actualmem[BUFSIZ];
    
    Node n = make_node("act_0", ACT_READY, ACTION, 3);
    PID(n) = 0;

    expected = fopen("expected.xml", "w");
    fprintf(expected, "<action name=\"act_0\" state=\"READY\">\n");
    fprintf(expected, "<script>\nscript\n</script>\n");
    fprintf(expected, "<req_resource name=\"r1\" value=\"r1val\" qualifier=\"abstract\"></req_resource>\n");
    fprintf(expected, "<req_resource name=\"r2\" value=\"r2val\" qualifier=\"\"></req_resource>\n");
    fprintf(expected, "<prov_resource name=\"r1\" value=\"r1val\" qualifier=\"abstract\"></prov_resource>\n");
    fprintf(expected, "<prov_resource name=\"r2\" value=\"r2val\" qualifier=\"\"></prov_resource>\n");
    fprintf(expected, "</action>\n");
    fclose(expected);
    mark_point();

    expected = fopen("expected.xml", "r");
    memset(expectedmem, 0, BUFSIZ);
    nbytes = fread(expectedmem, sizeof(char), BUFSIZ, expected);
    fclose(expected);
    mark_point();
 
    actual = fopen("actual.xml", "w");

    print_action_node(n, actual);
    fclose(actual);

    mark_point();

    actual = fopen("actual.xml", "r");
    memset(actualmem, 0, BUFSIZ);
    abytes = fread(actualmem, sizeof(char), BUFSIZ, actual);
    fail_unless(abytes == nbytes, "file size");
    fclose(actual);
    mark_point();

    fail_unless (strcmp(actualmem, expectedmem) == 0, "print_action:proc table contents differ");

    unlink("expected.xml");
    unlink("actual.xml");

}
END_TEST
        
START_TEST(test_print_action_node_1)
{
    int i = 0;
    char* line = (char*)malloc(sizeof(char) * 255);
    FILE *actual;
    Node n = make_node("act_0", ACT_READY, ACTION, 3);
    PID(n) = 0;
    actual = fopen("actual.xml", "w");

    process_table[0].num_resources = 2;
    process_table[0].resources = (peos_resource_t*)calloc(2, sizeof(peos_resource_t));

    print_action_node(n, actual);

    fclose(actual);
    actual = fopen("actual.xml", "r");
    while (fgets(line, 255, actual) != NULL) {
        if (strcmp(line, "<req_resource name=\"r1\" value=\"r1val_eval\" qualifier=\"abstract\"></req_resource>\n") == 0)
            i++;
        else if (strcmp(line, "<req_resource name=\"r2\" value=\"r2val_eval\" qualifier=\"\"></req_resource>\n") == 0)
            i++;
    }
    fclose(actual);
    fail_unless(i == 2, "print action node failed");
}
END_TEST

START_TEST(test_print_after_escaping)
{
    int abytes, nbytes;	
    FILE *expected, *actual;	
    char expectedmem[BUFSIZ], actualmem[BUFSIZ];
    char str[20]="<input type=\"&test\">";

    expected = fopen("expected.xml", "w");
    fprintf(expected, "&lt;input type=&quot;&amp;test&quot;&gt;");
    fclose(expected);
    mark_point();

    expected = fopen("expected.xml", "r");
    memset(expectedmem, 0, BUFSIZ);
    nbytes = fread(expectedmem, sizeof(char), BUFSIZ, expected);
    fclose(expected);
    mark_point();
 
    actual = fopen("actual.xml", "w");
    

    print_after_escaping(str, actual);
    fclose(actual);

    mark_point();

    actual = fopen("actual.xml", "r");
    memset(actualmem, 0, BUFSIZ);
    abytes = fread(actualmem, sizeof(char), BUFSIZ, actual);
    fail_unless(abytes == nbytes, "file size");
    fclose(actual);
    mark_point();

    fail_unless (strcmp(actualmem, expectedmem) == 0, "print_after_escaping : not escaped");

    unlink("expected.xml");
    unlink("actual.xml");

}
END_TEST

START_TEST(test_print_xml_iteration)
{
    int abytes, nbytes;	
    FILE *expected, *actual;	
    char expectedmem[BUFSIZ], actualmem[BUFSIZ];
    
    Graph g = (Graph) malloc(sizeof(struct graph));
    Node source, sink, act_0;

    source = make_node("p", ACT_READY, PROCESS, 0);
    PID(source) = 0;
    act_0 = make_node("act_0", ACT_READY, ACTION, 1);
    source -> next = act_0;
    PID(act_0) = 0;
    sink = make_node("p", ACT_READY, PROCESS, 2);
    act_0 -> next = sink;
    sink -> next = NULL;

    source -> predecessors = NULL;
    source -> successors = (List) make_list(act_0, NULL, NULL, NULL, NULL);
    act_0 -> predecessors = (List) make_list(source,act_0,NULL,NULL,NULL);
    act_0 -> successors = (List) make_list(sink,act_0,NULL,NULL,NULL);
    sink -> predecessors = (List) make_list(act_0, NULL, NULL, NULL, NULL);
    sink -> successors = (List) ListCreate();
 
    g -> source = source;
    g -> sink = sink;

    

    expected = fopen("expected.xml", "w");

    fprintf(expected, "<iteration>\n");
    fprintf(expected, "<action name=\"act_0\" state=\"READY\">\n");
    fprintf(expected, "<script>\nscript\n</script>\n");
    fprintf(expected, "<req_resource name=\"r1\" value=\"r1val\" qualifier=\"abstract\"></req_resource>\n");
    fprintf(expected, "<req_resource name=\"r2\" value=\"r2val\" qualifier=\"\"></req_resource>\n");
    fprintf(expected, "<prov_resource name=\"r1\" value=\"r1val\" qualifier=\"abstract\"></prov_resource>\n");
    fprintf(expected, "<prov_resource name=\"r2\" value=\"r2val\" qualifier=\"\"></prov_resource>\n");
    fprintf(expected, "</action>\n");
    fprintf(expected, "</iteration>\n");
    fclose(expected);
    mark_point();

    expected = fopen("expected.xml", "r");
    memset(expectedmem, 0, BUFSIZ);
    nbytes = fread(expectedmem, sizeof(char), BUFSIZ, expected);
    fclose(expected);
    mark_point();
 
    actual = fopen("actual.xml", "w");

    print_graph(g, actual);
    fclose(actual);

    mark_point();

    actual = fopen("actual.xml", "r");
    memset(actualmem, 0, BUFSIZ);
    abytes = fread(actualmem, sizeof(char), BUFSIZ, actual);
    fail_unless(abytes == nbytes, "file size");
    fclose(actual);
    mark_point();

    fail_unless (strcmp(actualmem, expectedmem) == 0, "print_graph_iteration:proc table xml contents differ");

   unlink("expected.xml");
  unlink("actual.xml");
}
END_TEST
    

Graph create_xml_test_graph()
{

    Graph g = (Graph) malloc(sizeof(struct graph));
    Node source, sink, sel, join, br, ren, act_0, act_1, act_2, act_3, act_4;

    source = make_node("p", ACT_READY, PROCESS, 0);
    PID(source) = 0;
    sel = make_node("sel", ACT_READY, SELECTION, 1);
    source -> next = sel;
    PID(sel) = 0;
    act_0 = make_node("act_0", ACT_READY, ACTION, 2);
    PID(act_0) = 0;
    sel -> next = act_0;
    br = make_node("br", ACT_READY, BRANCH, 3);
    PID(br) = 0;
    act_1 = make_node("act_1", ACT_READY, ACTION, 4);
    PID(act_1) = 0;
    act_2 = make_node("act_2", ACT_READY, ACTION, 5);
    PID(act_2) = 0;
    ren = make_node("br", ACT_READY, RENDEZVOUS, 6);
    PID(ren) = 0;
    act_3 = make_node("act_3", ACT_READY, ACTION, 7);
    PID(act_3) = 0;
    act_4 = make_node("act_4", ACT_READY, ACTION, 8);
    PID(act_4) = 0;
    join = make_node("sel", ACT_READY, JOIN, 9);
    PID(join) = 0;
    sink = make_node("p", ACT_READY, PROCESS, 10);
    PID(sink) = 0;

    act_0 -> next = br;
    br -> next = act_1;
    act_1 -> next = act_2;
    act_2 -> next = ren;
    ren -> next = act_3;
    act_3 -> next = act_4;
    act_4 -> next = join;
    join -> next = sink;
    sink -> next = NULL;

    source -> predecessors = NULL;
    source -> successors = (List) make_list(sel,NULL,NULL,NULL,NULL);
    
    sel -> predecessors = (List) make_list(source,NULL,NULL,NULL,NULL);
    sel -> successors = (List) make_list(act_0, br, act_3,NULL,NULL);
    
    act_0 -> predecessors = (List) make_list(sel, NULL,NULL,NULL,NULL);
    act_0 -> successors = (List) make_list(join, NULL,NULL,NULL,NULL);
    
    br -> predecessors = (List) make_list(sel, NULL,NULL,NULL,NULL);
    br -> successors = (List) make_list(act_1, act_2,NULL,NULL,NULL);

    act_1 -> predecessors = (List) make_list(br, NULL,NULL,NULL,NULL);
    act_1 -> successors = (List) make_list(ren, NULL,NULL,NULL,NULL);
    
    act_2 -> predecessors = (List) make_list(br, NULL,NULL,NULL,NULL);
    act_2 -> successors = (List) make_list(ren, NULL, NULL,NULL,NULL);
    
    ren -> predecessors = (List) make_list(act_1, act_2,NULL,NULL,NULL);
    ren -> successors = (List) make_list(join, NULL,NULL,NULL,NULL);
    
    act_3 -> predecessors = (List) make_list(sel, act_4,NULL,NULL,NULL);
    act_3 -> successors = (List) make_list(act_4,NULL,NULL,NULL,NULL);
    
    act_4 -> predecessors = (List) make_list(act_3, NULL,NULL,NULL,NULL);
    act_4 -> successors = (List) make_list(act_3,join,NULL,NULL,NULL);
    
    join -> predecessors = (List) make_list(act_0, ren, act_4,NULL,NULL);
    join -> successors = (List) make_list(sink,NULL, NULL,NULL,NULL);
    
    sink -> predecessors = (List) make_list(join,NULL,NULL,NULL,NULL);
    sink -> successors = ListCreate();

    sel -> matching = join;
    join -> matching = sel;

    br -> matching = ren;
    ren -> matching = br;

    g -> source = source;
    g -> sink = sink;

    return g;

}


void print_expected_xml(FILE *expected)
{
	
    fprintf(expected, "<selection>\n");
    
    fprintf(expected, "<sequence>\n");
    fprintf(expected, "<action name=\"act_0\" state=\"READY\">\n");
    fprintf(expected, "<script>\nscript\n</script>\n");
    fprintf(expected, "<req_resource name=\"r1\" value=\"r1val\" qualifier=\"abstract\"></req_resource>\n");
    fprintf(expected, "<req_resource name=\"r2\" value=\"r2val\" qualifier=\"\"></req_resource>\n");
    fprintf(expected, "<prov_resource name=\"r1\" value=\"r1val\" qualifier=\"abstract\"></prov_resource>\n");
    fprintf(expected, "<prov_resource name=\"r2\" value=\"r2val\" qualifier=\"\"></prov_resource>\n");
    fprintf(expected, "</action>\n");
    fprintf(expected, "</sequence>\n");
    
    fprintf(expected, "<sequence>\n");
    fprintf(expected, "<branch>\n");
    fprintf(expected, "<sequence>\n");
    fprintf(expected, "<action name=\"act_1\" state=\"READY\">\n");
    fprintf(expected, "<script>\nscript\n</script>\n");
    fprintf(expected, "<req_resource name=\"r1\" value=\"r1val\" qualifier=\"abstract\"></req_resource>\n");
    fprintf(expected, "<req_resource name=\"r2\" value=\"r2val\" qualifier=\"\"></req_resource>\n");
    fprintf(expected, "<prov_resource name=\"r1\" value=\"r1val\" qualifier=\"abstract\"></prov_resource>\n");
    fprintf(expected, "<prov_resource name=\"r2\" value=\"r2val\" qualifier=\"\"></prov_resource>\n");
    fprintf(expected, "</action>\n");
    fprintf(expected, "</sequence>\n");
    fprintf(expected, "<sequence>\n");
    fprintf(expected, "<action name=\"act_2\" state=\"READY\">\n");
    fprintf(expected, "<script>\nscript\n</script>\n");
    fprintf(expected, "<req_resource name=\"r1\" value=\"r1val\" qualifier=\"abstract\"></req_resource>\n");
    fprintf(expected, "<req_resource name=\"r2\" value=\"r2val\" qualifier=\"\"></req_resource>\n");
    fprintf(expected, "<prov_resource name=\"r1\" value=\"r1val\" qualifier=\"abstract\"></prov_resource>\n");
    fprintf(expected, "<prov_resource name=\"r2\" value=\"r2val\" qualifier=\"\"></prov_resource>\n");
    fprintf(expected, "</action>\n");
    fprintf(expected, "</sequence>\n");
    fprintf(expected, "</branch>\n");
    fprintf(expected, "</sequence>\n");
    
    fprintf(expected, "<sequence>\n");
    fprintf(expected, "<iteration>\n");
    fprintf(expected, "<action name=\"act_3\" state=\"READY\">\n");
    fprintf(expected, "<script>\nscript\n</script>\n");
    fprintf(expected, "<req_resource name=\"r1\" value=\"r1val\" qualifier=\"abstract\"></req_resource>\n");
    fprintf(expected, "<req_resource name=\"r2\" value=\"r2val\" qualifier=\"\"></req_resource>\n");
    fprintf(expected, "<prov_resource name=\"r1\" value=\"r1val\" qualifier=\"abstract\"></prov_resource>\n");
    fprintf(expected, "<prov_resource name=\"r2\" value=\"r2val\" qualifier=\"\"></prov_resource>\n");
    fprintf(expected, "</action>\n");
    fprintf(expected, "<action name=\"act_4\" state=\"READY\">\n");
    fprintf(expected, "<script>\nscript\n</script>\n");
    fprintf(expected, "<req_resource name=\"r1\" value=\"r1val\" qualifier=\"abstract\"></req_resource>\n");
    fprintf(expected, "<req_resource name=\"r2\" value=\"r2val\" qualifier=\"\"></req_resource>\n");
    fprintf(expected, "<prov_resource name=\"r1\" value=\"r1val\" qualifier=\"abstract\"></prov_resource>\n");
    fprintf(expected, "<prov_resource name=\"r2\" value=\"r2val\" qualifier=\"\"></prov_resource>\n");
    fprintf(expected, "</action>\n");
    fprintf(expected, "</iteration>\n");
    fprintf(expected, "</sequence>\n");
    
    fprintf(expected, "</selection>\n");
}
    

START_TEST(test_print_graph)
{
	
    int abytes, nbytes;	
    FILE *expected, *actual;	
    char expectedmem[BUFSIZ], actualmem[BUFSIZ];
    
    Graph g = (Graph) create_xml_test_graph();

    expected = fopen("expected.xml", "w");

    print_expected_xml(expected);
    

    fclose(expected);
    mark_point();

    expected = fopen("expected.xml", "r");
    memset(expectedmem, 0, BUFSIZ);
    nbytes = fread(expectedmem, sizeof(char), BUFSIZ, expected);
    fclose(expected);
    mark_point();
 
    actual = fopen("actual.xml", "w");

    print_graph(g, actual);
    fclose(actual);

    mark_point();

    actual = fopen("actual.xml", "r");
    memset(actualmem, 0, BUFSIZ);
    abytes = fread(actualmem, sizeof(char), BUFSIZ, actual);
    fail_unless(abytes == nbytes, "file size");
    fclose(actual);
    mark_point();

    fail_unless (strcmp(actualmem, expectedmem) == 0, "print_graph:proc table xml contents differ");

    unlink("expected.xml");
    unlink("actual.xml");
}
END_TEST
    
START_TEST(test_save_proc_table_xml)
{
	
    int abytes, nbytes;	
    int i;
    FILE *expected, *actual;	
    char expectedmem[BUFSIZ], actualmem[BUFSIZ];
    peos_context_t *context;
    
    Graph g = (Graph) create_xml_test_graph();

    context = &(process_table[0]);
    context->process_graph = g;
    sprintf(context->model, "test.pml");
    context->status = PEOS_RUNNING;
    for(i=1; i < PEOS_MAX_PID; i++) {
      //  context = &(process_table[i]);
      //  context -> process_graph = NULL;
	  process_table[i].process_graph = NULL;
    }
    
    unlink("proc_table.dat.xml");
    expected = fopen("expected.xml", "w");
    fprintf(expected, "<process_table>\n");
    fprintf(expected, "<process pid=\"0\" model=\"test.pml\" status=\"4\">\n");

    print_expected_xml(expected);
    
    fprintf(expected, "</process>\n</process_table>\n");

    fclose(expected);
    mark_point();

    expected = fopen("expected.xml", "r");
    memset(expectedmem, 0, BUFSIZ);
    nbytes = fread(expectedmem, sizeof(char), BUFSIZ, expected);
    fclose(expected);
    mark_point();
 

    save_proc_table_xml();

    mark_point();

    actual = fopen("proc_table.dat.xml", "r");
    memset(actualmem, 0, BUFSIZ);
    abytes = fread(actualmem, sizeof(char), BUFSIZ, actual);
    fail_unless(abytes == nbytes, "file size");
    fclose(actual);
    mark_point();

    fail_unless (strcmp(actualmem, expectedmem) == 0, "save table proc table xml contents differ");

    unlink("expected.xml");
    unlink("proc_table.dat.xml");
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
    tcase_add_test(tc, test_load_context);
    tcase_add_test(tc, test_save_context);
    tcase_add_test(tc, test_load_proc_table);
    tcase_add_test(tc, test_save_proc_table);

    tc = tcase_create("set/get resource binding/qualifier");
    suite_add_tcase(s, tc);
    tcase_add_test(tc,test_set_resource_binding);
    tcase_add_test(tc,test_set_resource_binding_overflow);
    tcase_add_test(tc,test_set_resource_binding_no_resource);
    tcase_add_test(tc, test_get_resource_binding);
    tcase_add_test(tc, test_get_resource_qualifier);

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

    tc = tcase_create("xml file");
    suite_add_tcase(s, tc);
    
    tcase_add_test(tc, test_print_action_node_0);
    tcase_add_test(tc, test_print_action_node_1);
    
    tcase_add_test(tc, test_print_after_escaping);
    tcase_add_test(tc, test_print_graph);
    tcase_add_test(tc, test_print_xml_iteration);
    tcase_add_test(tc, test_save_proc_table_xml);

    sr = srunner_create(s);

    srunner_set_fork_status(sr, fork_status);

    srunner_run_all(sr, verbosity);
    nf = srunner_ntests_failed(sr);
    srunner_free(sr);
    suite_free(s);
    return (nf == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
