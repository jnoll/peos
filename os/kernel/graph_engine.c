#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "process.h"
#include "action.h"
#include "pmlheaders.h"
#include "process_table.h"
#include "graph_engine.h"

# define TRUE 1
# define FALSE 0



void handle_selection(Node n);
void mark_successors(Node n, vm_act_state state);
void add_iteration_lists(Graph g);


int  annotate_graph(Graph g, peos_action_t *actions, int num_actions, peos_other_node_t *other_nodes, int num_other_nodes)
{
    int i;
    Node node;
                                                                      
    for(node = g -> source; node != NULL; node = node -> next) {
        if (node -> type == ACTION) {
            STATE(node) = get_act_state(node -> name,actions,num_actions);
	}
	else {
	    if((node->type == SELECTION) || (node->type == BRANCH)) {
	        for(i=0;i < num_other_nodes; i++) {
	            if (strcmp(node->name,other_nodes[i].name)==0) {
	                STATE(node) = other_nodes[i].state;
	                STATE(node->matching) = other_nodes[i].state;
	            }
	        }
	     }
	 }
    }
	
    return 1;   
}

void sanitize(Graph g)
{
    Node n;
    for(n = g -> source; n != NULL; n = n -> next) {
        MARKED_0(n) = FALSE;
	MARKED_1(n) = FALSE;
	MARKED_2(n) = FALSE;
	MARKED_3(n) = FALSE;
	MARKED_4(n) = FALSE;
    }
}


Graph makegraph(char *file)
{
    filename = file;
    lineno = 1;
    yyin = NULL;
    yyin = fopen(filename, "r");
	
    if(yyin == NULL) {
        return NULL;
    }
	
    if(yyparse()) {
        return NULL;
    }
        
    else {	
        if(yyin) {
            fclose (yyin);
	    if(program != NULL) {
	        initialize_graph(program);
	        return program;
	    }
	    else
	        return NULL;
	}
	else
	    return NULL;
     }
}

void mark_for_iteration(Graph g)
{
    Node node,parent,child;
    int i,k;

    MARKED_0(g -> source) = TRUE;
    for(node = g -> source->next; node != NULL; node = node -> next) {
        for(i = 0; i < ListSize(node -> predecessors); i++) {
            parent = (Node) ListIndex(node -> predecessors,i);
            if (MARKED_0(parent) == FALSE) {
	        ITER_START(node) = TRUE;
	    }
	}

	MARKED_0(node) = TRUE;
        for(k = 0; k < ListSize(node -> successors); k++) {
            child = (Node) ListIndex(node -> successors,k);
            if(MARKED_0(child) == TRUE) {
	        ITER_END(node) = TRUE;
	    }
	}
    }

}


void add_iteration_lists(Graph g)
{
    Node node,child,parent,child1,child2;
    int i,j,k,l;

    MARKED_1(g -> source) = TRUE;

    for(node = g -> source->next; node != NULL; node = node -> next) {
        for(i = 0; i < ListSize(node -> predecessors); i++) {
	    parent = (Node) ListIndex(node -> predecessors,i);
	    if (MARKED_1(parent) == FALSE) {
	        for(j=0; j < ListSize(parent -> successors); j++) {
		    child = (Node) ListIndex(parent->successors,j);
		    if((strcmp(child->name,node->name) != 0) && (ORDER(child) > ORDER(parent)))
		    {
                         ListPut(ITER_END_NODES(node),child);
		    }
		}
	     }
	}
	MARKED_1(node) = TRUE;
        for(k = 0; k < ListSize(node -> successors); k++) {
            child1 = (Node) ListIndex(node -> successors,k);
	    if(MARKED_1(child1) == TRUE) {
	        for(l = 0; l < ListSize(node -> successors); l++) {
	            child2 = (Node) ListIndex(node -> successors,l);
		    if (MARKED_1(child2) == FALSE) {
		        ListPut(ITER_START_NODES(child2),child1);
		    }
	        }
	    }
	 }
    }
}
				

Node find_node(Graph g, char *node_name)
{
    Node n;
    for(n = g -> source; n!= NULL; n = n -> next) {
        if((n -> name) && (n -> type == ACTION)) {
            if (strcmp((n -> name),node_name) == 0) {
	        return n;
	    }
	}
    }
    return NULL;
}	

void insert_resource(char *id, peos_resource_t **rlist, int *num_resources, int *rsize) 
{
    int i = 0;
    peos_resource_t *resource_list = *rlist;

    while((i < *num_resources) && (strcmp(resource_list[i].name, id)) != 0) {
        i++;
    }
    if (i == *num_resources) 
    {
        if(*num_resources == *rsize) {
	    *rsize = *rsize + INST_ARRAY_INCR;
	    resource_list = realloc(resource_list,(*rsize) * sizeof(peos_resource_t));
	}
	strcpy(resource_list[*num_resources].name, id);
	strcpy(resource_list[*num_resources].value,"\0");
	*num_resources = *num_resources + 1;
    }
    *rlist = resource_list;
}

void make_resource_list(Tree t, peos_resource_t **rlist, int *num_resources, int *rsize)
{
    peos_resource_t *resource_list = *rlist;
    if(t) {
	if (IS_OP_TREE(t)) {
	    switch TREE_OP(t) {
	    case DOT: 
	    case EQ: 
	    case NE:
	    case GE:
	    case LE:
	    case LT:
	    case GT: {
		make_resource_list(t->left, &resource_list, num_resources, rsize);
	    }
	    break;
	    case QUALIFIER: {
		make_resource_list(t->right, &resource_list, num_resources, rsize);
	    }
	    break;
	    }
	} else if (IS_ID_TREE(t)) {
	    insert_resource(TREE_ID(t), &resource_list, num_resources, rsize);
	} else {                       
	    make_resource_list(t->left, &resource_list,num_resources, rsize);
	    make_resource_list(t->right, &resource_list,num_resources, rsize);
	}
    }
    *rlist = resource_list;
    return;
}

peos_resource_t *get_resource_list_action_requires(int pid, char *act_name, int *total_resources)
{
    Graph g;
    Node n;
    int i,j;
    int rsize = 256;
    int num_resources = 0;
    peos_context_t *context = peos_get_context(pid);
    peos_resource_t *proc_resources = context -> resources;
    int num_proc_resources = context -> num_resources;
    char *model_file;

    peos_resource_t *act_resources = (peos_resource_t *) calloc(rsize,sizeof(peos_resource_t));

    model_file = context->model;
    g = context -> process_graph;
    if(g != NULL) {
        n = find_node(g,act_name);
        if(n == NULL) {
            fprintf(stderr,"get_resource_list_action :cannot find action");
	    return NULL;
	}
	make_resource_list(n -> requires, &act_resources, &num_resources, &rsize);
	*total_resources = num_resources;
        for(i = 0; i < num_resources; i++) {
            for(j = 0; j < num_proc_resources; j++) {
	        if(strcmp(act_resources[i].name,proc_resources[j].name) == 0) {
	            strcpy(act_resources[i].value,proc_resources[j].value);
		    break;
		}
	    }
	}

        return act_resources;
    }    
    else
        return NULL;
}

peos_resource_t *get_resource_list_action_provides(int pid, char *act_name, int
		*total_resources)
{
    Graph g;
    Node n;
    int i,j;
    int rsize = 256;
    int num_resources = 0;
    peos_context_t *context = peos_get_context(pid);
    peos_resource_t *proc_resources = context -> resources;
    int num_proc_resources = context -> num_resources;
    char *model_file;
				                                                                                   
    peos_resource_t *act_resources = (peos_resource_t *) calloc(rsize,sizeof(peos_resource_t));
                                                                                    
    model_file = context->model;
    g = context -> process_graph;   
    if(g != NULL) {
        n = find_node(g,act_name);
        if(n == NULL) {
            fprintf(stderr,"get_resource_list_action :cannot find action");
            return NULL;
        }
	make_resource_list(n -> provides, &act_resources, &num_resources, &rsize);
        *total_resources = num_resources;
        for(i = 0; i < num_resources; i++) {
            for(j = 0; j < num_proc_resources; j++) {
                if(strcmp(act_resources[i].name,proc_resources[j].name) == 0) {
                    strcpy(act_resources[i].value,proc_resources[j].value);
                    break;
                }
            }
        }
        return act_resources;
    }
    else
        return NULL;
}


peos_resource_t *get_resource_list_action(int pid, char *act_name, int *total_resources)
{
    Graph g;
    Node n;
    int i,j;
    int rsize = 256;
    int num_resources = 0;
    peos_context_t *context = peos_get_context(pid);
    peos_resource_t *proc_resources = context -> resources;
    int num_proc_resources = context -> num_resources;
    char *model_file;
    peos_resource_t *act_resources = (peos_resource_t *) calloc(rsize,sizeof(peos_resource_t));

    model_file = context->model;

    g = context -> process_graph;
    if(g != NULL) {
        n = find_node(g,act_name);
	if(n == NULL) {
	    fprintf(stderr,"get_resource_list_action :cannot find action");
	    return NULL;
	}
	make_resource_list(n -> requires, &act_resources,&num_resources, &rsize);
	make_resource_list(n -> provides, &act_resources,&num_resources, &rsize);
	*total_resources = num_resources;
	for(i = 0; i < num_resources; i++) {
	    for(j = 0; j < num_proc_resources; j++) {
 	        if(strcmp(act_resources[i].name,proc_resources[j].name) == 0) {
		    strcpy(act_resources[i].value,proc_resources[j].value);
		    break;
		}
	    }
	}

        return act_resources;
    }
    else 
        return NULL;
}

	
// this function is used to get the list of resources for the whole process
peos_resource_t *get_resource_list(char *model, int *total_resources)
{
    int rsize = 256;
    Graph g;
    Node n;
    int num_resources = 0;
    peos_resource_t *resource_list = (peos_resource_t *) calloc(rsize,sizeof(peos_resource_t));

    g = makegraph(model);

    if(g != NULL) {	
        for(n = g->source->next; n != NULL; n = n -> next) {
	    if(n -> type == ACTION) {
                make_resource_list(n->requires, &resource_list, &num_resources, &rsize);
		make_resource_list(n->provides, &resource_list, &num_resources, &rsize);
	    }
	}
	*total_resources = num_resources;
	return resource_list;
    }
    else
        return NULL;
}


void set_iter_none(Node n, Node original)
{
    Node iter_start_node,iter_end_node;
    int i;

    for(i = 0; i < ListSize(ITER_START_NODES(n)); i++) {
        iter_start_node = (Node) ListIndex(ITER_START_NODES(n),i);
        if((iter_start_node->type == SELECTION) || (iter_start_node->type == BRANCH) ||(iter_start_node->type == ACTION)) {
	    if((strcmp(iter_start_node -> name, original -> name) != 0) && (MARKED_2(iter_start_node) == FALSE)) {
	        MARKED_2(iter_start_node) = TRUE;
                mark_successors(iter_start_node,ACT_NONE);
                set_iter_none(iter_start_node,original);
	    }
        }
    }


    for(i = 0; i < ListSize(ITER_END_NODES(n)); i++) {
        iter_end_node = (Node) ListIndex(ITER_END_NODES(n),i);
	if((iter_end_node->type == SELECTION) || (iter_end_node->type == BRANCH) ||(iter_end_node->type == ACTION)) {
	    if((strcmp(iter_end_node -> name, original -> name) != 0) && (MARKED_2(iter_end_node) == FALSE)) {	     
	        MARKED_2(iter_end_node) = TRUE;
	        mark_successors(iter_end_node,ACT_NONE);
	        set_iter_none(iter_end_node,original);
	    }
	}
    }
	
}


void mark_iter_nodes(Node n)
{
    Node iter_end_node;
    int i;
    if(STATE(n) == ACT_READY) {
        for(i = 0; i <  ListSize(ITER_END_NODES(n)); i++) {
            iter_end_node = (Node) ListIndex(ITER_END_NODES(n),i);
	    if((iter_end_node->type == SELECTION) || (iter_end_node->type == BRANCH) ||(iter_end_node->type == ACTION)) {
	        mark_successors(iter_end_node,ACT_READY);
	    }
	}
    }
    else {
        if(STATE(n) == ACT_RUN)	{
	    set_iter_none(n,n);
	}
    }
}


void mark_successors(Node n, vm_act_state state)
{
    int i;
    Node child;
    if (n -> type == ACTION) {
        STATE(n) = state;
	mark_iter_nodes(n);
	return;
    }
    else
    if((n -> type == BRANCH) || (n -> type == SELECTION) || (n -> type == JOIN))
    {
        if((n->type == BRANCH) || (n->type == SELECTION)) {
	    STATE(n) = state;
	    mark_iter_nodes(n);
	    STATE(n->matching) = state;
	}
	for(i = 0; i < ListSize(n -> successors); i++) {
	    child = (Node) ListIndex(n -> successors, i);
	    mark_successors(child,state);
	}
    }
    else
        return;
}
			       	
void propogate_join_done(Node n)
{
    int i;
    Node child;
    if (n -> type == JOIN) {
        STATE(n) = ACT_DONE;
	STATE(n -> matching) = ACT_DONE;
	for(i = 0; i < ListSize(n->successors); i++) {
	    child = (Node) ListIndex(n->successors,i);
	    propogate_join_done(child);
	}
    }
    else
        return;
}
		

void set_rendezvous_state(Node n)
{
    int i;
    Node child,parent;
    int status = 1;
    if (n -> type == RENDEZVOUS) {
        for(i = 0; i < ListSize(n -> predecessors); i++) {
	    parent = (Node) ListIndex(n -> predecessors,i);
	    if (STATE(parent) != ACT_DONE) {
	        status = 0;
	    }
	 }
	 if (status == 1) {
	     STATE(n) = ACT_DONE;
	     STATE(n -> matching) = ACT_DONE;
	     for(i = 0; i< ListSize(n -> successors); i++) {
	         child = (Node) ListIndex(n -> successors,i);
		 mark_successors(child,ACT_READY);
		 set_rendezvous_state(child);
	     }
	  }
    }
    else
        return;
}
	
void set_process_state(Graph g)
{
    Node parent;
    int i;
    int status = 1;

    for(i = 0; i < ListSize(g -> sink -> predecessors); i++) {
        parent = (Node) ListIndex(g -> sink -> predecessors,i);
	if (STATE(parent) != ACT_DONE)
	    status = 0;
    }
    
    if (status == 1) {
        STATE(g -> source) = ACT_DONE;
	STATE(g -> sink) = ACT_DONE;
    }
}


int action_run(Graph g, char *act_name)
{
    Node n;

    n = find_node(g, act_name);
    if(n != NULL) {
        STATE(n) = ACT_RUN;
	mark_iter_nodes(n);
	handle_selection(n);
        sanitize(g);
    }
    else {
        fprintf(stderr, "Error in run_action");
	return -1;
    }
    return 1;
}



void handle_selection(Node n)
{
    int i,j;
    Node parent;
    Node child;
 
    if ((n -> predecessors == NULL) || (MARKED_3(n) == TRUE))
        return;
		                                                                         
    MARKED_3(n) = TRUE;
 
    for(i = 0; i < ListSize(n -> predecessors); i++) {
        parent = (Node) ListIndex(n -> predecessors,i);
        if ((parent -> type) == SELECTION) {
	    STATE(parent) = ACT_RUN;
	    mark_iter_nodes(parent);
            STATE(parent -> matching) = ACT_RUN;	 
            for(j=0; j < ListSize(parent -> successors); j++) {
                child = (Node) ListIndex(parent -> successors,j);
                if(strcmp((child->name),n->name) != 0) {
                    mark_successors(child,ACT_NONE);
                }
	     }
	}
        else
	    if (parent -> type == BRANCH) {
	        STATE(parent) = ACT_RUN;
	        mark_iter_nodes(parent);
	        STATE(parent->matching) = ACT_RUN;
	    }
            if(ORDER(n) >  ORDER(parent))  
            handle_selection(parent);
    }
    return;
}
					
vm_exit_code action_done(Graph g, char *act_name)
{
    Node n;
    Node child;
    int i,num_successors;

    n = find_node(g,act_name);
    if(n != NULL) {
        STATE(n) = ACT_DONE;
	num_successors = ListSize(n -> successors);
	for(i = 0; i < num_successors; i++) {
	    child = (Node) ListIndex(n -> successors, i);
	    if((child -> type == JOIN) && (num_successors == 1)) {
	        propogate_join_done(child);
	    }
	    if(child -> type != RENDEZVOUS) {
	        mark_successors(child, ACT_READY);
	    }
	    else {
	        if(num_successors == 1)	
		set_rendezvous_state(child);
	    }
	}	    
	if (num_successors == 1)
	    set_process_state(g);
    }
    else {
        fprintf(stderr, "Error in action_done");
	return VM_INTERNAL_ERROR;
    }
    if(STATE(g->source) == ACT_DONE) {
        return VM_DONE;
    }
    else
        return VM_CONTINUE;
 
}

char *get_script_graph(Graph g, char *action_name)
{
    Node n;
    n = find_node(g,action_name);
    if (n == NULL) {
        fprintf(stderr,"\n Error : get_script action node not found\n");
        return NULL;
    }	
    else
        return(n -> script ? n -> script : "(no script)");
}

void initialize_graph(Graph g)
{
    Node n;
    int i = 0;
        
    for(n = g -> source; n != NULL; n = n -> next) {
        n -> data = (void *) malloc (sizeof (struct data));
        MARKED_0(n) = FALSE;
        MARKED_1(n) = FALSE;
        MARKED_2(n) = FALSE;
        MARKED_3(n) = FALSE;
        MARKED_4(n) = FALSE;
        STATE(n) = ACT_NONE;
        ORDER(n) = i;
        i++;
        ITER_START(n) = FALSE;
        ITER_END(n) = FALSE;
        ITER_START_NODES(n) = ListCreate();
        ITER_END_NODES(n) = ListCreate();
    }
    mark_for_iteration(g);
    sanitize(g);
    add_iteration_lists(g);
    sanitize(g);
    mark_successors(g->source->next,ACT_READY);
}


vm_exit_code handle_action_change_graph(int pid, char *action, vm_act_state state)
{
    Graph g;
    vm_exit_code exit_status;
    
    peos_context_t *context = peos_get_context(pid);

    g = context -> process_graph;
    if(g == NULL) {
        fprintf(stderr,"Handle Action Error: Unable to find graph");
        return VM_INTERNAL_ERROR;
    }
    exit_status = set_act_state_graph(g,action,state);
    if(exit_status == VM_INTERNAL_ERROR) {  
        fprintf(stderr, "Handle Action Error: Unable to change action state");
        return exit_status;
    }
                                                                
    return exit_status;
}


vm_exit_code set_act_state_graph(Graph g, char *action, vm_act_state state)
{
	switch(state)
	{
       	    case(ACT_DONE) :  {
			          return action_done(g,action); 
			      }
				     		      
	    case(ACT_READY) : {
			          Node n = find_node(g,action);
				  if (n!=NULL) {
				      STATE(n) = ACT_READY;
				      return VM_CONTINUE;
				  }
				  else 
				      return VM_INTERNAL_ERROR;
		               }
				  
	     case(ACT_RUN) :   {
	                           if(action_run(g,action) >  0)
				       return VM_CONTINUE;
				   else
				       return VM_INTERNAL_ERROR; 
			       }

             case(ACT_NONE) :  {
			           Node n = find_node(g,action);
				   if (n != NULL) {
			               STATE(n) = ACT_NONE;	 
		                       return VM_CONTINUE;
				   }
				   else
				       return VM_INTERNAL_ERROR;
	                        }
 
            case(ACT_SUSPEND) : {
	                            Node n = find_node(g,action);
				    if (n != NULL) {
				        STATE(n) = ACT_SUSPEND;
				        return VM_CONTINUE;
				    }
				    else
				        return VM_INTERNAL_ERROR;
				}
			         
             case(ACT_ABORT) : {
			           Node n = find_node(g,action);
				   if (n != NULL) {
			               STATE(n) = ACT_ABORT;	 
	                               return VM_CONTINUE;
				   }
				   else
				       return VM_INTERNAL_ERROR;
	                        }
               
              case(ACT_NEW) :   {
				    Node n = find_node(g,action);
				    if (n != NULL) {
				        STATE(n) = ACT_NEW;
				        return VM_CONTINUE;
				    }
				    else
				        return VM_INTERNAL_ERROR;
					 
                                 }
                
	       default : {
	                     fprintf(stderr, "Error Changing Action : Invalid Action State\n");
		             return -1;
		         }
	}
	
}
# ifdef UNIT_TEST
#include "test_graph_engine.c"
#endif
