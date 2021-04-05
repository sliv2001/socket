#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdlib.h>

#include "logger.h"
#include "utils.h"

#define PORT 32001

int establish_TCP(const char*);
