#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "kernel/pmlheaders.h"


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
  char *mapout, *gifout, *tempchild, *tempn, *mapchmod, *gifchmod, *dotchmod;
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
  fprintf(file,"digraph %s {\n", temp);

  while(n != NULL)
  {
    for(i = 0; (child = (Node) ListIndex(n->successors, i)); i++)
      {
        if(strcmp(child->name, action) == 0)
	  fprintf(file,"node[style=filled color=%s];\n", color);
	if(child->dominator)
	{
	  if(strcmp(child->dominator->name, child->name) == 0)
	  {
       	    tempchild = (char *) calloc(strlen(child->name)+5, sizeof(char));
	    strcpy(tempchild, child->name);
	    strcat(tempchild, "_end");
	    if(n->type == 287 || n->type == 288)
	    {
	      tempn = (char *) calloc(strlen(n->name)+5, sizeof(char));
	      strcpy(tempn, n->name);
	      strcat(tempn, "_end");
	      fprintf(file,"%s->%s;\n", tempn, tempchild);
	    }
	    else
	    {
	      fprintf(file,"%s->%s;\n", n->name, tempchild);
	    }
	  }
	  else if((n->type == 287 && child->type == 270) || (n->type == 288 && child->type == 259)) 
	  {
	    tempn = (char *) calloc(strlen(n->name)+5, sizeof(char));
	    strcpy(tempn, n->name);
	    strcat(tempn, "_end");
	    fprintf(file, "%s->", tempn);
	    fprintf(file, "%s;\n", child->name);
	  }
	  else
	    fprintf(file, "%s->%s;\n", n->name, child->name);
	}
	else
	  fprintf(file, "%s->%s;\n", n->name, child->name);
      }
      fprintf(file, "node[style=unfilled color=black];\n");
      n = n->next;
  }
	fprintf(file, "}");
	fclose(file);

	mapout = (char *) calloc(2*strlen(filename) + 22, sizeof(char));
	gifout = (char *) calloc(2*strlen(filename) + 22, sizeof(char));
	mapchmod = (char *) calloc(strlen(filename) + 12, sizeof(char));
	gifchmod = (char *) calloc(strlen(filename) + 12, sizeof(char));
	dotchmod = (char *) calloc(strlen(filename) + 12, sizeof(char));

	strcat(mapout, "dot -Timap ");
	strcat(gifout, "dot -Tgif ");
	strcat(filename,".dot");
        strcpy(dotchmod, "chmod a+rw ");
	strcat(dotchmod, filename);
	strcat(mapout, filename);
	strcat(gifout,filename);
	strcat(mapout, " > ");
	strcat(gifout, " > ");
	strtok(filename, ".");
	strcat(filename, ".map");
	strcat(mapout, filename);
        strcpy(mapchmod, "chmod a+rw ");
	strcat(mapchmod, filename);
	strtok(filename, ".");
	strcat(filename, ".gif");
	strcat(gifout, filename);
        strcpy(gifchmod, "chmod a+rw ");
	strcat(gifchmod, filename);

	system("setup graphviz");
	system(mapout);
	system(gifout);
	system(dotchmod);
	system(mapchmod);
	system(gifchmod);
}

