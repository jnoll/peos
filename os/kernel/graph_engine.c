#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "process.h"
#include "action.h"
#include "graph.h"
#include "pmlheaders.h"
#include "process_table.h"
#include "graph_engine.h"


void handle_selection(Node n);
void mark_successors(Node n, vm_act_state state);
void add_iteration_lists(Graph g);
void set_rendezvous_state(Node n);
vm_act_state set_node_state (Node n, vm_act_state state);

void sanitize(Graph g)
{
    Node n;
    for(n = g -> source; n != NULL; n = n -> next) {
	sanitize_node(n);
    }
}


/* 
 * Find iteration begin and end nodes.  
 * XXX this function depends on all iteration begins to appear before
 * their respective iteration ends in the node list.
 */


/* 
 * Find Iteration begin and End Nodes. If the node is a beginning of an 
 * iteration, then the flag ITER_START is set to true. If the node is at the 
 * end of an iteration, then the flag ITER_END will be set to true. 
 * All the nodes in the node list are arranged in lexicographic order. 
 * We start with the first node after the source and mark it. If a node 
 * has its predecessor which is not marked, then the predecessor is 
 * the end of an iteration. Similarly, if the node has a successor 
 * which is marked, then the successor is the start of an iteration 
 *
 */

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

/* 
 * At the beginning of an iteration, the first action(or a selection or a 
 * branch) in an iteration and the action following that iteration will be 
 * ready. When an iteration starts, we need to set the action 
 * (or selection or branch) following that iteration to ACT_NONE and 
 * vice versa. So there is a list of nodes associated with each action. 
 * The list ITER_END_NODES will be associated with a starting node of 
 * an iteration and it contains the list of nodes following the iteration 
 * for which this node is a start node.There can be more than one 
 * iterations with a given node as a start node and hence this list can 
 * have more than one node. Similarly, the list ITER_START_NODES is 
 * associated with contains the list of all nodes which are the 
 * starting nodes of the iterations with this node as the node following 
 * that iteration. And since more than one iteration can end at a given 
 * node, this is also a list of nodes rather than a single node. This function 
 * just makes these lists for each node.
 *
 */

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

/* 
 * Every action node will also have a list of super nodes associated with it. 
 * These nodes are the control flow nodes (selection or branch) under whose 
 * influence the given action node falls. For example for the folowing 
 * PML program: 
 * selection s1 {
 *   selection s2 {
 *      action a {} -- super nodes : s1,s2
 *      action b {} -- super nodes : s1.s2
 *      }
 *   action c {}  -- super nodes: s1
 *   }
 * 
 * This function adds these lists.
 *
 */
				
void add_super_node_lists(Graph g)
{
    Node n;

    for(n = g -> source -> next; n != NULL; n = n -> next) {
        if((n -> type == SELECTION) || (n -> type == BRANCH)) {
            Node slave;
	    for(slave = n -> next; slave != n -> matching; slave = slave -> next) {
	        if(slave -> type  == ACTION) {
		    ListPut(SUPER_NODES(slave),n);
		}
	    }
	}
    }
}
				    

/*
 * When a action gets set to run, it has to be checked if that action is 
 * part of an iteration. If yes, then the nodes, following that 
 * iteration should be set to none. This function does that. Also we have 
 * to do this recursively, because there can be iterations within iterations.
 *
 */


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

/*
 * When a node is set to ready and that node is the start node of an iteration,
 * then all the nodes following that iteration should also be ready. The 
 * first part of this function does that. Also, when a node is set to ACT_RUN 
 * and the node is  a start node of an iteration, then all the nodes following 
 * that iteration are set to none. And if the node is the end of an iteration, 
 * then the start of that iteration is to be set to NONE. This is done by 
 * calling the set_iter_none(node,original) function. The original node is 
 * the same node when called for the first time. This to prevent that node 
 * from being set to ACT_NONE, since it can be a member of ITER_START_NODES 
 * and ITER_END_NODES lists of other nodes.  
 *
 */

void mark_iter_nodes(Node n)
{
    Node iter_end_node;
    int i;
    if((STATE(n) == ACT_READY) || (STATE(n) == ACT_BLOCKED)){
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

/*
 * This function sets all the super nodes of an action node to RUN. Every 
 * time a node is set to ACT_RUN, iterations have to be handled. Hence the 
 * call to mark_iter_nodes(..) after setting the state to ACT_RUN.
 *
 */

void set_super_nodes_run(Node n)
{
    Node super;
    int i;
    for(i = 0; i <  ListSize(SUPER_NODES(n)); i++) {
        super = (Node) ListIndex(SUPER_NODES(n),i);
	set_node_state(super, ACT_RUN);
	set_node_state(super -> matching, ACT_RUN);
	mark_iter_nodes(super);
    }
}

/* 
 * This function marks the state of a given action/construct to the specified 
 * state. If the given node is a control flow construct, then it recursively 
 * traverses the successors of that node until it finds an action node and 
 * sets it state  to the state passed as an argument. After setting the 
 * state to ACT_RUN mark_iter_nodes is called to handle iterations.
 *
 */

void mark_successors(Node n, vm_act_state state)
{
    int i;
    Node child;
    if (n -> type == ACTION) {
	set_node_state(n, state);    
	mark_iter_nodes(n);
	return;
    }
    else
    if((n -> type == BRANCH) || (n -> type == SELECTION) || (n -> type == JOIN))
    {
        if((n->type == BRANCH) || (n->type == SELECTION)) {
	    set_node_state(n, state);	
	    mark_iter_nodes(n);
	    set_node_state(n -> matching,state);
	}
	for(i = 0; i < ListSize(n -> successors); i++) {
	    child = (Node) ListIndex(n -> successors, i);
	    mark_successors(child,state);
	}
    }
    else
        return;
}

/* 
 * 
 * This function is called after an action is done and the node following it 
 * is a join node. It sets the join and the selection to ACT_DONE and since
 * this join can be followed by another join (for selection within selection)
 * or a rendezvous (for selection within branch), it recursively calls itself
 * and then calls the function to set the state of the rendezvous.
 *
 */


void propogate_join_done(Node n)
{
    int i;
    Node child;
    if (n -> type == JOIN) {
	set_node_state(n, ACT_DONE);    
        set_node_state(n -> matching, ACT_DONE);
	for(i = 0; i < ListSize(n->successors); i++) {
	    child = (Node) ListIndex(n->successors,i);
	    propogate_join_done(child);
	    set_rendezvous_state(child);
	}
    }
    else
        return;
}
		

/*
 * 
 * Same as propogate_join_done(..) except that the rendezvous will not be 
 * done before all the nodes in that branch are done.
 *
 */

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
	     set_node_state(n, ACT_DONE);
             set_node_state(n -> matching, ACT_DONE);	     
	     for(i = 0; i< ListSize(n -> successors); i++) {
	         child = (Node) ListIndex(n -> successors,i);
		 if(child -> type == JOIN) {
	             propogate_join_done(child);
		 }
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
        STATE(g -> sink) =  ACT_DONE;	
    }
}


int action_run(Graph g, char *act_name)
{
    Node n;

    n = find_node(g, act_name);
    if(n != NULL) {
	set_node_state(n, ACT_RUN);    
	mark_iter_nodes(n);  /* handle iterations */
	handle_selection(n); /* handle selections */
	set_super_nodes_run(n); /*set super nodes to run */
        sanitize(g); /* sanitize the markers used */
    }
    else {
        fprintf(stderr, "Error in run_action");
	return -1;
    }
    return 1;
}

/* 
 * This function handles selections. When an action within a selection is set 
 * to ACT_RUN, then all the siblings have to be set to ACT_NONE. Also, there 
 * can be recursive selections. So this function has to be recursive.  
 *
 */

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
            for(j=0; j < ListSize(parent -> successors); j++) {
                child = (Node) ListIndex(parent -> successors,j);
                if(strcmp((child->name),n->name) != 0) {
                    mark_successors(child,ACT_NONE);
                }
	     }
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
	set_node_state(n, ACT_DONE);    
	num_successors = ListSize(n -> successors);
	for(i = 0; i < num_successors; i++) {
	    child = (Node) ListIndex(n -> successors, i);
	    /*
	     * (num_successors == 1) is a check to see that it is 
	     * not an iteration.
	     */
	    if((child -> type == JOIN) && (num_successors == 1)) {
	        propogate_join_done(child);
	    }
	    if(child -> type != RENDEZVOUS) {
		/* 
		 * if a child is not a rendezvous or a join, it has to 
		 * be a selection or branch or action, so mark it ready. If 
		 * its sink, that is handled again by set_process_state(..) 
		 */
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
	sanitize_node(n);
	STATE(n) = ACT_NONE;
	RESOURCE_STATE(n) = REQUIRES_FALSE;
        ORDER(n) = i;
        i++;
        ITER_START(n) = FALSE;
        ITER_END(n) = FALSE;
        ITER_START_NODES(n) = ListCreate();
        ITER_END_NODES(n) = ListCreate();
	SUPER_NODES(n) = ListCreate();
    }
    add_super_node_lists(g); /* add the node lists */
    mark_for_iteration(g); /* mark beginning and end of iterations */
    add_iteration_lists(g);  /* add the iteration lists */
    sanitize(g); /* sanitize markers */
    mark_successors(g->source->next,ACT_READY);
}


vm_exit_code handle_resource_event(int pid, char *action, vm_resource_state state)
{
    Graph g;
    Node n;
    peos_context_t *context = peos_get_context(pid);
    
    g = context -> process_graph;
    if(g == NULL) {
        fprintf(stderr,"Handle Action Error: Unable to find graph");
	return VM_INTERNAL_ERROR;
    }

    n = find_node(g,action);

    if(n != NULL) {
	if(state == REQUIRES_TRUE) {
            if(STATE(n) == ACT_BLOCKED) {
	        RESOURCE_STATE(n) = state;
	        set_node_state(n, ACT_READY);
	        return VM_CONTINUE;
	    }
	    else {
	        RESOURCE_STATE(n) = state;
	        return VM_CONTINUE;
	    }
        }
        else {
            if(state == PROVIDES_TRUE) {
	        if((STATE(n) == ACT_READY) || (STATE(n) == ACT_RUN) || (STATE(n) == ACT_PENDING)) {
	            RESOURCE_STATE(n) = PROVIDES_TRUE;
		    handle_action_change(pid, n->name, ACT_RUN);
		    return handle_action_change(pid, n->name, ACT_DONE);
		}
	    }
	    else return VM_CONTINUE;
	}
    }
    else
        return VM_INTERNAL_ERROR;
}


vm_act_state set_node_state(Node n, vm_act_state state)
{
    vm_act_state state_set = state;
    
    if(n->type != ACTION) {
        STATE(n) = state_set;
	return state_set;
    }

    switch(state_set) {
        
        case(ACT_READY) : {
			      if ((RESOURCE_STATE(n) == REQUIRES_TRUE) || (RESOURCE_STATE(n) == PROVIDES_TRUE)) {
			          STATE(n) = state_set;
		                  return state_set;
			      }
	                      else {
		                  if (RESOURCE_STATE(n) == REQUIRES_FALSE) {
		                      state_set = ACT_BLOCKED;
				      STATE(n) = state_set;
	                              return state_set;
				  }
				  else {
				      return STATE(n);
				  }
			      }
			      break;
			  }
        case(ACT_DONE) : {
			     if(RESOURCE_STATE(n) == PROVIDES_TRUE) {
			         STATE(n) = state_set;
				 return state_set;
			     }
			     else {
			         state_set = ACT_PENDING;
				 STATE(n) = state_set;
				 return state_set;
			     }
			     break;
			 }
			 
			  
	default: {
		     STATE(n) = state_set;
		 }
			      				  
    }
    return state_set;
}
	    

vm_exit_code handle_action_change(int pid, char *action, vm_act_state state)
{
    Graph g;
    char msg[256], *this_state;
    vm_exit_code exit_status;
       
    peos_context_t *context = peos_get_context(pid);

    this_state = act_state_name(state);
    sprintf(msg, "jnoll %s %s %d ", this_state, action, pid);
    log_event(msg);

    g = context -> process_graph;
    if(g == NULL) {
        fprintf(stderr,"Handle Action Error: Unable to find graph");
        return VM_INTERNAL_ERROR;
    }

    if ((exit_status =  set_act_state_graph(g, action, state)) == VM_DONE) {
        sprintf(msg,"jnoll DONE %s %d", context->model, pid);
        log_event(msg);
        delete_entry(context->pid);
        context->status = PEOS_DONE;
	return exit_status;
    }
    
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
				      set_node_state(n, ACT_READY);
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
				       set_node_state(n, ACT_NONE);
				       return VM_CONTINUE;
				   }
				   else
				       return VM_INTERNAL_ERROR;
	                        }
 
            case(ACT_SUSPEND) : {
	                            Node n = find_node(g,action);
				    if (n != NULL) {
					set_node_state(n, ACT_SUSPEND);    
				        return VM_CONTINUE;
				    }
				    else
				        return VM_INTERNAL_ERROR;
				}
			         
             case(ACT_ABORT) : {
			           Node n = find_node(g,action);
				   if (n != NULL) {
				       set_node_state(n, ACT_ABORT);	   
			               return VM_CONTINUE;
				   }
				   else
				       return VM_INTERNAL_ERROR;
	                        }
               
              case(ACT_NEW) :   {
				    Node n = find_node(g,action);
				    if (n != NULL) {
					set_node_state(n, ACT_NEW);    
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
