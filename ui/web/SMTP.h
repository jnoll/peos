/* Header file for SMTP */
#ifndef _SMTP_
#define _SMTP_

typedef struct {
	char *address;
	int status;
	struct recipient *next;
	} recipient;

recipient *create_rcptlist();
void displayrec(recipient *head);
void clear_rcptlist(recipient *head);
int sendmailto(char *smtphost,char *mailto,char *mailcc,
char *mailbcc,char *mailfrom,char *subject,char *content,recipient *head);

#endif
