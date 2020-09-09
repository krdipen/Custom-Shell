#include<stdio.h>
#include<string.h>

int main() {
    char str[1024];
    fgets(str,sizeof(str),stdin);
    char* token=strtok(str,"\n");
    printf("name %s\n",token);
    return 0;
}
