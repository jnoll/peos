#ifndef KERNEL_GRAPH_H
#define KERNEL_GRAPH_H
#include <pml/graph.h>
extern void yyrestart( FILE *input_file );
extern Graph makegraph(char *file);
extern Node find_node(Graph g, char *node_name);

#endif
