SRCDIR	= ../../../src

.c.o:
	gcc -ansi -Wall -O0 -g -I$(SRCDIR) -c $*.c

all:	a.out
	./a.out

a.out:	B4_test.o $(SRCDIR)/inrx.a
	gcc -ansi -Wall -O0 -g B4_test.o $(SRCDIR)/inrx.a

clean:
	rm -rf *.o a.out
