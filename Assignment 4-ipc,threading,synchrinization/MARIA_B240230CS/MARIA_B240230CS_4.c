//cigarette smokers problem
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#include<time.h>
pthread_t agentThread,a,b,c;
sem_t tandp,pandm,mandt,doneSmoking;
int stop=0;
void * tobaccoandpaper(void *arg){
  //person w matches->i.e a picks up tobacco and paper
  while(1){
    sem_wait(&tandp);
    if(stop)break;
    printf("Smoker picks up tobacco and paper\n");
    printf("Smoker with matches starts smoking\n");
    sem_post(&doneSmoking);
  }
  return NULL;
}
void * paperandmatches(void *arg){
  while(1){
    sem_wait(&pandm); //wait until paper and matches appears
    if(stop)break;
    printf("Smoker picks up paper and matches\n");
    printf("Smoker with tobacco starts smoking\n");
    sem_post(&doneSmoking);
  }
  return NULL;
}
void * tobaccoandmatches(void *arg){
  while(1){
    sem_wait(&mandt); //wait until paper and matches appears
    if(stop)break;
    printf("Smoker picks up tobacco and matches\n");
    printf("Smoker with paper starts smoking\n");
    sem_post(&doneSmoking);
  }
  return NULL;
}
void *agent(void *arg){
  for(int j=0;j<5;j++){
    sem_wait(&doneSmoking);
    int x=rand()%3;
    if(x==0){
        //then tobacco and paper are placed on the table 
        printf("Agent places tobacco and paper on table\n");
        sem_post(&tandp);
    }
    else if(x==1){
      printf("Agent places paper and matches on table\n");
        sem_post(&pandm);
    }
    else{
      printf("Agent places tobacco and matches on table\n");
        sem_post(&mandt);
    }
  }
  stop=1;
  sem_post(&tandp);
  sem_post(&pandm);
  sem_post(&mandt);
  return NULL;
}
int main(){
  srand(time(NULL));
  sem_init(&tandp,0,0);
  sem_init(&pandm,0,0);
  sem_init(&mandt,0,0);
  sem_init(&doneSmoking,0,1);
  pthread_create(&agentThread,NULL,agent,NULL);
  pthread_create(&a,NULL,tobaccoandpaper,NULL);
  pthread_create(&b,NULL,paperandmatches,NULL);
  pthread_create(&c,NULL,tobaccoandmatches,NULL);
  pthread_join(agentThread,NULL);
  pthread_join(a,NULL);
  pthread_join(b,NULL);
  pthread_join(c,NULL);
  sem_destroy(&doneSmoking);
  sem_destroy(&tandp);
  sem_destroy(&pandm);
  sem_destroy(&mandt);
}
  
