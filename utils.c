#include "utils.h"

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

char* Bash(char* inp, char* outp){

        return NULL;
}

int bashInit(){
	char* slave_path;
	int fd = open("/dev/ptmx", O_RDWR|O_NOCTTY);
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
	if (pid>0){
		dup2(slave_fd, STDIN_FILENO);
		dup2(slave_fd, STDOUT_FILENO);
		dup2(slave_fd, STDERR_FILENO);
		execl(BASH_PATH, BASH_PATH, (char*)0);
	}
	if (pid == 0)
		return fd;
	return 0;
}
