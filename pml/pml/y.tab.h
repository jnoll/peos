#ifndef YYERRCODE
#define YYERRCODE 256
#endif

#define ACTION 257
#define AGENT 258
#define BRANCH 259
#define CREATES 260
#define EXECUTABLE 261
#define INPUT 262
#define ITERATION 263
#define MANUAL 264
#define OUTPUT 265
#define PROCESS 266
#define PROVIDES 267
#define REQUIRES 268
#define SCRIPT 269
#define SELECTION 270
#define SEQUENCE 271
#define TOOL 272
#define OR 273
#define AND 274
#define EQ 275
#define NE 276
#define LE 277
#define GE 278
#define LT 279
#define GT 280
#define NOT 281
#define DOT 282
#define QUALIFIER 283
#define ID 284
#define NUMBER 285
#define STRING 286
#define JOIN 287
#define RENDEZVOUS 288
typedef union {
    int           val;
    struct tree  *tree;
    struct graph *graph;
    char         *string;
} YYSTYPE;
extern YYSTYPE yylval;
