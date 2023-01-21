#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<time.h>

int main(int argc, char *argv[]){
    if(argc==1) exit(0);
    int size=argc;
    if(argc==1) exit(0);
    int n=atoi(argv[size-1]);
    printf("Twice : Current process id: %d, Current result: %d ", getpid(), n*2);
    printf("\n");
    for(int i=0; i<size-1; i++){
        argv[i]=argv[i+1];
    }
    argv[size-1]=NULL;
    //convert int to string
    char str[10];
    sprintf(str, "%d", n*2);
    argv[size-2]=str;
    execvp (argv[0], argv);
}
