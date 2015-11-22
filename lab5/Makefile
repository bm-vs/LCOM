# Makefile for the lab5 driver

COMPILER_TYPE= gnu

CC=gcc

PROG=	lab5
SRCS=	lab5.c   test5.c   timer.c   kbd.c   vbe.c   video_gr.c   read_xpm.c   pixmap.c   sprite.c

CCFLAGS= -Wall

DPADD+=	${LIBDRIVER} ${LIBSYS}
LDADD+=	-llm -ldriver -lsys	liblm.a -lm

LDFLAGS+= -L .

MAN=

.include <bsd.prog.mk>
.include <bsd.gcc.mk>