CC = gcc
CFLAGS = -Wall -g

all: getopt mycommand myFind

getopt: getopt.c
	$(CC) $(CFLAGS) -o getopt getopt.c

mycommand: mycommand.c
	$(CC) $(CFLAGS) -o mycommand mycommand.c

myFind: myFind.c parseArguments.c
	$(CC) $(CFLAGS) -o myFind myFind.c parseArguments.c

parseArguments: parseArguments.c
	$(CC) $(CFLAGS) -o parseArguments parseArguments.c

clean:
	rm -f getopt mycommand myFind
