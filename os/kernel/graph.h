#ifndef KERNEL_GRAPH_H
#define KERNEL_GRAPH_H
#include <pml/graph.h>

extern Graph makegraph(char *file);
extern Node find_node(Graph g, char *node_name);
extern void sanitize_node(Node n);

#endif
