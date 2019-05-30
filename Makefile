# This target compiles the program.
CFLAGS=-c -g

all: GoL

GoL: main.o gameOfLife.o
	gcc main.o gameOfLife.o -lpthread -o GoL

main.o: main.c
	gcc $(CFLAGS) main.c

gameOfLife.o: gameOfLife.c
	gcc $(CFLAGS) gameOfLife.c

clean: 
	rm -f *o GoL


