#ifndef KERNEL_GRAPH_H
#define KERNEL_GRAPH_H
#include <pml/graph.h>
#include <pml/features.h>

extern Graph makegraph(char *file) INIT_SECTION;
extern Node find_node(Graph g, char *node_name);
extern void sanitize_node(Node n) INIT_SECTION;

#endif
