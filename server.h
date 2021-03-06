#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <err.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define	SOCKET_PATH	"/tmp/s_connect"
#define	PORT		32001
#define	PEER_MAX	50
#define	BASIC_STRLEN	65536
#define MAX_ATTEMPTS	10
int	setup_socket();

int	connect_to_Socket();

int	initialize(int, char**);

void	finalize(int, char*);
