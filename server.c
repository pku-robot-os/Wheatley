#include "server.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

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
