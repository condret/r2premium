all:
	gcc -o installer installer.c $(shell pkg-config --cflags --libs r_util)

install:
	./installer
