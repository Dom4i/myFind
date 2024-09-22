CC = gcc
CFLAGS = -Wall -g

all:  myFind


myFind: myFind.c parseArguments.c searchDirectory.c
	$(CC) $(CFLAGS) -o myFind myFind.c parseArguments.c searchDirectory.c

parseArguments: parseArguments.c
	$(CC) $(CFLAGS) -o parseArguments parseArguments.c

searchDirectory: searchDirectory.c
	$(CC) $(CFLAGS) -o searchDirectory searchDirectory.c

clean:
	rm -f getopt mycommand myFind
