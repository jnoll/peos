/*****************************************************************/
/* File:        vm.c
/* Author:      Tingjun Wen
/* Date:        7/19/99
/* Description: Virtual machine. 
/*****************************************************************/   

#include <stdio.h>
#include <stdlib.h>
#include <gdbm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <dirent.h>
#include "pmlengine.h"

/* static variables local to this file so as to make local functions
   simple */
static char proc[255];             /* current process name */ 
static char model[255];            /* current model name */
static int PC = 0, SP = INT_MAX, A = 0; /* A is a register */
static char parent[255]; 
static GDBM_FILE dbfp, dbfm, dbfs; /* p: process, m: model, s: state */
static struct context_t context;   /* context of current process */

char * nextOPCode();
void push(int arg);
int pop();
int setState(char * act_name, int state);
int setWait(char * act_name, int wait);
int assert();
int storeContext();
int fetchContext();
int openDBs();
int closeDBs();

int execute(char * proc_name)
{
    char op[255];
    int ret = 0;
    int cond = 1; /* condition for the following while loop */

    strcpy(proc, proc_name);

    openDBs();
    fetchContext();
    
    while (cond) {
        /* fetch next opcode */
        nextOPCode(op);

        /* interpret opcode */
        if (strncmp(op, "start", 5) == 0) {
        }
        else if (strncmp(op, "end", 3) == 0) {
            PC--;
            cond = 0;
        }
        else if (strncmp(op, "push", 4) == 0) {
            int arg;
            sscanf(op+4, "%d", &arg);
            push(arg);
        }
        else if (strncmp(op, "pop", 3) == 0) {
            A = pop();
        }
        else if (strncmp(op, "goto", 4) == 0) {
            PC = atoi(op+4);
        } 
        else if (strncmp(op, "jump", 4) == 0) {
            int i;
            char * token;
            A = pop();
            token = strtok(op, " ");
            for (i = 0; i <= A; i++) {
                token = strtok(0, " ");
            }
            PC = atoi(token);
        } 
        else if (strncmp(op, "jzero", 5) == 0) {
            A = pop();
            if (A == 0) {
                char * token;
                token = strtok(op+5, " ");
                PC = atoi(token);
            }
            push(A);
        }
        else if (strncmp(op, "incr", 4) == 0) {
            A = pop();
            A++;
            push(A);
        }
        else if (strncmp(op, "decr", 4) == 0) {
            A = pop();
            A--;
            push(A);
        }
        else if (strncmp(op, "call set ready", 14) == 0) {
            char * token;
            token = strtok(op+14, " ");
            while (token) {
                ret = setState(token, READY);
                token = strtok(0, " ");
            }
            push(-1);
        }
        else if (strncmp(op, "call set none", 13) == 0) {
            char * token;
            token = strtok(op+13, " ");
            while (token) {
                ret = setState(token, NONE);
                token = strtok(0, " ");
            }
            push(-1);
        }
        else if (strncmp(op, "call wait done", 14) == 0) {
            char * token;
            int order = 0;
            ret = -1;
            token = strtok(op+14, " ");
            while (token && ret == -1) {
                order++;
                ret = setWait(token, DONE);
                token = strtok(0, " ");
            }
            if (ret == -1) {
                cond = 0;
                PC--;
            } else {
                push(order);
            }
        }
        else if (strncmp(op, "call wait active", 16) == 0) {
            char * token;
            int order = 0;
            token = strtok(op+16, " ");
            ret = -1;
            while (token && ret == -1) {
                order++;
                ret = setWait(token, RUN);
                
                token = strtok(0, " ");
            }
            if (ret == -1) {
                cond = 0;
                PC--;
            } else {
                push(order);
            }
        }
        else if (strncmp(op, "call assert", 11) == 0) {
            A = assert();
            push(A);
        }
        else if (strncmp(op, "call fork", 9) == 0) {
            int start;
            char me[255];
            char * child;
            
            push(-1);
            storeContext();
            closeDBs();
            strcpy(me, proc);

            sscanf(op+9, "%d", &start);
            child = createProcess(model, start, me);
            if (child != NULL) {
                execute(child);
                free(child);
            }
            
            strcpy(proc, me);
            openDBs();
            fetchContext();
        }
        else if (strncmp(op, "call join", 9) == 0) {
            push(-1);
            cond = 0;
        }
        else if (strncmp(op, "call exit", 9) == 0) {
            push(-1);
            if (strlen(parent) != 0) {
                storeContext();
                closeDBs();
                execute(parent);
                openDBs();
                fetchContext();
            }
            cond = 0;
        }
        else if (strncmp(op, "call err", 8) == 0) {
            push(-1);
            cond = 0;
        }
        /* else if (strncmp(op, "call", 4) == 0) {
            push(-1);
            cond = 0;
        }*/
    };
    storeContext();
    closeDBs();

    return 0;
}

char * nextOPCode(char * op)
{
    datum key, content;

    /* fetch next opcode */
    key.dptr = (char *)&PC;
    key.dsize = sizeof(PC);
    content = gdbm_fetch(dbfm, key);
    if (content.dptr == NULL) {
        perror("gdbm_fetch");
        gdbm_close(dbfp);
        return NULL;
    }
    strncpy(op, content.dptr, content.dsize);
printf("%d %s\n", PC, op); fflush(stdout);
    free(content.dptr);
    PC++;
    return op;
}

void push(int arg)
{
    datum key, content;
    int ret = -1;

    key.dptr = (char *)&SP;
    key.dsize = sizeof(SP);
    content.dptr = (char *)&arg;
    content.dsize = sizeof(arg);
    ret = gdbm_store(dbfp, key, content, GDBM_REPLACE);
    if (ret != 0) {
        perror("gdbm_store");
    }
    SP--;
}

int pop()
{
    datum key, content;

    if (SP == INT_MAX) {
        perror("stack underflow");
    }
    SP++;
    key.dptr = (char *)&SP;
    key.dsize = sizeof(SP);
    content = gdbm_fetch(dbfp, key);
    if (content.dptr == 0) {
        return -1;
    }
    return *((int *)content.dptr);
}

int setState(char * act_name, int state)
{
    datum key, content;
    int ret = -1;
    struct act_t act;

    key.dptr = act_name;
    key.dsize = strlen(act_name) + 1;
    content = gdbm_fetch(dbfs, key);
    if (content.dptr == NULL) {
        printf("create a dummy action: %s   \n", act_name);
        act.PC = 139;
        act.state = state;
        act.wait = NEW;
    } else {
        memcpy(&act, content.dptr, content.dsize);
        free(content.dptr);
    }

    if (state == READY && act.state != NEW) {
        return 1;
    }
    if (state == DONE && act.state != RUN) {
        return 1;
    }

    key.dptr = act_name;
    key.dsize = strlen(act_name) + 1;
    act.state = state;
    content.dptr = (char *)&act;
    content.dsize = sizeof(act);
    ret = gdbm_store(dbfs, key, content, GDBM_REPLACE);
    if (ret != 0) {
        perror("gdbm_store");
    }

    return ret;
}

int setWait(char * act_name, int wait)
{
    datum key, content;
    int ret = -1;
    struct act_t act;

    key.dptr = act_name;
    key.dsize = strlen(act_name) + 1;
    content = gdbm_fetch(dbfs, key);
    if (content.dptr == NULL) {
        printf("create a dummy action: %s   \n", act_name);
        act.PC = 139;
        act.state = NEW;
        act.wait = wait;
    } else {
        memcpy(&act, content.dptr, content.dsize);
        free(content.dptr);
    }

    if (act.state == wait)
        return 0;    
    key.dptr = act_name;
    key.dsize = strlen(act_name) + 1;
    act.wait = wait;
    content.dptr = (char *)&act;
    content.dsize = sizeof(act);
    ret = gdbm_store(dbfs, key, content, GDBM_REPLACE);
    if (ret != 0) {
        perror("gdbm_store");
    }

    return -1;
}


int assert()
{
    /* need to be implemented later */
    return 1;
}

int fetchContext()
{
    datum key, content;
    int line;

    /* fetch context at key 0 */
    line = 0;
    key.dptr = (char *)&line;
    key.dsize = sizeof(line);
    content = gdbm_fetch(dbfp, key);
    if (content.dptr == NULL) {
        gdbm_close(dbfp);
        return -1;    
    }    
    memcpy(&context, content.dptr, content.dsize);
    free(content.dptr);
    PC = context.PC;
    SP = context.SP;
    strcpy(parent, context.parent);
printf("FETCH:proc = %s, parent=%s,PC=%d, SP=%d\n", proc, parent, PC, SP);

    return 0;
}

int storeContext()
{
    datum key, content;
    int ret = -1;
    int line;

    /* store context */
    context.PC = PC;
    context.SP = SP;
    strcpy(context.parent, parent);
    line = 0;
    key.dptr = (char *)&line;
    key.dsize = sizeof(line);
    content.dptr = (char *)&context;
    content.dsize = sizeof(struct context_t);
    ret = gdbm_store(dbfp, key, content, GDBM_REPLACE);
    if (ret != 0) {
        perror("gdbm_store");
    }
printf("STORE:proc = %s, parent=%s,PC=%d, SP=%d\n\n", proc, parent,PC, SP);
    return ret;
}

int openDBs()
{
    char path[255];

    /* open proc db */    
    strcpy(path, uname);
    strcat(path, "/");
    strcat(path, proc);
    dbfp = gdbm_open(path, 0, GDBM_WRITER, 0777, 0);
    if (dbfp == NULL) {
        sendUI("500 can not open proc db\n");
        return -1;
    }

    /* open state db */    
    strcpy(path, uname);
    strcat(path, "/");
    strcat(path, proc);
    strcat(path, ".state");
    dbfs = gdbm_open(path, 0, GDBM_WRITER, 0777, 0);
    if (dbfs == NULL) {
        gdbm_close(dbfp);
        sendUI("500 can not open state db\n");
        return -1;
    }

    /* get model name */    
    strcpy(model, proc);
    strcpy(strrchr(model, '.'), "");

    /* open model db */
    strcpy(path, "model/");
    strcat(path, proc);
    strcpy(strrchr(path, '.'), ".gdbm");
    dbfm = gdbm_open(path, 0, GDBM_READER, 0777, 0);
    if (dbfm == NULL) {
        gdbm_close(dbfp);
        gdbm_close(dbfs);
        sendUI("500 can not open model db\n");
        return -1;
    }
    return 0;
}

int closeDBs()
{
    gdbm_close(dbfp);
    gdbm_close(dbfm);
    gdbm_close(dbfs);

    return 0;
}
