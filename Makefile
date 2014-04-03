CFLAGS=-Wall -g

main:
	cc src/main.c -o main -lpthread
	cc src/server.c -o server
	cc src/client.c -o client
clean:
	rm -f main
	rm -f client
	rm -f server
