#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<time.h>

int main(){
    char s[] = "Hello World";
    printf("char  pid  Time(sec)\n");
    int i = 0;
    while(s[i] != '\0'){
        int pid = fork();
        if(pid == 0){
            printf("%c    %d    %d\n", s[i], getpid(),time(NULL));
            sleep(2);
            i++;
        }else{
            wait(NULL);
            exit(0);
        }
    }
    return 0;
}