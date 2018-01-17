#include<stdio.h>

int main() {
	char ch[10000];
	gets(ch);
	printf("Your string is: %s\n",ch);
}