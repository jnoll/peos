#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "action.h"
#include "pmlheaders.h"
#include "vm.h"
#include "process_table.h"
#include "graph_engine.h"

# define TRUE 1
# define FALSE 0


# define ACTION 257
# define BRANCH 259
# define RENDEZVOUS 286
# define SELECTION 270
# define JOIN 285
# define PROCESS 266

void handle_selection(Node n);
void mark_successors(Node n, vm_act_state state);

int  annotate_graph(Graph g, peos_context_t *context)
{
	
  Node node;

  for(node = g -> source; node != NULL; node = node -> next)
   {
    if (node -> type == ACTION)
     {
        MARKED(node) = FALSE;
	STATE(node) = get_act_state(node -> name,context -> actions, context -> num_actions); 
     }
   }

return 1;   
}

void sanitize(Graph g)
{
	Node n;
	for(n = g -> source; n != NULL; n = n -> next)
	{
		MARKED(n) = FALSE;
	}
}


Graph makegraph(char *file)
{
	filename = file;
	lineno = 1;
	yyin = fopen(filename, "r");
	yyparse();
	if(yyin)
	{
	fclose (yyin);
	if(program != NULL)
	{
	initialize_graph(program);
	}
	return program;
	}
	else
		return NULL;
}
	

Node find_node(Graph g, char *node_name)
{
	Node n;
	for(n = g -> source; n!= NULL; n = n -> next)
	{
	  if((n -> name) && (n -> type == ACTION))
	  {
	    if (strcmp((n -> name),node_name) == 0)
	    {
	      return n;
	    }
	  }
	}
        return NULL;
}	

void mark_successors(Node n, vm_act_state state)
{
	int i;
	Node child;
	if (n -> type == ACTION)
	{
		STATE(n) = state;
		return;
	}
	else
		if((n -> type == BRANCH) || (n -> type == SELECTION) || (n -> type == JOIN))
		{
			for(i = 0; i < ListSize(n -> successors); i++)
			{
				child = (Node) ListIndex(n -> successors, i);
				mark_successors(child,state);
			}
		}
	        else
			return;
}
			       	

int action_run(Graph g, char *act_name)
{
	Node n;
	
	n = find_node(g, act_name);
	if(n != NULL)
	{
		STATE(n) = ACT_RUN;
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
 if ((n -> predecessors == NULL) || (MARKED(n) == TRUE))
        return;
					                                                                         
 MARKED(n) = TRUE;
 for(i = 0; i < ListSize(n -> predecessors); i++)
   {
      parent = (Node) ListIndex(n -> predecessors,i);
      if ((parent -> type) == SELECTION)
        {
         for(j=0; j < ListSize(parent -> successors); j++)                                {
											    child = (Node) ListIndex(parent -> successors,j);
											    if(strcmp((child->name),n->name) != 0)
											     {
											      mark_successors(child,ACT_NONE);
											     }
											 }
										     }
										     handle_selection(parent);
										   }
}








					
int action_done(Graph g, char *act_name)
{
	Node n;
	Node child;
	int i;

	n = find_node(g,act_name);
	if(n != NULL)
	{
		STATE(n) = ACT_DONE;
		for(i = 0; i < ListSize(n -> successors); i++)
		{
			child = (Node) ListIndex(n -> successors, i);
			mark_successors(child, ACT_READY);
		}
	}
	else
	{
		fprintf(stderr, "Error in action_done");
		return -1;
	}
	return 1;
}



void initialize_graph(Graph g)
{
	Node n;
        
        for(n = g -> source; n != NULL; n = n -> next)
	{
	 
	  
	    n -> data = (void *) malloc (sizeof (struct data));
            MARKED(n) = FALSE;
            STATE(n) = ACT_NONE;
	  
	}
	mark_successors(g->source->next,ACT_READY);
}


vm_exit_code handle_action_change_graph(int pid, char *action, vm_act_state state)

{

    Graph g;
    
    peos_context_t *context = peos_get_context(pid);
    char *model_file;
	                                                                                            
    model_file = context->model;
    g = makegraph(model_file);
    if(g == NULL)
    {
     fprintf(stderr,"Handle Action Error: Unable to build graph");
     return VM_INTERNAL_ERROR;
     }
     if(annotate_graph(g,context) < 0)
     {
       fprintf(stderr, "Handle Action Error: Unable to annotate graph");
      return VM_INTERNAL_ERROR;
      }
      if(set_act_state_graph(g,action,state) < 0)
       {
        fprintf(stderr, "Handle Action Error: Unable to change action state");
        return VM_INTERNAL_ERROR;
        }
       if(update_context(g,context) < 0)
        {
          fprintf(stderr,"Handle Action Error: Unable to update context");
         return VM_INTERNAL_ERROR;
         }
						                                                                                            
	 return VM_DONE;
}


int set_act_state_graph(Graph g, char *action, vm_act_state state)
{
	switch(state)
	{
       	 case(ACT_DONE) :  {
				  if(action_done(g,action) > 0) 
				      return 1;
				  else 
					  return -1;
	     		 }
				 
	 case(ACT_READY) : {
	        		  Node n = find_node(g,action);
				  if (n!=NULL)
				  {
				  STATE(n) = ACT_READY;
				  return 1;
				  }
				  else 
				  return -1;
		           }
				  
	 case(ACT_RUN) :   {
				  if(action_run(g,action) >  0)
					 return 1;
				  else
					 return -1; 
			   }

	 case(ACT_NONE) :  {
				 Node n = find_node(g,action);
				 if (n != NULL)
				 {
			         STATE(n) = ACT_NONE;	 
		                 return 1;
				 }
				 else
					 return -1;
	                  }

	 case(ACT_SUSPEND) : {
				 Node n = find_node(g,action);
				 if (n != NULL)
				 {
				 STATE(n) = ACT_SUSPEND;
				 return 1;
				 }
				 else
					 return -1;
			     }

	 case(ACT_ABORT) : {
				 Node n = find_node(g,action);
				 if (n != NULL)
				 {
			         STATE(n) = ACT_ABORT;	 
	                         return 1;
				 }
				 else
					 return -1;
	                   }
               
         case(ACT_NEW) :   {
				 Node n = find_node(g,action);
				 if (n != NULL)
				 {
				 STATE(n) = ACT_NEW;
				 return 1;
				 }
				 else
					 return -1;
					 
                           }
                
	  default : {
	              fprintf(stderr, "Error Changing Action : Invalid Action State\n");
		      return -1;
		    }
	}
}

int update_context(Graph g, peos_context_t *context)
{
	Node n;

	for(n = g -> source; n!= NULL; n = n -> next)
	{
		if(n -> type == ACTION)
		{
			if (set_act_state(n -> name, STATE(n), context -> actions, context -> num_actions) < 0)
			{
				return -1;
			}
		}
	}
	return 1;
}
			
# ifdef UNIT_TEST
#include "test_graph_engine.c"
#endif








      	      
