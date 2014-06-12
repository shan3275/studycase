include ../Make.defines.linux

PROGS = conf options

all:	${PROGS}

conf:	conf.c


options:	options.c


clean:
	rm -f ${PROGS} ${TEMPFILES} options.c conf.c
