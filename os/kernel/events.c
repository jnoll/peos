/*****************************************************************/
/* File:        events.c
/* Author:      Tingjun Wen
/* Date:        7/19/99
/* Description: Event handling functions.
/*****************************************************************/   

#include <stdio.h>
#include <stdlib.h>
#include <gdbm.h>
#include <sys/stat.h>
#include <sys/types.h>
#define __USE_XOPEN
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <dirent.h>
#include <pwd.h>
#include <errno.h>
#include "pmlengine.h"

/*********************** pmlLogin ********************
* pmlLogin() always return with success.
*/
void pmlLogin()
{
    char in_buf[4096] = {0};
    int ret = -1;
    char cmd[17] = {0};
    char msg[1024];

    /*** login - what first shows up when you run PMLServer ***/
    sendHelp();
    sendUI("\nPlease login to use the system:\n");
    do {
        memset(in_buf, 0, sizeof(in_buf));
		readUI(in_buf, sizeof(in_buf));
        if (sscanf(in_buf, "%16s", cmd) != 1)
		{
			printf("sscanf error\n");
            continue;
		}
        if (strcasecmp(cmd, "LOGIN") == 0) {
           if (sscanf(in_buf+5, "%9s%14s", uname, passwd) != 2) {
                sendUI("500 please use correct format! <'login' username pwd>\n");
                continue;
            } 
            ret = authenticate(uname, passwd);
            if (ret != 0)
                sendUI("500 login failed, please try again\n");
            else {
	      sprintf(msg, "100 login successful.  Welcome, %s!\n", uname);
	      sendUI(msg);
	    }
        }
	
        else if (strcasecmp(cmd, "EXIT") == 0) {
	  sendUI("Goodbye.\n");
	  close(newsockfd);
	  exit(0);
        }
	
        else if (strcasecmp(cmd, "HELP") == 0) {
            sendHelp();
        }

        else {
            sendUI("500 please login first!\n");
        }
    } while (ret != 0);
} /* pmlLogin */

/********************** authenticate *********************
* Makes a directory with the same name as the user-name
* given by the user at login, in which to store all
* the state files for instantiated processes.
*/
int authenticate(char * uname, char * passwd)
{
    struct passwd * uinfo;
    char pwd[13];
        char salt[2];
    int ret = -1;

// Ming, I comment out the lines. It blocked me.
//   uinfo = getpwnam(uname);
//   if (uinfo == NULL)
//       return -1;

    ret = mkdir(uname, 0777);
    if (ret != 0 && errno != EEXIST) {
        sendUI("500 error making user's working directory\n");
        return -1;
    }

    /* skip password checking at this time */
    /* 
    salt[0] = uinfo->pw_passwd[0];
    salt[1] = uinfo->pw_passwd[1];
    if (strncmp(uinfo->pw_passwd, crypt(passwd, salt), 13) != 0) {
        return -1;
    }
    */ 

    return 0;
} /* authenticate */

/************************ listModel ***********************
* Opens the "model" directory and displays all the
* available models (all the .gdbm files, stripped of
* their extensions). The user can then create instances
* of one or more of the available process models.
*/
int listModel()
{
    DIR * dir;
    struct dirent * ent;
    char * ext;
    char msg[4096];

    dir = opendir("model");
    if (dir == NULL) {
        sendUI("500 model directory does not exist\n");
        return -1;
    }
    ent = readdir(dir);
    while (ent) {
        ext = strrchr(ent->d_name, '.');
        /* no extension */
        if (ext == NULL) {
            ent = readdir(dir);
            continue;
        }
        if (strcmp(ext, ".gdbm") != 0) {
            ent = readdir(dir);
            continue;
        }
        ext[0] = 0;
        sprintf(msg, "100-%s\n", ent->d_name);
        sendUI(msg);
        ent = readdir(dir);
    };
    sendUI("100 \n");
    
    return 0;
} /* listModel */

#ifdef USE_VM_VERSION
/***************************** createProcess *******************
* -when creating a new process, assign startPC 0,
* -when creating a child process (fork), assign startPC to the start PC 
* This function handles the instantiation of a process model.
*/ 
char * createProcess(char * model, int startPC, char * parent)
{
    struct context_t context;
    int ret = 0;
    GDBM_FILE dbfm, dbfp, dbfs; /* model, proc and state db */
    int PC = 0;
    datum key, content;
    char * suffix;
    int suf;
    char path[255];
    char * ext;
    char msg[1024];
    char * proc;
    char act_name[255];
    struct act_t act;

    proc = (char *)malloc(255);
    if (proc == NULL) {
      sprintf(msg, "500 create of %s failed due to memory limit\n", model);
      sendUI(msg);
      return NULL;
    }
    
    /* open database/folder of available models */
    /* - these models are in the form of compiled .gdbm files */
    strcpy(path, "model/");
    strcat(path, model);
    strcat(path, ".gdbm");
    dbfm = gdbm_open(path, 0, GDBM_READER, 0777, 0);
    if (dbfm == NULL) {
      sprintf(msg, "500 %s does not exist. Try 'list' to see what's available.\n", model);
      sendUI(msg); 
      return NULL;
    };

    /* find an available suffix. max of 999 active processes per user */
    suf = 1;
    dbfp = NULL;
    do {
        if (dbfp)
            gdbm_close(dbfp);
        strcpy(path, uname);
        strcat(path, "/");
        strcpy(proc, model);
        sprintf(proc+strlen(proc), ".%03d", suf);
        strcat(path, proc);
        dbfp = gdbm_open(path, 0, GDBM_READER, 0777, 0); 
        suf++;
    } while (suf < 999 && dbfp != NULL); 

    if (suf == 999) {
        gdbm_close(dbfm);
        sendUI("500 You have too many (999) proceses running!\n");
        return NULL;
    }

    /* create proc db */
    dbfp = gdbm_open(path, 0, GDBM_WRCREAT, 0777, 0); 
    if (dbfp == NULL) {
        gdbm_close(dbfm);
        sendUI("500 create process failed\n");
        return NULL;
    }

    /* create state db */    
    strcat(path, ".state");
    dbfs = gdbm_open(path, 0, GDBM_WRCREAT, 0777, 0); 
    if (dbfs == NULL) {
        gdbm_close(dbfm);
        gdbm_close(dbfp);
        sendUI("500 create state failed\n");
        return NULL;
    }

    /* initialize state informatin and find first PC */
    PC = 0;
    while(1) {
        key.dptr = (char *)&PC;
        key.dsize = sizeof(PC);
        content = gdbm_fetch(dbfm, key);
        if (content.dptr == NULL)
            break;
        if (strcmp(content.dptr, "start") == 0) {
            context.PC = PC;
            break;
        }
        memset(act_name, 0, sizeof(act_name));
        sscanf(content.dptr, "%s", act_name);
        free(content.dptr);
        key.dptr = (char *)act_name;
        key.dsize = strlen(act_name) + 1; 
        act.PC = PC;
        act.state = NEW;
        act.wait = 0;
        content.dptr = (char *)&act;
        content.dsize = sizeof(act);
        gdbm_store(dbfs, key, content, GDBM_REPLACE);
        PC++;
    }
 
    gdbm_close(dbfm);
    gdbm_close(dbfs);
    
    strcpy(context.parent, "");
    if (startPC != 0) {
        context.PC = startPC;
        strcpy(context.parent, parent);
    }

    /* store context to key 0 */
    context.SP = INT_MAX;
    PC = 0;
    key.dptr = (char *)&PC;
    key.dsize = sizeof(PC);
    content.dptr = (char *)&context;
    content.dsize = sizeof(struct context_t);
    gdbm_store(dbfp, key, content, GDBM_REPLACE);
    gdbm_close(dbfp);
    
    if (startPC == 0) {
      sprintf(msg, "100 create of %s was successful!\n", model);
      sendUI(msg);
    }
    
    return proc;
} /* createProcess */
#endif

/***************************** queryActions *********************
 * Handles the determination of the "available" or next-to-do
 * actions of all ongoing processes for the user's account.
 * It works by accessing/querying the .state files in the
 * 'username' directory..
 */
int queryActions(int state)
{
    DIR * dir;
    struct dirent * ent;
    char path[255];
    char * ext;
    GDBM_FILE dbfs, dbfm;
    datum keys, keym, content;
    char act_name[255];
    struct act_t act;
    char msg[1024];
    char proc[255];

    memset(msg, 0, 1024);
    strcpy(msg, "100 ");    
    
    strcpy(path, uname);
    strcat(path, "/");
    dir = opendir(uname);
    if (dir == NULL) {
        sendUI("500 no available processes\n");
        return -1;
    }

    ent = readdir(dir); 
    while (ent) { 
        ext = strrchr(ent->d_name, '.');
        if (ext == NULL) {
            ent = readdir(dir);
            continue;
        }
        if (strcmp(ext, ".state") !=0){
            ent = readdir(dir);
            continue;
        }
        /* strip .state extension */
        strcpy(proc, ent->d_name);
        ext = rindex(proc, '.');
        ext[0] = 0;

        strcpy(path, uname);
        strcat(path, "/");
        strcpy(path+strlen(uname)+1, ent->d_name);
        dbfs = gdbm_open(path, 0, GDBM_READER, 0777, 0);
        if (dbfs == NULL) {
            printf("open state db error\n");
            break;
        }

        /* strip .state extension from filename*/
        strcpy(proc, ent->d_name);
        ext = rindex(proc, '.');
        ext[0] = 0;

        strcpy(path, "model/");
        strcat(path, ent->d_name);
        ext = rindex(path, '.');
        ext[0] = 0;
        ext = rindex(path, '.');
        strcpy(ext, ".gdbm");

        dbfm = gdbm_open(path, 0, GDBM_READER, 0777, 0);
        if (dbfm == NULL) {
            printf("open model db error\n");
            break;
        }

        keys = gdbm_firstkey(dbfs);
        while (keys.dptr) {
            content = gdbm_fetch(dbfs, keys);
            if (content.dptr == NULL)
                break;
            memcpy(&act, content.dptr, sizeof(struct act_t));
            free(content.dptr);
            if (act.state == state) {
                keym.dptr = (char *)&act.PC;
                keym.dsize = sizeof(act.PC);
                content = gdbm_fetch(dbfm, keym);
                if (content.dptr != NULL) {
                    strcpy(msg, "100-");
                    strcat(msg, proc);
                    strcat(msg, " ");
                    strcat(msg, content.dptr);
                    strcat(msg, "\n");
                    sendUI(msg);
                    free(content.dptr);
                } else {
                    memset(msg, 0, sizeof(msg));
                    strcpy(msg, "100-");
                    strcat(msg, proc);
                    strcat(msg, " ");
                    strncat(msg, keys.dptr, keys.dsize);
                    strcat(msg, "\n");
                    sendUI(msg);
                }
            }
            keys = gdbm_nextkey(dbfs, keys);
        };

        gdbm_close(dbfm);
        gdbm_close(dbfs);

        ent = readdir(dir);
    };
    closedir(dir);

    sendUI("100 \n");

    return 0;
} /* queryActions */

#ifdef USE_VM_VERSION
/****************************** runAction ********************
 * Just what it sounds like - starts an action running (at the
 * user's request, with a command-line I/O). The action must be
 * the next-to-do action from a current process (see queryActions()
 * above).
 */
int runAction(char * proc, char * act_name)
{
    int ret = 0;
    char path[255];
    GDBM_FILE dbfs;
    datum key, content;
    struct act_t act;
    char msg[1024]; //w/ sprintf(), to format output strings for sendUI

    strcpy(path, uname);
    strcat(path, "/");
    strcat(path, proc);
    strcat(path, ".state");
    dbfs = gdbm_open(path, 0, GDBM_WRCREAT, 0777, 0); 
    if (dbfs == NULL) {
      sprintf(msg, "500 can not open process %s\n", proc);
      sendUI(msg);
      return -1;
    }

    key.dptr = act_name;
    key.dsize = strlen(act_name) + 1;
    content = gdbm_fetch(dbfs, key);
    if (content.dptr == NULL) {
      sprintf(msg, "500 no action %s in process %s \n", act_name, proc);
      sendUI(msg);
      gdbm_close(dbfs);    
      return -1;
    }
    memcpy(&act, content.dptr, content.dsize);
    free(content.dptr);

    if (act.state != READY) {
      sprintf(msg, "500 %s not an available action - see 'available'\n", act_name);
      sendUI(msg);
      gdbm_close(dbfs);    
      return -1;
    }

    key.dptr = act_name;
    key.dsize = strlen(act_name) + 1;
    act.state = RUN;
    content.dptr = (char *)&act;
    content.dsize = sizeof(act);
    ret = gdbm_store(dbfs, key, content, GDBM_REPLACE);
    if (ret != 0) {
        perror("gdbm_store");
    }

    gdbm_close(dbfs);    
    sprintf(msg, "100 successful run of action: %s, of process: %s\n", act_name, proc);
    sendUI(msg);

    if (act.wait == RUN) {
        execute(proc);
    }
    
    return 0;
} /* runAction */
#endif

/********************** doneAction ***************************
 * Signals the finish of a currently-running action.
 * Also updates the .state, so 'available' shows the -next-
 * new action to do. 
 */
int doneAction(char * proc, char * act_name)
{
    int ret = 0;
    char path[255];
    GDBM_FILE dbfs;
    datum key, content;
    struct act_t act;
    char msg[1024]; //w/ sprintf(), to format strings for sendUI()

    strcpy(path, uname);
    strcat(path, "/");
    strcat(path, proc);
    strcat(path, ".state");
    dbfs = gdbm_open(path, 0, GDBM_WRCREAT, 0777, 0); 
    if (dbfs == NULL) {
      sprintf(msg, "500 cannot open process %s\n", proc);
      sendUI(msg);
      return -1;
    }

    key.dptr = act_name;
    key.dsize = strlen(act_name) + 1;
    content = gdbm_fetch(dbfs, key);
    if (content.dptr == NULL) {
      sprintf(msg, "500 action %s in %s isn't running or doesn't exist\n", act_name, proc);
      sendUI(msg);
      gdbm_close(dbfs);    
      return -1;
    }
    memcpy(&act, content.dptr, content.dsize);
    free(content.dptr);

    if (act.state != RUN) {
      sprintf(msg, "500 %s is not currently running - see 'running'\n", act_name);
      sendUI(msg);
      gdbm_close(dbfs);    
      return -1;
    }

    key.dptr = act_name;
    key.dsize = strlen(act_name) + 1;
    act.state = DONE;
    content.dptr = (char *)&act;
    content.dsize = sizeof(act);
    ret = gdbm_store(dbfs, key, content, GDBM_REPLACE);
    if (ret != 0) {
        perror("gdbm_store");
    }

    gdbm_close(dbfs);    
    
    sprintf(msg, "100 successfully finished %s\n", act_name);
    sendUI(msg);
    
    if (act.wait == DONE) {
      execute(proc);
    }

    return 0;
} /* doneAction */

/*************************** sendHelp *************************
 * Prints out, in menu form, a list of all valid commands...
 */
void sendHelp()
{
  sendUI("100- ********************* Help *************************************\n");
    sendUI("100-'login' <username> <password>\n");
    sendUI("100-'sample' \t\t- see a sample process instantiated, run.\n");
    sendUI("100-'list' \t\t- see all process models.\n");
    sendUI("100-'create' <modelname>- start a process.\n");
    sendUI("100-'available' \t- see next action(s) for current process(es).\n");
    sendUI("100-'run' <proc> <act> \t- run an action in 'available'.\n");
    sendUI("100-'running' \t\t- see what actions you have running.\n");
    sendUI("100-'done' <proc> <act> - finish a currently-running action.\n");
    // logout will only be relevant when we implement username/pwd change
    // without exiting the system entirely. For now, 'logout' does exactly
    // the same thing as 'exit'.
    /* sendUI("100-'logout'\n");  redundant w/ exit */
    sendUI("100-'exit' \t\t- close the connection.\n");
    sendUI("100-'help' \t\t- see this menu again.\n");
    sendUI("100- ****************************************************************\n");
} /* sendHelp */           

/*************************** sampleRun ************************
 * Prints out, step by step, a sample stepping-through of the
 * process "sample".  Provided to assist the user in understanding
 * the process of using this command-line-IF to run a process...
 */
void sampleRun()
{
  sendUI("100- ************** Running the process \"sample\" *******************\n");
  sendUI("100- 'list' \t\t\t-- after login, shows available processes.\n");
  sendUI("100- 'create sample' \t\t-- we'll choose the process called \"sample\".\n");
  sendUI("100- 'available' \t\t-- to see what the first action in \"sample\" is.\n");
  sendUI("100- 'run sample.001 edit' \t-- to begin that first action.\n");
  sendUI("100- 'running' \t\t\t-- should show \"sample.001 edit\".\n");
  sendUI("100- 'done sample.001 edit' \t-- when you've finished the action.\n");
  sendUI("100- 'run..', 'done..' \t\t-- for actions \"compile\", \"test\", and \"debug\".\n");
  sendUI("100- 'available' \t\t-- no more actions for \"sample.001\" - done!\n");
  sendUI("100- ****************************************************************\n");
} /* sampleRun */
