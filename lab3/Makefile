# Makefile for the lab3 driver

COMPILER_TYPE= gnu

CC=gcc

PROG=	lab3
SRCS=	timer.c  lab3.c   kbd.c   test3.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+=	-ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>