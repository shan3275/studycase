PROG    = server

ECFLAGS = -g -O0 -I$(SRCDIR)/zmq/libzmq/zeromq/include  -I. #-Werror

ELDFLAGS = -L $(SRCDIR)/zmq/libzmq
LDLIBS  =  -Wl -lzmq  -lrt
LDFLAGS += -lstdc++

include $(MKDIR)/make.app

