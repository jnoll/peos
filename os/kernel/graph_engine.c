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
# define RENDEZVOUS 288
# define SELECTION 270
# define JOIN 287
# define PROCESS 266

void handle_selection(Node n);
void mark_successors(Node n, vm_act_state state);

int  annotate_graph(Graph g, peos_context_t *context)
{
	
  int i;
  Node node;
                                                                        
  for(node = g -> source; node != NULL; node = node -> next)
     {
       MARKED(node) = FALSE;
       if (node -> type == ACTION)
        {
           STATE(node) = get_act_state(node -> name,context -> actions, context->num_actions);
	}
	else
	{
	 if((node->type == SELECTION) || (node->type == BRANCH))
	  {
	   for(i=0;i < context->num_other_nodes; i++)
	    {
	       if (strcmp(node->name,context->other_nodes[i].name)==0)
	       {
	         STATE(node) = context->other_nodes[i].state;
	         STATE(node->matching) = context->other_nodes[i].state;
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
			if((n->type == BRANCH) || (n->type == SELECTION))
			{
				STATE(n) = state;
				STATE(n->matching) = state;
			}
			for(i = 0; i < ListSize(n -> successors); i++)
			{
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
	if (n -> type == JOIN)
	{
		STATE(n) = ACT_DONE;
		STATE(n -> matching) = ACT_DONE;
		for(i = 0; i < ListSize(n->successors); i++)
		{
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

	if (n -> type == RENDEZVOUS)
	{
	 for(i = 0; i < ListSize(n -> predecessors); i++)
	 {
		 parent = (Node) ListIndex(n -> predecessors,i);
		 if (STATE(parent) != ACT_DONE)
		 {
			 status = 0;
		 }
	 }
	 if (status == 1)
	 {
		 STATE(n) = ACT_DONE;
		 STATE(n -> matching) = ACT_DONE;
		 for(i = 0; i< ListSize(n -> successors); i++)
		 {
			 child = (Node) ListIndex(n -> successors,i);
			 mark_successors(child,ACT_READY);
			 set_rendezvous_state(child);
		 }
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
	 STATE(parent) = ACT_RUN;
         STATE(parent -> matching) = ACT_RUN;	 
         for(j=0; j < ListSize(parent -> successors); j++)                                {
           child = (Node) ListIndex(parent -> successors,j);
           if(strcmp((child->name),n->name) != 0)
            {
             mark_successors(child,ACT_NONE);
            }
	 }
     }
      else
	  if (parent -> type == BRANCH)
	  {
		  STATE(parent) = ACT_RUN;
		  STATE(parent->matching) = ACT_RUN;
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
			if(child -> type == JOIN)
			{
			propogate_join_done(child);
			}
			if(child -> type != RENDEZVOUS)
			{
			mark_successors(child, ACT_READY);
			}
			else
			{
			set_rendezvous_state(child);
			}	
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
	int i;
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
          else
           {
            if((n->type == SELECTION) || (n->type == BRANCH))
             {
               for(i=0;i < context->num_other_nodes; i++)
                 {
                  if (strcmp(n->name,context->other_nodes[i].name)==0)
                   {
                    context->other_nodes[i].state = STATE(n);
                   }
                 }
            }
          }
       }

	return 1;
}
			
# ifdef UNIT_TEST
#include "test_graph_engine.c"
#endif








      	      
