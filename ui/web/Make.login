CC=gcc
OBJECTS =login.o SMTP.o getcgivars.o interface.o smpsock.o strlib.o config.o

login.cgi:  $(OBJECTS)
	$(CC) -o login.cgi $(OBJECTS)

