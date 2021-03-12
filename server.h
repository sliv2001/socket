#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <err.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include <stdlib.h>

#define	SOCKET_PATH	"/tmp/s_connect"
#define	PORT		32001
#define	PEER_MAX	50
#define	BASIC_STRLEN	128

struct	peer{
	uint32_t s_addr;
	int online;
	int csfd;
};

int	setup_socket();

char*	receive();

int	getNearestPeer();

int	initialize();

void	finalize(int, char*);
