#pragma once

#include "server.h"
#include "utils.h"

int finalizeChild();

int getData(char* buf, int len);

int receive();

char* processing(char* inp);

int sendMessage(char* inp);
