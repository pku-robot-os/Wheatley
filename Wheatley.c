#include <signal.h>
#include "util.h"
#include "server.h"
#include "pronounce/pronounce.h"

void sigint_handler(int signum);
void sigchld_handler(int signum);
int gui_init();
int microphone_init();
void new_dialog(char* id,int pid);
void kill_dialog();
volatile int gpid, mpid;
int dpid;
int pid;
int main(){
	Signal(SIGINT,sigint_handler);
	Signal(SIGCHLD,sigchld_handler);
	gpid = gui_init();
	int listenfd = open_listenfd("9000");
	pronounce("你好我是Wheatley");
	mpid = microphone_init();
	socklen_t clientlen;

	struct sockaddr_storage clientaddr;
	int clientfd = open_clientfd("127.0.0.1","9002");
	close(clientfd);

	while (1) {
		clientlen = sizeof(clientaddr);
		int connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
		char buf[MAX_LINE+3];
		memset(buf,0,sizeof(buf));
		read(connfd, buf, MAX_LINE+3);
		close(connfd);
		int type;
		//puts(buf);
		sscanf(buf, "%d", &type);
		if (type==0) { //new dialog
			if (strstr(buf, "再见") !=NULL || strstr(buf, "关机") !=NULL) {
				const char* output = "再见，我关机啦。";
				int clientfd = open_clientfd("127.0.0.1","9002");
				write(clientfd, output, strlen(output));
				close(clientfd);
				pronounce(output);
				system("pkill firefox");
				kill(0, SIGINT);
			}
			new_dialog(buf+2,mpid);
		}else if (type==1){
			kill(dpid,SIGTERM);
		}else {//error
			pronounce("__networkfail");
		}
	}
	return 0;
}

void sigint_handler(int signum){
	printf("Bye Bye\n");
	kill(mpid,SIGKILL);
	kill(gpid,SIGKILL);
	exit(0);
}
void sigchld_handler(int signum){
	while(waitpid(-1, NULL, WNOHANG) > 0)
		;
}
int gui_init(){
	int pid = fork();
	char *buf[] = {"python3","gui/gui.py",NULL};
	if (pid == 0) {
		execvp("python3",buf);
	}
	return pid;
}
int microphone_init(){
	int pid = fork();
	char *buf[] = {"./mic",NULL};
	if (pid == 0) {
		execvp("./mic",buf);
	}
	return pid;
}
void new_dialog(char *question,int pid){
	dpid = fork(); 
	if (dpid == 0) {
		char buf1[6];
		char *buf[] = {"./dialog",question,buf1,NULL};
		sprintf(buf1,"%d",pid);
		execvp("./dialog",buf);
	}
}
