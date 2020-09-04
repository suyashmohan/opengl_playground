IDIR = -I include/
LDIR = -L lib/
CFLAGS = -Wall -pedantic

SRC=$(wildcard ./src/*.c) $(wildcard ./src/*/*.c)

gl_mac: $(SRC)
	clang -o $@ $^ $(IDIR) $(LDIR) $(CFLAGS) -lglfw -std=c99

gl_win: $(SRC)
	tcc -o $@.exe $^ $(IDIR) $(LDIR) $(CFLAGS) -lglfw3 -DSTBI_NO_SIMD

astyle:
	astyle --options="astyle-code-format.cfg" "src/*.c,*.h"