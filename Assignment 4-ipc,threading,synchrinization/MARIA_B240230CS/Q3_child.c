#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>

int id;
int playing = 1;
int last_result = 0;

// SIGUSR2: ball thrown to this child
void throw_handler(int sig){
    if(!playing) return;
    int r = rand() % 10;
    if(r < 8){
        last_result = 1;
        kill(getppid(), SIGUSR1); 
    } else {
        last_result = -1;
        playing = 0;
        kill(getppid(), SIGUSR2);
    }
    fflush(stdout);
}
void print_handler(int sig){
    if(last_result == 1)       printf("|%-7s", "CATCH");
    else if(last_result == -1) printf("|%-7s", "MISS");
    else if(playing)           printf("|%-7s", "....");
    else                       printf("|%-7s", "");
    fflush(stdout);
    last_result = 0;
    FILE*fp = fopen("childpid.txt","r");
    int n;
    fscanf(fp,"%d",&n);
    pid_t pids[n];
    for(int i=0;i<n;i++) fscanf(fp,"%d",&pids[i]);
    fclose(fp);
    if(id < n){
        kill(pids[id], SIGUSR1);
    } else {
        printf("|\n");
        fflush(stdout);
        FILE*df = fopen("dummycpid.txt","r");
        int dummy;
        fscanf(df,"%d",&dummy);
        fclose(df);
        kill(dummy, SIGINT);
    }
}

void exit_handler(int sig){
    if(playing) printf("+++ Child %d: Yay! I am the winner!\n", id);
    fflush(stdout);
    exit(0);
}

int main(int argc, char*argv[]){
    id = atoi(argv[1]);
    srand(getpid());
    signal(SIGUSR2, throw_handler);
    signal(SIGUSR1, print_handler);
    signal(SIGINT,  exit_handler);
    sleep(1); 
    while(1) pause();
}
