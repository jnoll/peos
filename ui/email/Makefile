SHELL = /bin/bash

OUTPUT = mailui

CC = gcc

OBJS =	SMTP.o \
	config.o \
	mailui.o \
	interface.o \
	smpsock.o \
	strlib.o

all: mailui

.c.o:
	$(CC) -c $*.c

clean:
	rm -f core
	rm -f *.o
	rm -f $(OUTPUT)

install: 
	cp $(OUTPUT) ~
	cp mailui.cfg ~
# Ugh.  This would clobber anything the user already has!
	cp procmailrc ~/.promailrc


mailui:	$(OBJS)
	$(CC) -o $(OUTPUT) $(OBJS)
