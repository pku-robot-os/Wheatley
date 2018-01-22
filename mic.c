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
//#include "portaudio.h"

#ifndef MAX_LINE
#define MAX_LINE 1000
#endif

#define SAMPLERATE 11025
#define CYCLE 3
#define LEN (SAMPLERATE*CYCLE)
#define INTR_THRESH 70

//char *path = "mic.py";
char *host = "127.0.0.1";
char *port = "9000";
volatile int state = 1;// 1: listen 0: intup //dddd
float pa_buf[LEN];

void sigusr_handler(int signum);
void listenwords(char *str);


int main(int argc, char **argv) {
	puts("Now in mic.c");
	Signal(SIGUSR1,sigusr_handler);
	//FILE *f = fopen(path, "r");
	char str[1000];
	char buf[1000];
	int clientfd;
	while(1) {
		if (state == 1) {
			printf("aaaaa\n");
			listenwords(str);
			printf("str=%s\n",str);
			if (strcmp(str, "")==0 || strcmp(str, "Nothing")==0)
				continue;
			clientfd = open_clientfd(host, port);
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
/*			PaStream *pa;
			Pa_Initialize();
			int r = Pa_OpenDefaultStream(&pa, 1, 0, paFloat32, SAMPLERATE, paFramesPerBufferUnspecified, NULL, NULL);
			if(r != paNoError) {
				printf("err\n");
				continue;
			}
			Pa_StartStream(pa);
			Pa_ReadStream(pa, pa_buf, LEN);

			float volume = 0;
			for(int i = 0; i < LEN; i++) {
				volume += pa_buf[i]*pa_buf[i];
			} printf("volume: %f\n", volume);
			if(volume > INTR_THRESH) {
				write(clientfd, "1", strlen("1"));
			}*/
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
	if(system("cd record; ./record 1 > ../listentmp")==-1) {
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
