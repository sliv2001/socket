#include "server.h"
#include "utils.h"
#include "server-child.h"
#define LOG_PATH "./log.txt"
#include "logger.h"

int 		sfd, csfd=-1;
struct sockaddr_in peer_addr;
int	tcp=0, udp=0;
static int Port = PORT;

static void sig_int(int signo){
	finalize(0, "\n");
}

int setup_socket(){
	int res;
	struct sockaddr_in addr;
	struct in_addr internet_adress;
	int socket_d=-1;
	if (tcp){
		socket_d = socket(AF_INET, SOCK_STREAM, 0);
	}
	else {
		socket_d = socket(AF_INET, SOCK_DGRAM, 0);
	}
	sfd = socket_d; /* socket_d оставлен для легаси */
	if (socket_d<0)
		finalize(-1, "Could not connect to socket");

	memset(&addr, 0, sizeof(addr));
	memset(&internet_adress, 0, sizeof(internet_adress));
	addr.sin_family = AF_INET;
	internet_adress.s_addr = htonl(INADDR_ANY);
	addr.sin_addr = internet_adress;
	addr.sin_port = htons(Port);
	res = bind(socket_d, (struct sockaddr*)&addr, sizeof(addr));
	if (res<0){
		pr_err("Could not bind socket");
		close(sfd);
		exit(-1);
	}
	if (tcp){
		res = listen(socket_d, PEER_MAX);
		if (res<0)
			finalize(res, "Error in setting up for listening");
	}
	return 0;
}

int connect_to_Socket(){
	int counter=0;
	pid_t pid;
	int size=sizeof(struct sockaddr_in);
	while (tcp){
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
			receive(tcp);
		if (pid<0)
			finalize(-1, "Could not make a fork()");
		if (pid>0)
			close(csfd);
	}
	while (udp)
		receive(tcp);
	return 0;
}

int initialize(int argc, char** argv){
	int place;
	int port;
	if ((place=contains(argc, argv, "-p"))>-1)
		if (sscanf(argv[place+1], "%d", &port)>0)
			Port = port;
	if (contains(argc, argv, "--tcp")){
		tcp=1;
		return 0;
	}
	if (contains(argc, argv, "--udp")&&tcp!=1){
		udp = 1;
		return 0;
	}
	pr_err("Unset --tcp or --udp or both are set");
	return -1;
}

void finalize(int res, char* str){
	close(sfd);
	close(csfd);

	if (res==0){
		printf("\n");
		exit(0);
	}
	else
		err(res, "%s", str);
}

int main(int argc, char** argv){
	log_init_fd(STDOUT_FILENO);
	if (argc<2){
		pr_err("Not enough args: only %d", argc);
		return -1;
	}
	initialize(argc, argv);
#ifdef DAEMON
	daemonize();
#else
	if (signal(SIGINT, sig_int)== SIG_ERR||signal(SIGTERM, sig_int)==SIG_ERR)
		pr_warn("Couldnot intercept SIGINT");
#endif
	setup_socket();
	connect_to_Socket();
	return 0;
}
