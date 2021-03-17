#include "server.h"
#include "utils.h"
#include "server-child.h"

int 		sfd, csfd=-1;
struct sockaddr_in peer_addr;

int setup_socket(){
	int res;
	struct sockaddr_in addr;
	struct in_addr internet_adress;
	int socket_d = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sfd = socket_d; /* socket_d оставлен для легаси */
	if (socket_d<0)
		finalize(-1, "Could not connect to socket");

	memset(&addr, 0, sizeof(addr));
	memset(&internet_adress, 0, sizeof(internet_adress));
	addr.sin_family = AF_INET;
	internet_adress.s_addr = INADDR_ANY;
	addr.sin_addr = internet_adress;
	addr.sin_port = PORT;
	res = bind(socket_d, (struct sockaddr*)&addr, sizeof(addr));
	if (res<0)
		finalize(res, "Could not bind socket");

	res = listen(socket_d, PEER_MAX);
	if (res<0)
		finalize(res, "Error in setting up for listening");
	return 0;
}

int connect_to_Socket(){
	int counter=0;
	pid_t pid;
	int size=sizeof(struct sockaddr_in);
	while (1){
		csfd = accept(sfd, (struct sockaddr*)&peer_addr, (socklen_t*)&size);
		if (csfd<0&&counter>=MAX_ATTEMPTS)
			finalize(-1, "Multiple errors in acception");
		if (csfd<0){
			warn("Error in acception #%d", counter);
			++counter;
			continue;
		}
		counter = 0;

		if ((pid=fork())==0)
			receive();
		if (pid<0)
			finalize(-1, "Could not make a fork()");
		if (pid>0)
			close(csfd);
	}
}

int initialize(){
	return 0;
}

void finalize(int res, char* str){
	if (res<=0)
		close(sfd);
	close(csfd);

	if (res==0)
		exit(0);
	else
		err(res, "%s", str);
}

int main(int argc, char** argv){
	initialize();
#ifdef DAEMON
	daemonize();
#endif
	sfd = setup_socket();
	connect_to_Socket();
	return 0;
}
