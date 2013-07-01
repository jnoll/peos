/************************************************************************
 * File:	global.c						*
 *									*
 * $Revision: 1.1 $
 *                                                                      *
 * Description:	This file contains the public and private function	*
 *		definitions for the global data-flow checks.		*
 ************************************************************************/

# include <stdio.h>
# include <stdlib.h>
# include <ctype.h>
# include <pml/tokens.h>
# include <pml/scanner.h>
# include <expr/expr.h>
# include "common.h"
# include "link/link.h"
# include "global.h"
# include "main.h"

/* Private type declarations */

typedef struct data {
    short marked;
    short status;
    short availability;
    List  necessary;
} *Data;


/* Private function declarations */

static void CheckConsumers (
# ifdef ANSI_PROTOTYPES
    Tree		/* tree */,
    Node		/* node */,
    Graph               /* graph */
# endif
);

static void FindConsumers (
# ifdef ANSI_PROTOTYPES
    Node		/* node  */,
    Node		/* start */,
    Tree		/* tree  */
# endif
);

static void PrintConsumerWarning (
# ifdef ANSI_PROTOTYPES
    Tree		/* tree  */,
    Node		/* node  */,
    int                 /* blocked */
# endif
);

static void CheckProducers (
# ifdef ANSI_PROTOTYPES
    Tree		/* tree  */,
    Node		/* node  */,
    Graph		/* graph */
# endif
);

static void FindProducers (
# ifdef ANSI_PROTOTYPES
    Node		/* node  */,
    Node		/* start */,
    Tree		/* tree  */
# endif
);

static void PrintProducerWarning (
# ifdef ANSI_PROTOTYPES
    Tree		/* tree  */,
    Node		/* node  */,
    int                 /* blocked */
# endif
);

static void GetResourceStatus (
# ifdef ANSI_PROTOTYPES
    Tree		/* tree */,
    Tree		/* expr */,
    Node                /* node */
# endif
);

static void CheckExpressions (
# ifdef ANSI_PROTOTYPES
    Tree		/* tree  */,
    Node		/* node  */,
    Graph		/* graph */
# endif
);

static void FindExpressions (
# ifdef ANSI_PROTOTYPES
    Node		/* node  */,
    Node		/* start */,
    Tree		/* tree  */
# endif
);

static int IdentityCheck (
# ifdef ANSI_PROTOTYPES
    Tree		/* tree  */
# endif
);

static int NumberStringCheck (
# ifdef ANSI_PROTOTYPES
    Tree		/* tree  */
# endif
);

static void PrintExpressionWarning (
# ifdef ANSI_PROTOTYPES
    Tree		/* tree  */,
    Node		/* node  */,
    int                 /* blocked */
# endif
);

static void GetExprStatus (
# ifdef ANSI_PROTOTYPES
    Tree		/* tree */,
    Tree		/* expr */,
    Node                /* node */
# endif
);

static void FindConcurrent (
# ifdef ANSI_PROTOTYPES
    Node		/* node */,
    Node		/* stop */
# endif
);

static void NodeDestroy (
# ifdef ANSI_PROTOTYPES
    Node                /* node */
# endif
);


/* Private macro definitions */

# define UNKNOWN 3
# define TRUE    2
# define MAYBE   1
# define FALSE   0

# define MARKED(n)	(((Data) (n) -> data) -> marked)
# define AVAIL(n)	(((Data) (n) -> data) -> availability)
# define STATUS(n)      (((Data) (n) -> data) -> status)
# define NECESSARY(n)	(((Data) (n) -> data) -> necessary)

/* Private variable definitions */

extern String link_filename;

static List roots;
static List concurrent;

static int j_table [4] [4] = {
  {FALSE, MAYBE, MAYBE, FALSE},	/* FALSE combined with ...   */
  {MAYBE, MAYBE, MAYBE, MAYBE}, 	/* MAYBE combined with ...   */
  {MAYBE, MAYBE, TRUE,  TRUE}, 	/* TRUE combined with ...    */
  {FALSE, MAYBE, TRUE,  UNKNOWN},	/* UNKNOWN combined with ... */
};

static int r_table [4] [4] = {
  {FALSE, MAYBE, TRUE,  FALSE},	/* FALSE combined with ...   */
  {MAYBE, MAYBE, TRUE,  MAYBE},	/* MAYBE combined with ...   */
  {TRUE,  TRUE,  TRUE,  TRUE},	/* TRUE combined with ...    */
  {FALSE, MAYBE, TRUE,  UNKNOWN},	/* UNKNOWN combined with ... */
};

static int j_etable [4] [4] = {
  {NEVER,     SOMETIMES, SOMETIMES, NEVER},	  /* NEVER combined with ...     */
  {SOMETIMES, SOMETIMES, SOMETIMES, SOMETIMES}, /* SOMETIMES combined with ... */
  {SOMETIMES, SOMETIMES, ALWAYS,    ALWAYS}, 	  /* ALWAYS combined with ...    */
  {NEVER,     SOMETIMES, ALWAYS,    UNKNOWN},	  /* UNKNOWN combined with ...   */
};

static int r_etable [4] [4] = {
  {NEVER,     SOMETIMES, ALWAYS,  NEVER},	/* NEVER combined with ...     */
  {SOMETIMES, SOMETIMES, ALWAYS,  SOMETIMES},	/* SOMETIMES combined with ... */
  {ALWAYS,    ALWAYS,    ALWAYS,  ALWAYS},	/* ALWAYS combined with ...    */
  {NEVER,     SOMETIMES, ALWAYS,  UNKNOWN},	/* UNKNOWN combined with ...   */
};

static int j_stable [5] [5] = {
  {UNSPECIFIED,         PARTIALLY_SPECIFIED, PARTIALLY_SPECIFIED, UNSPECIFIED,         CONTRADICTION},	   /* UNSPECIFIED combined with ...   */
  {PARTIALLY_SPECIFIED, PARTIALLY_SPECIFIED, PARTIALLY_SPECIFIED, PARTIALLY_SPECIFIED, CONTRADICTION},  /* PARTIALLY_SPECIFIED combined with ...   */
  {PARTIALLY_SPECIFIED, SPECIFIED,           SPECIFIED,           SPECIFIED,           CONTRADICTION}, 	   /* SPECIFIED combined with ...    */
  {UNSPECIFIED,         PARTIALLY_SPECIFIED, SPECIFIED,           UNKNOWN,             CONTRADICTION},	           /* UNKNOWN combined with ... */
  {CONTRADICTION,       CONTRADICTION,       CONTRADICTION,       CONTRADICTION,       CONTRADICTION},
};

static int r_stable [5] [5] = {
  {UNSPECIFIED,         PARTIALLY_SPECIFIED,  SPECIFIED,           UNSPECIFIED,         CONTRADICTION},	   /* UNSPECIFIED combined with ...   */
  {PARTIALLY_SPECIFIED, PARTIALLY_SPECIFIED,  SPECIFIED,           PARTIALLY_SPECIFIED, CONTRADICTION}, /* PARTIALLY_SPECIFIED combined with ...   */
  {SPECIFIED,           SPECIFIED,            SPECIFIED,           SPECIFIED,           CONTRADICTION},	   /* SPECIFIED combined with ...    */
  {UNSPECIFIED,         PARTIALLY_SPECIFIED,  SPECIFIED,           UNKNOWN,             CONTRADICTION},	           /* UNKNOWN combined with ... */
  {CONTRADICTION,       CONTRADICTION,        CONTRADICTION,       CONTRADICTION,       CONTRADICTION},
};

static String producer_warnings [3] [3] = {
  /* available = FALSE and blocked = {FALSE, MAYBE, TRUE} */
  
  {"unprovided",
   "possibly blocked (otherwise unprovided)",
   "blocked"},
  
  
  /* available = MAYBE and blocked = {FALSE, MAYBE, TRUE} */
  
  {"possibly unprovided",
   "possibly blocked (otherwise only possibly provided)",
   "blocked (and possibly provided)"},

  /* available = TRUE  and blocked = {FALSE, MAYBE, TRUE} */
  
  {"unprovided",
   "possibly blocked (otherwise unprovided)",
   "blocked"}

};

static String consumer_warnings [3] [3] = {

  /* available = FALSE and blocked = {FALSE, MAYBE, TRUE} */

  {"not consumed",
   "possibly not consumed",
   "possibly not consumed"},

  /* available = MAYBE and blocked = {FALSE, MAYBE, TRUE} */

  {"possibly not consumed",
   "possibly not consumed",
   "possibly not consumed"},

  /* available = TRUE and blocked = {FALSE, MAYBE, TRUE} */

  {"not consumed",
   "possibly not consumed",
   "possibly not consumed"}

};

static String contradiction_warnings [3] [3] = {
  /* available = FALSE and blocked = {FALSE, MAYBE, TRUE} */
  
  {"a contradiction",
   "possibly a contradiction",
   "possibly a contradiction"},
  
  
  /* available = MAYBE and blocked = {FALSE, MAYBE, TRUE} */
  
  {"possibly a contradiction",
   "possibly a contradiction",
   "possibly a contradiction"},

  /* available = TRUE  and blocked = {FALSE, MAYBE, TRUE} */
  
  {"a contradiction",
   "possibly a contradiction",
   "possibly a contradiction"}

};

static String expression_warnings_sometimes  [3] [3] = {
  /* available = FALSE and blocked = {FALSE, MAYBE, TRUE} */
  
  {"unsatisfiable",
   "possibly blocked (otherwise sometimes unsatisfiable)",
   "blocked (and sometimes unsatisfiable)"},
  
  
  /* available = MAYBE and blocked = {FALSE, MAYBE, TRUE} */
  
  {"possibly unsatisfiable (sometimes)",
   "possibly blocked (otherwise only possibly satisfied, sometimes)",
   "blocked (and possibly satisfied sometimes)"},

  /* available = TRUE  and blocked = {FALSE, MAYBE, TRUE} */
  
  {"sometimes unsatisfied",
   "possibly blocked (otherwise unsatisfied)",
   "blocked"}

};

static String expression_warnings_never  [3] [3] = {
  /* available = FALSE and blocked = {FALSE, MAYBE, TRUE} */
  
  {"unsatisfiable",
   "possibly blocked (otherwise unsatisfiable)",
   "blocked"},
  
  
  /* available = MAYBE and blocked = {FALSE, MAYBE, TRUE} */
  
  {"possibly unsatisfiable",
   "possibly blocked (otherwise only possibly satisfied)",
   "blocked (and possibly satisfied)"},

  /* available = TRUE  and blocked = {FALSE, MAYBE, TRUE} */
  
  {"unsatisfied",
   "possibly blocked (otherwise unsatisfied)",
   "blocked"}

};

/************************************************************************
 * Function:	CheckConsumers						*
 *                                                                      * 
 * Description: Checks that all resources in the specified tree that    *
 *              are produced by the specified node in the graph are     *
 *              required by a later node in the graph.                  *
 ************************************************************************/

static void CheckConsumers (tree, node, graph)
     Tree tree;
     Node node;
     Graph graph;
{
  int  i;
  int  blocked;
  Node root;
  Node n;
  
  /* Walk down the resource tree until we hit a resource */
  
  if (!IsResource(tree)) {

    if(IS_OP_TREE (tree) && (TREE_OP (tree) == AND || TREE_OP (tree) == OR)) {
      if(!IsNumber (tree -> left))
	CheckConsumers(tree -> left, node, graph);
      
      if(!IsNumber (tree -> right))
	CheckConsumers(tree -> right, node, graph);
    }
    
    if(IsExpression(tree)) {
      if(IsResource(tree -> left))
	CheckConsumers(tree -> left, node, graph);
      
      if(IsResource(tree -> right))
	CheckConsumers(tree -> right, node, graph);
    }
  } 
  
  /* We have reached a resource (string represents the deprecated use of "resource") */

  if (IsResource(tree) || IsString(tree)) {
    
    roots = ListCreate ( );
    concurrent = ListCreate ( );
    
    /* Find all consumers that reach this node. */
    
    for (n = graph -> source; n != NULL; n = n -> next) {
      MARKED (n) = FALSE;
      AVAIL  (n) = UNKNOWN;
      STATUS (n) = UNKNOWN;
    }
    
    FindConsumers (node, node, tree); 
    
    /* If the resource is not defined or is only possibly defined, then
       search for possible producers in parallel. */
    
    if ((STATUS(node) != SPECIFIED && STATUS (node) != PARTIALLY_SPECIFIED) || 
	 AVAIL (node) != TRUE) {
      blocked = FALSE;
      
      for (i = 0; i < ListSize (concurrent); i ++) {
	n = (Node) ListIndex (concurrent, i);
	MARKED (n) = FALSE;
	AVAIL  (n) = UNKNOWN;
	STATUS (n) = UNKNOWN;
      }
      
      for (i = 0; i < ListSize (roots); i ++) {
	root = (Node) ListIndex (roots, i);
	FindConsumers (root, NULL, tree);
	blocked       = r_table  [AVAIL  (root)] [blocked];
	STATUS (node) = r_stable [STATUS (root)] [STATUS (node)];
      }
  
      PrintConsumerWarning(tree, node, blocked);
    }
    
    /* Destroy any allocated data structures. */
    
    ListDestroy (concurrent);
    ListDestroy (roots);
  }
}


/************************************************************************
 * Function:	FindConsumers						*
 ************************************************************************/

static void FindConsumers (node, start, tree)
     Node node;
     Node start;
     Tree tree;
{
  int  i;
  Node successor;
  
  if(MARKED (node) == TRUE) {
    AVAIL (node) = FALSE;
    return;
  }

  MARKED (node) = TRUE;
  STATUS (node) = UNKNOWN;
  
  if((IsResource(tree) || IsString(tree)) && node != start && node -> requires != NULL)
    GetResourceStatus(tree, node->requires, node);
  
  /* We have found what we are looking for */

  if(STATUS (node) == SPECIFIED && AVAIL (node) == TRUE)
    return;
  
  /* We have found a contradiction */

  if(STATUS (node) == CONTRADICTION)
    return;

  /* If we reach the end, then we haven't found it */

  if(ListSize(node -> successors) == 0) {
    AVAIL  (node) = r_table  [AVAIL (node)]  [FALSE];
    STATUS (node) = r_stable [STATUS (node)] [UNSPECIFIED];
    return;
  }
  
  /* If in a branch, build list of concurrent nodes */

  if(node -> type == RENDEZVOUS && MARKED (node -> matching) == FALSE)
    FindConcurrent(node -> matching, node);
  
  /* For branches and selections, look at all paths */

  if(node -> type == BRANCH || node -> type == SELECTION) {
    
    MARKED (node -> matching) = TRUE;
    
    for(i=0; i < ListSize(node -> successors); i++) {
      successor = (Node) ListIndex(node -> successors, i);
      
      FindConsumers (successor, start, tree);
      
      if(node -> type == SELECTION) {
	AVAIL  (node) = j_table  [AVAIL  (node)] [AVAIL  (successor)]; 
	STATUS (node) = j_stable [STATUS (node)] [STATUS (successor)];
      } else {
	AVAIL  (node) = r_table  [AVAIL  (node)] [AVAIL  (successor)];
	STATUS (node) = r_stable [STATUS (node)] [STATUS (successor)];
      }
    }
    
    MARKED (node -> matching) = FALSE;
    
    successor = node -> matching;
    FindConsumers (successor, start, tree);
    AVAIL  (node) = r_table  [AVAIL  (node)] [AVAIL  (successor)];
    STATUS (node) = r_stable [STATUS (node)] [STATUS (successor)];
    
  } else {
    
    /* Step to the next node like this so we don't get caught in an iteration */

    successor = (Node) ListIndex(node -> successors, ListSize(node -> successors) - 1);
    FindConsumers (successor, start, tree);
    AVAIL  (node) = r_table  [AVAIL  (node)] [AVAIL  (successor)];
    STATUS (node) = r_stable [STATUS (node)] [STATUS (successor)];
  }
}

/************************************************************************
 * Function:	PrintConsumerWarning     				*
 ************************************************************************/

static void PrintConsumerWarning (tree, node, blocked)
     Tree tree;
     Node node;
     int  blocked;
{
  if(node -> type == PROCESS)
    printf("%s:%d: ", link_filename, GetLineNumber(tree));
  else    
    printf("%s:%d: ", filename, GetLineNumber(tree));

  if(STATUS (node) == PARTIALLY_SPECIFIED && HasAttribute(tree))
    printf("attribute %s of ", GetAttributeName(tree));

  printf("%s in action '%s' is ", GetResourceName(tree), node -> name);
    
  if(STATUS (node) != CONTRADICTION)
    printf("%s\n", consumer_warnings[AVAIL (node)][blocked]);
  else
    printf("%s\n", contradiction_warnings[AVAIL (node)][blocked]);
}


/************************************************************************
 * Function:	CheckProducers						*
 *									*
 * Description:	Checks the all resources in the specified tree that are	*
 *		consumed by the specified node in the graph are		*
 *		produced by a prior node in the graph.			*
 ************************************************************************/

static void CheckProducers (tree, node, graph)
     Tree  tree;
     Node  node;
     Graph graph;
{
  int  i;
  int  blocked;
  Node root;
  Node n;
  
  /* Walk down the resource tree until we hit a resource */

  if (!IsResource(tree)) {
    
    if(IS_OP_TREE (tree) && (TREE_OP (tree) == AND || TREE_OP (tree) == OR)) {
      if(!IsNumber (tree -> left))
	CheckProducers(tree -> left, node, graph);
      
      if(!IsNumber (tree -> right))
	CheckProducers(tree -> right, node, graph);
    }
    
    if(IsExpression(tree)) {
      if(IsResource(tree -> left))
	CheckProducers(tree -> left, node, graph);
      
      if(IsResource(tree -> right))
	CheckProducers(tree -> right, node, graph);
    }
  } 
  
  /* We have reached a resource (string represents the deprecated use of "resource") */
  
  if (IsResource(tree) || IsString(tree)) {
	
    roots = ListCreate ( );
    concurrent = ListCreate ( );
   
    /* Find all producers that reach this node. */
    
    for (n = graph -> source; n != NULL; n = n -> next) {
      MARKED (n) = FALSE;
      AVAIL  (n) = UNKNOWN;
      STATUS (n) = UNKNOWN;
    }
    
    FindProducers (node, node, tree); 
  
    /* If the resource is not defined or is only possibly defined, then
       search for possible producers in parallel. */
    
    if (STATUS (node) != SPECIFIED || AVAIL (node) != TRUE) {
      blocked = FALSE;

      for (i = 0; i < ListSize (concurrent); i ++) {
	n = (Node) ListIndex (concurrent, i);
	MARKED (n) = FALSE;
	AVAIL  (n) = UNKNOWN;
	STATUS (n) = UNKNOWN;
      }
      
      for (i = 0; i < ListSize (roots); i ++) {
	root = (Node) ListIndex (roots, i);
	FindProducers (root, NULL, tree);
	blocked       = r_table  [AVAIL  (root)] [blocked];
	STATUS (node) = r_stable [STATUS (root)] [STATUS (node)];
      }

      PrintProducerWarning(tree, node, blocked);  
    }
	
    /* Destroy any allocated data structures. */
    
    ListDestroy (concurrent);
    ListDestroy (roots);
  }
}


/************************************************************************
 * Function:	FindProducers						*
 ************************************************************************/

static void FindProducers (node, start, tree)
    Node node;
    Node start;
    Tree tree;
{
  int  i;
  Node predecessor;

  if(MARKED (node) == TRUE) {
    AVAIL (node) = FALSE;
    return;
  }

  MARKED (node) = TRUE;
  STATUS   (node) = UNKNOWN;
  
  if((IsResource(tree) || IsString(tree)) && node != start && node -> provides != NULL)
    GetResourceStatus(tree, node->provides, node);
  
  /* We have found what we are looking for */

  if(STATUS (node) == SPECIFIED && AVAIL (node) == TRUE)
    return;

  /* We have found a contradiction */

  if(STATUS (node) == CONTRADICTION)
    return;

  /* If we reach the end, then we haven't found it */

  if(ListSize(node -> predecessors) == 0) {
    AVAIL  (node) = r_table  [AVAIL  (node)] [FALSE];
    STATUS (node) = r_stable [STATUS (node)] [UNSPECIFIED];
    return;
  }
  
  /* If in a branch, build list of concurrent nodes */

  if(node -> type == BRANCH && MARKED (node -> matching) == FALSE)
    FindConcurrent(node, node -> matching);
  
  /* For rendezvous and joins, look at all paths */

  if(node -> type == RENDEZVOUS || node -> type == JOIN) {
	
    MARKED (node -> matching) = TRUE;
    
    for(i=0; i < ListSize(node -> predecessors); i++) {
      predecessor = (Node) ListIndex(node -> predecessors, i);
      
      FindProducers(predecessor, start, tree);

      if(node -> type == JOIN) {
	AVAIL  (node) = j_table  [AVAIL  (node)] [AVAIL  (predecessor)];  
	STATUS (node) = j_stable [STATUS (node)] [STATUS (predecessor)];
      } else {
	AVAIL  (node) = r_table  [AVAIL  (node)] [AVAIL  (predecessor)];
	STATUS (node) = r_stable [STATUS (node)] [STATUS (predecessor)];
      }
    }
    
    MARKED (node -> matching) = FALSE;

    predecessor = node -> matching;
    FindProducers(predecessor, start, tree);
    AVAIL  (node) = r_table  [AVAIL  (node)] [AVAIL  (predecessor)];
    STATUS (node) = r_stable [STATUS (node)] [STATUS (predecessor)];

  } else {

    /* Indexing like this is not necessary here, but we'll be consistent with the consumer check */

    predecessor = (Node) ListIndex(node -> predecessors, ListSize(node -> predecessors) - 1);
    FindProducers(predecessor, start, tree);
    AVAIL  (node) = r_table  [AVAIL  (node)] [AVAIL  (predecessor)];
    STATUS (node) = r_stable [STATUS (node)] [STATUS (predecessor)];
  }
}

/************************************************************************
 * Function:	PrintProducerWarning     				*
 ************************************************************************/

static void PrintProducerWarning (tree, node, blocked)
     Tree tree;
     Node node;
     int  blocked;
{
  printf("%s:%d: ", filename, GetLineNumber(tree));

  if(STATUS (node) == PARTIALLY_SPECIFIED && HasAttribute(tree))
    printf("attribute %s of ", GetAttributeName(tree));

  printf("%s in action '%s' is ", GetResourceName(tree), node -> name);
  
  if(STATUS (node) != CONTRADICTION)
    printf("%s\n", producer_warnings[AVAIL (node)][blocked]);
  else
    printf("%s\n", contradiction_warnings[AVAIL (node)][blocked]);
}



/************************************************************************
 * Function:	CheckExpressions					*
 *									*
 * Description:	Checks to see if expressions in the given node are      *
 *              satisfiable by a prior node in the graph.                *
 ************************************************************************/

static void CheckExpressions (tree, node, graph)
     Tree  tree;
     Node  node;
     Graph graph;
{
  int  i;
  int  c_status;
  Node root;
  Node n;

  /* Step down the tree until we hit an expression */

  if(IS_OP_TREE (tree)) {

    if(TREE_OP (tree) == AND || TREE_OP (tree) == OR) {
      if(IS_OP_TREE (tree -> left) && 
	 (TREE_OP (tree -> left) == AND || 
	  TREE_OP (tree -> left) == OR))
	CheckExpressions(tree -> left, node, graph);
      
      if(IS_OP_TREE (tree -> right) && 
	 (TREE_OP (tree -> right) == AND || 
	  TREE_OP (tree -> right) == OR))
	CheckExpressions(tree -> right, node, graph);
      
      if(IsExpression(tree -> left))
	CheckExpressions(tree -> left, node, graph);

      if(IsExpression(tree -> right))
	CheckExpressions(tree -> right, node, graph);
    }
  }

  if(IsExpression(tree)) {
    
    /* Check for identity expression */
    if(IdentityCheck(tree))
      return;

    /* Check expressions that consist only of strings or numbers */
    if(NumberStringCheck(tree))
      return;

    roots = ListCreate ( );
    concurrent = ListCreate ( );
    
    
    /* Find all expressions that reach this node. */
    
    for (n = graph -> source; n != NULL; n = n -> next) {
      MARKED (n) = FALSE;
      AVAIL  (n) = UNKNOWN;
      STATUS (n) = NEVER;
    }
    
    FindExpressions (node, node, tree);
    
    if (STATUS (node) != ALWAYS || AVAIL (node) != ALWAYS) {
      c_status = FALSE;
      
      for (i = 0; i < ListSize (concurrent); i ++) {
	n = (Node) ListIndex (concurrent, i);
	MARKED (n) = FALSE;
	AVAIL  (n) = UNKNOWN;
	STATUS (n) = NEVER;
      }
      
      for (i = 0; i < ListSize (roots); i ++) {
	root = (Node) ListIndex (roots, i);
	FindExpressions (root, NULL, tree);
	c_status      = r_table  [AVAIL  (root)] [c_status];
	STATUS (node) = r_etable [STATUS (root)] [STATUS (node)];
      }	
      
      PrintExpressionWarning(tree, node, c_status);
    }
    
    ListDestroy (concurrent);
    ListDestroy (roots);      
  }
}
  

/************************************************************************
 * Function:	FindExpressions						*
 ************************************************************************/

static void FindExpressions (node, start, tree)
    Node node;
    Node start;
    Tree tree;
{
  int  i;
  Node predecessor;

  if(MARKED (node) == TRUE) {
    AVAIL (node) = FALSE;
    return;
  }
   
  MARKED (node) = TRUE;
  STATUS (node) = NEVER;

  if(IsExpression(tree) && node != start && node -> provides != NULL)
    if(IsExpression(tree))
      GetExprStatus(tree, node -> provides, node);
  
  
  /* If the expression is satified in any way, we have what we want */

  if(STATUS (node) == ALWAYS || STATUS (node) == SOMETIMES) {
    AVAIL (node) = TRUE;
    return;
  }

  /* If we reach the end, we haven't found it */

  if(ListSize(node -> predecessors) == 0) {
    AVAIL  (node) = r_table  [AVAIL  (node)] [FALSE];
    STATUS (node) = r_etable [STATUS (node)] [NEVER];
    return;
  }

  /* If in a branch, build list of concurrent nodes */

  if(node -> type == BRANCH && MARKED (node -> matching) == FALSE)
    FindConcurrent(node, node -> matching);

  if(node -> type == RENDEZVOUS || node -> type == JOIN) {

    MARKED (node -> matching) = TRUE; 

    for(i=0; i < ListSize(node -> predecessors); i++) {
      predecessor = (Node) ListIndex(node -> predecessors, i);
      
      FindExpressions (predecessor, start, tree);

      if(node -> type == JOIN) {
	AVAIL  (node) = j_table  [AVAIL  (node)] [AVAIL  (predecessor)];      
	STATUS (node) = j_etable [STATUS (node)] [STATUS (predecessor)];
      } else {
	AVAIL  (node) = r_table  [AVAIL  (node)] [AVAIL  (predecessor)];
	STATUS (node) = r_etable [STATUS (node)] [STATUS (predecessor)];
      }
    }

    MARKED (node -> matching) = FALSE;

    predecessor = node -> matching; 
    FindExpressions(predecessor, start, tree);
    AVAIL  (node) = r_table  [AVAIL  (node)] [AVAIL  (predecessor)];
    STATUS (node) = r_etable [STATUS (node)] [STATUS (predecessor)];
    
  } else {

    /* Indexing like this is not necessary here, but we'll be consistent with the other checks */

    predecessor = (Node) ListIndex(node -> predecessors, ListSize(node -> predecessors) - 1);
    FindExpressions(predecessor, start, tree);
    AVAIL  (node) = r_table  [AVAIL  (node)] [AVAIL  (predecessor)];
    STATUS (node) = r_etable [STATUS (node)] [STATUS (predecessor)];
  }
}

/************************************************************************
 * Function:	IdentityCheck           				*
 *                                                                      *
 * Description: Check to see if the expression tree is an identity and  *
 *              return non-zero if it is.                               *
 ************************************************************************/

static int IdentityCheck(tree)
     Tree tree;
{
  if(IS_OP_TREE (tree))
    switch(TREE_OP (tree)) {

    case EQ: 
    case LE:
      if(IsEqual(tree -> left, tree -> right))
	return 1;
      break;
    case NE:
    case LT:
      break;
    }
  
  return 0;
}

/************************************************************************
 * Function:	NumberStringCheck           				*
 *                                                                      *
 * Description: Check if a number and a string appear on opposite sides *
 *              of an expression.                                       *
 ************************************************************************/

static int NumberStringCheck(tree)
     Tree tree;
{
  if(!IsExpression(tree))
    return 0;

  if((IsNumber(tree -> left) || IsString(tree -> left)) &&
     (IsNumber(tree -> right) || IsString(tree -> right)))
    if(IsSatisfied(tree, NULL) == ALWAYS)
      return 1;

  return 0;
}


/************************************************************************
 * Function:	PrintExpressionWarning     				*
 *                                                                      *
 *                       Needs Rewrite                                  *
 ************************************************************************/

static void PrintExpressionWarning (tree, node, blocked)
     Tree tree;
     Node node;
     int  blocked;
{
  printf("%s:%d: ", filename, GetLineNumber(tree));

  printf("%s in action '%s' is ", GetExpression(tree), node -> name);

  if(STATUS (node) == SOMETIMES)
    printf("%s\n", expression_warnings_sometimes[AVAIL (node)][blocked]); 
  else
    printf("%s\n", expression_warnings_never[AVAIL (node)][blocked]);
}


/************************************************************************
 * Function:	GetResourceStatus    					*
 *                                                                      *
 * Description: Decend down a tree checking for the existance of a      *
 *              resource.  This function preserves both the availability*
 *              and the level of specification of a resource.           *
 ************************************************************************/

static void GetResourceStatus (tree, expr, node)
     Tree tree;
     Tree expr;
     Node node;
{
  Node left;
  Node right;
  
  left = NodeCreate("left", 0, 0);
  right = NodeCreate("right", 0, 0);
  
  left  -> data = (void *) malloc (sizeof (struct data));
  right -> data = (void *) malloc (sizeof (struct data));
  
  STATUS (left) = UNKNOWN;
  AVAIL  (left) = FALSE;
  
  STATUS (right) = UNKNOWN;
  AVAIL  (right) = FALSE;
  
  if((IS_OP_TREE (expr) && TREE_OP (expr) == AND) ||
     IsExpression(expr)) {
    GetResourceStatus(tree, expr -> left, left);
    STATUS (node) = r_stable [STATUS (node)] [STATUS (left)];
    AVAIL  (node) = r_table  [AVAIL  (node)] [AVAIL  (left)];
    
    GetResourceStatus(tree, expr -> right, right);
    STATUS (node) = r_stable [STATUS (node)] [STATUS (right)];
    AVAIL  (node) = r_table  [AVAIL  (node)] [AVAIL  (right)];
    
  } else if(IS_OP_TREE (expr) && TREE_OP (expr) == OR) {
    
    GetResourceStatus(tree, expr -> left, left);
    STATUS (node) = j_stable [STATUS (node)] [STATUS (left)];
    AVAIL  (node) = j_table  [AVAIL  (node)] [AVAIL  (left)];
    
    GetResourceStatus(tree, expr -> right, right);
    STATUS (node) = j_stable [STATUS (node)] [STATUS (right)];
    AVAIL  (node) = j_table  [AVAIL  (node)] [AVAIL  (right)];
  } else if(IsResource(tree) || IsString(tree)) {
    STATUS (node) = IsSpecified(tree, expr);
    
    if(STATUS (node) == SPECIFIED ||
       STATUS (node) == PARTIALLY_SPECIFIED ||
       STATUS (node) == CONTRADICTION)
      AVAIL (node) = TRUE;
    
  } else {

    STATUS (node) = r_stable [STATUS (node)] [UNSPECIFIED];
    AVAIL  (node) = r_table  [AVAIL  (node)] [FALSE];   
  } 

  free(left -> data);
  free(right -> data);

  NodeDestroy(left);
  NodeDestroy(right);
}


/************************************************************************
 * Function:	GetExprStatus    					*
 *                                                                      *
 * Description: Decend down the expr tree looking for expressions that  *
 *              Satisfy the tree expression preserving availability and *
 *              level of satisfaction.                                  *
 ************************************************************************/

static void GetExprStatus (tree, expr, node)
     Tree tree;
     Tree expr;
     Node node;
{
  Node left;
  Node right;

  left = NodeCreate("left", 0, 0);
  right = NodeCreate("right", 0, 0);

  left  -> data = (void *) malloc (sizeof (struct data));
  right -> data = (void *) malloc (sizeof (struct data));

  STATUS (left) = UNKNOWN;
  AVAIL  (left) = FALSE;
  
  STATUS (right) = UNKNOWN;
  AVAIL  (right) = FALSE;
  
  if(IsExpression(tree) && IsExpression(expr)) 
    if(IsSpecified(tree->left, expr) == SPECIFIED ||
       IsSpecified(tree->right, expr) == SPECIFIED) {
      STATUS (node) = IsSatisfied(tree, expr);

    if(STATUS(node) == ALWAYS || STATUS (node) == SOMETIMES)
      AVAIL (node) = TRUE;
  }

  if(IS_OP_TREE (expr) && TREE_OP (expr) == AND) {
    GetExprStatus(tree, expr -> left, left);
    STATUS (node) = r_etable [STATUS (node)] [STATUS (left)];
    AVAIL  (node) = r_table  [AVAIL  (node)] [AVAIL  (left)];

    GetExprStatus(tree, expr -> right, right);
    STATUS (node) = r_etable [STATUS (node)] [STATUS (right)];
    AVAIL  (node) = r_table  [AVAIL  (node)] [AVAIL  (right)];
  }

  if(IS_OP_TREE (expr) && TREE_OP (expr) == OR) {
    GetExprStatus(tree, expr -> left, left);
    STATUS (node) = r_etable [STATUS (node)] [STATUS (left)];
    AVAIL  (node) = j_table  [AVAIL  (node)] [AVAIL  (left)];

    GetExprStatus(tree, expr -> right, right);
    STATUS (node) = r_etable [STATUS (node)] [STATUS (right)];
    AVAIL  (node) = j_table  [AVAIL  (node)] [AVAIL  (right)];
  }

  free(left -> data);
  free(right -> data); 

  NodeDestroy(left);
  NodeDestroy(right);
}


/************************************************************************
 * Function:	FindConcurrent						*
 *									*
 * Description:	Finds and records all concurrently executing nodes	*
 *		starting from the specified node (initially a branch	*
 *		node) until the specified stopping point (a rendezvous	*
 *		node).							*
 ************************************************************************/

static void FindConcurrent (node, stop)
    Node node;
    Node stop;
{
  int  i;
  Node successor;
  
  MARKED (node) = TRUE;
  ListPut (concurrent, node);
  
  for (i = 0; i < ListSize (node -> successors); i ++) {
    successor = (Node) ListIndex (node -> successors, i);
    
    if (successor == stop)
      ListPut (roots, node);
    else if (MARKED (successor) == FALSE)
      FindConcurrent (successor, stop);
  }
}

/************************************************************************
 * Function:	NodeDestroy						*
 *									*
 * Description:	Destroys the structures associated with the node.	*
 *		                                                        *
 ************************************************************************/

void NodeDestroy (node)
     Node node;
{
  if (node -> agent != NULL)
    TreeDestroy (node -> agent);
  
  if (node -> provides != NULL)
    TreeDestroy (node -> provides);
  
  if (node -> requires != NULL)
    TreeDestroy (node -> requires);
  
  free (node);
}


/************************************************************************
 * Function:	DoGlobalChecks						*
 *									*
 * Description:	Performs the data-flow analyses global to a node in the	*
 *		process graph.						*
 ************************************************************************/

void DoGlobalChecks (graph)
     Graph graph;
{
  Node node;
  
  /* Allocate private data for each node. */
  
  for (node = graph -> source; node != NULL; node = node -> next) {
    node -> data = (void *) malloc (sizeof (struct data));
    NECESSARY (node) = ListCreate ( );
  }
  
  /* Check if consumed resources are produced. */
  
  if(r_flag)
    for (node = graph -> source; node != NULL; node = node -> next)
      if (node -> requires != NULL)
	CheckProducers (node -> requires, node, graph);


  /* Check if produced resources are consumed. */

  if(p_flag)
    for (node = graph -> source; node != NULL; node = node -> next)
      if (node -> provides != NULL)
	CheckConsumers (node -> provides, node, graph);

  /* Check if expressions are satisfiable. */  

  if(x_flag)
    for (node = graph -> source; node != NULL; node = node -> next)
      if (node -> requires != NULL)
	CheckExpressions (node -> requires, node, graph); 

 
  /* Deallocate private data allocated for each node. */
  
  for (node = graph -> source; node != NULL; node = node -> next) {
    ListDestroy (NECESSARY (node));
    free (node -> data);
  }
  
}

