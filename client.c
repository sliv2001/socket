#include "client.h"

int sfd = -1;

int establish_TCP(const char* str_addr){
	struct sockaddr_in addr;
	struct in_addr a;
	sfd = socket(AF_INET, SOCK_STREAM, 0);
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
	if (connect(sfd, (struct sockaddr*)&addr, sizeof(addr))<0){
		pr_err("Couldnot connect to socket %s", str_addr);
		exit(-1);
	}
	return 0;
}

int sendData(char* buf){
	int i, n, wrong=0;
	char small_buf[BASIC_STRLEN];
	memset(small_buf, 0, BASIC_STRLEN);
	for (i=0; i<BUFSZ-BASIC_STRLEN; i+=BASIC_STRLEN){
		if (send(sfd, buf+i, BASIC_STRLEN, 0)<0)
			wrong=1;
	}
	i = n;
	for (i=BUFSZ-i; i<BUFSZ; i++){
		small_buf[i]=buf[n+i];
	}
	if (send(sfd, small_buf, BASIC_STRLEN, 0)<0||wrong==1){
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

/*TODO сделать универсальный метод чтения и записи в utils */
/*TODO сделать отправляемую строку переменного размера */
int main(int argc, char** argv){
	char buf[BUFSZ];
	int len = BUFSZ;
	char* rec = stralloc(&len);
	int i;
	if (log_init_fd(STDERR_FILENO)<0)
		return -1;
	if (argc<3)
		pr_err("Wrong args");
	establish_TCP(argv[argc-1]);
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
