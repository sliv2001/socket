#pragma once

#include <stdio.h>

#include "utils.h"
#include "server.h"

int finalizeChild();

int getData(char* buf, int len);

int receive();

int Print(char*);

int processing(char* inp, char* outp);

int sendMessage(char* inp);
