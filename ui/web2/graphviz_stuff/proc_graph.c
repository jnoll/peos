#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../../../os/kernel/pmlheaders.h"


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

void makedot(Node n, char *filename)
{
  FILE *file;
  int i, count = 0;
  char *mapout, *gifout, *tempchild, *tempn;
  Node child;
  Node parent;
  struct names *list;

  strtok(filename, ".");

  strcat(filename,".dot");
  file = fopen(filename,"w");

  strtok(filename, ".");
  fprintf(file,"digraph %s {\n", filename);
  fprintf(file, "style = filled;\ncolor = grey;\n");

  while(n != NULL)
  {
	  for(i = 0; (child = (Node) ListIndex(n->successors, i)); i++)
       	  {
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
	  n = n->next;
  }
	fprintf(file, "}");
	fclose(file);

	mapout = (char *) calloc(2*strlen(filename) + 52, sizeof(char));
	gifout = (char *) calloc(2*strlen(filename) + 52, sizeof(char));

	strcat(mapout, "/home/jntestuser/bin/bin/dot -Timap ");
	strcat(gifout, "/home/jntestuser/bin/bin/dot -Tgif ");
	strcat(filename,".dot");
	strcat(mapout, filename);
	strcat(gifout,filename);
	strcat(mapout, " > ");
	strcat(gifout, " > ");
	strtok(filename, ".");
	strcat(filename, ".map");
	strcat(mapout, filename);
	strtok(filename, ".");
	strcat(filename, ".gif");
	strcat(gifout, filename);

	system(mapout);
	system(gifout);
}

int main(int argc, char *argv[])
{
	Graph graph_name;

	graph_name = make_graph(argv[1]);
  
 	makedot(graph_name->source, argv[1]); 

  return 0;
}
