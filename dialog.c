#include "util.h"
#include "pronounce.h"
#include "server.h"

int main(int argc, char **argv){
	if (argc!=3) {
		fprintf(stderr, "ERROR");
		exit(1);
	}
	int mpid;
	sscanf(argv[2],"%d",&mpid);
	int clientfd = open_clientfd("127.0.0.1","9001");
	char input[MAX_LINE],output[MAX_LINE];
	recongnition(argv[1],input);
	write(clientfd, input, strlen(input));
	server_exec(input,output);
	write(clientfd, output, strlen(output));
	pronounce(output);
	close(clientfd);
	//kill(mpid,SIGUSR1);
	return 0;
}
