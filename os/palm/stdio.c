
#include "stdio.h"
#include <PalmOS.h>

Err * err;



UInt32 cut_mode(const char*str)
{
/*	if (strcmp(str,"r") == 0)
		return fileModeReadOnly;
	if (strcmp(str,"w+") == 0)
		return fileModeReadWrite;
	if (strcmp(str,"r+") == 0)
		return fileModeUpdate;
	if (strcmp(str, "a+") ==0)
		return fileModeAppend;
*/
	return 0;
}

FILE * fopen(const char *name, const char *mode_str)
{
	FILE *f;
	LocalID id;
//	UInt32 mode = cut_mode(mode_str);
	f = malloc(sizeof(struct file));
	id=DmFindDatabase(0,name);
	//update to use cut_mode below for 3rd param
	f->db= DmOpenDatabase(0, id, dmModeReadOnly); 
	f->recordBuf= MemHandleLock(DmQueryRecord(f->db,0)); 
	f->place=0;
	return f; 
}

char* strdup(char*string)
{
        char *newString;
        Int32 len,i;
        len=strlen(string)+1;
        newString =malloc(len);

        for (i=0; i<len; i++)
        {
                newString[i]=string[i];
        }

        return newString;

}

int getc(FILE* stream)
{
	int oneByte, retValue;
	retValue=fread(&oneByte,1,1, stream);
	return retValue == 0 ? EOF : oneByte; 	
}

Int32  fwrite(const  void  *ptr,  Int32  size,  Int32  nmemb,  FILE * stream)
{
	// do nothing for now... don't see a need to write to file?

/*	return FileWrite(*stream, ptr, size, nmemb, err); */
}


Int32 fread(void *ptr, size_t size, size_t nmemb, FILE* stream)
{
	int i;
	if (size==1)
	{
		for(i=0; i<nmemb; i++)
		{
			if (stream->recordBuf[stream->place] == '\200')
				break;
			if (stream->recordBuf[stream->place] == '\0')
				break;

			((char*)ptr)[i]=(stream->recordBuf[stream->place]);
			stream->place++; //increment place in file
		}	
		return i == 0 ? 0 : i ;
	}	
//	return FileRead(*stream, ptr, size, nmemb, err);
}

Int32 ferror(FILE* stream)
{
//	*err = FileError (*stream);
	return 0;
}

Int32 fclose(FILE * stream)
{
	DmCloseDatabase(stream->db);
	free(stream);
//	FileClose(*stream);
	return -1;
}

void doNothing()
{

}
