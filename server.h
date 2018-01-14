#ifndef __SERVER_H__
#define __SERVER_H__
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct service_node {
	char* name;
	char* pattern;
	char* cmd;
};
char service_exec(char input);
int service_init();
int service_save();
int service_ins(char* name, char* pattern, char* cmd);
struct server_node* service_search(char*);
void service_del(int id);
#endif
