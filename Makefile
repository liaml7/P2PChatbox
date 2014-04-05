CFLAGS=-Wall -g

main:
	cc src/main.c -o main -lpthread
clean:
	rm -f main
