#ifndef PALMFUNCTIONS_H
#define PALMFUNCTIONS_H

#define exit(a) 0

#include <include/stdio.h>
#include <FileStream.h>
#include <PalmOS.h>
#include <stdlib.h>
#include <string.h>

typedef FileHand FILE;

FILE * fopen(const char*, const char*);
char* strdup(char*s);
char getc(FILE *);
Int32  fwrite(const  void*,  Int32,  Int32,  FILE *);
Int32 fread(void *, size_t, size_t, FILE *);
Int32 ferror(FILE *);

#endif
