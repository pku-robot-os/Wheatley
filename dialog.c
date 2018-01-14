#include "util.h"
#include "speech_rec.h"
#include "pronunce.h"
#include "server.h"

int main(int argc, char **argv){
	if (argc!=3) {
		fprintf(stderr, "ERROR");
		exit(1);
	}
	int mpid;
	sscanf(argv[2],"%d",&mpid);
	
	kill(mpid,SIGUSR1);
}
