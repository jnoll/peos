// OSDefs.h: definitions of data types.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_OSDEFS_H)
#define _OSDEFS_H

#if!defined(_XOPEN_SOURCE)
#define _XOPEN_SOURCE
#endif

#if !defined(_WIN32)
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <semaphore.h>
#include <iostream.h>
#include <dirent.h>
#include <pwd.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <errno.h> 
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <signal.h>
#else
#include <windows.h>
#include <io.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <errno.h> 
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <signal.h>
#include <iostream>
using namespace std;
#endif

#if defined(_WIN32)
#ifndef _MT
#define _MT
#endif
#define THREAD_TYPE HANDLE
#define THREAD_FUNCTION_RETURN_TYPE int
#define THREAD_FUNCTION_RETURN_VALUE 0
#define MUTEX_TYPE  HANDLE
#define SEMAPHORE_TYPE  HANDLE
#define sleep   Sleep
#define strcasecmp strcmp
#define PROTOCOL IPPROTO_TCP
#define MAX_PATH_LEN 256
#define SHUT_RDWR SD_BOTH
#define DIRECTORY_SEPARATOR "\\"
#else
#define THREAD_TYPE pthread_t
#define THREAD_FUNCTION_RETURN_TYPE void* 
#define THREAD_FUNCTION_RETURN_VALUE NULL
#define MUTEX_TYPE	pthread_mutex_t
#define SEMAPHORE_TYPE	sem_t
#define PROTOCOL 0
#define MAX_PATH_LEN 256
#define DIRECTORY_SEPARATOR "/"
#endif

#if !defined(SHUT_RDWR)
#define SHUT_RDWR 2
#endif

const int  SLEEP_TIME = 5; // 5 seconds
const char MESSAGE_DELIMETER = '\n';
const char END_OF_STRING = '\0';
const char SC_WAIT = 'a';
const char SC_FORK = 'b';
const char SC_JOIN = 'c';
const char SC_SELECT = 'd';
const char SC_EXIT = 'e';

enum Event
{
	NEW       = 0,
	READY     = 1,
	RUN       = 2,
	DONE      = 3,
	SUSPEND   = 4,
	ABORT     = 5,
	NONE      = 6
};

enum ProcessState  
{
	PS_NEW          = 7,
	PS_READY_CHILD  = 8,
        PS_READY_ACT    = 9,
	PS_RUN          = 10,
	PS_WAIT         = 11,
	PS_DONE         = 12,
        PS_ABORT        = 13
};


/* process context */
struct context_t {
    int PC;           /* key of .gdbm file */
    int SP;           /* key of .gdbm file */
    char parent[255]; /* parent process name, ie. martini.001 */
};

/* state infomation for an action */
struct act_t {
    int PC;           /* where to return when the wait event comes */
    int state;        /* state */
    int wait;         /* which event is this action waiting for */
};

/*
class Pipe
{
public:
	Pipe() { readFD = -1; writeFD = -1; }
	Pipe( int read, int write ) { readFD = read; writeFD = write; }
	virtual ~Pipe() {};
	void SetReadFD( int FD ) { readFD = FD; }
	void SetWriteFD( int FD ) { writeFD = FD; }
	int  GetReadFD() const { return readFD; }
	int  GetWriteFD() const { return writeFD; }
	void ClearReadFD() { readFD = -1; }
	void ClearWriteFD() { writeFD = -1; }
	void ClearFDs() { readFD = -1; writeFD = -1; }
	bool IsValid() const { return ( readFD > 0 && writeFD > 0 ); }
	bool IsReadValid() const { return ( readFD > 0 ); }
	bool IsWriteValid() const { return ( writeFD > 0 ); }
private:
	int readFD;
	int writeFD;
};
*/
#endif // !defined(_OSDEFS_H)
