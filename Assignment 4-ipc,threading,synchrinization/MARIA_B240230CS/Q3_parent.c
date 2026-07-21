#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#define MAX 100

pid_t child[MAX];
int status[MAX]; 
int n;
volatile int caught = -1;

void catch_handler(int sig){ caught = 1; }
void miss_handler(int sig)  { caught = 0; }

void print_separator(){
    for(int i=0;i<n;i++) printf("+-------");
    printf("+\n");
    fflush(stdout);
}

void print_header(){
    print_separator();
    for(int i=1;i<=n;i++) printf("|%7d",i);
    printf("|\n");
    print_separator();
    fflush(stdout);
}

int main(int argc, char*argv[]){
    if(argc != 2){
        printf("Usage: %s n\n", argv[0]);
        exit(1);
    }
    n = atoi(argv[1]);

    signal(SIGUSR1, catch_handler);
    signal(SIGUSR2, miss_handler);

    printf("Parent: %d child processes created\n", n);

    FILE*fp = fopen("childpid.txt","w");
    fprintf(fp,"%d\n",n);
    for(int i=0;i<n;i++){
        pid_t pid = fork();
        if(pid == 0){
            char id[10];
            sprintf(id,"%d",i+1);
            execl("./child","child",id,NULL);
            exit(1);
        }
        child[i] = pid;
        status[i] = 1;
        fprintf(fp,"%d\n",pid);
    }
    fclose(fp);
    printf("Parent: Waiting for child processes to read child database\n");
    sleep(2);
    print_header();
    int alive = n;
    int cur = 0;
    while(alive > 1){
        // skip dead children
        if(status[cur] == 0){
            cur = (cur+1) % n;
            continue;
        }
        // throw ball to child[cur]
        caught = -1;
        kill(child[cur], SIGUSR2);
        while(caught == -1) pause();

        if(caught == 0){
            status[cur] = 0;
            alive--;
        }
        pid_t dummy = fork();
        if(dummy == 0){
            execl("./dummy","dummy",NULL);
            exit(1);
        }
        FILE*df = fopen("dummycpid.txt","w");
        fprintf(df,"%d\n",dummy);
        fclose(df);
        kill(child[0], SIGUSR1);
        waitpid(dummy, NULL, 0);
        print_separator();
        cur = (cur+1) % n;
    }
    print_header();
    for(int i=0;i<n;i++){
        kill(child[i], SIGINT);
    }
    while(wait(NULL) > 0);
    return 0;
}
