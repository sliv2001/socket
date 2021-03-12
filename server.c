#include "server.h"
#include "utils.h"

int 		sfd, csfd;
struct peer	peers[PEER_MAX + 1];
/*Здесь возможна ошибка из-за неправильного выделения памяти*/

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

int getNearestPeer(){
	for (int i=0; i<PEER_MAX+1; i++){
		if (!peers[i].online)
			return i;
	}
	finalize(-1, "Too many clients. Disconnecting did not work.");
}

char* receive(){
	const size_t size = sizeof(struct peer);
	int len = BASIC_STRLEN;
	char* buff = stralloc(&len);
	if (len<BASIC_STRLEN||buff==NULL)
		finalize(-1, "Could not allocate memory");

	while (1){
		int p = getNearestPeer();
		csfd = accept(sfd,
			(struct sockaddr*)&peers[p].s_addr,
			(socklen_t*)&size);
		if (csfd<0)
			finalize(-1, "Could not create client socket");
		
	}
}

int initialize(){
	for (int i=0; i<PEER_MAX+1; i++){
		peers[i].s_addr = 0;
		peers[i].online = 0;
		peers[i].csfd = -1;
	}
	return 0;
}

void finalize(int res, char* str){

	close(sfd);
	for (int i = 0; i<PEER_MAX+1; i++){
		if (peers[i].csfd>=0)
			close(peers[i].csfd);
	}

	if (res==0)
		exit(0);
	else
		err(res, "%s", str);
}

int main(int argc, char** argv){
	initialize();
	sfd = setup_socket();
	receive();
	return 0;
}
