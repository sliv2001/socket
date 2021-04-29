#pragma once

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
#define BASIC_STRLEN 65536

int establish(const char*);

int sendData(char*);

int getData(char*, int);

int receive(char*, int*);
