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



void handle_selection(Node n);
void mark_successors(Node n, vm_act_state state);
void add_iteration_lists(Graph g);




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
	
	if(yyparse())
	{
	return NULL;
	}
        else
	{	
	  if(yyin)
	  {
            fclose (yyin);
	    if(program != NULL)
	      {
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

	
void add_iteration_lists(Graph g)
{
	Node node,child,parent,child1,child2;
	int i,j,k,l;

	MARKED(g -> source) = TRUE;

	for(node = g -> source->next; node != NULL; node = node -> next)
	{
          MARKED(node) = TRUE;
	  
	  for(i = 0; i < ListSize(node -> predecessors); i++)
	   {
	     parent = (Node) ListIndex(node -> predecessors,i);
	     if (MARKED(parent) == FALSE)
	     {
	        for(j=0; j < ListSize(parent -> successors); j++) 
	        {
		   child = (Node) ListIndex(parent->successors,j);
		   if(strcmp(child->name,node->name) != 0)
		   {
                     ListPut(ITER_END_NODES(node),child);
		   }
		}
	     }
	   }

	  for(k = 0; k < ListSize(node -> successors); k++)
	  {
            child1 = (Node) ListIndex(node -> successors,k);
	    if(MARKED(child1) == TRUE)
	    {
	      for(l = 0; l < ListSize(node -> successors); l++)
	      {
	        child2 = (Node) ListIndex(node -> successors,l);
		if (MARKED(child2) == FALSE)
		{
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

void make_resource_list(Tree t,peos_resource_t *resource_list,int *num_resources)
{
	int i;
	int rsize = 256;
	
   if(t)
       {
        if((IS_OP_TREE(t)) && (TREE_OP(t) == DOT))
          {
	      i = 0;	  
	      while((i < *num_resources) && (strcmp(resource_list[i].name,TREE_ID(t->left))) != 0)
	      {	      
		i++;
	      }
	      if(i == *num_resources)
	      {
              strcpy(resource_list[*num_resources].name,TREE_ID(t->left));
	      strcpy(resource_list[*num_resources].value,"\0");
	      *num_resources = *num_resources + 1;
	      if(*num_resources > rsize)
	      {
		      rsize = rsize*2;
		      realloc(resource_list,rsize);
	      }

	      }
              return;
          }
        else
          {
              if(IS_ID_TREE(t))
               {
		 i = 0;
                 while((i < *num_resources) && (strcmp(resource_list[i].name,TREE_ID(t))) != 0)
                  {
                    i++;
                  }
                 if(i == *num_resources)
                 {
         	 strcpy(resource_list[*num_resources].name,TREE_ID(t));
		 strcpy(resource_list[*num_resources].value,"\0");
	         *num_resources = *num_resources + 1;
		 if(*num_resources > rsize)
		 {
		    rsize = rsize*2;
		    realloc(resource_list,rsize);
		 }

		 }
                 return;
              }
               else
               {
                if((IS_OP_TREE(t)) && (TREE_OP(t) == EQ))
                 {
                  make_resource_list(t->left,resource_list,num_resources);
                 }
                else
                 {                       
		   make_resource_list(t->left,resource_list,num_resources);
		   make_resource_list(t->right,resource_list,num_resources);
                 }
              }
         }
      }
       else
            return;
}


/* 
 * This function is used to get the list of resources for a particular action
 * The return value is a list of peos_resource_t and not a list of strings, because
 * this lets me reuse the function make_resource_list above. If I had to retun a list of strings
 * then I would have to write another fuunction which would be same as make_resource_list
 * but would make a list of strings.
 *
 */
peos_resource_t *get_resource_list_action(int pid, char *act_name, int *total_resources)
{
	Graph g;
	Node n;
	int rsize = 256;
	int num_resources = 0;
	peos_context_t *context = peos_get_context(pid);
        char *model_file;
	peos_resource_t *act_resources = (peos_resource_t *) calloc(rsize,sizeof(peos_resource_t));

        model_file = context->model;
        g = makegraph(model_file);
	if(g != NULL)
	{
		n = find_node(g,act_name);
		make_resource_list(n -> requires,act_resources,&num_resources);
		make_resource_list(n -> provides,act_resources,&num_resources);
		*total_resources = num_resources;
		GraphDestroy(g);
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
	if(g != NULL)
	{	
	for(n = g->source->next; n != NULL; n = n -> next)
	{
		if(n -> type == ACTION)
		{
                  make_resource_list(n -> requires,resource_list,&num_resources);
		  make_resource_list(n -> provides,resource_list,&num_resources);
		}
	}
	*total_resources = num_resources;
	GraphDestroy(g);
	return resource_list;
	}
	else
		return NULL;
}


void mark_iter_nodes(Node n)
{
	Node iter_start_node,iter_end_node;
	int i;

	if(STATE(n) == ACT_READY)
	{
          for(i = 0; i <  ListSize(ITER_END_NODES(n)); i++)
            {
            iter_end_node = (Node) ListIndex(ITER_END_NODES(n),i);
	    if((iter_end_node->type == SELECTION) || (iter_end_node->type == BRANCH) ||(iter_end_node->type == ACTION))
	      {
	        mark_successors(iter_end_node,ACT_READY);
	      }
	   }
	}
        else 
	{
	if(STATE(n) == ACT_RUN)
	{
	  for(i = 0; i < ListSize(ITER_END_NODES(n)); i++)
	   {
	    iter_end_node = (Node) ListIndex(ITER_END_NODES(n),i);
	    if((iter_end_node->type == SELECTION) || (iter_end_node->type == BRANCH) ||(iter_end_node->type == ACTION))
	       {
	         mark_successors(iter_end_node,ACT_NONE);
	       }
	   }
           
	  for(i = 0; i < ListSize(ITER_START_NODES(n)); i++)
	   {
	    iter_start_node = (Node) ListIndex(ITER_START_NODES(n),i);
	    if((iter_start_node->type == SELECTION) || (iter_start_node->type == BRANCH) ||(iter_start_node->type == ACTION))
	      {
	       mark_successors(iter_start_node,ACT_NONE);
	      }
	   }
	}
       }
}



void mark_successors(Node n, vm_act_state state)
{
	int i;
	Node child;
	if (n -> type == ACTION)
	{
		STATE(n) = state;
		mark_iter_nodes(n);
		return;
	}
	else
		if((n -> type == BRANCH) || (n -> type == SELECTION) || (n -> type == JOIN))
		{
			if((n->type == BRANCH) || (n->type == SELECTION))
			{
				STATE(n) = state;
				mark_iter_nodes(n);
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
	
void set_process_state(Graph g)
{
	Node parent;
	int i;
	int status = 1;

	for(i = 0; i < ListSize(g -> sink -> predecessors); i++)
	{
		parent = (Node) ListIndex(g -> sink -> predecessors,i);
		if (STATE(parent) != ACT_DONE)
			status = 0;
	}

	if (status == 1)
	{
		STATE(g -> source) = ACT_DONE;
		STATE(g -> sink) = ACT_DONE;
	}
}


int action_run(Graph g, char *act_name)
{
	Node n;
	
	n = find_node(g, act_name);
	if(n != NULL)
	{
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
 if ((n -> predecessors == NULL) || (MARKED(n) == TRUE))
        return;
					                                                                         
 MARKED(n) = TRUE;
 for(i = 0; i < ListSize(n -> predecessors); i++)
   {
      parent = (Node) ListIndex(n -> predecessors,i);
      if ((parent -> type) == SELECTION)
        {
	 STATE(parent) = ACT_RUN;
	 mark_iter_nodes(parent);
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
		  mark_iter_nodes(parent);
		  STATE(parent->matching) = ACT_RUN;
	  }
										     handle_selection(parent);
										   }
}
					
int action_done(Graph g, char *act_name)
{
	Node n;
	Node child;
	int i,num_successors;

	n = find_node(g,act_name);
	if(n != NULL)
	{
		STATE(n) = ACT_DONE;
		num_successors = ListSize(n -> successors);
		for(i = 0; i < num_successors; i++)
		{
			child = (Node) ListIndex(n -> successors, i);
			if((child -> type == JOIN) && (num_successors == 1))
			{
			propogate_join_done(child);
			}
			if(child -> type != RENDEZVOUS)
			{
			mark_successors(child, ACT_READY);
			}
			else
			{
			if(num_successors == 1)	
			set_rendezvous_state(child);
			}	
		}
		if (num_successors == 1)
			set_process_state(g);
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
	    ITER_START_NODES(n) = ListCreate();
	    ITER_END_NODES(n) = ListCreate();
	  
	}
	add_iteration_lists(g);
	sanitize(g);
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

       GraphDestroy(g);
						                                                                                            
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
	    else
	    {
	      if(n->type == PROCESS)
	      {
		      if (STATE(n) == ACT_DONE)
			      context -> status = PEOS_DONE;
	      }
	    }
          }
       }

	return 1;
}
			
# ifdef UNIT_TEST
#include "test_graph_engine.c"
#endif








      	      
