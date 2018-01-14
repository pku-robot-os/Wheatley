#ifndef __SERVER_H__ 
#define __SERVER_H__
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
struct server_node{
	char* name;
	char* pattern;
	char* cmd;
};
char* server_exec(char* input);
int server_ins(struct server_node); 
struct server_node* server_search(char* input);
int server_del(int id);
#endif
