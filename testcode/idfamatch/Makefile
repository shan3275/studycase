CC = gcc -std=gnu99 -g 

OBJ=    xtlvsender.o cpcre.o

libs = -lpcre

Bin = xtlv

$(Bin):   $(OBJ)
	$(CC) -o $@  $^ $(libs)

clean:
	rm -rf $(OBJ) $(Bin)

