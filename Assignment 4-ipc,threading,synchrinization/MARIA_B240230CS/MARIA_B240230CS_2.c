#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<semaphore.h>
#define SIZE 5
int buffer[SIZE];
sem_t empty;
sem_t mutex;
sem_t full;
int in=0;
int out=0;
void *producer(void *arg){
  for(int item=1;item<=10;item++){
    sem_wait(&empty);
    sem_wait(&mutex);
    buffer[in]=item;
    printf("Item %d is inserted at position %d\n",item,in);
    in=(in+1)%SIZE;
    sem_post(&mutex);
    sem_post(&full);
  }
  pthread_exit(0);
}
void*consumer(void *arg){
  int item;
  for(int i=0;i<10;i++){
    sem_wait(&full);
    sem_wait(&mutex);
    item=buffer[out];
    printf("Item %d is removed from position %d\n",item,out);
    out=(out+1)%SIZE;
    sem_post(&mutex);
    sem_post(&empty);
  }
  pthread_exit(0);
}
int main(){
  pthread_t prods,cons;
  sem_init(&empty,0,SIZE);
  sem_init(&full,0,0); 
  sem_init(&mutex,0,1);
  pthread_create(&prods,NULL,producer,NULL);
  pthread_create(&cons,NULL,consumer,NULL);
  pthread_join(prods,NULL);
  pthread_join(cons,NULL);
  sem_destroy(&mutex);
  sem_destroy(&full);
  sem_destroy(&empty);
}
