#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "smpsock.h"
#include "strlib.h"
#include "SMTP.h"

#define MAILDELIM ','
#define CRLF "\r\n"
#define LOG stdout

int okresponse(simplesock s, char *expect);
void sendquit(simplesock s);
int sing_send(simplesock s,char *rcptto,recipient *head);
int mult_send(simplesock s,char *emailist,char delim,recipient *head);
void submit_content(simplesock s,char *inputstr);

int okresponse(simplesock s, char *expect)	// 'x' means don't care.
{
char buf[BUFSIZ];
	do {					// In case of multiple lines
		fgets(buf,BUFSIZ,s.S_in);	// reply, ignore everything
	} while (buf[3] == '-');		// except the last line.
	if (((expect[0] == 'x') || (expect[0] == buf[0])) &&
	    ((expect[1] == 'x') || (expect[1] == buf[1])) &&
	    ((expect[2] == 'x') || (expect[2] == buf[2]))) {
		return(1);
	}
	fprintf(LOG," SMTP: %s\n",buf);
	return(0);
}

void sendquit(simplesock s)
{
	fprintf(s.S_out,"quit\n");
	fflush(s.S_out);
}

int sendmailto(char *smtphost,char *mailto,char *mailcc,
char *mailbcc,char *mailfrom,char *subject,char *content,recipient *head)
{
int tempreturn = 0;

simplesock s;

	if (mailto == NULL || strlen(mailto) == 0) return(0); 
	/* Exit if there are no recipient */

	s = init_socket(smtphost,25);
	if (strlen(s.errmessage) != 0) {
		fprintf(LOG," SMTP: %s\n",s.errmessage);
		return(0); 	// Unable to contact the server
	}

	if (okresponse(s,"2xx") == 0) return(0);

	fprintf(s.S_out,"HELO %s\n",smtphost);
	fflush(s.S_out);

	if (okresponse(s,"2xx") == 0) {
		sendquit(s);
		return(0);
	}

	fprintf(s.S_out,"MAIL FROM:<%s>\n",mailfrom);
	fflush(s.S_out);

	if (okresponse(s,"2xx") == 0) {
		sendquit(s);
		return(0);
	}

	if (mailto != NULL && strlen(mailto) > 0) 
		tempreturn = tempreturn + mult_send(s,mailto,MAILDELIM,head);
	if (mailcc != NULL && strlen(mailcc) > 0)
		tempreturn = tempreturn + mult_send(s,mailcc,MAILDELIM,head);
	if (mailbcc != NULL && strlen(mailbcc) > 0)
		tempreturn = tempreturn + mult_send(s,mailbcc,MAILDELIM,head);
	if (tempreturn == 0) {	// Don't have any available reciepient
		sendquit(s);
		return(0);
	}

	fprintf(s.S_out,"DATA\n"); // Send Data header
	fflush(s.S_out);

	if (okresponse(s,"354") == 0) {
		sendquit(s);
		return(0);
	}

	fprintf(s.S_out,"From: %s\n",mailfrom); /* Sending header lines */
	fprintf(s.S_out,"Subject: %s\n",subject);
	if (mailto != NULL) fprintf(s.S_out,"To: %s\n",mailto);
	if (mailcc != NULL) fprintf(s.S_out,"CC: %s\n",mailcc);
	fprintf(s.S_out,"Content-Type: text/plain; charset=us-ascii\n");
	fprintf(s.S_out,"Content-Transfer-Encoding: 7bit\n");
	submit_content(s,content);
	fprintf(s.S_out,"\n.\n");
	fflush(s.S_out);

	if (okresponse(s,"250") == 0) {
		sendquit(s);
		return(0);	// Error occurs
	}
	sendquit(s);
	return(1);
}

recipient *create_rcptlist()
{
recipient *head;
	head = (recipient *)malloc(sizeof(recipient));  // Initialize recipient list
	head->address = NULL;
	head->next = NULL;
	return(head);
}

void clear_rcptlist(recipient *head)
{
recipient *tail = head;
	while (tail != NULL) {
		tail = (recipient *)tail->next;
		free(head->address);
		free(head);
		head = tail;
	}
}

void displayrec(recipient *head)
{
recipient *tail = head;
	while (tail != NULL) {
		if (tail->address != NULL) {
			printf("%d, %s\n",tail->status,tail->address);
		}
		tail = (recipient *)tail->next;
	}
}

int sing_send(simplesock s,char *rcptto,recipient *head)
{
recipient *tail, *item;

	alltrim(rcptto);
	if (strlen(rcptto) == 0) return(0);
	item = head;
	while (item != NULL) {
		tail = item;
		item = (recipient *)item->next;
	}
	item = (recipient *)malloc(sizeof(recipient));
	item->next = NULL;
	item->status = 1;
	tail->next = (struct recipient *)item;
	item->address = (char *)malloc((strlen(rcptto) + 1) * sizeof(char));
	strcpy(item->address,rcptto);

        fprintf(s.S_out,"RCPT TO:<%s>%s",rcptto,CRLF);
	fflush(s.S_out);

	if (okresponse(s,"2xx") == 0) item->status = 0;

	return(item->status);
}

int mult_send(simplesock s,char *emailist,char delim,recipient *head)
{
int listlen = strlen(emailist);
char *tempstr = (char *)malloc((listlen + 1) * sizeof(char));
char *getleft = (char *)malloc((listlen + 1) * sizeof(char));
char *temppos;
int tempreturn = 0;
	strcpy(tempstr,emailist);
	do {
		temppos = strchr(tempstr,delim);
		if (temppos != NULL) {
			strncpy(getleft,tempstr,strlen(tempstr) - strlen(temppos));
			memmove(tempstr,temppos + 1,strlen(temppos) * sizeof(char));
			if (strlen(tempstr) != strlen(temppos))
				tempreturn = tempreturn + sing_send(s,getleft,head);
		}
		else {
			tempreturn = tempreturn + sing_send(s,tempstr,head);
		}
	} while (temppos != NULL);

	free(tempstr);
	free(getleft);

	if (tempreturn == 0) return(0);	// None of the recipients available.
	return(1);
}

void submit_content(simplesock s,char *inputstr)
{
unsigned long int txtpos;

	if (inputstr != NULL) {
		for (txtpos = 0;txtpos < strlen(inputstr);txtpos++) {
			if ((inputstr[txtpos] == '.') &&
			    (txtpos == 0 || inputstr[txtpos-1] == '\n')) 
				fprintf(s.S_out,".");	
				// Insert a period for transparency purpose.
			fprintf(s.S_out,"%c",inputstr[txtpos]);
		}
	}
}
