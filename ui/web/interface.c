#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smpsock.h"
#include "interface.h"

#define LOG stdout
#define LOGLVL 2 /* changed from 2 12/99 Mark S. */

engine_response *engineresponse(simplesock s)
{
char buf[BUFSIZ];
engine_response *ret_value = NULL;

	if (fgets(buf,BUFSIZ,s.S_in) != NULL) {
		
		if (LOGLVL > 1) {
			fprintf(LOG,"	Engine: %s",buf);
		}
		
		ret_value = (engine_response *)malloc(sizeof(engine_response));
		strncpy(ret_value->status,buf,3);
		ret_value->status[3] = '\0';
		ret_value->value = (char *)malloc((strlen(buf) - 3) * sizeof(char));
		strcpy(ret_value->value,buf+4);
		alltrim(ret_value->value);
		if (buf[3] == '-') ret_value->cont = 1; else ret_value->cont = 0;
	}
	return(ret_value);
}

char *getmultiple(char *inputstr)
{
char tempstr[BUFSIZ];
char tempret[BUFSIZ] = "";
unsigned int templen;
char *ret_value;

	do {
		sscanf(inputstr,"%s",tempstr);
		templen = strlen(tempstr);
		if ((strcmp(tempstr,"{") != 0) && (strcmp(tempstr,"}") != 0)) {
			if (strlen(tempret) != 0)
				strcat(tempret," ");
			strcat(tempret,tempstr);
		}
		memmove(inputstr,inputstr+templen+1,strlen(inputstr) - templen);
	} while (strcmp(tempstr,"}") != 0);
	alltrim(tempret);
	if (strlen(tempret) > 0) {
		ret_value = (char *)malloc((strlen(tempret) + 1) * sizeof(char));
		strcpy(ret_value,tempret);
	}
	else {
		ret_value = (char *)malloc(1 * sizeof(char));
		strcpy(ret_value,"");
	}
	return(ret_value);
}

engine_reply *enginereply(char *inputstr)
{
engine_reply *ret_value;
unsigned int templen;
char tempstr[BUFSIZ];
char tempvalue[BUFSIZ];

	if (inputstr == NULL) return(NULL);
	ret_value = (engine_reply *)malloc(sizeof(engine_reply));

	sscanf(inputstr,"%s",tempstr);			// process
	templen = strlen(tempstr);
	ret_value->process = (char *)malloc((templen + 1) * sizeof(char));
	strcpy(ret_value->process,tempstr);
	memmove(inputstr,inputstr+templen+1,strlen(inputstr) - templen);

	sscanf(inputstr,"%s",tempstr);			// task
	templen = strlen(tempstr);
	ret_value->task = (char *)malloc((templen + 1) * sizeof(char));
	strcpy(ret_value->task,tempstr);
	memmove(inputstr,inputstr+templen+1,strlen(inputstr) - templen);
	
	sscanf(inputstr,"%s %s",tempstr,tempvalue);	// type
	templen = strlen(tempvalue);
	ret_value->type = (char *)malloc((templen + 1) * sizeof(char));
	strcpy(ret_value->type,tempvalue);
	templen = templen + strlen(tempvalue);
	memmove(inputstr,inputstr+templen,strlen(inputstr) - templen + 1);
	
	sscanf(inputstr,"%s %s",tempstr,tempvalue);     // mode
	templen = strlen(tempvalue);
	ret_value->mode = (char *)malloc((templen + 1) * sizeof(char));
	strcpy(ret_value->mode,tempvalue);
	templen = templen + strlen(tempvalue);
	memmove(inputstr,inputstr+templen,strlen(inputstr) - templen + 1);

	sscanf(inputstr,"%s",tempstr);			// requires
	if (strcmp(tempstr,"requires") == 0) {
		templen = strlen(tempstr);
		memmove(inputstr,inputstr+templen+1,strlen(inputstr) - templen);
		ret_value->requires = getmultiple(inputstr);
	}
	else {
		ret_value->requires = (char *)malloc(1 * sizeof(char));
		strcpy(ret_value->requires,"");
	}

	sscanf(inputstr,"%s",tempstr);			// provides
	if (strcmp(tempstr,"provides") == 0) {
		templen = strlen(tempstr);
		memmove(inputstr,inputstr+templen+1,strlen(inputstr) - templen);
		ret_value->provides = getmultiple(inputstr);
	}
	else {
		ret_value->provides = (char *)malloc(1 * sizeof(char));
		strcpy(ret_value->provides,"");
	}

	sscanf(inputstr,"%s",tempstr);			// agent
	if (strcmp(tempstr,"agent") == 0) {
		templen = strlen(tempstr);
		memmove(inputstr,inputstr+templen+1,strlen(inputstr) - templen);
		ret_value->agent = getmultiple(inputstr);
	}
	else {
		ret_value->agent = (char *)malloc(1 * sizeof(char));
		strcpy(ret_value->agent,"");
	}

	sscanf(inputstr,"%s",tempstr);			// script
	if (strcmp(tempstr,"script") == 0) {
		templen = strlen(tempstr);
		memmove(inputstr,inputstr+templen+1,strlen(inputstr) - templen);
		ret_value->script = getmultiple(inputstr);
	}
	else {
		ret_value->script = (char *)malloc(1 * sizeof(char));
		strcpy(ret_value->script,"");
	}

	return(ret_value);
}

void freeresponse(engine_response *address)
{
	free(address->value);
	free(address);
}

void freereply(engine_reply *address)
{
	free(address->process);
	free(address->task);
	free(address->type);
	free(address->mode);
	free(address->requires);
	free(address->provides);
	free(address->agent);
	free(address->script);

	free(address);
}

int login(simplesock s,char *name,char *password)
{
engine_response *response;

	fprintf(s.S_out,"login %s %s\n",name,password);
	fflush(s.S_out);
	
	if (LOGLVL > 1) {
		fprintf(LOG,"	WebUI: login %s %s<br>\n",name,password);
	}
	
	response = engineresponse(s);
	if (response == NULL) return(0);
	if (strcmp(response->status,"100") == 0) {
		freeresponse(response);
		return(1);
	}
	freeresponse(response);
	return(0);
}

void logout(simplesock s)
{
	fprintf(s.S_out,"logout\n");
	
	if (LOGLVL > 1) {
		fprintf(LOG,"	WebUI: logout<br>\n");
	}

	fflush(s.S_out);
}

char *summary(simplesock s,char *ret_value)
{
engine_response *response;
engine_reply *reply;

	fprintf(s.S_out,"available\n");
	fflush(s.S_out);
	if (LOGLVL > 1) {
		fprintf(LOG,"   WebUI: available<br>\n");
	}
	do {
		response = engineresponse(s);
		if (response != NULL)
		if (response->cont ==1) {
			reply = enginereply(response->value);
			ret_value = add_message(ret_value,"action: run ");
			ret_value = add_message(ret_value,reply->process);
			ret_value = add_message(ret_value," ");
			ret_value = add_message(ret_value,reply->task);
			ret_value = add_message(ret_value,"<br>\n");
			freereply(reply);
		}
		freeresponse(response);
	} while(response->cont == 1);

	fprintf(s.S_out,"running\n");
	fflush(s.S_out);
	if (LOGLVL > 1) {
		fprintf(LOG,"   WebUI: running<br>\n");
	}
	do {
		response = engineresponse(s);
		if (response != NULL)
		if (response->cont ==1) {
			reply = enginereply(response->value);
			ret_value = add_message(ret_value,"action: done ");
			ret_value = add_message(ret_value,reply->process);
			ret_value = add_message(ret_value," ");
			ret_value = add_message(ret_value,reply->task);
			ret_value = add_message(ret_value,"<br>\n");
			freereply(reply);
		}
		freeresponse(response);
	} while(response->cont == 1);
	return(ret_value);
}

char *list(simplesock s,char *ret_value)
{
engine_response *response;
int tempcont;
int modelcount = 0;

	fprintf(s.S_out,"list\n");
	fflush(s.S_out);

	if (LOGLVL > 1) {
		fprintf(LOG,"	WebUI: list<br>\n");
	}

        ret_value = add_message(ret_value,"<ul >");

	do {
		response = engineresponse(s);
		if (response == NULL) return(NULL);
		tempcont = response->cont;
		if (response->cont == 1) {
                        ret_value = add_message(ret_value,"<li>");
			modelcount++;
			ret_value = add_message(ret_value,response->value);
			ret_value = add_message(ret_value,"\n");
		}
		freeresponse(response);
	} while(response->cont == 1);

        ret_value = add_message(ret_value,"</ul>\n");

	if (modelcount == 0) ret_value = add_message(ret_value,"None.<br>\n<br>\n");
	return(ret_value);
}

char *available(simplesock s,char *ret_value)
{
engine_response *response;
engine_reply *reply;
int tempcont;
int taskcount = 0;

	fprintf(s.S_out,"available\n");
	fflush(s.S_out);
	
	if (LOGLVL > 1) {
		fprintf(LOG,"	WebUI: available<br>\n");
	}
	
	do {
		response = engineresponse(s);
		if (response == NULL) return(NULL);
		tempcont = response->cont;
		if (response->cont == 1) {	// Has some reply
			taskcount++;
			reply = enginereply(response->value);
			ret_value = add_message(ret_value,"Process : ");
			ret_value = add_message(ret_value,reply->process);
			ret_value = add_message(ret_value,"<br>\nTask : ");
			ret_value = add_message(ret_value,reply->task);
			ret_value = add_message(ret_value,"<br>\nType : ");
			ret_value = add_message(ret_value,reply->type);
			ret_value = add_message(ret_value,"<br>\nMode : ");
			ret_value = add_message(ret_value,reply->mode);
			ret_value = add_message(ret_value,"<br>\nRequires : ");
			ret_value = add_message(ret_value,reply->requires);
			ret_value = add_message(ret_value,"<br>\nProvides : ");
			ret_value = add_message(ret_value,reply->provides);
			ret_value = add_message(ret_value,"<br>\nAgent : ");
			ret_value = add_message(ret_value,reply->agent);
			ret_value = add_message(ret_value,"<br>\nScript : ");
			ret_value = add_message(ret_value,reply->script);

			ret_value = add_message(ret_value,"<br>\n<br>\n");
			
			freereply(reply);
		}
		freeresponse(response);
	} while(response->cont == 1);
	if (taskcount == 0) ret_value = add_message(ret_value,"None.<br>\n<br>\n");
	return(ret_value);
}

char *running(simplesock s,char *ret_value)
{
engine_response *response;
engine_reply *reply;
int tempcont;
int taskcount = 0;
 
        fprintf(s.S_out,"running\n");
        fflush(s.S_out);
		
		if (LOGLVL > 1) {
			fprintf(LOG,"	WebUI: running<br>\n");
		}
		
        do {
                response = engineresponse(s);
                if (response == NULL) return(NULL);
                tempcont = response->cont;
                if (response->cont == 1) {      // Has some reply
                        taskcount++;
                        reply = enginereply(response->value);
                        ret_value = add_message(ret_value,"Process : ");
                        ret_value = add_message(ret_value,reply->process);
                        ret_value = add_message(ret_value,"<br>\nTask : ");
                        ret_value = add_message(ret_value,reply->task);
                        ret_value = add_message(ret_value,"<br>\nType : ");
                        ret_value = add_message(ret_value,reply->type);
                        ret_value = add_message(ret_value,"<br>\nMode : ");
                        ret_value = add_message(ret_value,reply->mode);
                        ret_value = add_message(ret_value,"<br>\nRequires : ");
                        ret_value = add_message(ret_value,reply->requires);
                        ret_value = add_message(ret_value,"<br>\nProvides : ");
                        ret_value = add_message(ret_value,reply->provides);
                        ret_value = add_message(ret_value,"<br>\nAgent : ");
                        ret_value = add_message(ret_value,reply->agent);
                        ret_value = add_message(ret_value,"<br>\nScript : ");
                        ret_value = add_message(ret_value,reply->script);

			ret_value = add_message(ret_value,"<br>\n<br>\n");
			freereply(reply);
                }
                freeresponse(response);
        } while(response->cont == 1);
        if (taskcount == 0) ret_value = add_message(ret_value,"None.<br>\n<br>\n");
	return(ret_value);
}

int run(simplesock s,char *process,char *task)
{
engine_response *response;

	fprintf(s.S_out,"run %s %s\n",process,task);
	fflush(s.S_out);
	
	if (LOGLVL > 1) {
		fprintf(LOG,"	WebUI: run %s %s<br>\n",process,task);
	}
	
	response = engineresponse(s);
	if (response == NULL) return(0);
	if (strcmp(response->status,"100") == 0) {
		freeresponse(response);
		return(1);
	}
	freeresponse(response);
	return(0);
}

int done(simplesock s,char *process,char *task)
{
engine_response *response;
 
	fprintf(s.S_out,"done %s %s\n",process,task);
	fflush(s.S_out);
	
	if (LOGLVL > 1) {
		fprintf(LOG,"	WebUI: done %s %s<br>\n",process,task);
	}
	
	response = engineresponse(s);
	if (response == NULL) return(0);
	if (strcmp(response->status,"100") == 0) {
		freeresponse(response);
		return(1);
	}
	freeresponse(response);
	return(0);
}

int create(simplesock s,char *model)
{
engine_response *response;

	fprintf(s.S_out,"create %s\n",model);
	fflush(s.S_out);

	if (LOGLVL > 1) {
		fprintf(LOG,"	WebUI: create %s<br>\n",model);
	}

	response = engineresponse(s);
	if (response == NULL) return(0);
	if (strcmp(response->status,"100") == 0) {
		freeresponse(response);
		return(1);
	}
	freeresponse(response);
	return(0);
}

char *add_message(char *msg_address,char *txt2add)
{
	if (msg_address == NULL) {              // New message
		msg_address = (char *)malloc((strlen(txt2add) + 1) * sizeof(char));
		strcpy(msg_address,txt2add);
	}
	else {
		msg_address = (char *)realloc(msg_address,(strlen(msg_address) + 
			strlen(txt2add) + 1) * sizeof(char));
		strcat(msg_address,txt2add);
	}
	return(msg_address);
}
