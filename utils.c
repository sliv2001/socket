#include "utils.h"

char* stralloc(int* len){
	char* res=(char*)malloc(*len);
	if (res==NULL){
		*len=0;
		return NULL;
	}
	res[0]=0;
	return res;
}

char* strrealloc(char* str, int* len){
	char* res;
	if (*len==0)
		return str;
	res=(char*)realloc(str, *len);
	if (res==NULL){
		len=0;
		return str;
	}
	res[sizeof(str)]=0;
	return res;
}

void daemonize(){
	pid_t pid;
	umask(0);
	if ((pid=fork())>0)
		exit(0);
	if (pid<0)
		err(-1, "Daemonization failure");
	setsid();
	if ((pid=fork())>0)
		exit(0);
	if (pid<0)
		err(-1, "Daemonization failure");
	chdir("/");
}

int getCommand(char* inp){
	int i=0;
	for (i; i<strlen(inp); i++){
		if (isspace(inp[i])){
			inp[i]=0;
			exit(0);
		}
	}
	return -1;
}
