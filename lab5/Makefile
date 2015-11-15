# Makefile for the lab5 driver

COMPILER_TYPE= gnu

CC=gcc

PROG=	
SRCS=	

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+=	-ldriver -lsys

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>