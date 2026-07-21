#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
sem_t semaphore;
int shared=10;
void*fun1(void*arg){
  sem_wait(&semaphore);
  printf("Thread 1:Value before increment=%d\n",shared);
  int local=shared;
  local++;
  sleep(1);
  shared=local;
  printf("Thread 1:Value after increment=%d\n",shared);
  sem_post(&semaphore);
  pthread_exit(NULL);
}
void *fun2(void *arg){
  sem_wait(&semaphore);
  printf("Thread 1:Value before decrement=%d\n",shared);
  int local=shared;
  local--;
  sleep(1);
  shared=local;
  printf("Thread 1:Value after decrement=%d\n",shared);
  sem_post(&semaphore);
  pthread_exit(NULL);
}
int main(){
  pthread_t t1,t2;
  sem_init(&semaphore,0,1);
  pthread_create(&t1,NULL,fun1,NULL);
  pthread_create(&t2,NULL,fun2,NULL);
  pthread_join(t1,NULL);
  pthread_join(t2,NULL);
  printf("Final value of shared=%d\n",shared);
  sem_destroy(&semaphore);
  return 0;
}
  
