#pragma once

#define _XOPEN_SOURCE 500

#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <fcntl.h>

#define BASH_PATH "/bin/bash"
#define BASH_PARAMS "-s -v"
#define MAX_STRLEN 65536

int getCommand(char*);

char* stralloc(int*);

char* strrealloc(char*, int*);

void daemonize();

int flush(int);

int Bash(int, char*, char*, int*);

int bashInit();

/* Uppercase-independent parser */
/*
int parse(int, char**, const char*);
int parse(int, char**, const char*, char*);
*/
