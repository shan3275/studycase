PROG    = vtysh

ECFLAGS = -g -O0  -I$(SRCDIR)/libzebra  -I. #-Werror

ELDFLAGS = -L $(SRCDIR)/libzebra
LDLIBS  =   -lzebra -lcrypt -lreadline -lrt

include $(MKDIR)/make.app

