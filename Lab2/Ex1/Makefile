all: child.o
	gcc child.o -o child
child.o:	child.c
	gcc -c child.c

clean:
	rm -f *.o child
