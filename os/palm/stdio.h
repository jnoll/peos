#ifndef PALMFUNCTIONS_H
#define PALMFUNCTIONS_H



void doNothing();


#define exit(a) doNothing();
#define fprintf(...) doNothing();
#define load_process_table() 1 
#define save_process_table() 1
#define assert(...) doNothing();
#define get_resource_list_action(...) NULL
#define get_resource_list_action_provides(...) NULL
#define get_resource_list_action_requires(...) NULL
#define log_event(...) doNothing();
#define is_requires_true(...) 1
#define is_provides_true(...) 1

//for bison 
#define EINTR 4

#define BUFSIZ 8192

#include <include/stdio.h>
//#include <FileStream.h>
#include <PalmOS.h>
//#include <PalmTypes.h>
//#include <SystemPublic.h>
#include <stdlib.h>
#include <string.h>


typedef struct file FILE;

struct file{
	int place;
	char *recordBuf;
	DmOpenRef db;
};


//typedef FileHand FILE;

UInt32 cut_mode(const char*);
FILE * fopen(const char*, const char*);
char* strdup(char*s);
char getc(FILE *);
Int32  fwrite(const  void*,  Int32,  Int32,  FILE *);
Int32 fread(void *, size_t, size_t, FILE *);
Int32 ferror(FILE *);
Int32 fclose(FILE *);
#endif
