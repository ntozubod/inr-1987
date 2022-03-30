SRCDIR	= ../../../src

.c.o:
	gcc -ansi -Wall -O0 -g -I$(SRCDIR) -c $*.c

dotest:	B4_test.o $(SRCDIR)/inrx.a
	gcc -ansi -Wall -O0 -g B4_test.o $(SRCDIR)/inrx.a -o dotest

clean:
	rm -rf *.o dotest
