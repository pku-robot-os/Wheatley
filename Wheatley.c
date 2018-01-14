#include <signal.h>
#include "util.h"
#include "pronunce.h"

void sigkill_handler(int signum);
void gui_init();
int microphone_init();
void new_dialog(char* id,int pid);
void kill_dialog();
int dpid,mpid;
int main(){
	Signal(SIGKILL,sigkill_handler);
	gui_init();
	int listenfd = open_listenfd("9000");
	mpid = microphone_init();
	socklen_t clientlen;
	struct sockaddr_storage clientaddr;

	while (1) {
		clientlen = sizeof(clientaddr);
		int connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
		char buf[10];
		read(connfd, buf, 9);
		int type;
		sscanf(buf, "%d", &type);
		if (type==0) { //new dialog 
			new_dialog(buf,mpid);
		}else {
			kill(dpid,SIGTERM);
		}
		close(connfd);
	}
	return 0;
}
void sigkill_handler(int signum){
	printf("Bye Bye\n");
	kill(0,SIGKILL);
	exit(0);
}

void gui_init(){
}
int microphone_init(){
	return 0;
}
void new_dialog(char *filename,int pid){
	dpid = fork(); 
	if (dpid == 0) {
		char buf1[10];
		char *buf[] = {filename,buf1};
		sprintf(buf1,"%d",pid);
		execvp("dialog",buf);
	}
}

