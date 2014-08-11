PROG    = clitest

ECFLAGS = -g -O0  -I$(SRCDIR)/fc/libcli  -I$(SRCDIR)/fc/md -I. #-Werror

ELDFLAGS = -L $(SRCDIR)/fc/libcli -L $(SRCDIR)/fc/md
LDLIBS  =   -lcli -lmd -lcrypt -lpthread

include $(MKDIR)/make.app

