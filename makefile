primary_source_files = utils.c server-child.c

all: debug release debug-daemon client

client:
	gcc utils.c client.c -std=c11 -O0 -g -o client

debug:
	gcc $(primary_source_files) server.c -std=c11 -O0 -g -o server

release:
	gcc $(primary_source_files) server.c -std=c11 -D DAEMON -o server.d

debug-daemon:
	gcc $(primary_source_files) server.c -std=c11 -D DAEMON -o server-d.d
