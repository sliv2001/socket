#include "server-child.h"

/*TODO make some free() */

extern int sfd, csfd;
extern struct sockaddr_in peer_addr;

int finalizeChild(){
        close(csfd);
        exit(0);
}

int bashInit(){
	return 0;
}

int getData(char* buf, int len){
        int res=recv(csfd, buf, len, 0);
        if (res<=0){
                finalizeChild();
        }
        return res;
}

int receive(){
        int result, len=BASIC_STRLEN;
        char* buf=stralloc(&len);
        char* res=stralloc(&len);
        if (buf==NULL||len==0||res==NULL)
                finalizeChild();
	if (bashInit()<0)
		finalizeChild();
        while (1){
                getData(buf, len);
                while (buf[len-1]!=0){
                        len+=BASIC_STRLEN;
                        strrealloc(buf, &len);
                        getData(&buf[len-BASIC_STRLEN], len);
                }
                res=processing(buf);
                sendMessage(res);
        }
        return 0;
}

int Print(char* inp){
	char* data = &inp[strlen(inp)];
	if (printf("%s", data)<0)
		return -1;
	else
		return 0;
}

char* Bash(char* inp){
	return NULL;
}

char* processing(char* inp){
	int len=BASIC_STRLEN;
	char* res = stralloc(&len);
	if (res==NULL||len==0)
		finalizeChild();
	if (getCommand(inp)<0)
		return NULL;

	if (!strcmp(inp, "bash"))
		res = Bash(inp);
	if (!strcmp(inp, "print"))
		Print(inp);
	if (!strcmp(inp, "exit"))
		finalizeChild();

        return res;
}

int sendMessage(char* inp){
	return 0;
}
