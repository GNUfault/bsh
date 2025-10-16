CC     = gcc
SRC    = bsh.c
BIN    = bsh
BITS   = 32
MARCH  = i686
MTUNE  = $(MARCH)
OPT    = s
STD    = c99
FLAGS  = -static
CFLAGS = -m$(BITS) -march=$(MARCH) -mtune=$(MTUNE) -O$(OPT) -std=$(STD) $(FLAGS)

all:
	$(CC) $(SRC) -o $(BIN) $(CFLAGS)
	objcopy --strip-all $(BIN)

clean:
	rm $(BIN)
