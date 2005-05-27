#ifndef YYERRCODE
#define YYERRCODE 256
#endif

#define INPUT 257
#define OUTPUT 258
#define ID 284
#define AND 274
#define DOT 282
typedef union {
  struct tree *tree;
  char   *string;
} YY_LINK_STYPE_TYPE;
extern YY_LINK_STYPE_TYPE yy_linklval;
