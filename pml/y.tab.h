#ifndef BISON_Y_TAB_H
# define BISON_Y_TAB_H

#ifndef YYSTYPE
typedef union {
    struct tree  *tree;
    struct graph *graph;
    char         *string;
} yystype;
# define YYSTYPE yystype
# define YYSTYPE_IS_TRIVIAL 1
#endif
# define	ACTION	257
# define	AGENT	258
# define	BRANCH	259
# define	CREATES	260
# define	EXECUTABLE	261
# define	INPUT	262
# define	ITERATION	263
# define	MANUAL	264
# define	OUTPUT	265
# define	PROCESS	266
# define	PROVIDES	267
# define	REQUIRES	268
# define	SCRIPT	269
# define	SELECTION	270
# define	SEQUENCE	271
# define	TOOL	272
# define	OR	273
# define	AND	274
# define	EQ	275
# define	NE	276
# define	LE	277
# define	GE	278
# define	LT	279
# define	GT	280
# define	DOT	281
# define	ID	282
# define	NUMBER	283
# define	STRING	284
# define	JOIN	285
# define	RENDEZVOUS	286


extern YYSTYPE yylval;

#endif /* not BISON_Y_TAB_H */
