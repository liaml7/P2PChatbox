CFLAGS=-Wall -g

main:
	cc src/main.c -o main -lpthread `libgcrypt-config --cflags --libs`
clean:
	rm -f main
