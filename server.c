#include "server.h"

#define MAX_NODE 1000
#define MAX_LINE 1000

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

