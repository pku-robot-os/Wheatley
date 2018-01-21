#include "server.h"

int main() {

	puts("");
	puts("");
	if (service_init() == -1) {
		puts("Oops!");
		puts("The manager couldn't find the service file or the file is not consistent.");
		return -1;
	}
	puts("The manager of external services");
	puts("May I help you?");
	printf("\n-- add -s <service> -p <pattern> -c <command> : ");
	printf(" create a new external service named <service>, run <command> when");
	printf(" the sentence has a substring matching the regex <pattern>.\n");
	puts("   The command must have the three arguments, and the order must be \"-s -p -c\"");
	printf("   If you already have the external service named <service>, ");
	printf("then the old one will be replaced.\n");
	printf("   Also, if you are typing commands with Chinese characters, don't use the backspace (\'\\b\'), for there is something wrong with linux characters handling.\n");
	printf("\n-- printall : print the list of all external services ");
	printf("by priority in decreasing order\n");
	printf("\n-- delete <service> : delete the new external service named <service> in the list.\n");
	printf("\n-- top <service> : set the service named <service> the highest priority ");
	printf("other services will keep the order\n");
	printf("\n-- run [<service>] : run the command of <service> if <service> exist\n");
	printf("   run the default service if <service> is not given.\n");
	puts("\n-- save : save the changes");
	puts("\n-- quit or ctrl+C : exit the program without saving");
	puts("\n-- exit : save the changes and exit the program");
	char *command = (char*) malloc(MAX_LINE);
	size_t command_MAX_LINE = MAX_LINE;
	while (1) {
		printf("\nservice_manager > ");
		int len = getline(&command,&command_MAX_LINE,stdin);
		command[len-1] = 0;
		//for(int i=0;i<len-1;++i)
		//	printf("%d ",command[i]);
		char *p = command;
		while (*p && !isspace(*p))
			++p;
		//printf("pos=%d len=%d\n",p-command,len);
		char *q = p;
		while (isspace(*q))
			++q;

		*p = 0;
		if (strcmp(command, "add") == 0) {
			if (p-command == len-1)
			{
				puts("The add command doesn\'t have enough arguments");
				continue;
			}
			char *p1 = strstr(p+1, "-s ");
			//printf("pos=%d\n",p1-command);
			char *p2 = p1==NULL ? NULL : strstr(p1, " -p ");
			//printf("pos=%d\n",p2-command);
			char *p3 = p2==NULL ? NULL : strstr(p2, " -c ");
			//printf("pos=%d\n",p3-command);
			/*
			char *p1 = q;
			while(*p1 && *p1!='-') ++p1;
			char *p2 = *p1 ? p1+1 : p1;
			while(*p2 && *p2!='-') ++p2;
			char *p3 = *p2 ? p2+1 : p2;
			while(*p3 && *p3!='-') ++p3;*/
			if(p3 == NULL)
				puts("The add command doesn\'t have enough arguments");
			else if(p1[1]!='s' || p2[2]!='p' || p3[2]!='c')
				puts("The add command doesn\'t have proper arguments");
			else {
				++p2, ++p3;
				char *q1 = p2-1, *q2 = p3-1;
				while(isspace(*q1)) --q1;
				while(isspace(*q2)) --q2;
				q1[1] = q2[1] = '\0';
				p1+=2, p2+=2, p3+=2;
				while(isspace(*p1)) ++p1;
				while(isspace(*p2)) ++p2;
				while(isspace(*p3)) ++p3;
				if(*p1==0 || *p2==0 || *p3==0)
					puts("Some arguments of the add command are missing");
				else {
					int status = service_ins(p1, p2, p3);
					switch(status) {
						case -1: puts("There are too many services"); break;
						case 0: printf("Insert service %s successfully\n", p1); break;
						case 1: printf("Replace old service %s successfully\n", p1); break;
						default: ;
					}
				}
			}
		}
		else if (strcmp(command, "printall") == 0) {
			service_printall();
		}
		else if (strcmp(command, "delete") == 0) {
			if (service_del(service_search_by_name(q)) == -1)
				printf("Delete service %s failed. Perhaps the service doesn\'t exist.\n", q);
			else
				printf("Delete service %s successfully\n", q);
		}
		else if (strcmp(command, "top") == 0) {
			if (service_top(service_search_by_name(q)) == -1)
				printf("Set service %s failed. Perhaps the service doesn\'t exist.\n", q);
			else
				printf("Set service %s successfully\n", q);
		}
		else if (strcmp(command, "run") == 0) {
			if (service_run(service_search_by_name(q)) == -1) {
				printf("Run service %s failed. Perhaps the service doesn\'t exist", q);
				printf(" or there is something wrong with system calls\n");
			}
		}
		else if (strcmp(command, "input") == 0) {
			//puts("The input part");
			char output[MAX_LINE]={};
			service_exec(q, output);
			printf("The result of your input is:\n%s\n", output);
		}
		else if (strcmp(command, "save") == 0) {
			if (service_save() == -1)
				puts("Save failed");
			else
				puts("Save successfully");
		}
		else if (strcmp(command, "quit") == 0) {
			exit(0);
		}
		else if (strcmp(command, "exit") == 0) {
			if (service_save() == -1) {
				puts("Save failed");
				puts("Please retry after a while");
			}
			else {
				puts("Save successfully");
				exit(0);
			}
		}
		else
			puts("Unknown command.");
	}
}
