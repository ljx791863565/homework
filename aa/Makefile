CC:=gcc
CFLAGS+= -w -g -o 
RM:=rm -rf *~
LIBS:=-lpthread

all:
	$(CC) $(CFLAGS) ser server.c  $(LIBS)
	$(CC) $(CFLAGS) cli client.c client_data.c $(LIBS)

clean:
	$(RM)
