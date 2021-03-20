#pragma once

#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

int getCommand(char*);

char* stralloc(int*);

char* strrealloc(char*, int*);

void daemonize();

char* Bash(char*, char*);

int bashInit();
