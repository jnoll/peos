#ifndef _GRAPH_ENGINE_H
#define _GRAPH_ENGINE_H

#include "pmlheaders.h"
#include "action.h"

/* Node mark values. */
# define TRUE (1)
# define FALSE (0)

# define MARKED_0(n) (((Data) (n) -> data) -> marked[0])
# define MARKED_1(n) (((Data) (n) -> data) -> marked[1])
# define MARKED_2(n) (((Data) (n) -> data) -> marked[2])
# define MARKED_3(n) (((Data) (n) -> data) -> marked[3])
# define MARKED_4(n) (((Data) (n) -> data) -> marked[4])
# define STATE(n) (((Data) (n) -> data) -> state)
# define ITER_START(n) (((Data) (n) -> data) -> iter_start)
# define ITER_END(n) (((Data) (n) -> data) -> iter_end)
# define ITER_END_NODES(n) (((Data) (n) -> data) -> iter_end_nodes)
# define ITER_START_NODES(n) (((Data) (n) -> data) -> iter_start_nodes)
# define ORDER(n) (((Data) (n) -> data) -> order)
# define SUPER_NODES(n) (((Data) (n) -> data) -> super_nodes)
# define PID(n) (((Data) (n) -> data) -> pid)


			
typedef struct data
{
int pid;	
int  marked[5];
int iter_start;
int iter_end;
int order;
vm_act_state state;

/* This is a list of nodes which are successors of the last node in an iteration. This list is associated with the first node in an iteration. */ 
List iter_end_nodes; 

/* This is a list of nodes which are the first nodes of iterations. This list is associated with the successors of the last node of an iteration */ 

List iter_start_nodes;

/* This is the list of nodes (selection and branch) which are the dominators of a given action node */
List super_nodes;
} *Data;



extern vm_exit_code handle_action_change(int pid , char *act_name, vm_act_state); 

extern vm_exit_code update_process_state(int pid);

extern void initialize_graph(Graph g, int pid) INIT_SECTION;

extern char *get_script_graph(Graph g, char * act_name);

extern vm_act_state get_act_state_graph(int pid, char *act_name);
#endif
