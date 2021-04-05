#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>

#include "utils.h"
#define LOG_PATH ""
#include "logger.h"

#define PORT 32001
#define BASIC_STRLEN 128
int establish_TCP(const char*);

int sendData(char*);
