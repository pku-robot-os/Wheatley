#include "util.h"
void gui_init();
int microphone_init();
void new_dialog();
void kill_dialog();
int main(){
	gui_init();
	int listenfd = open_listenfd("9000");
	int mpid = microphone_init();
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
			int id;
			sscanf(buf, "%d", &id);
			new_dialog(id,mpid);
		}else {
			kill_dialog();
		}
		close(connfd);
	}
	return 0;
}

void gui_init(){
}
int microphone_init(){
	return 0;
}
void new_dialog(int id,int pid){
	
}
void kill_dialog(){
}

