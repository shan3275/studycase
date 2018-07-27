.PHONY:all

all:zeromq-4.2.3 libzmq.a

libzmq.a:
	ln -s  zeromq-4.2.3/.libs/libzmq.a

zeromq-4.2.3:
	tar xvf zeromq-4.2.3.tgz>>/dev/null
	ln -s zeromq-4.2.3 zeromq
	cd zeromq &&  chmod +x configure
	cd zeromq && ./configure CFLAGS=-g --without-libsodium  >>/dev/null
	cd zeromq &&  make >>/dev/null


clean:
	rm -rf  zeromq-4.2.3
	rm -rf  libzmq.a
	rm -rf  zeromq
