#include "utils.h"
#include "server.h"

/*TODO Set up appropriate logging*/

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
		*len = 0;
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
			return i;
		}
	}
	return -1;
}

int flush(int fd){
	return 0;
}

int tryReadFrom(int fd, char* outp, int outp_len){
	#ifndef BASIC_STRLEN
		#define BASIC_STRLEN 65536
		#warning Had to redefine BASIC_STRLEN
	#endif
	if (read(fd, outp, outp_len)<0)
		return -1;
	return 0;
}

int Bash(int fd, char* inp, char* outp, int outp_len){
	struct pollfd p;
	int pres, length;
	p.fd = fd;
	p.events = POLLIN;
	if (poll(&p, 1, 2000)>0)
		tryReadFrom(fd, outp, outp_len);
	if (inp[strlen(inp)-1]!='\n'){
		inp[strlen(inp)]='\n';
		inp[strlen(inp)+1]='\0';
	}
	else
		inp[strlen(inp)]=0;
	if (write(fd, inp, strlen(inp))<0)
		return -1;
	tryReadFrom(fd, outp, strlen(inp)+3);
	length = 0;
	while ((pres = poll(&p, 1, 2000))>0&&length<BASIC_STRLEN){
		if (tryReadFrom(fd, outp+length, outp_len)<0){
			return -1;
		}
		length = strlen(outp);

	}
        return 0;
}

int bashInit(){
	char* slave_path;
	int fd = posix_openpt(O_RDWR|O_NOCTTY);
	int slave_fd;
	pid_t pid;
	if (fd<0)
		return -1;
	if (grantpt(fd)<0)
		return -1;
	if (unlockpt(fd)<0)
		return -1;
	slave_path = ptsname(fd);
	if (slave_path == NULL)
		return -1;
	if ((slave_fd=open(slave_path, O_RDWR))<0)
		return -1;
	if ((pid = fork())<0)
		return -1;
	if (pid==0){
		dup2(slave_fd, STDIN_FILENO);
		dup2(slave_fd, STDOUT_FILENO);
		dup2(slave_fd, STDERR_FILENO);
		execl(BASH_PATH, BASH_PATH, (char*)0);
	}
	if (pid > 0)
		return fd;
	return 0;
}

int contains(int argc, char** argv, const char* val){
	int i;
	for (i=1; i<argc; i++){
		if (!strcasecmp(val, argv[i]))
			return i;
	}
	return 0;
}
