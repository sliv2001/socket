#include "client.h"

int establish_TCP(char* str_addr){
	struct sockaddr_in addr;
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd<0){
		pr_err("Couldnot create socket");
		exit(-1);
	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	return 0;
}

int main(int argc, char** argv){
	if (argc<3)
		pr_err("Wrong args");
	establish_TCP(argv[argc-1]);
	return 0;
}
