all:
	gcc -o installer installer.c $(shell pkg-config --cflags --libs r_util)
	gcc -o core_r2premium.so -fPIC $(shell pkg-config --cflags --libs r_core) core_r2premium.c -shared

install:
	./installer -i

uninstall:
	./installer -u
