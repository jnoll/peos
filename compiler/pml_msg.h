#ifndef _PML_MSG_H
#define _PML_MSG_H

#define MSG_SIZE 1024

typedef enum pmlmsgtype {
	ERROR, 
	WARN,
	INFO
}pmltype;

typedef enum pmlerrno { 
	PMLINP,
	PMLSYN,
	PMLDD,
	PMLMEM,
	PMLFOPN,
	PMLPARSE,
   	PMLCPP,
	PMLVER,
	PMLCOM,
	PMLNULL
}pmlerr;

static char *pmlmsg[] = {
	"ERROR:",
	"WARNING:",
	"INFO:"
};

void
pmlprint(pmltype, pmlerr, char *,char *);

#endif
