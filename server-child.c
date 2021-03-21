#include "server-child.h"

extern int sfd, csfd;
extern struct sockaddr_in peer_addr;
static int bash_session_fd = -1;

int finalizeChild(){
        close(csfd);
	if (bash_session_fd>=0)
		close(bash_session_fd);
        exit(0);
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
	bash_session_fd = bashInit();
        while (1){
                getData(buf, len);
                while (buf[len-1]!=0){
                        len+=BASIC_STRLEN;
                        strrealloc(buf, &len);
                        getData(&buf[len-BASIC_STRLEN], len);
                }
                processing(buf, res);
                sendMessage(res);
        }
	free(buf);
	free(res);
        return 0;
}

int Print(char* inp){
	char* data = &inp[strlen(inp)];
	if (printf("%s", data)<0)
		return -1;
	else
		return 0;
}

char* processing(char* inp, char* outp){
	if (inp==NULL||outp==NULL)
		finalizeChild();
	if (getCommand(inp)<0)
		return NULL;

	if (!strcmp(inp, "bash")||!strcmp(inp, "shell")){
		if (bash_session_fd>=0)
			outp = Bash(&inp[strlen(inp)], outp);
		else{
			if ((bash_session_fd=bashInit())<0){
				outp = "Unable to create bash session";
			}
			else
				outp = Bash(&inp[strlen(inp)], outp);
		}
	}
	if (!strcmp(inp, "print"))
		Print(inp);
	if (!strcmp(inp, "exit"))
		finalizeChild();

        return outp;
}

int sendMessage(char* inp){
	return 0;
}
