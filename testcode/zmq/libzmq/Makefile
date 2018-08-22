.PHONY:all

all:zeromq-4.2.3 libzmq.a

libzmq.a:
	ln -s  zeromq-4.2.3/src/.libs/libzmq.a

zeromq-4.2.3:
	tar xvf zeromq-4.2.3.tar.gz>>/dev/null
	ln -s zeromq-4.2.3 zeromq
	cd zeromq &&  mkdir -p src/.libs
	cd zeromq &&  mkdir -p tools/.libs
	cd zeromq &&  mkdir -p perf/.libs
	cd zeromq && ./configure CFLAGS=-g --without-libsodium  >>/dev/null
	cd zeromq &&  make >>/dev/null


clean:
	rm -rf  zeromq-4.2.3
	rm -rf  libzmq.a
	rm -rf  zeromq
