#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#define N 5
sem_t forks[N];
pthread_t phil[N];
void eat(int i){
  printf("Philosopher %d is eating\n",i);
  sleep(1);
}
void think(int i){
   printf("Philosopher %d is thinking\n",i);
   sleep(2);
}
void* philosopher(void *num){
  int i=*(int*)num;
  int left=i;
  int right=(i+1)%N;
  for(int j=0;j<5;j++){
  think(i);
  if(i%2==0){
    //pick up left fork then right fork
    sem_wait(&forks[left]);
    printf("Philosopher %d picked up left fork %d\n",i,left);
    sem_wait(&forks[right]);
    printf("Philosopher %d picked up right fork %d\n",i,right);
  }else{
    sem_wait(&forks[right]);
    printf("Philosopher %d picked up right fork %d\n",i,right);
    sem_wait(&forks[left]);
    printf("Philosopher %d picked up left fork %d\n",i,left);

  }
  eat(i);
  sem_post(&forks[left]);
  sem_post(&forks[right]);
  } 
  return NULL;
}
int main(){
  int id[N];
  for(int i=0;i<N;i++){
    sem_init(&forks[i],0,1);
  }
  for(int i=0;i<N;i++){
    id[i]=i;
    pthread_create(&phil[i],NULL,philosopher,&id[i]);
  }
  for(int i=0;i<N;i++){
    pthread_join(phil[i],NULL);
  }
  for(int i=0;i<N;i++){
    sem_destroy(&forks[i]);
  }
}
