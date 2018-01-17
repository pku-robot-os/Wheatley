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

int service_exec(char *input, char *output) {
    const int STDIN = 0, STDOUT = 1;
    const struct service_node *t = service_search(input);
    //printf("Service:\n name: %s\n-- pattern: %s\n-- command: %s\n",
    //        t->name, t->pattern, t->cmd);

    char tmpin[] = "services/tmpin", tmpout[] = "services/tmpout";
    FILE *in = fopen(tmpin, "w+");
    if (in == NULL) {
        puts("There is something wrong with tmp files");
        return -1;
    }
    fprintf(in, "%s", input);
    fclose(in);

    char realcmd[MAX_LINE] = {};
    sprintf(realcmd, "cd services; %s 0<%s 1>%s", t->cmd, "tmpin", "tmpout");
    //printf("The command is %s\n", realcmd);
    if (system(realcmd) == -1) {
        puts("There is something wrong with system calls");
        return -1;
    }
    FILE *out = fopen(tmpout, "r");
    if (out == NULL) {
        puts("There is something wrong with tmp files");
        return -1;
    }
    fread(output, MAX_LINE, sizeof(char), out);
    fclose(out);
}
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

int service_match(char* input, char* pattern) {
    regex_t reg;
    regcomp(&reg, pattern, REG_EXTENDED);
    regmatch_t pmatch[1];
    int status = regexec(&reg, input, 1, pmatch, 0);
    regfree(&reg);
    return status != REG_NOMATCH;
}
const struct service_node* service_search(char* input) {
    for (int i = 0; i < tot; ++i)
        if (service_match(input, node_list[i].pattern))
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
    return 0;
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
    return 0;
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

