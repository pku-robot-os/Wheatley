/*
Description: Run python script "mic.py" in C.
Compile: gcc -I/usr/include/python2.7 <filename>.c -o <filename> -lpython2.7
Run: ./<filename> mic.py
PS: You may need python2.7-dev installed
*/
//#include <Python.h>
#include "util.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef MAX_LINE
#define MAX_LINE 1000
#endif

//char *path = "mic.py";
char *host = "127.0.0.1";
char *port = "9000";
volatile int state = 1;// 1: listen 0: intup //dddd

void sigusr_handler(int signum);
void listenwords(char *str);


int main(int argc, char **argv) {
	puts("Now in mic.c");
	Signal(SIGUSR1,sigusr_handler);
	//FILE *f = fopen(path, "r");
	char str[1000];
	char buf[1000];
	while(1) {
		// Reset seek pointer, so PyRun_SimpleFile can read from the beginning of file
		if (state == 1) {
			listenwords(str);
			printf("str=%s\n",str);
			printf("strlen=%d\n",strlen(str));
			for(int i=0;i<strlen(str);++i)
				printf("%d ",str[i]);
			puts("");
			if (strcmp(str, "")==0 || strcmp(str, "Nothing")==0) continue;
			int clientfd = open_clientfd(host, port);
			if (strcmp(str, "Error")==0) {	
				sprintf(buf,"%d %s\n",2,str);
				//send error message
			}else {
				sprintf(buf,"%d %s\n",0,str);
				//get the true message 
				state = 0;
			}
			write(clientfd,buf,strlen(buf));
			close(clientfd);
		}else {
			//intr 
		}
	}
	return 0;
}

void sigusr_handler(int signum){
	state = 1;
}

void listenwords(char *str) {
	
	const char* tmp = "listentmp";
	puts("begin to real_listen()");
	sleep(1);
	//usleep(300*1000);
	if(system("./record 1 > listentmp")==-1) {
		puts("error");
		sleep(100);
	}

	FILE *in = fopen("listentmp", "r");
	char ch;
	while((ch=fgetc(in))!='[' && ch!=EOF)
		;
	int pos=0;
	if(!feof(in)) {
		while(pos+1<MAX_LINE && (ch=fgetc(in))!=']' && (ch!=EOF))
			str[pos++]=ch;
	}
	str[pos]='\0';
	fclose(in);
	/*
	Python version
	rewind(f);
	Py_Initialize();

	PyRun_SimpleFile(f, path);

	PyObject *m = PyImport_AddModule("__main__");
	PyObject *v = PyObject_GetAttrString(m, "mic");
	// Get "mic" from Python environment
	// "mic" stores what you said to the microphone
	strcpy(str, PyString_AsString(v));
	printf("%s\n", str);

	Py_DECREF(v);
	Py_Finalize();
	*/
}
