#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "kernel/pmlheaders.h"

/* Path to 'dot' graph processor executable.  This should be defined by make. */
#ifndef DOTBIN
#define DOTBIN "/usr/local/bin/dot"
#endif

Graph make_graph(char *file)
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
                 return program;
              }
             else
                  return NULL;
           }
           else
                return NULL;
        }
}

void makedot(Node n, char *filename, char *action, char *state)
{
  FILE *file;
  int i, count = 0;
  char cmd[BUFSIZ],  *tempchild, *tempn;
  char *color, *temp;
  Node child;
  Node parent;
  struct names *list;

  color = (char *)calloc(20, sizeof(char));

  if(action == NULL)
    strcpy(color,"black");
  if(strcmp(state, "ready") == 0)
    strcpy(color,"yellow");
  else if(strcmp(state, "active") == 0)
    strcpy(color,"green");
  else if(strcmp(state, "suspend") == 0)
    strcpy(color,"red");


  temp = strchr(filename, '/');
  temp++;
  strtok(temp, ".");
  strcat(temp, ".dot");
  file = fopen(temp,"w");

  strtok(temp, ".");
  fprintf(file,"digraph %s {\nrankdir=LR;\nsize=\"10,7.5\";\n", temp);

  while(n != NULL)
  {
    if(strcmp(n->name, action) == 0) {
      fprintf(file,"%s [style=filled color=%s];\n", n->name, color);
    } else {
      fprintf(file, "%s [style=unfilled color=black];\n", n->name);
    }
    for(i = 0; (child = (Node) ListIndex(n->successors, i)); i++)
      {
	if(child->dominator)
	{
	  if(strcmp(child->dominator->name, child->name) == 0)
	  {
	    if(n->type == 287 || n->type == 288)
	    {
	      fprintf(file,"%s_end->%s_end;\n", n->name, child->name);
	    }
	    else
	    {
	      fprintf(file,"%s->%s_end;\n", n->name, child->name);
	    }
	  }
	  else if((n->type == 287 && child->type == 270) || (n->type == 288 && child->type == 259)) 
	  {
	    fprintf(file, "%s_end->%s\n", n->name, child->name);
	  }
	  else
	    fprintf(file, "%s->%s;\n", n->name, child->name);
	}
	else
	  fprintf(file, "%s->%s;\n", n->name, child->name);
      }

      n = n->next;
  }
  fprintf(file, "}");
  fclose(file);


  snprintf(cmd, BUFSIZ, "%s -Timap %s.dot > %s.map 2> /dev/null",
	   DOTBIN, filename, filename);
  system(cmd);

  snprintf(cmd, BUFSIZ, "%s -Tjpg %s.dot > %s.jpg 2> /dev/null", 
	   DOTBIN, filename, filename);
  system(cmd);

  snprintf(cmd, BUFSIZ, "chmod a+rw %s.dot", filename);
  system(cmd);

  snprintf(cmd, BUFSIZ, "chmod a+rw %s.jpg", filename);
  system(cmd);

  snprintf(cmd, BUFSIZ, "chmod a+rw %s.map", filename);
  system(cmd);
}

