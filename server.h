#ifndef __SERVER_H__
#define __SERVER_H__
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define MAX_NODE 1000
#define MAX_LINE 1000

struct service_node {
	char* name;
	char* pattern;
	char* cmd;
};
void service_exec(char *input, char *output);
int service_ins(char* name, char* pattern, char* cmd);
const struct service_node* service_search(char* input);
const struct service_node* service_search_by_name(char* name);
int service_del(const struct service_node* pos);
int service_top(const struct service_node* pos);
int service_run(const struct service_node* pos);
int service_init();
int service_save();
void service_printall();
#endif
