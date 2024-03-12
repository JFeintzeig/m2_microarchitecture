targets := $(wildcard *.c)
CFLAGS := -Wall -Wpedantic -O2

main: kpc.o

clean:
	rm main kpc.o
