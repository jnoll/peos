
#include "stdio.h"

Err * err;



UInt32 cut_mode(const char*str)
{
	if (strcmp(str,"r") == 0)
		return fileModeReadOnly;
	if (strcmp(str,"w+") == 0)
		return fileModeReadWrite;
	if (strcmp(str,"r+") == 0)
		return fileModeUpdate;
	if (strcmp(str, "a+") ==0)
		return fileModeAppend;
}

FILE * fopen(const char *name, const char *mode_str)
{
	FileHand *f;
	UInt32 mode = cut_mode(mode_str);
	*f = FileOpen(0, name, 0, 0, mode, err);
	return f; 
}

char* strdup(char*string)
{
        char *newString;
        Int32 len,i;
        len=strlen(string);
        newString =malloc(len);

        for (i=0; i<len; i++)
        {
                newString[i]=string[i];
        }

        return newString;

}

char getc(FILE* stream)
{
	char *oneByte;
	FileRead(*stream, oneByte, 1, 1, err);
	return *oneByte; 	
}

Int32  fwrite(const  void  *ptr,  Int32  size,  Int32  nmemb,  FILE * stream)
{
	return FileWrite(*stream, ptr, size, nmemb, err);
}


Int32 fread(void *ptr, size_t size, size_t nmemb, FILE* stream)
{
	return FileRead(*stream, ptr, size, nmemb, err);
}

Int32 ferror(FILE* stream)
{
	*err = FileError (*stream);
	return -1;
}

