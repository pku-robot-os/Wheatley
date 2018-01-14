#include "server.h"
char* server_exec(char *input){ 
	struct server_node *t = server_search(input);
	char *argv = {"bash","-c",t->cmd};
	execve("bash",argv);
}
