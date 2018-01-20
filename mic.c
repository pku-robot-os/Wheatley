/*
Description: Run python script "mic.py" in C.
Compile: gcc -I/usr/include/python2.7 <filename>.c -o <filename> -lpython2.7
Run: ./<filename> mic.py
PS: You may need python2.7-dev installed
*/
#include <Python.h>
#include "util.h"

char *path = "mic.py";
char *host = "127.0.0.1";
char *port = "9000";
int state = 1;// 1: listen 0: intup

void sigusr_handler(int signum);
void listen(char *str);


int main(int argc, char **argv) {
	Signal(SIGUSR1,sigusr_handler);
	FILE *f = fopen(path, "r");
	char str[50];
	while(1) {
		// Reset seek pointer, so PyRun_SimpleFile can read from the beginning of file
		if (state == 1){
			listen(str);
			if (strcmp(str, "Nothing")) continue;
			int clientfd = open_clientfd(host, port);
			if (strcmp(str, "Error")) {
				ssprintf(buf,"%d %s\n",2,str);
				//send error message
			}else {
				ssprintf(buf,"%d %s\n",0,str);
				//get the true message 
				state = 0;
			}
			write(client,buf,strlen(buf));
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

void listen(char *str) {
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
}
