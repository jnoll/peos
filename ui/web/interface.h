#ifndef _INTERFACE_
#define _INTERFACE_

typedef struct {
	char status[4];
	char *value;
	int cont;
	} engine_response;

typedef struct {
	char *process;
	char *task;
	char *type;
	char *mode;
	char *requires;
	char *provides;
	char *agent;
	char *script;
	} engine_reply;

int login(simplesock s,char *name,char *password);
void logout(simplesock s);
char *summary(simplesock s,char *ret_value, int createForm);
char *list(simplesock s,char *ret_value, int createForm);
char *available(simplesock s,char *ret_value, int createForm);
char *running(simplesock s,char *ret_value, int createForm);
int run(simplesock s,char *process,char *task);
int done(simplesock s,char *process,char *task);
int create(simplesock s,char *model);

char *add_message(char *msg_address,char *txt2add);

#endif
