#include "client.h"

int sfd = -1;
int tcp = 0;

int establish(const char* str_addr){
	struct sockaddr_in addr;
	struct in_addr a;
	if (tcp){
		sfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	}
	else {
		sfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	}
	if (sfd<0){
		pr_err("Couldnot create socket");
		exit(-1);
	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	a.s_addr = inet_addr(str_addr);
	if (a.s_addr==INADDR_NONE){
		pr_err("Wrong IP address: %s", str_addr);
		exit(-1);
	}
	addr.sin_addr = a;
	if (tcp){
		if (connect(sfd, (struct sockaddr*)&addr, sizeof(addr))<0){
			pr_err("Couldnot connect to socket %s", str_addr);
			exit(-1);
		}
	}
	return 0;
}

int sendData(char* buf){
	int i, n;
	int len = strlen(buf);
	if (send(sfd, buf, len, MSG_NOSIGNAL)<0){
		pr_err("Couldnot send data");
		return -1;
	}
	return 0;
}

int GetData(char* buf, int len){
        int res=recv(sfd, buf, len, 0);
        if (res<=0){
	        pr_err("couldnot get data responce from socket");
		return -1;
        }
        return res;
}

int Receive(char* buf, int* buf_len){
	int buf_len_BA;
	if (GetData(buf, BASIC_STRLEN)<0)
		return -1;
        while (buf[*buf_len-1]!=0){
                *buf_len+=BASIC_STRLEN;
                buf_len_BA = *buf_len;
                strrealloc(buf, buf_len);
                if (buf_len_BA <= *buf_len){
			flush(sfd);
                        break;
                }
                if (GetData(&buf[*buf_len-BASIC_STRLEN], BASIC_STRLEN)<0)
			return -1;
        }
}

int main(int argc, char** argv){
	int len = BUFSZ;
	char buf[BUFSZ];
	char* rec = stralloc(&len);
	int i;
	if (log_init_fd(STDERR_FILENO)<0)
		return -1;
	if (argc<3)
		pr_err("Wrong args");
	if (contains(argc, argv, "--tcp"))
		tcp = 1;
	if (contains(argc, argv, "--udp"))
		tcp=0;
	else
		if (tcp!=1){
			pr_err("Unset --tcp or --udp or both are set");
			return -1;
		}
	establish(argv[argc-1]);
	while (1){
		if (read(STDIN_FILENO, &buf, BUFSZ)<0){
			pr_err("Couldnot read stdin");
			return -1;
		}
		for (i=0; i<BUFSZ-1; i++)
			if (buf[i]=='\n'){
				buf[i+1]=0;
				break;
			}
		sendData(buf);
		Receive(rec, &len);
		printf("%s", rec);
	}
	return 0;
}
