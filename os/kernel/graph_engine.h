#include "pmlheaders.h"


# define MARKED(n) (((Data) (n) -> data) -> marked)
# define STATE(n) (((Data) (n) -> data) -> state)
			

typedef struct data
{
char * marked;
vm_act_state state;
} *Data;

extern vm_exit_code handle_action_change_graph(int , char *, vm_act_state);		

extern Graph makegraph(char *);

extern int annotate_graph(Graph g, peos_context_t *context);

extern int set_act_state_graph(Graph g, char *action, vm_act_state state);

extern int update_context(Graph g, peos_context_t *context);



extern void initialize_graph(Graph g);
