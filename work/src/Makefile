HFILES	= O.h local.h
CFILES	= stamp.c Acrea.c Aopen.c Aload.c Atrim.c Alameq.c Alamcm.c Aclos.c \
Asubs.c Amin.c Abasic.c Aenum.c Acompose.c Ajoin.c Aretape.c Apowers.c \
Alenmin.c Asseq.c AGMsseq.c ALMsseq.c Aclsseq.c Astems.c Tn.c P.c Q.c V.c \
R.c U.c T2.c B.c B4.c B8.c S.c util.c Lex.c Colon.c Aunicode.c Apr.c Asave.c \
ABcomp.c INR_main.c
OFILES	= stamp.o Acrea.o Aopen.o Aload.o Atrim.o Alameq.o Alamcm.o Aclos.o \
Asubs.o Amin.o Abasic.o Aenum.o Acompose.o Ajoin.o Aretape.o Apowers.o \
Alenmin.o Asseq.o AGMsseq.o ALMsseq.o Aclsseq.o Astems.o Tn.o P.o Q.o V.o \
R.o U.o T2.o B.o B4.o B8.o S.o util.o Lex.o Colon.o Aunicode.o Apr.o Asave.o \
y.tab.o ABcomp.o INR_main.o
SOURCE	= Makefile $(HFILES) $(CFILES) Parse.y

.c.o:
	gcc -ansi -Wall -O0 -g -c $*.c
#	gcc -DUSE_MALLOC_ALLOCATOR -ansi -Wall -O0 -g -c $*.c

all:	xinr inrx.a

xinr:	$(OFILES)
	gcc -ansi -Wall -O0 -g $(OFILES) -o xinr
	cp -p xinr ~/bin/xinr

inrx.a:	$(OFILES)
	echo hi >inrx.a
	rm inrx.a
	ar q inrx.a $(OFILES)
	ranlib inrx.a

install:	xinr inrx.a
	cp -p xinr ~/bin/xinr

lint:	y.tab.c
	lint $(CFILES) y.tab.c

$(OFILES):	O.h local.h

y.tab.c y.tab.h:	Parse.y
	yacc -d Parse.y

Lex.o:	y.tab.h

clean:
	rm -f $(OFILES) y.tab.h y.tab.c *.orig inrx.a
