#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>

void handler(int sig){
    exit(0);
}

int main(){
    signal(SIGINT, handler);
    while(1) pause();
    return 0;
}
