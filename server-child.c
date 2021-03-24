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
/*TODO add checking of refused connection */
        int res=recv(csfd, buf, len, 0);
        if (res<=0){
                finalizeChild();
        }
        return res;
}

int receive(){
        int result, buf_len=BASIC_STRLEN, res_len=BASIC_STRLEN;
        char* buf=stralloc(&buf_len);
        char* res=stralloc(&res_len);
        if (buf==NULL||buf_len==0||res==NULL||res_len==0)
                finalizeChild();
	if (bashInit()<0)
		finalizeChild();
	bash_session_fd = bashInit();
        while (1){
                getData(buf, BASIC_STRLEN);
		/*TODO Change this while() so that like in Bash() */
                while (buf[buf_len-1]!=0){
                        buf_len+=BASIC_STRLEN;
                        strrealloc(buf, &buf_len);
                        getData(&buf[buf_len-BASIC_STRLEN], BASIC_STRLEN);
                }
                processing(buf, res, &res_len);
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

int processing(char* inp, char* outp, int* outp_len){
	if (inp==NULL||outp==NULL)
		finalizeChild();
	if (getCommand(inp)<0)
		return -1;
	memset(outp, 0, *outp_len);
	if (!strcmp(inp, "bash")||!strcmp(inp, "shell")){
		if (bash_session_fd>=0)
			Bash(bash_session_fd, &inp[strlen(inp)], outp, outp_len);
		else{
			if ((bash_session_fd=bashInit())<0){
				outp = "Unable to create bash session";
			}
			else
				Bash(bash_session_fd, &inp[strlen(inp)], outp, outp_len);
		}
	}
	if (!strcmp(inp, "print"))
		Print(inp);
	if (!strcmp(inp, "exit"))
		finalizeChild();

        return 0;
}

int sendMessage(char* inp, int* inp_len){
	if (send(csfd, inp, (size_t)*inp_len, MSG_NOSIGNAL)<0){
		finalizeChild();
	}
	return 0;
}
