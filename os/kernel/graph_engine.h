#ifndef _GRAPH_ENGINE_H
#define _GRAPH_ENGINE_H

#include "pmlheaders.h"



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
			
typedef struct data
{
int  marked[5];
int iter_start;
int iter_end;
int order;
vm_act_state state;
List iter_end_nodes;
List iter_start_nodes;
} *Data;


extern vm_exit_code handle_action_change_graph(int , char *, vm_act_state);		
extern peos_resource_t *get_resource_list(char *model_file,int *num_resources);

extern peos_resource_t *get_resource_list_action(int pid,char *act_name,int *num_resources);

extern peos_resource_t *get_resource_list_action_requires(int pid,char *act_name,int *num_resources);

extern peos_resource_t *get_resource_list_action_provides(int pid,char *act_name,int *num_resources);

extern Graph makegraph(char *);

extern int annotate_graph(Graph g, peos_action_t *actions, int num_actions,peos_other_node_t *other_nodes, int num_other_nodes);

extern vm_exit_code set_act_state_graph(Graph g,char *act_name, vm_act_state state);

extern void initialize_graph(Graph g);

extern char *get_script_graph(Graph g, char * act_name);

#endif
