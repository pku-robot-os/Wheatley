#include "util.h"
#include "pronounce.h"
#include "server.h"

int main(int argc, char **argv){
	if (service_init() == -1) {
		puts("Oops!");
		puts("The manager couldn't find the service file or the file is not consistent.");
		return -1;
	}
	if (argc!=3) {
		fprintf(stderr, "ERROR");
		exit(1);
	}
	int mpid;
	sscanf(argv[2],"%d",&mpid);
	int clientfd = open_clientfd("127.0.0.1","9001");
	char *input = argv[1];
	for(int i=0;i<strlen(input);++i)
		printf("%d ",input[i]);
	puts("");
	//char input[MAX_LINE];
	char output[MAX_LINE];
	//recongnition(argv[1],input);
	write(clientfd, input, strlen(input));
	service_exec(input,output);
	write(clientfd, output, strlen(output));
	pronounce(output);
	close(clientfd);
	kill(mpid,SIGUSR1);
	return 0;
}
