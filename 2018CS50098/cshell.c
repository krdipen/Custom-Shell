#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

void execArgs(char** parsed) {
    if(fork()==0) {
        if(parsed[1]!=NULL) {
            int infile_desc=open(parsed[1],O_RDONLY);
            dup2(infile_desc,STDIN_FILENO);
            close(infile_desc);
        }
        if(parsed[2]!=NULL) {
            int outfile_desc=open(parsed[2],O_WRONLY);
            dup2(outfile_desc,STDOUT_FILENO);
            close(outfile_desc);
        }
        if(execvp(parsed[0],parsed)<0) {
            printf("command not found\n");
            exit(0);
        }
    }
    else {
        wait(NULL);
        return;
    }
}

void execArgsPiped(char** parsed1,char** parsed2) {
    int pipefd[2];
    pipe(pipefd);
    if(fork()==0) {
        close(pipefd[0]);
        dup2(pipefd[1],STDOUT_FILENO);
        close(pipefd[1]);
        if(parsed1[1]!=NULL) {
            int infile_desc=open(parsed1[1],O_RDONLY);
            dup2(infile_desc,STDIN_FILENO);
            close(infile_desc);
        }
        if(parsed1[2]!=NULL) {
            int outfile_desc=open(parsed1[2],O_WRONLY);
            dup2(outfile_desc,STDOUT_FILENO);
            close(outfile_desc);
        }
        if(execvp(parsed1[0],parsed1)<0) {
            printf("command not found\n");
            exit(0);
        }
    }
    else {
        if(fork()==0) {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            if(parsed2[1]!=NULL) {
                int infile_desc=open(parsed2[1],O_RDONLY);
                dup2(infile_desc,STDIN_FILENO);
                close(infile_desc);
            }
            if(parsed2[2]!=NULL) {
                int outfile_desc=open(parsed2[2],O_WRONLY);
                dup2(outfile_desc,STDOUT_FILENO);
                close(outfile_desc);
            }
            if(execvp(parsed2[0],parsed2)<0) {
                printf("command not found\n");
                exit(0);
            }
        }
        else {
            wait(NULL);
            wait(NULL);
        }
    }
}

void parseSpace(char* tokena,char* tokenb,char* tokenc,char** parsed) {
    char* temp=tokena;
    char* temp1=tokenb;
    char* temp2=tokenc;
    parsed[0]=strtok(temp," <>");
    strtok(temp1,"<");
    parsed[1]=strtok(NULL," <>");
    strtok(temp2,">");
    parsed[2]=strtok(NULL," <>");
}

int main() {
    printf("\e[1;1H\e[2J");
    printf("###############################################################################\n");
    printf("###############################################################################\n");
    while(true) {
        printf("shell> ");
        char command[1024];
        char commandCopya[1024];
        char commandCopyb[1024];
        char commandCopyc[1024];
        fgets(command,sizeof(command),stdin);
        strcpy(commandCopya,command);
        strcpy(commandCopyb,command);
        strcpy(commandCopyc,command);
        char* token=strtok(command," \n");
        if(token!=NULL) {
            if(strcmp(token,"cd")==0) {
                token=strtok(NULL," \n");
                if(token==NULL) {
                    token="/Users/dipenkumar";
                }
                if(chdir(token)!=0) {
                    printf("No such file or directory\n");
                }
            }
            else if(strcmp(token,"pwd")==0) {
                char cwd[1024];
                getcwd(cwd,sizeof(cwd));
                printf("%s\n",cwd);
            }
            else if(strcmp(token,"mkdir")==0) {
                token=strtok(NULL," \n");
                if(token==NULL) {
                    printf("mkdir [-pv] [-m mode] directory ...\n");
                }
                else if(mkdir(token,0777)!=0) {
                    printf("No such file or directory\n");
                }
            }
            else if(strcmp(token,"rmdir")==0) {
                token=strtok(NULL," \n");
                if(token==NULL) {
                    printf("rmdir [-p] directory ...\n");
                }
                else if(rmdir(token)!=0) {
                    printf("No such file or directory\n");
                }
            }
            else if(strcmp(token,"exit")==0) {
                printf("logout\n");
                break;
            }
            else {
                char* parsed1[4]={NULL,NULL,NULL,NULL};
                char* parsed2[4]={NULL,NULL,NULL,NULL};
                char* token1a=strtok(commandCopya,"|\n");
                char* token2a=strtok(NULL,"|\n");
                char* token1b=strtok(commandCopyb,"|\n");
                char* token2b=strtok(NULL,"|\n");
                char* token1c=strtok(commandCopyc,"|\n");
                char* token2c=strtok(NULL,"|\n");
                if(token2a==NULL) {
                    parseSpace(token1a,token1b,token1c,parsed1);
                    execArgs(parsed1);
                }
                else {
                    parseSpace(token1a,token1b,token1c,parsed1);
                    parseSpace(token2a,token2b,token2c,parsed2);
                    execArgsPiped(parsed1,parsed2);
                }
            }
        }
    }
    return 0;
}
