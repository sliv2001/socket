#include "server-child.h"

extern int sfd, csfd;
extern struct sockaddr_in peer_addr;

int finalizeChild(){
        close(csfd);
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
                finalize(getpid(), "Could not allocate memory");
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

char* processing(char* inp){
        return NULL;
}

int sendMessage(char* inp){
	return 0;
}
