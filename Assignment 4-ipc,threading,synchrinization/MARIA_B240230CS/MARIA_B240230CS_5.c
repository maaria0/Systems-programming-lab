#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
int balance=1000;
pthread_mutex_t lock;
typedef struct {
  int atm_id;
  int amt;
  int type;
}transaction;
void *withdraw(void *arg){
  transaction *t=(transaction*)arg;
  pthread_mutex_lock(&lock); 
  if(balance>=t->amt){
    balance-=t->amt;
    printf("balance remaining :%d\n",balance);
  }else{
    printf("insufficent balance");
  }
  pthread_mutex_unlock(&lock);
  return NULL;
}
void *deposit(void *arg){
  transaction*t=(transaction*)arg;
  pthread_mutex_lock(&lock);
  balance+=t->amt;
  printf("new balance is : %d\n",balance);
  pthread_mutex_unlock(&lock);
  return NULL;
}
int main(){
  int opt,amt,atm_count=0;
  pthread_t tid;
  transaction t;
  pthread_mutex_init(&lock,NULL);
  while(1){
    atm_count++;
    printf("Enter choice :1. withdraw 2.deposit 3.exit");
    scanf("%d",&opt);
    if(opt==1){
      printf("Enter amount to withdraw: ");
      scanf("%d", &amt);
      t.atm_id=atm_count;
      t.amt=amt;
      pthread_create(&tid,NULL,withdraw,&t);
      pthread_join(tid,NULL);
    }else if(opt==2){
       printf("Enter amount to deposit: ");
      scanf("%d", &amt);
      t.atm_id=atm_count;
      t.amt=amt;
      pthread_create(&tid,NULL,deposit,&t);
      pthread_join(tid,NULL);
    }else{
      break;
    }
    }
    pthread_mutex_destroy(&lock);
    return 0;
} 
