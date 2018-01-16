#include "server.h"

static int tot = 0;
static struct service_node node_list[MAX_NODE];
static const struct service_node default_service = {"", "", "PKUrobot"};
static const char* file_name = "servicelist";

int service_ins(char* name, char* pattern, char* cmd) {

    for (int i = 0; i < tot; ++i)
        if (strcmp(node_list[i].name, name) == 0) {
            free(node_list[i].pattern);
            free(node_list[i].cmd);
            node_list[i].pattern = (char*)calloc(strlen(pattern) + 1, sizeof(char));
            node_list[i].cmd = (char*)calloc(strlen(cmd) + 1, sizeof(char));
            strcpy(node_list[i].pattern, pattern);
            strcpy(node_list[i].cmd, cmd);
            return 1;
        }
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
/*
void server_exec(char *input, char *output) {
    const int STDIN = 0, STDOUT = 1;
    const struct server_node *t = server_search(input);
    int fd1[2], fd2[2];

    pid_t pid = fork();
    if (pipe(fd1) == -1 || pipe(fd2) == -1)
        //puts("create pipe err.\n");
        err_quit("create pipe err.\n");

    if (pid == 0) {
        close(fd1[1]);
        close(fd2[0]);

        dup2(STDIN, fd1[0]);
        dup2(STDOUT, fd2[1]);
        const char *argv[] = {"bash", "-c", t->cmd};
        execve("bash", argv);

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
}*/
int service_init() {
    puts("Open the data file...");
    FILE* in = fopen(file_name, "r");
    if (in == NULL) {
        puts("The data file is not exist...");
        puts("Create one? (y/n)");
        char ch;
        while ((ch = getchar()) && ch != 'y' && ch != 'n')
            ;
        if (ch == 'n')
            return -1;
        in = fopen(file_name, "w+");
        fclose(in);
        in = fopen(file_name, "r");
    }

    puts("Reading the data file...");
    char name[MAX_LINE] = {};
    char pattern[MAX_LINE] = {};
    char cmd[MAX_LINE] = {};
    fgets(name, MAX_LINE, in);
    while (!feof(in)) {
        fgets(pattern, MAX_LINE, in);
        fgets(cmd, MAX_LINE, in);
        name[strlen(name) - 1] = 0;
        pattern[strlen(pattern) - 1] = 0;
        cmd[strlen(cmd) - 1] = 0;
        if (feof(in))
            return -1;
        if (service_ins(name, pattern, cmd) == -1)
            return -1;
        fgets(name, MAX_LINE, in);
    }
    fclose(in);
    puts("OK");
    //if (tot == 0)
    //    return -1;
    return 0;
}

int service_save() {
    FILE* out = fopen(file_name, "w+");
    if (out == NULL)
        return -1;
    for (int i = 0; i < tot; ++i) {
        fputs(node_list[i].name, out);
        fputc('\n', out);
        fputs(node_list[i].pattern, out);
        fputc('\n', out);
        fputs(node_list[i].cmd, out);
        fputc('\n', out);
    }
    fclose(out);
    return 0;
}

const struct service_node* service_search(char* input) {
    for (int i = 0; i < tot; ++i)
        if (strstr(input, node_list[i].pattern) != NULL)
            return node_list + i;
    return &default_service;
}

const struct service_node* service_search_by_name(char* name) {
    for (int i = 0; i < tot; ++i)
        if (strcmp(name, node_list[i].name) == 0)
            return node_list + i;
    if (strcmp(name, default_service.name) == 0)
        return &default_service;
    return NULL;
}

int service_del(const struct service_node* pos) {
    if (pos == &default_service || pos == NULL)
        return -1;
    int id = pos - node_list;
    if (id < 0 || id >= tot)
        return -1;
    free(node_list[id].name);
    free(node_list[id].pattern);
    free(node_list[id].cmd);
    for (int i = id; i + 1 < tot; ++i)
        node_list[i] = node_list[i + 1];
    --tot;
}

int service_top(const struct service_node* pos) {
    if (pos == &default_service || pos == NULL)
        return -1;
    int id = pos - node_list;
    if (id < 0 || id >= tot)
        return -1;

    struct service_node tmp = *pos;
    for (int i = id; i > 0; --i)
        node_list[i] = node_list[i - 1];
    node_list[0] = tmp;
}

int service_run(const struct service_node* pos) {
    if (pos == NULL)
        return -1;
    return system(pos->cmd);
}

void service_printall() {
    printf("There are %d services\n", tot + 1);
    for (int i = 0; i < tot; ++i)
        printf("Service %d:\n-- name: %s\n-- pattern: %s\n-- command: %s\n", i + 1,
               node_list[i].name, node_list[i].pattern, node_list[i].cmd);
    printf("default_service:\n-- command: %s\n", default_service.cmd);
}
/*
char* service_exec(char *input) {
    struct server_node *t = service_search(input);
    char *argv = {"bash", "-c", t->cmd};
    execve("bash", argv);
}*/

