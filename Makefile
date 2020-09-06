IDIR = -I include/
LDIR = -L lib/
CFLAGS = -Wall -pedantic

SRC=$(wildcard ./src/*.c) $(wildcard ./src/*/*.c)

mingw: $(SRC)
	gcc -o $@ $^ $(IDIR) $(LDIR) $(CFLAGS) -lglfw3dll -std=c99

tcc: $(SRC)
	tcc -o $@.exe $^ $(IDIR) $(LDIR) $(CFLAGS) -lglfw3 -DSTBI_NO_SIMD

astyle:
	astyle --options="astyle-code-format.cfg" "src/*.c,*.h"