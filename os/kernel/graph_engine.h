#include "pmlheaders.h"


# define MARKED(n) (((Data) (n) -> data) -> marked)
# define STATE(n) (((Data) (n) -> data) -> state)
# define ITER_START(n) (((Data) (n) -> data) -> iter_start)
# define ITER_END(n) (((Data) (n) -> data) -> iter_end)
# define ITER_END_NODES(n) (((Data) (n) -> data) -> iter_end_nodes)
# define ITER_START_NODES(n) (((Data) (n) -> data) -> iter_start_nodes)
			

typedef struct data
{
int  marked;
int iter_start;
int iter_end;
vm_act_state state;
List iter_end_nodes;
List iter_start_nodes;
} *Data;

extern vm_exit_code handle_action_change_graph(int , char *, vm_act_state);		

extern peos_resource_t *get_resource_list(char *model_file,int *num_resources);

extern peos_resource_t *get_resource_list_action(int pid,char *act_name,int *num_resources);

extern Graph makegraph(char *);

extern int annotate_graph(Graph g, peos_context_t *context);

extern int set_act_state_graph(Graph g, char *action, vm_act_state state);

extern int update_context(Graph g, peos_context_t *context);



extern void initialize_graph(Graph g);
