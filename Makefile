CFLAGS=-Wall -g

main:
	cc src/tracker.c -o tracker
	cc src/server.c -o server
	cc src/client.c -o client
clean:
	rm -f tracker
	rm -f client
	rm -f server
