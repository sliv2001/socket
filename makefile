all: debug release debug-daemon
debug:
	gcc server.c -std=c11 -O0 -g -o server
release:
	gcc server.c -std=c11 -D DAEMON -o server.d
debug-daemon:
	gcc server.c -std=c11 -D DAEMON -o server-d.d
