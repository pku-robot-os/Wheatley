#include "server.h"

static int tot = 0;
static struct service_node node_list[MAX_NODE];
static const char* file_name[] = "servicelist";

int service_ins(char* name, char* pattern, char* cmd) {

    if (tot >= MAX_NODE)
        return -1;
    node_list[tot].name = (char*)calloc(strlen(name) + 1, sizeof(char));
    node_list[tot].pattern = (char*)calloc(strlen(pattern) + 1, sizeof(char));
    node_list[tot].cmd = (char*)calloc(strlen(cmd) + 1, sizeof(char));
    strcpy(node_list[tot].name, name);
    strcpy(node_list[tot].pattern, pattern);
    strcpy(node_list[tot].cmd, cmd);
    ++tot;
    return 0;
}

void server_exec(char *input, char *output){ 
	const int STDIN = 0, STDOUT = 1;
	struct server_node *t = server_search(input);
	int fd1[2], f2[2];

	pid = fork();
	if(pipe(fd1) == -1 || pipe(fd2) == -1)
		err_quit("create pipe err.\n");

	if (pid == 0) {
		close(fd1[1]);
		close(fd2[0]);

		dup2(STDIN, fd1[0]);
		dup2(STDOUT, fd2[1]);
		char *argv = {"bash","-c",t->cmd};
		execve("bash",argv);

		close(fd1[0]);
		close(fd2[1]);
		exit(0);
	}
	else {
		close(fd1[0]);
		close(fd2[1]);

		// dup2(STDIN, fd2[0]);
		// dup2(STDOUT, fd1[1]);

		write(fd1[1], input, strlen(input));
		read(fd2[0], output, strlen(output));

		close(fd1[1]);
		close(fd2[0]);

	}

	int status;
	wait(&status);

	return output;
}
int service_init() {
    FILE* in = fopen(file_name, "r");
    char name[MAX_LINE] = {};
    char pattern[MAX_LINE] = {};
    char cmd[MAX_LINE] = {};
    while (!feof(in)) {
        fgets(name, MAX_LINE, in);
        fgets(pattern, MAX_LINE, in);
        fgets(cmd, MAX_LINE, in);
        if (feof(in))
            return -1;
        if (service_ins(name, pattern, cmd) == -1)
            return -1;
    }
    fclose(in);
}
int service_save() {
    FILE* out = fopen(file_name, "w");
    for (int i = 0; i < tot; ++i) {
        fputs(node_list[i].name, out);
        fputs(node_list[i].pattern, out);
        fputs(node_list[i].cmd, out);
    }

}
struct service_node* search(char* input) {
    for (int i = 0; i < tot; ++i)
        if (strstr(input, node_list[i].pattern) != NULL)
            return node_list + i;
    return NULL;
}

void service_del(int id) {
    free(node_list[id]->name);
    free(node_list[id]->pattern);
    free(node_list[id]->cmd);
    for (int i = id; i + 1 < tot; ++i)
        node_list[i] = node_list[i + 1];
    --tot;
}
char* service_exec(char *input) {
    struct server_node *t = service_search(input);
    char *argv = {"bash", "-c", t->cmd};
    execve("bash", argv);
}

