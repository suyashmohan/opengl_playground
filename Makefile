# This Makefile allows to run using Mingw-w64 on author's system only

IDIR = -I ../deps/include/ -I ./include
LDIR = -L ../deps/lib/
CFLAGS = -Wall -O2 -lglfw3 -lopengl32 -lgdi32 -std=c99

SRC=$(wildcard ./src/*.c) $(wildcard ./src/*/*.c) $(wildcard ../deps/src/*.c)

mingw: $(SRC)
	gcc -o main.exe $^ $(IDIR) $(LDIR) $(CFLAGS)