#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/wait.h>
#include<sys/shm.h>
#include<time.h>
int main(int argc,char *argv[]){
  if(argc!=3){
    fprintf(stderr,"Usage: %s n t\n",argv[0]);
    exit(1);
  }
  int n=atoi(argv[1]);
  int t=atoi(argv[2]);
  srand(time(NULL));
  //creating shared memory
  int shmid=shmget(IPC_PRIVATE,2*sizeof(int),IPC_CREAT | 0666);
  if(shmid<0){
    perror("shmget");
    exit(1);
  }
  //connecting shared memory to process space
  int *M=(int *)shmat(shmid,NULL,0);
  if(M==(void*)-1){
    perror("shmat");
    exit(1);
  }
  M[0]=0;
  int *count=calloc(n,sizeof(int));
  int *sum=calloc(n,sizeof(int));
  printf("Producer is alive\n");
  for(int i=1;i<=n;i++){
    pid_t pid=fork();
    if(pid==0){
      int mycount=0;
      int mysum=0;
      printf("Consumer %d is alive\n",i);
      while(1){
        while(M[0]!=i && M[0]!=-1);
        if(M[0]==-1) break;
        int item=M[1];
#ifdef VERBOSE
  printf("Consumer %d read %d\n",i,item);
#endif
      mycount++;
      mysum+=item;
      M[0]=0;
      }
      printf("Consumer %d has read %d items:Checksum=%d\n",i,mycount,mysum);
      shmdt(M);
      exit(0);
    }
  }
  for(int i=0;i<t;i++){
    int item=100+rand()%900;
    int c=1+rand()%n;
    while(M[0]!=0);
    M[0]=c;
  #ifdef SLEEP
    usleep(1+rand()%10);
  #endif
    M[1]=item;
  #ifdef VERBOSE
    printf("Producer produces %d for Consumer %d\n",item,c);
  #endif
    count[c-1]++;
    sum[c-1]+=item;
    }
    while(M[0]!=0);
    M[0]=-1;
    for(int i=0;i<n;i++)wait(NULL);
    printf("\nProducer has produced %d items\n",t);
    for(int i=0;i<n;i++){
      printf("%d items for Consumer %d: Checksum=%d\n",count[i],i+1,sum[i]);
    }
    shmdt(M);
    shmctl(shmid,IPC_RMID,NULL);
    free(count);
    free(sum);
    return 0;
  }
  
  
  
