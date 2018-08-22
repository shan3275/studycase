CC = gcc -std=gnu99 -g

OBJ=    main.o confile.o yd_config.o yd_capture.o util.o pkt_parser.o filter_rule_main.o \
	common.o buffer.o dict.o base64.o zhelpers.o  cJSON.o zmalloc.o yd_resource.o\
	ecr_util_hash.o yd_worker_thread.o

CFLAGS= -I ../incude/pcap -I../include/event -I../include/zlog -I../include/zeromq

libs = -L../lib/pcap -lpcap -L../lib/event -levent  -L../lib/zlog/ -lzlog -L../lib/zeromq -lzmq -lm -lrt

Bin = yeecapture

$(Bin):   $(OBJ)
	$(CC) -o $@  $^ $(libs)
	cp $(Bin) ../bin/

clean:
	rm -rf $(OBJ) $(Bin)

