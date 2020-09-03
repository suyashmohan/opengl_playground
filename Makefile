CC = clang
LIBS  = -lglfw
IDIR = -I include/
LDIR = -L lib/
CFLAGS = -Wall -pedantic

SRC=$(wildcard ./src/*.c)

gl_mac: $(SRC)
	$(CC) -o $@ $^ $(IDIR) $(LDIR) $(CFLAGS) $(LIBS)

clean:
	rm gl_mac