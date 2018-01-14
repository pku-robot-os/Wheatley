#ifndef __SERVER_H__
#define __SERVER_H__
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_NODE 1000
#define MAX_LINE 1000

struct service_node {
	char* name;
	char* pattern;
	char* cmd;
};
void server_exec(char *input, char *output);
int service_ins(char* name, char* pattern, char* cmd);
struct server_node* server_search(char* input);
int server_del(int id);
int service_init();
int service_save();
#endif
