#include <signal.h>
#include "util.h"
#include "pronounce.h"

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
		char buf[MAXLINE+3];
		read(connfd, buf, MAXLINE+3);
		close(connfd);
		int type;
		sscanf(buf, "%d", &type);
		if (type==0) { //new dialog 
			new_dialog(buf+2,mpid);
		}else if (type==1){
			kill(dpid,SIGTERM);
		}else {//error
			pronounce("__networkfail");
		}
	}
	return 0;
}

void sigkill_handler(int signum){
	printf("Bye Bye\n");
	kill(0,SIGKILL);
	exit(0);
}

void gui_init(){
	pid = fork();
	char *buf[] = {"python3","./mic"}
	if (pid == 0) {
		execvp("python3",buf);
	}
}
int microphone_init(){
	pid = fork();
	char *buf[] = {"./mic"};
	if (pid == 0) {
		execvp("./mic",buf);
	}
	return pid;
}
void new_dialog(char *question,int pid){
	dpid = fork(); 
	if (dpid == 0) {
		char buf1[6];
		char *buf[] = {question,buf1};
		sprintf(buf1,"%d",pid);
		execvp("./dialog",buf);
	}
}
