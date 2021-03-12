#pragma once

#include <stdlib.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


char*	stralloc(int*);

char*	strrealloc(char*, int*);

void	daemonize();
